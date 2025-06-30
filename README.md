# Terminal Pong

Pong game implemented in C, rendered directly in the terminal.

## Prerequisites

To build and run this project, you will need:

- A C compiler (e.g., GCC, MinGW for Windows)
- CMake (version 3.10 or higher)

## Building the Project

Follow these steps to build the project:

1.  Navigate to the project root directory in your terminal:

2.  Create a `build` directory and generate the build files using CMake. This command will also compile the project:

    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

    *Note: On Windows, CMake might default to Visual Studio generators. If you prefer MinGW/NMake, you might need to specify the generator, e.g., `cmake .. -G "MinGW Makefiles"`.*

## Running the Game

After a successful build, you can run the game from the `build` directory:

```bash
.\Debug\pong.exe
```

(On Unix-like systems, it would typically be `./pong` or `./Debug/pong` depending on the build type and generator.)


