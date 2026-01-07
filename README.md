# Endgen

### About
This is a small project where I experiment with creating a navigatable scene using C++ and the SDL library.

### Dependencies
This project uses CMake, C++, and SDL
```bash
sudo apt install cmake g++ libsdl2-dev libsdl2-ttf-dev
```

### Build
```bash
git clone https://github.com/EnderHubris/Endgen.git
cd Endgen
mkdir build; cd build
cmake ../ && make endgen

# run the scene normally
./bin/endgen

# run the scene with inspector window
# to modify scene objects at runtime
./bin/endgen inspect
```