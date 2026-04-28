# Data Structure Visualization

<p align="center">
  <img src="https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++ 17">
  <img src="https://img.shields.io/badge/SFML-3.0.2-green?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="SFML 3.0.2">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge" alt="Multi-Platform">
  <img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge" alt="Completed">
</p>

> An interactive, graphical application built with **C++17** and **SFML 3.0.2** to visualize core data structures and algorithms through step-by-step animations.

**Course:** CS163 - Data Structures | **University:** Ho Chi Minh City University of Science (HCMUS)

---

## 👥 Contributors - Group 01 - Class 25A02

| Name               | Student ID | Username      | Role                                    |
| ------------------ | ---------- | ------------- | --------------------------------------- |
| Đặng Minh Nhật     | 25125046   | mnhat249      | Team Leader, MST logic & UI             |
| Huỳnh Trần Gia Hân | 25125042   | htghan-dotcom | Hash logic & UI, AppLayout helper       |
| Trần Hồng Vy       | 25125049   | thvy2507      | RBT logic & UI, AppLayout main designer |
| Lê Nguyễn Hải Đăng | 25125081   | taiyou2007    | DLL logic & UI                          |

---

## 📖 Table of Contents

- [Project Overview](#-project-overview)
- [Main Features](#-main-features)
- [Supported Data Structures](#-supported-data-structures)
- [Technical Constraints](#-technical-constraints--implementation)
- [Installation & Compilation](#-compilation--execution)
- [How to Use](#-how-to-use-instructions)
- [Keyboard Shortcuts](#-keyboard-shortcuts)
- [File Structure](#-file-structure)
- [Limitations](#-limitations)

---

## 🎯 Project Overview

**Data Structure Visualization** is an interactive, graphical application built with **C++17** and **SFML 3.0.2**, developed as a group project for the course **CS163 - Data Structures** at **Ho Chi Minh City University of Science (HCMUS)**.

The program helps users visualize and understand how core data structures and algorithms operate under the hood through step-by-step animations. The application heavily utilizes a custom **State Snapshot System**, ensuring that users can trace back their steps safely without breaking the data integrity.

---

## ✨ Main Features

### Supported Data Structures

| No   | Structure                 | Operations                                         |
| ---- | ------------------------- | -------------------------------------------------- |
| 01   | **Doubly Linked List**    | Init, Insert, Delete, Search, Update               |
| 02   | **Hash Table (Chaining)** | Init, Insert, Delete, Search, Update               |
| 03   | **Red-Black Tree**        | Init, Insert, Delete, Search, Auto-balance, Update |
| 04   | **Graph (MST)**           | Build Graph, Prim, Kruskal                         |

### Interactive UI/UX Engine

- **Theme Manager:** Supports Light & Dark modes with 4 distinct color palettes (Default, Pinky, Navy, Golden)
- **Expand-on-click Sidebar:** Inline input fields for clean and distraction-free interaction
- **Randomizer (Dice):** Quickly fill inputs with random values for rapid testing

### Animation & Step-by-Step Execution

- **Playback Controls:** Play, pause, step forward/backward, and skip to start/end
- **Adjustable Speed:** Dynamic slider to speed up or slow down animations in real-time
- **Code Tracing:** Real-time pseudocode highlighting and descriptive text for each algorithmic step

### State Persistence & Undo System

- Every action triggers a state capture
- Users can instantly revert mistakes or step back through operation history using built-in Undo functionality

---

## 🛠️ Technical Constraints & Implementation

| Category             | Specification                                                |
| -------------------- | ------------------------------------------------------------ |
| **Language**         | C++17                                                        |
| **Graphics Library** | SFML 3.0.2                                                   |
| **Build System**     | CMake (3.16+)                                                |
| **Architecture**     | OOP with custom Snapshot-based history tracking              |
| **Interface**        | Fully graphical GUI from scratch using SFML shapes and textures |

---

## 🚀 Compilation & Execution

### Prerequisites

- C++17 compiler
- [SFML 3.0.2](https://www.sfml-dev.org/download.php)
- CMake 3.16+

### Build Instructions

```bash
# 1. Clone the repository
git clone https://github.com/htghan-dotcom/DataStructureVisualization.git
cd DataStructureVisualization

# 2. Configure and build
cmake -S . -B build
cmake --build build
```

> **Note:** By default, CMake looks for SFML at `C:/SFML/SFML-3.0.2/`. If yours is elsewhere, set the path manually:
>
> bash
>
> ```bash
> cmake -S . -B build -DSFML_DIR="C:/your/path/to/SFML/lib/cmake/SFML"
> ```

### Running the Application

The executable will be located in the `build/` directory. The `assets/` folder is automatically copied next to it.

bash

```bash
# On Windows
./build/DataStructureVisualization.exe

# On Linux/macOS
./build/DataStructureVisualization
```



------

## 🎮 How to Use (Instructions)

### Mouse Interactions

1. Launch the application and select a Data Structure from the Home Menu
2. Click **New** to generate a random structure or load data from a `.txt` file
3. Use the **Sidebar** to perform operations (Insert, Delete, Search, Update)
4. Use the bottom **Playback Bar** to play, pause, or skip through the animation steps

------

## ⌨️ Keyboard Shortcuts

### Global Shortcuts (DLL, Hash, RBT)

| Key         | Action                                                       |
| :---------- | :----------------------------------------------------------- |
| `I`         | Open Insert input                                            |
| `D`         | Open Delete input                                            |
| `S`         | Open Search input                                            |
| `U`         | Open Update input                                            |
| `N`         | Open New / Init menu                                         |
| `R`         | Random value (when input open) / Random new table (when idle) |
| `Backspace` | Delete last digit in input                                   |
| `Enter`     | Confirm current operation                                    |
| `Tab`       | Switch between Old / New field (Update only)                 |
| `Space`     | Play / Pause animation                                       |
| `←` `→`     | Step backward / forward                                      |
| `Ctrl + Z`  | Undo last operation                                          |

### Graph — MST Shortcuts

| Key      | Action                     |
| :------- | :------------------------- |
| `N`      | New graph                  |
| `B`      | Build graph                |
| `R`      | Random graph               |
| `A`      | Add node                   |
| `E`      | Add edge (edge mode)       |
| `D`      | Remove node                |
| `K`      | Run Kruskal's algorithm    |
| `P`      | Run Prim's algorithm       |
| `M`      | Toggle Graph / Matrix view |
| `Space`  | Play / Pause animation     |
| `←` `→`  | Step backward / forward    |
| `Escape` | Back to menu               |

------

## 📁 File Structure

```tex
DataStructureVisualization/
├── assets/
│   ├── audio/
│   ├── fonts/
│   └── images/
├── include/
│   ├── DoublyLinkedList/
│   ├── HashChaining/
│   ├── RedBlackTree/
│   ├── MinimumSpanningTree/
│   ├── AppLayout.h
│   ├── Common.h
│   ├── GUI.h
│   ├── Menu.h
│   ├── SettingsScreen.h
│   ├── ThemeManager.h
│   └── WelcomeScreen.h
├── src/
│   ├── DoublyLinkedList/
│   ├── HashChaining/
│   ├── RedBlackTree/
│   ├── MinimumSpanningTree/
│   ├── AppLayout.cpp
│   ├── Menu.cpp
│   ├── SettingsScreen.cpp
│   ├── ThemeManager.cpp
│   ├── WelcomeScreen.cpp
│   └── main.cpp
├── vendor/
│   └── tinyfiledialogs.c
└── CMakeLists.tx
```

------

## ⚠️ Limitations

- The application runs purely on Desktop (Windows/Linux/macOS) and is not optimized for mobile platforms
- Requires exactly **SFML 3.0.2** and **C++17** to compile correctly due to the usage of `[[nodiscard]]`attributes and modern C++ features

------

## 🙏 Acknowledgments

- **Course Instructors:** Dinh Ba Tien, Truong Phuoc Loc, Ho Tuan Thanh
- **Course:** CS163 - Data Structures | FIT HCMUS, VNU-HCM
- **Tools:** Visual Studio Code, Xcode, Git, CMake, SFML

------

*Project completed April 2026*
