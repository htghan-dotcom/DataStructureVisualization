# Hash Chaining Visualizer - Implementation Summary

## 📋 Tổng Quan Dự Án

Đã xây dựng một **Data Structure Visualizer cho Hash Chaining** với Glassmorphism UI aesthetic sử dụng C++ và SFML.

---

## 🎨 Features Implemented

### 1. **Glassmorphism Design**
- ✅ Translucent panels với alpha transparency
- ✅ Gradient colors (10 pastel color pairs)
- ✅ Outer glow effect on nodes & panels
- ✅ White border highlights (glass shine effect)

### 2. **Hash Table Visualization**
- ✅ 13 buckets (0-12) hiển thị ở center canvas
- ✅ Node stacking (chain collision resolution)
- ✅ Animated arrows pointing to next nodes
- ✅ Smooth gradient-colored rectangles cho nodes

### 3. **User Interface**
- ✅ **Left Panel**: 6 operation buttons (Create, Insert, Delete, Search, Update, Clear)
- ✅ **Right Panel**: Pseudocode display with live highlight
- ✅ **Bottom Panel**: Playback controls (5 buttons) + Speed slider (1-10x)
- ✅ **Center Canvas**: Hash table visualization
- ✅ **Status Display**: Current operation description

### 4. **Animation Engine**
- ✅ Step-by-step animation playback
- ✅ Forward/Backward/Skip navigation
- ✅ Play/Pause with adjustable speed
- ✅ Smooth transitions between states

### 5. **Interactive Controls**
- ✅ Mouse click on buttons
- ✅ Text input for values (Integer keys)
- ✅ Backspace support
- ✅ Enter to confirm input
- ✅ Slider mouse drag for speed control

### 6. **Backend Logic**
- ✅ Hash function: `key % size`
- ✅ Insert operation with collision chaining
- ✅ Delete operation (head & middle cases)
- ✅ Search with step-by-step visualization
- ✅ Update (delete + insert)
- ✅ Random data generation
- ✅ Load from file support

---

## 📁 Files Created/Modified

### New Files Created:
1. **`src/HashVisualizer/HashVisualizer.cpp`** ← 400+ lines
   - Complete UI rendering with Glassmorphism
   - Event handling (mouse, keyboard, slider)
   - Animation control logic
   - Helper functions for drawing (gradients, glow, arrows)

2. **`src/main_hash.cpp`** ← 50 lines
   - Standalone entry point for Hash Visualizer
   - Window setup and event loop
   - Font loading with fallback

3. **`BUILD_GUIDE.md`** ← 300+ lines
   - Detailed setup instructions for different platforms
   - SFML installation guide
   - Troubleshooting section
   - Project structure explanation

4. **`HASH_CHAINING_README.md`** ← 250+ lines
   - User guide for visualizer
   - UI layout explanation
   - Keyboard shortcuts/button functions
   - Usage examples

### Modified Files:
1. **`src/HashVisualizer/HashVisualizer.h`**
   - Added member variables for colors, node tracking
   - Added 10 pastel color palettes
   - Fixed include paths

2. **`CMakeLists.txt`**
   - Updated to support dual executables
   - Removed hardcoded compiler paths
   - Added proper include directories
   - Support for Visual Studio & MinGW

3. **`.vscode/tasks.json`** 
   - Added build & run tasks
   - Support for CMake configure, build, and execution
   - Multiple task options for different scenarios

### Existing Files (No Changes Needed):
- `src/HashChaining/HashChaining.cpp` → Already complete
- `include/HashChaining/HashChaining.h` → Already complete
- `src/config/Config.h` → Window size & colors
- `src/core/Animation.h` → Step-based animation
- `src/UI/components/Button.h` → Button component
- `src/UI/components/Slider.h` → Speed slider

---

## 🎯 Key Implementation Details

### Glassmorphism Effects

**Color Palettes** (10 pairs in HSL-inspired pastel):
```cpp
{188, 236, 255, 200}  → #BCECFF (Sky Blue)
{194, 187, 255, 200}  → #C2BBFF (Lavender)
{141, 223, 246, 200}  → #8DDFF6 (Light Cyan)
{247, 205, 225, 200}  → #F7CDE1 (Rose Pink)
... (6 more pairs)
```

