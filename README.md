# Maze Generator & Solver

### Description:

<dl>
    <dd>
      This is a C/C++ application that implements the <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Wilson's_algorithm">Wilson's Algorithm</a> to generate a maze, 
      and the <a href="https://en.wikipedia.org/wiki/A*_search_algorithm">A* Algorithm</a> to solve the maze. 
      There is also an optional gradient from red to green to visualize which cells were visited during the process of solving.</dd>
</dl>


### Preview:

<details open>
  <summary>Show</summary>
  <img width="504px" height="504px" src="https://github.com/user-attachments/assets/fcd59b15-6427-43b0-ad1e-75cc7c2aa7df" alt="image">

  <br>
</details>

### Tooling:
- [x] C/C++ Language 
- [x] SDL3
- [x] ImGUI 
- [x] Vcpkg
- [x] CMake

---
### Developer Notes:

```bash

# CONFIGURE
cmake -S. -Bbuild --preset windows

# BUILD
cmake --build build

```
