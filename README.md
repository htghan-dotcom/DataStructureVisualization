# Data Structure Visualization (C++ SFML Application)

## 1. Project Overview

**Data Structure Visualization** is an interactive, graphical application built with **C++17** and **SFML 3.0.2**, developed as a group project for the course **CS163 - Data Structures** at **Ho Chi Minh City University of Science (HCMUS)**.

The program helps users visualize and understand how core data structures and algorithms operate under the hood through step-by-step animations. The application heavily utilizes a custom **State Snapshot System**, ensuring that users can trace back their steps safely without breaking the data integrity.

--- 
## 2. Main Features

### 2.1 Supported Data Structures
| No | Structure | Operations |
|---|-----------|------------|
| 01 | **Doubly Linked List** | Init, Insert, Delete, Search, Update |
| 02 | **Hash Table (Chaining)** | Init, Insert, Delete, Search, Update |
| 03 | **Red-Black Tree** | Init, Insert, Delete, Search, Auto-balance, Update |
| 04 | **Graph (MST)** | Build Graph, Prim, Kruskal |

### 2.2 Interactive UI/UX Engine
* **Theme Manager:** Supports Light & Dark modes with 4 distinct color palettes (Default, Pinky, Navy, Golden).
* **Expand-on-click Sidebar:** Inline input fields for clean and distraction-free interaction.
* **Randomizer (Dice):** Quickly fill inputs with random values for rapid testing.

### 2.3 Animation & Step-by-Step Execution
* **Playback Controls:** Play, pause, step forward/backward, and skip to start/end.
* **Adjustable Speed:** A dynamic slider to speed up or slow down animations in real-time.
* **Code Tracing:** Real-time Pseudocode highlighting and descriptive text for each algorithmic step.

### 2.4 State Persistence & Undo System
* Every action triggers a state capture.
* Users can instantly revert mistakes (like accidentally clearing the table) or step back through their operation history using the built-in Undo functionality.

---

## 3. Technical Constraints & Implementation

* **Language:** C++17
* **Graphics Library:** SFML 3.0.2 (Simple and Fast Multimedia Library)
* **Build System:** CMake (3.16+)
* **Architecture:** Object-Oriented Programming (OOP) with custom Snapshot-based history tracking.
* **Interface:** Fully graphical GUI constructed from scratch using SFML shapes and textures.

---

## 4. Compilation & Execution
### 4.1 Compilation
#### Prerequisites
- C++17 compiler (MinGW-w64 recommended on Windows)
- [SFML 3.0.2](https://www.sfml-dev.org/download.php)
- CMake 3.16+

```bash
git clone https://github.com/htghan-dotcom/DataStructureVisualization.git
cd DataStructureVisualization

cmake -S . -B build
cmake --build build
```

*Note: By default, CMake looks for SFML at `C:/SFML/SFML-3.0.2/`. If yours is elsewhere, set the path manually during the config step:*
```bash
cmake -S . -B build -DSFML_DIR="C:/your/path/to/SFML/lib/cmake/SFML"
```

### 4.2 Running the Application
The executable will be located in the `build/` directory. The `assets/` folder is automatically copied next to it.
* On Windows:
```bash
./build/DataStructureVisualization.exe
```

---

## 5. How to Use (Instructions)

### 5.1 Mouse Interactions
1. Launch the application and select a Data Structure from the Home Menu.
2. Click **New** to generate a random structure or load data from a `.txt` file.
3. Use the **Sidebar** to perform operations (Insert, Delete, Search, Update).
4. Use the bottom **Playback Bar** to play, pause, or skip through the animation steps.

### 5.2 Keyboard Shortcuts
**Global Shortcuts (DLL, Hash, RBT):**
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
| `←` `→` | Step backward / forward |
| `Ctrl + Z` | Undo last operation |

**Graph — MST Shortcuts:**
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

## 6. File Structure

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

## 7. Limitations
* The application runs purely on Desktop (Windows/Linux/macOS) and is not optimized for mobile platforms.
* Requires exactly **SFML 3.0.2** and **C++17** to compile correctly due to the usage of `[[nodiscard]]` attributes and modern C++ features.

---

## 8. Developers

Developed by **Group 01 - Class 25A02** (HCMUS)

| Name | Student ID | Role |
|------|-----------|------|
| Đặng Minh Nhật | 25125046 | Team Leader, MST logic & UI |
| Huỳnh Trần Gia Hân | 25125042 | Hash logic & UI, AppLayout helper |
| Trần Hồng Vy | 25125049 | RBT logic & UI, AppLayout main designer |
| Lê Nguyễn Hải Đăng | 25125081 | DLL logic & UI |

---
