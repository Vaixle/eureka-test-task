# eureka-test-task

[![Project Status: WIP – Initial development is in progress, but there has not yet been a stable, usable release suitable for the public.](https://www.repostatus.org/badges/latest/inactive.svg)](https://www.repostatus.org/#inactive)
[![GitHub](https://img.shields.io/github/license/Vaixle/eureka-test-task)](https://github.com/Vaixle/eureka-test-task/blob/main/LICENSE.md)

## Conntents:

- [RU](#RU) 
   - [Основные технологии](#Основные-технологии) 
   - [Тестовое задание](#Тестовое-задание) 
   - [Описание](#Описание) 
   - [Требования](#Требования) 
   - [Решение](#Решение) 
      - [Начало работы](#Начало-работы) 
- [EN](#EN) 
   - [Main Technologies](#Main-technologies) 
   - [Test task](#Test-task) 
   - [Description](#Description) 
   - [Requirments](#Требования) 
   - [Solution](#Solution)
      - [Getting started](#Getting-started) 


## RU

### Основные технологии

| **Backend**  |![NodeJS](https://img.shields.io/badge/node.js-6DA55F?style=for-the-badge&logo=node.js&logoColor=white) ![Express.js](https://img.shields.io/badge/express.js-%23404d59.svg?style=for-the-badge&logo=express&logoColor=%2361DAFB)|
|:------------:|:------------:|

### Тестовое задание

#### Описание


Необходимо написать простое web-приложение с использованием node.js node-addon-api WinAPI для проверки наличия пользователя (например, Администратор) в списке пользователей операционной системы Windows.

#### Требования

Приложение должно работать по следующему сценарию

1. Пользователь запускает web-сервер из корня проекта:
   server.bat
2. Автоматически открывается браузер с начальной web страницей, на которой отображаются
- поле ввода имени пользователя
- кнопка "Проверить"
3. Пользователь вводит любое имя пользователя и нажимает на "проверить"
4. Если введённый пользователь присутствует в списке пользователей Windows, отображается страница с надписью "Пользователь есть", иначе "Пользователя нет". Под надписью есть кнопка "Назад", при нажатии на которую происходит возврат на начальную страницу.
5. Проверку наличия введённого имени пользователя в списке пользователей ОС Windows запрещено проводить путём сравнения текущего пользователя, от которого запущен процесс сервера и введённого пользователя, а также запрещено использовать названия домашних каталогов пользователей в каталоге C:\Users (С:\Пользователи).

Список пользователей для проверки можно получить через Управление компьютером->Служебные программы->Локальные пользователи и группы->Пользователи.

6. В корне проекта создать файл README.MD с текстом тестового задания в кодировке UTF-8

7. Проект выложить на github.com (другие ресурсы не рассматриваются).

8. Ссылку на проект присылайте на job@eurekabpo.ru

9. Срок выполнения задания 7 дней начиная с завтрашнего дня.

#### Решение

#### Начало работы


Запуск приложения


Запуск с помощью bat файла.

---


> Клонировать репозиторий

```
git clone https://github.com/Vaixle/talkme-shop-api.git
```

> Перейти в каталог проекта запустить server.bat
```
server.bat
```
---

## EN

### Main technologies

| **Backend**  |![NodeJS](https://img.shields.io/badge/node.js-6DA55F?style=for-the-badge&logo=node.js&logoColor=white) ![Express.js](https://img.shields.io/badge/express.js-%23404d59.svg?style=for-the-badge&logo=express&logoColor=%2361DAFB)|
|:------------:|:------------:|

### Test task.

#### Description

You need to write a simple web application using node.js node-addon-api WinAPI to check if a user (for example, Administrator) is on the user list of the Windows operating system.

#### Requirments

The application should work according to the following scenario

1. The user runs the web server from the root of the project:
   server.bat
2. a browser opens automatically with an initial web page displaying
- The input field of the user name
- "Check" button
3. The user enters any username and clicks on "check".
4. If the entered user is present in the list of Windows users, the page with the inscription "User present" is displayed, otherwise "User not present". Under the description, there is a "Back" button, which, when clicked, returns to the start page.
5. Verifying the presence of entered user names in the Windows user list is prohibited by comparing the current user from which the server process is running and entered user. It is also forbidden to use names of home directories of users in C:\Users directory.

You can get the list of users for checking from Computer Management->Services->Local Users and Groups->Users.

6. At the root of the project create README.MD file with the text of test task in UTF-8 coding

7. The project should be uploaded to github.com (other resources are not considered).

8. Send the link to your project to job@eurekabpo.ru.

9. The deadline for the task is 7 days, starting tomorrow.
