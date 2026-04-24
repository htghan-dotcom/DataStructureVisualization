# Build & Setup Guide - Hash Chaining Visualizer

## Prerequisites

Bạn cần có:
- **Visual Studio 2022** hoặc GCC/MinGW
- **CMake** 3.10+
- **SFML** 2.6+

## Installation Steps

### 1. Install SFML

#### Cách 1A: Windows (MSVC - Visual Studio)

```bash
# Tải SFML từ: https://www.sfml-dev.org/download/sfml/2.6.1/
# Chọn phiên bản Visual Studio tương ứng

# Giải nén vào một folder, ví dụ: C:\SFML-2.6.1
```

#### Cách 1B: Windows (MSYS2 - Recommended)

```bash
# Nếu bạn dùng MSYS2 ucrt64:
pacman -S mingw-w64-ucrt-x86_64-sfml

# SFML sẽ được cài ở: C:/msys64/ucrt64/
```

#### Cách 1C: Linux/macOS

```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev

# Fedora
sudo dnf install SFML-devel

# macOS (Homebrew)
brew install sfml
```

### 2. Configure CMake

#### Option A: Visual Studio 2022 (Windows)

Nếu bạn cài SFML từ package tải xuống:

```powershell
# Mở PowerShell ở folder project
cd DataStructureVisualization

# Configure
cmake -B build -G "Visual Studio 17 2022" `
    -DSFML_DIR="C:\SFML-2.6.1\lib\cmake\SFML"

# Build
cmake --build build --config Release

# Run
.\build\Release\HashVisualizer.exe
```

Nếu bạn dùng MSYS2:

```bash
# SFML tự động được tìm thấy
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\HashVisualizer.exe
```

#### Option B: MinGW Makefiles (MSYS2)

```bash
# Từ MSYS2 terminal (ucrt64)
cd DataStructureVisualization
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/HashVisualizer.exe
```

#### Option C: Linux/macOS

```bash
cd DataStructureVisualization
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/HashVisualizer
```

### 3. Troubleshooting

#### SFML Not Found

**Lỗi:**
```
CMake Error: Could not find a package configuration file provided by "SFML"
```

**Giải pháp:**

1. Verify SFML installation:
```bash
# Windows (MSVC)
ls "C:\SFML-2.6.1\lib\cmake\SFML"  # Phải có SFMLConfig.cmake

# MSYS2
ls /ucrt64/lib/cmake/SFML
```

2. Update CMakeLists.txt với đúng path:
```cmake
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
```

3. Hoặc set environment variable:
```powershell
# PowerShell
$env:SFML_DIR = "C:\SFML-2.6.1\lib\cmake\SFML"
cmake -B build -G "Visual Studio 17 2022"
```

#### Compiler Not Found

**Lỗi:**
```
CMAKE_CXX_COMPILER is not set
```

**Giải pháp:**

1. Cài Visual Studio 2022 hoặc MinGW
2. Verify compiler in PATH:
```bash
cl.exe /version      # For MSVC
g++ --version        # For GCC
```

3. Chọn đúng generator:
```bash
cmake --help  # List all available generators
```

## Build từ VS Code

### Setup (First Time)

1. Mở folder trong VS Code
2. Install extensions:
   - **C/C++ Extension Pack** (Microsoft)
   - **CMake Tools** (Microsoft)

3. Chọn Kit (Compiler):
   - Ctrl+Shift+P → `CMake: Select a Kit`
   - Chọn "Visual Studio Community 2022" hoặc GCC

4. Chọn Build Type:
   - Ctrl+Shift+P → `CMake: Select Variant`
   - Chọn "Release"

### Build

- **Ctrl+Shift+B** → Build
- **Ctrl+F5** → Debug & Run
- **Ctrl+Shift+P** → "CMake: Build"

Hoặc dùng Tasks:

```json
// Trong .vscode/tasks.json
{
    "label": "Build & Run HashVisualizer",
    "type": "shell",
    "command": "cmake",
    "args": ["--build", "build", "--config", "Release"],
    "problemMatcher": ["$msCompile"]
}
```

## Project Structure

```
DataStructureVisualization/
├── CMakeLists.txt                 # Build configuration
├── src/
│   ├── main.cpp                   # MST Visualizer
│   ├── main_hash.cpp              # Hash Chaining Visualizer
│   ├── HashChaining/
│   │   └── HashChaining.cpp       # Hash table logic
│   ├── HashVisualizer/
│   │   ├── HashVisualizer.h
│   │   └── HashVisualizer.cpp     # Glassmorphism UI
│   └── ... (other components)
├── include/
│   └── HashChaining/
│       └── HashChaining.h
├── assets/
│   └── fonts/                     # Font files
├── build/                         # Generated files (after build)
└── .vscode/
    ├── tasks.json                 # Build tasks
    ├── launch.json                # Debugger config
    └── c_cpp_properties.json      # IntelliSense config
```

## Running Executables

### From Terminal

```bash
# Windows
build\Release\HashVisualizer.exe

# Linux/macOS
./build/HashVisualizer
```

### From VS Code

- Press **Ctrl+F5** to run with debugger
- Or create custom task in tasks.json

## Common Errors & Fixes

| Error | Cause | Fix |
|-------|-------|-----|
| `SFML not found` | SFML not installed | Install SFML or set SFML_DIR |
| `Can't find compiler` | GCC/MSVC not in PATH | Install compiler or add to PATH |
| `Window doesn't open` | Video driver issue | Update GPU drivers |
| `Font not found` | Missing assets folder | Ensure `assets/fonts/` exists |
| `Link error (SFML)` | Wrong SFML libs | Rebuild SFML or reinstall |

## Performance Notes

- **Release Build** recommended (faster)
- Animation runs at 60 FPS
- Smooth gradients use SFML vertex arrays

## Next Steps

1. **Build successfully** ✓
2. **Run HashVisualizer.exe** and test features
3. **Explore** the code in `src/HashVisualizer/`
4. **Customize** colors, fonts, or animations

---

**Need Help?**
- Check HASH_CHAINING_README.md for usage guide
- Read comments in HashVisualizer.cpp for code details
- Consult SFML documentation: https://www.sfml-dev.org/documentation/2.6.1/

**Last Updated:** 2026-04-21
