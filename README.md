# Pacman-OpenGL

A classic Pacman clone written in **C** using **OpenGL** (GLUT/FreeGLUT). This project features a fully playable game with custom map loading, collision detection, and ghost movement logic.

## Features

* **OpenGL Rendering:** Custom graphics for the maze, player, and ghosts.
* **Map System:** Levels are loaded dynamically from text files (e.g., `Map0.txt`).
* **Game Logic:** Complete movement, collision, and scoring systems.
* **Visual Studio Support:** Includes a pre-configured `.sln` solution.

## Project Structure

* `main.c` - Entry point and game loop.
* `pacman.c/h` - Player movement and logic.
* `ghost.c/h` - Enemy movement and behavior.
* `maps.c/h` - Map loading and parsing.
* `wall.c/h` - Maze rendering.
* `utils.c/h` - Utility functions.
* `Map*.txt` - Level layout files.

## How to Run

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/joaomartinscode/Pacman-OpenGL.git](https://github.com/joaomartinscode/Pacman-OpenGL.git)
    ```
2.  **Open in Visual Studio:**
    * Launch the `pacman.sln` file.
3.  **Install Dependencies:**
    * Ensure **FreeGLUT** is installed (easily done via NuGet packages in Visual Studio or by linking the libraries manually).
4.  **Build:**
    * Press **F5** to compile and run.
