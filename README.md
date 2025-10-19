# Snake Game

A classic Snake game implementation with modern C++ and SFML.

## Features

- Multiple snake types with different colors and effects
- Various game items (apples, water bubbles, phantom apples) with unique effects
- Difficulty levels
- High score tracking
- Sound effects and music
- Settings persistence

## Requirements

- CMake 3.31 or higher
- C++20 compatible compiler (GCC, Clang, or MSVC)
- SFML 3.0.1 (automatically downloaded by CMake)
- nlohmann JSON 3.11.2 (automatically downloaded by CMake)

## Configuration

### Windows (MSYS2/MinGW)

1. Install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)
2. Install required packages:
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-make
   ```

### Linux (Ubuntu/Debian)

1. Install required packages:
   ```bash
   sudo apt update
   sudo apt install build-essential cmake libsfml-dev
   ```

### macOS

1. Install Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```
2. Install Homebrew from [https://brew.sh/](https://brew.sh/)
3. Install required packages:
   ```bash
   brew install cmake sfml
   ```

## Building

### Clone the Repository

```bash
git clone <repository-url>
cd snaike-game
```

### Build Steps

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Configure with CMake:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```
   Or on Unix-like systems:
   ```bash
   make
   ```

### Build Configurations

For a release build (optimized):
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

For a debug build (with debugging symbols):
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Running

After building, the executable will be located in the `build/bin/` directory.

### Windows

```bash
cd build/bin
./game.exe
```

### Linux/macOS

```bash
cd build/bin
./game
```

## Game Controls

- Arrow Keys: Move the snake
- ESC: Pause/Resume game
- P: Pause/Resume game (alternative)

## Settings

The game settings are stored in `settings.json` in the same directory as the executable. The settings file is automatically created with default values on first run.

### Settings File Location

- Windows: `build/bin/settings.json`
- Linux/macOS: `build/bin/settings.json`

### Available Settings

- `snakeSpeed`: Initial speed of the snake (1-10)
- `snakeType`: Type/color of the snake (purple, green, blue, red, black)
- `snakeSize`: Initial size of the snake
- `gameDifficultyLevel`: Difficulty level (easy, harderTheEasy, middle, harderThenMiddle, hard)
- `gameMusic`: Enable/disable background music (true/false)
- `gameSound`: Enable/disable sound effects (true/false)
- `gameCountdownEnabled`: Enable/disable countdown at game start (true/false)
- `gameCountdownInSeconds`: Countdown duration in seconds
- `gameCountdownSound`: Enable/disable countdown sound (true/false)
- `gameRecordTable`: Array of high scores

## Project Structure

```
snaike-game/
├── src/                    # Source files
│   ├── screens/           # Game screens
│   └── utils/             # Utility classes
├── resources/             # Game assets (images, sounds, fonts)
├── include/               # Header files
├── build/                 # Build output directory
└── CMakeLists.txt         # CMake configuration
```

## Packaging

To create a distributable package:

```bash
cd build
cpack
```

This will create a ZIP file containing the executable and all necessary resources.

## Troubleshooting

### Build Issues

1. **CMake version error**: Make sure you have CMake 3.31 or higher
2. **Compiler not found**: Ensure you have a C++20 compatible compiler installed
3. **SFML linking issues**: On Linux, make sure you have the SFML development libraries installed

### Runtime Issues

1. **Missing resources**: Ensure the `resources/` directory is copied to the same location as the executable
2. **Settings not saving**: Check that the application has write permissions to its directory

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License.