cmake_minimum_required(VERSION 3.27)
project(Practica_4_Examen_)

set(CMAKE_CXX_STANDARD 17)

add_executable(Practica_4_Examen_ main.cpp
        Character/Character.cpp
        Character/Character.h
        Player/Player.cpp
        Player/Player.h
        Enemy/Enemy.cpp
        Enemy/Enemy.h
        Utils.h
        Combat/Combat.cpp
        Combat/Combat.h
        Combat/Action.cpp
        Combat/Action.h
        Character/Character.cpp)
