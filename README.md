# sdl_game

[Build Status](https://hub.docker.com/r/andrejlevkovitch/sdl_game/builds/)

Build on Linux and by mingw (check on Linux!).
For byild you have to have installing sdl2.

For Linux you can simply install it by

- sudo apt install libsdl2-dev

For mingw you can download repository:
[sdl2-mingw](https://github.com/tonymagro/sdl2-mingw). All what you have to
do after: in file Makefile change variable like here

- CROSS_PATH := /usr

and run it as

- sudo make cross


Build on Linux:

- mkdir build

- cd build

- cmake ..

- cmake --build


Build by mingw (on Linux):

- mkdir build

- cd build

- cmake -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ ..

- cmake --build .
