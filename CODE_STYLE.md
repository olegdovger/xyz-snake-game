# Code Style Guidelines

This project uses consistent code formatting and style guidelines.

## Formatting Tools

### Clang-Format
- **Configuration**: `.clang-format` (Google style with customizations)
- **Indentation**: 2 spaces
- **Line length**: 100 characters
- **Standard**: C++17

### EditorConfig
- **Configuration**: `.editorconfig`
- **Applies to**: All file types
- **Settings**: UTF-8, LF line endings, trim whitespace

## VS Code Settings
- **Configuration**: `.vscode/settings.json`
- **Auto-format**: On save and paste
- **Clang-format**: Uses project `.clang-format` file

## Formatting Commands

### Using CMake
```bash
# Format all source files
cmake --build . --target format

# Or with make
make format
```

### Using Scripts
```bash
# Linux/Mac
./scripts/format_code.sh

# Windows
scripts/format_code.bat
```

### Manual Formatting
```bash
# Format specific file
clang-format -i src/Game.cpp

# Format all C++ files
find src -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

## Style Guidelines

### Naming Conventions
- **Classes**: PascalCase (`Game`, `MainMenu`)
- **Functions/Variables**: camelCase (`processEvents`, `currentScreen`)
- **Constants**: UPPER_CASE (`MAX_SCORE`)
- **Files**: snake_case (`main_menu.cpp`)

### Code Structure
- **Indentation**: 2 spaces (no tabs)
- **Line length**: Max 100 characters
- **Braces**: Attach style (`if (condition) {`)
- **Pointers**: Left alignment (`int* ptr`)
- **References**: Left alignment (`int& ref`)

### Includes
- **Order**: System headers first, then project headers
- **Format**: `#include <system>` then `#include "project"`
- **Grouping**: Blank line between groups

### Comments
- **Style**: `//` for single line, `/* */` for multi-line
- **Doxygen**: Use `///` for documentation comments
- **Trailing**: Align to 100 characters

### Error Handling
- **Critical Resources**: Use `assert()` to check critical resources (fonts, textures, etc.)
- **Examples**: `assert(font.loadFromFile("path"));` for essential game assets
- **Purpose**: Fail fast during development if critical resources are missing

## Pre-commit Hooks
Consider setting up pre-commit hooks to automatically format code before commits:

```bash
# Install pre-commit
pip install pre-commit

# Install hooks
pre-commit install
```

## IDE Integration
- **VS Code**: Uses project settings automatically
- **CLion**: Import `.clang-format` in Code Style settings
- **Vim**: Use `vim-clang-format` plugin
- **Emacs**: Use `clang-format` package
