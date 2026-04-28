# Data Structure Visualization

An interactive data structure visualizer built with **C++17** and **SFML 3.0.2**, developed as a group project for the course **CS163 - Data Structures** at **Ho Chi Minh City University of Science (HCMUS)**.

---

## Features

### Data Structures
| No | Structure | Operations |
|---|-----------|------------|
| 01 | **Doubly Linked List** | Init, Insert, Delete, Search, Update |
| 02 | **Hash Table (Chaining)** | Init, Insert, Delete, Search, Update |
| 03 | **Red-Black Tree** | Init, Insert, Delete, Search, Auto-balance, Update |
| 04 | **Graph (MST)** | Build Graph, Prim, Kruskal |

### UI/UX
- **Theme Engine** — Light & Dark mode with 4 palettes: Default (Green), Pinky, Navy, Golden
- **Step-by-step animation** — play, pause, step forward/backward, skip to start/end, adjustable speed
- **Snapshot-based rendering** — each animation step stores a full state snapshot, so skip-back correctly restores deleted nodes
- **Expand-on-click sidebar** — inline input with dice (random) button for each operation

---

## Keyboard Shortcuts

### Data Structures (Doubly Linked List, Hash Table, Red-Black Tree)

| Key | Action |
|-----|--------|
| `I` | Open Insert input |
| `D` | Open Delete input |
| `S` | Open Search input |
| `U` | Open Update input |
| `N` | Open New / Init menu |
| `R` | Random value (when input is open) / Random new table (when idle) |
| `Backspace` | Delete last digit in input |
| `Enter` | Confirm current operation |
| `Tab` | Switch between Old / New field (Update only) |
| `Space` | Play / Pause animation |
| `Ctrl + Z` | Undo last operation |

### Graph — MST

| Key | Action |
|-----|--------|
| `N` | New graph |
| `B` | Build graph |
| `R` | Random graph |
| `A` | Add node |
| `E` | Add edge (edge mode) |
| `D` | Remove node |
| `K` | Run Kruskal's algorithm |
| `P` | Run Prim's algorithm |
| `M` | Toggle Graph / Matrix view |
| `Space` | Play / Pause animation |
| `←` `→` | Step backward / forward |
| `Escape` | Back to menu |
---
## Build

### Prerequisites
- C++17 compiler (MinGW-w64 recommended on Windows)
- [SFML 3.0.2](https://www.sfml-dev.org/download.php)
- CMake 3.16+

### Steps

```bash
git clone https://github.com/htghan-dotcom/DataStructureVisualization.git
cd DataStructureVisualization

cmake -S . -B build
cmake --build build
```

The executable will be at `build/DataStructureVisualization.exe`. The `assets/` folder is automatically copied next to it by CMake.

### SFML path

By default CMake looks for SFML at `C:/SFML/SFML-3.0.2/`. If yours is elsewhere, set the path manually:

```bash
cmake -S . -B build -DSFML_DIR="C:/your/path/to/SFML/lib/cmake/SFML"
```

---

## Project Structure

```
DataStructureVisualization/
├── assets/
│   ├── audio/
│   ├── fonts/
│   └── images/
├── include/
|   ├── DoublyLinkedList/
│   ├── HashChaining/
│   ├── RedBlackTree/
|   ├── MinimumSpanningTree/
│   ├── AppLayout.h
|   ├── Common.h
│   ├── GUI.h
│   ├── Menu.h
│   ├── SettingsScreen.h
│   ├── ThemeManager.h
│   └── WelcomeScreen.h
├── src/
|   ├── DoublyLinkedList/
│   ├── HashChaining/
│   ├── RedBlackTree/
|   ├── MinimumSpanningTree/
│   ├── AppLayout.cpp
│   ├── Menu.cpp
│   ├── SettingsScreen.cpp
│   ├── ThemeManager.cpp
│   ├── WelcomeScreen.cpp
│   └── main.cpp
├── vendor/
│   └── tinyfiledialogs.c
└── CMakeLists.txt
```

---

## Contributors

Class **25A02** — HCMUS

| Name | Student ID | Role |
|------|-----------|------|
| Đặng Minh Nhật | 25125046 | Team Leader, MST logic & UI |
| Huỳnh Trần Gia Hân | 25125042 | Hash logic & UI, AppLayout helper |
| Trần Hồng Vy | 25125049 | RBT logic & UI, AppLayout main designer |
| Lê Nguyễn Hải Đăng | 25125081 | DLL logic & UI |

---
