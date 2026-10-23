# Engine For Friends (EFF)

Эта библиотека является оберткой на [raylib](https://github.com/raysan5/raylib) и нацелена на быструю разработку маленьких игр без траты времени на базовые вещи по типу менеджера сцен, ресурсов и прочей фигни.

## Использование
```CMake
include(cmake/CPM.cmake)

CPMAddPackage("gh:SFF-Developers-Team/EngineForFriends")

add_executable(MyGame main.cpp)

target_link_libraries(MyGame PRIVATE EFF::EFF)
```

## Примеры
Смотри в [examples](./examples).
