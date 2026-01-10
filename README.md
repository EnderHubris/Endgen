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

### Visuals
The inspector mode generates a row of position (x,y,z) modifiers where the user can change the position during run-time:
<img width="422" height="329" alt="colorful cubes" src="https://github.com/user-attachments/assets/1da01c82-a777-4459-9742-2cefc52ab39a" />
<img width="854" height="346" alt="inspector mode" src="https://github.com/user-attachments/assets/aea16f55-fb57-42c4-a4cb-24971076fac7" />
