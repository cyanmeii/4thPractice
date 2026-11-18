@echo off
chcp 65001 >nul
g++ -std=c++17 -Wall -Wextra Resource.cpp Queue.cpp Market.cpp Producer.cpp AnimalHouse.cpp WheatField.cpp CowShed.cpp ChickenCoop.cpp Farm.cpp Game.cpp main.cpp -o farm.exe && farm.exe
pause
