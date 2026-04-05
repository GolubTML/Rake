<img width="256" height="116" alt="Rake_logo" src="https://github.com/user-attachments/assets/df543b78-19d5-4064-9483-76e031dc2b2d" />

# Rake game engine and game itself. 

**Written with C++ and OpenGL.**

## Before compiling, we need to download some libraries

If you use **Arch** based system, use this command:
```
sudo pacman -S clang glfw-x11 assimp freetype2 mesa
```

**Ubuntu/Debian**:
```
sudo apt update
sudo apt install build-essential clang libglfw3-dev libassimp-dev libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev xorg-dev
```

## Compiling

Here are _make_ commands:
```
make run
```
Will compile and run Rake without buulding folder "build"

```
make build
```

Will create new folder "build" with all necessary folders and files

```
make clear
```
Delete folder "build". It automatically run with ``make build`` 
