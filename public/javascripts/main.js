const searchButton = document.getElementById("search-button");
const backButton = document.getElementById("back-button");
const usernameField = document.getElementById("username");
const resultContainer = document.getElementById("result");

if(searchButton) {
    searchButton.addEventListener("click", () => {
        const username = usernameField.value;
        window.location.href = window.location.origin + '/?username=' + username;

    });
} else {
    backButton.addEventListener("click", () => {
        window.location.href = window.location.origin;
    });
}