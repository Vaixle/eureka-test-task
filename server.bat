@echo off

echo Installing npm packages...
call npm install

echo Opening the default browser...
explorer "http://localhost:3000/"

echo Starting the application...
node .\bin\www