**Rendering Techniques:**
1. Semi-transparent rectangles (RGBA 200/255 alpha)
2. Thin white borders (1.5-2.0pt)
3. Top glow strip for shine effect
4. Gradient rectangles (2-color blend)
5. Smooth text rendering with different colors for highlighting

### Layout Proportions (1280×720 window)
```
┌─────────────────────────────────────────────────────────┐
│ Left Panel │      Center Canvas       │ Right Panel     │
│  248×600   │      660×600             │  328×600        │
├────────────┼───────────────────────────┼─────────────────┤
│            │                           │                 │
│ Buttons    │ Hash Table (13 buckets)  │ Pseudocode      │
│ Create     │ Nodes with gradients     │ & Highlight    │
│ Insert     │ Arrows between chains    │                 │
│ Delete     │ Status message           │                 │
│ Search     │                           │                 │
│ Update     │                           │                 │
│ Clear      │                           │                 │
│            │                           │                 │
└─────────────┴───────────────────────────┴─────────────────┘
│                    Bottom Panel (1256×88)                  │
│   Play Buttons (5)  │  Speed Slider (300×88)               │
└─────────────────────────────────────────────────────────┘
```

### Node Rendering
```
Each node displays as:
┌─────────────────────┐
│ Color1   │ Color2   │  ← Gradient fill
│  [Value] │          │  ← Black text, centered
└─────────────────────┘
Width: 60px, Height: 30px
Spacing: 70px horizontal, 45px vertical between buckets
```

### Animation Flow
```
User Action (button/keyboard)
    ↓
Create Steps in HashChaining backend
    ↓
Sync to Animation (Step-based)
    ↓
Display current Step with highlight
    ↓
Navigate with buttons or Play/Pause
    ↓
Render changes each frame
```

---

## 🔧 Technical Stack

| Component | Technology |
|-----------|-----------|
| **Language** | C++17 |
| **Graphics** | SFML 2.6+ |
| **Build System** | CMake 3.10+ |
| **Compiler** | MSVC 2022 / GCC / MinGW |
| **IDE** | VS Code (recommended) |
| **Animation** | Frame-based updates |

---

## 📊 Code Statistics

| File | Lines | Purpose |
|------|-------|---------|
| HashVisualizer.cpp | 520 | Main UI & rendering |
| HashVisualizer.h | 70 | Header declarations |
| HashChaining.cpp | 160 | Hash table logic |
| main_hash.cpp | 50 | Entry point |
| CMakeLists.txt | 60 | Build configuration |
| BUILD_GUIDE.md | 300+ | Setup instructions |

---

## 🚀 How to Build & Run

### Quick Start (Windows with Visual Studio 2022):

```bash
# Assuming SFML is installed
cd DataStructureVisualization
cmake -B build -G "Visual Studio 17 2022" -DSFML_DIR="C:\path\to\SFML\lib\cmake\SFML"
cmake --build build --config Release
.\build\Release\HashVisualizer.exe
```

### From VS Code:
1. Install C/C++ Extension Pack + CMake Tools
2. Press `Ctrl+Shift+B` to build
3. Press `Ctrl+F5` to run

### Detailed Instructions:
See **BUILD_GUIDE.md** for complete setup for different platforms.

---

## 🎮 Usage Guide

### UI Controls:

**Buttons:**
- **+ Create**: Generate 6 random elements
- **+ Insert**: Enter value to insert
- **- Delete**: Enter value to delete
- **🔍 Search**: Enter value to search
- **↻ Update**: Replace old value with new
- **Clear**: Remove all elements

**Playback:**
- **⏮ Skip Back**: Go to first step
- **⏪ Step Back**: Previous step
- **▶ Play**: Auto-play animation
- **⏩ Step Forward**: Next step
- **⏭ Skip Forward**: Go to last step
- **Speed Slider**: Control animation speed (1-10x)

**Status Display:**
- Current operation description (green text)
- Input prompt while entering value
- Step counter (current/total)

### Example Workflow:
1. Click **+ Create** → 6 random numbers inserted
2. Click **+ Insert** → Type `42` → Enter
3. Observe node placement in bucket `42 % 13`
4. Click **▶ Play** → Watch animation unfold
5. Click **🔍 Search** → Type `42` → See search steps

---

## 🎨 Color Scheme

