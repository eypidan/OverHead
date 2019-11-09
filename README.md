## OverHead

- An opengl program to simulate the view of a plane taking off.

### Install & Run

- Make sure your PC's opengl version >= 4.5.
  -  WINDOWS:[OpenGL Extension Viewer](http://download.cnet.com/OpenGL-Extensions-Viewer/3000-18487_4-34442.html) .
  -  Linux: call  **glxinfo**.

- Download GLFW and install it to your environment.

  - For me in windows: (I use MinGW and cmake to compile).

  - Ensure your mingw/bin directory is in your global environment .

  - ```shell
    cd /path/to/GLFW
    cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX="C:\mingw64\x86_64-w64-mingw32" -DCMAKE_CXX_COMPILER:FILEPATH=C:/mingw64/bin/g++.exe -DCMAKE_C_COMPILER:FILEPATH=C:/mingw64/bin/gcc.exe
    mingw32-make all
    mingw32-make install
    ```

  - Use `mingw32-make clean` to eliminate your build about glfw.

- **glad** is a web service, please go this [web service](http://glad.dav1d.de/) to download opengl in 4.5 version.

- **glm** is a  header third-party library about c++.
- use [assimp](https://github.com/assimp/assimp) to import model file formats. (assimp VERSION: [ v5.0.0](https://github.com/assimp/assimp/tree/v5.0.0))

- 

### Feature
- Implement SubWindow by framebuffer 