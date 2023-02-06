
#include <napi.h>

#include <Windows.h>
#include <lm.h>        // USER_INFO_xx and various #defines
#include <Sddl.h>      // ConvertSidToStringSid
#include <userenv.h>   // CreateProfile

#include <codecvt>
#include <vector>

#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "userenv.lib")

using namespace Napi;

std::wstring to_wstring(Value value) {
    size_t length;
    napi_status status = napi_get_value_string_utf16(
        value.Env(),
        value,
        nullptr,
        0,
        &length);
    NAPI_THROW_IF_FAILED_VOID(value.Env(), status);

    std::wstring result;
    result.reserve(length + 1);
    result.resize(length);
    status = napi_get_value_string_utf16(
        value.Env(),
        value,
        reinterpret_cast<char16_t*>(result.data()),
        result.capacity(),
        nullptr);
    NAPI_THROW_IF_FAILED_VOID(value.Env(), status);
    return result;
}

Value to_value(Env env, std::wstring_view str) {
    return String::New(
        env,
        reinterpret_cast<const char16_t*>(str.data()),
        str.size());
}

// like unique_ptr, but takes a deleter value, not type.
template <typename T, auto Deleter>
struct Ptr {
    T* value = NULL;

    Ptr() = default;
    // No copies
    Ptr(const Ptr&) = delete;
    Ptr& operator=(const Ptr&) = delete;
    // Moves
    Ptr(Ptr&& other) : value{ other.release() } {}
    Ptr& operator=(Ptr&& other) {
        assign(other.release());
    }

    ~Ptr() { clear(); }

    operator T*() const { return value; }
    T* operator ->() const { return value; }

    T* assign(T* newValue) {
        clear();
        return value = newValue;
    }

    T* release() {
        auto result = value;
        value = nullptr;
        return result;
    }

    void clear() {
        if (value) {
            Deleter(value);
            value = nullptr;
        }
    }
};

template <typename T>
using Win32Local = Ptr<T, LocalFree>;

std::wstring formatSystemError(HRESULT hr) {
    Win32Local<WCHAR> message_ptr;
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        LANG_USER_DEFAULT,
        (LPWSTR)&message_ptr,
        0,
        nullptr);

    if (!message_ptr) {
        return L"Unknown Error";
    }

    std::wstring message(message_ptr);

    // Windows ends it's system messages with "\r\n", which is bad formatting for us.
    if (auto last_not_newline = message.find_last_not_of(L"\r\n");
        last_not_newline != std::wstring::npos) {
        message.erase(last_not_newline + 1);
    }

    return message;
}

Error createWindowsError(napi_env env, HRESULT hr, const char* syscall) {
    napi_value error_value = nullptr;

    napi_status status = napi_create_error(
        env,
        nullptr,
        to_value(env, formatSystemError(hr)),
        &error_value);
    if (status != napi_ok) {
        throw Error::New(env);
    }

    auto error = Error(env, error_value);
    error.Value().DefineProperties({
        PropertyDescriptor::Value("errno", Number::New(env, hr)),
        PropertyDescriptor::Value("name", String::New(env, "WindowsError")),
        PropertyDescriptor::Value("syscall", String::New(env, syscall)),
    });
    return error;
}

template <typename USER_INFO_level, int level>
struct NetUserInfo : Ptr<USER_INFO_level, NetApiBufferFree> {
    bool get(Env env, LPCWSTR username) {
        return get(env, nullptr, username);
    }

    bool get(Env env, LPCWSTR servername, LPCWSTR username) {
        clear();
        auto nerr = NetUserGetInfo(servername, username, level, reinterpret_cast<LPBYTE*>(&value));
        if (nerr == NERR_UserNotFound) {
            return false;
        }
        if (nerr != NERR_Success) {
            throw createWindowsError(env, GetLastError(), "NetUserGetInfo");
        }
        return true;
    }
};

#define NET_USER_INFO(level) NetUserInfo<USER_INFO_ ## level, level>

Win32Local<WCHAR> sid_to_local_string(Env env, PSID sid) {
    Ptr<WCHAR, LocalFree> local;
    if (!ConvertSidToStringSidW(sid, &local.value)) {
        throw createWindowsError(env, GetLastError(), "ConvertSidToStringSidW");
    }
    return local;
}

Value sid_to_value(Env env, PSID sid) {
    auto local = sid_to_local_string(env, sid);
    return to_value(env, local.value);
}

Value get(CallbackInfo const& info) {
    auto env = info.Env();

    auto name = to_wstring(info[0]);

    NET_USER_INFO(23) user_info;

    if (!user_info.get(env, name.c_str())) {
        return env.Null();
    }

    auto res = Object::New(env);
    res.DefineProperties({
        PropertyDescriptor::Value("name", to_value(env, user_info->usri23_name), napi_enumerable),
        PropertyDescriptor::Value("full_name", to_value(env, user_info->usri23_full_name), napi_enumerable),
        PropertyDescriptor::Value("comment", to_value(env, user_info->usri23_comment), napi_enumerable),
        PropertyDescriptor::Value("flags", Number::New(env, user_info->usri23_flags), napi_enumerable),
        PropertyDescriptor::Value("sid", sid_to_value(env, user_info->usri23_user_sid), napi_enumerable),
    });
    return res;
}

#define NET_USER_SET_INFO(level, name, ...) {       \
    USER_INFO_##level user_info { __VA_ARGS__ };    \
    auto nerr = NetUserSetInfo(                     \
        nullptr,                                    \
        name,                                       \
        level,                                      \
        (LPBYTE) &info,                             \
        nullptr);                                   \
    if (nerr != NERR_Success) {                     \
        throw createWindowsError(env, nerr, "NetUserSetInfo"); \
    }                                               \
}

HANDLE get_handle(Env env, Value value) {
    if (!value.IsExternal()) {
        throw TypeError::New(env, "'handle' should be an External returned from logonUser()");
    }

    return value.As<External<void>>().Data();
}

#define EXPORT_FUNCTION(name) \
    PropertyDescriptor::Function(env, exports, #name, name, napi_enumerable)

Object module_init(Env env, Object exports) {
    exports.DefineProperties({
        EXPORT_FUNCTION(get),
    });
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, module_init);