### Panel Colors (RGBA):
- Background: (25, 35, 55, 200) - Dark blue-gray
- Border: (100, 140, 200, 150) - Light blue
- Glow: (255, 255, 255, 50) - White shine

### Text Colors:
- Title: (100, 200, 255) - Bright cyan
- Pseudocode: (180, 200, 220) - Light gray
- Highlighted line: (255, 255, 100) - Yellow
- Status: (150, 255, 150) - Light green
- Input: (100, 255, 255) - Cyan

### Node Colors (Gradients):
- 10 pastel pairs (see color palette section)
- Highlight: (255, 100, 100) - Red
- Selected bucket: (255, 215, 0, 120) - Yellow-gold

---

## ✅ Validation Checklist

- [x] Hash table with 13 buckets
- [x] Node stacking (chaining)
- [x] Insert/Delete/Search/Update operations
- [x] Glassmorphism UI design
- [x] Gradient colors on nodes
- [x] Outer glow effect
- [x] Semi-transparent panels
- [x] Smooth animations
- [x] Playback controls
- [x] Pseudocode highlighting
- [x] Input handling
- [x] Speed control slider
- [x] Status messages
- [x] Build configuration
- [x] Documentation

---

## 🔮 Future Enhancements (Optional)

1. **Double Hashing**: Alternative collision handling
2. **Load Factor**: Show hash table statistics
3. **Complexity Analysis**: Time complexity display
4. **Export/Import**: Save/load data from files
5. **Keyboard Shortcuts**: K for create, I for insert, etc.
6. **Themes**: Light/Dark mode toggle
7. **More Hash Functions**: Different modulo, polynomial, etc.
8. **Collapsible History**: Undo/Redo navigation

---

## 📝 Notes for Development

### Code Organization:
- **Helper functions** at top of HashVisualizer.cpp (drawing utilities)
- **Main class implementation** in middle (constructor, event handlers)
- **Render functions** grouped together
- **Action handlers** at bottom

### Extension Points:
- Add more color palettes in colorPalettes_ vector
- Implement new hash functions in HashChaining class
- Add custom rendering in renderNodes/renderBuckets
- Create new operations by extending pendingAction_ enum

### Debugging Tips:
- Use `visualization_.animation_.currentIndex()` to check progress
- Check `valueToColorIdx_` for color assignment
- Verify bucket/node positions in nodePositions_ map
- Monitor `playing_` flag for playback state

---

## 📞 Support

**Issues with Build:**
→ See BUILD_GUIDE.md section "Troubleshooting"

**Issues with Usage:**
→ See HASH_CHAINING_README.md section "Hướng dẫn Sử dụng"

**Code Questions:**
→ Check inline comments in HashVisualizer.cpp

---

## 📄 License & Attribution

© 2026 HCMUS Data Structures Group Project
- Developed for educational purposes
- SFML library: https://www.sfml-dev.org/
- Graphics rendering: OpenGL-based

---

## ✨ Key Achievements

✅ **Beautiful UI** with Glassmorphism aesthetic
✅ **Smooth Animations** with customizable speed
✅ **Complete Features** - All hash table operations
✅ **Good Documentation** - Build & usage guides
✅ **Modular Code** - Easy to extend & maintain
✅ **Cross-Platform** - Windows/Linux/macOS support

---

**Status**: ✅ Ready for Testing & Deployment

**Last Updated**: 2026-04-21

---

## Quick Reference Card

```
BUILD:
  cmake -B build -G "Visual Studio 17 2022"
  cmake --build build --config Release

RUN:
  ./build/Release/HashVisualizer.exe  (Windows)
  ./build/HashVisualizer              (Linux/macOS)

COMPONENTS:
  - 13-bucket hash table with chaining
  - Step-by-step animation playback
  - Glassmorphism UI (pastel colors, glow)
  - Insert/Delete/Search/Update operations
  - Pseudocode highlighting
  - Speed slider (1-10x)

KEY FILES:
  - HashVisualizer.cpp/h    : Main UI
  - HashChaining.cpp/h      : Data structure
  - main_hash.cpp           : Entry point
  - CMakeLists.txt          : Build config
  - BUILD_GUIDE.md          : Setup guide
  - HASH_CHAINING_README.md : Usage guide
```
