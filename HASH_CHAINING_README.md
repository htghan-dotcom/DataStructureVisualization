# Data Structure Visualizer - Hash Chaining Visualization

## Yêu cầu

- **Compiler**: GCC 9.0+ hoặc MSVC
- **CMAKE**: 3.10+
- **SFML**: 2.6+
- **C++ Standard**: C++17

## Cấu trúc dự án

```
DataStructureVisualization/
├── src/
│   ├── main.cpp                      # Visualizer chính (MST/Graph)
│   ├── main_hash.cpp                 # Hash Chaining Visualizer
│   ├── HashChaining/
│   │   ├── HashChaining.cpp          # Logic Hash Table
│   │   └── (header in include/)
│   ├── HashVisualizer/
│   │   ├── HashVisualizer.cpp        # UI & Rendering
│   │   └── HashVisualizer.h
│   ├── config/
│   │   └── Config.h                  # Cấu hình màu sắc & kích thước
│   ├── UI/
│   │   ├── Visualizer.h/cpp          # Main visualizer (MST)
│   │   └── components/               # Button, Slider, etc.
│   └── ...
├── include/
│   └── HashChaining/
│       └── HashChaining.h
├── assets/
│   └── fonts/                        # Font files
├── .vscode/
│   └── tasks.json                    # Build tasks
└── CMakeLists.txt
```

## Build từ Command Line

### Windows (MSYS2 ucrt64)

```bash
# Tạo build folder
cmake -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run Hash Visualizer
./build/HashVisualizer.exe

# Run MST Visualizer
./build/DataStructureVisualization.exe
```

### Linux/macOS

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/HashVisualizer
```

## Build từ VS Code

### Cách 1: Dùng VS Code Tasks (Recommended)

Nhấn `Ctrl+Shift+B` để build, hoặc chọn task từ Command Palette:

- `CMake: Build All` - Build toàn bộ project
- `Build & Run: HashVisualizer` - Build & Run Hash Visualizer  
- `Run: HashVisualizer` - Chạy Hash Visualizer (đã build)

### Cách 2: Terminal

```bash
cd DataStructureVisualization
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/HashVisualizer.exe
```

## Hướng dẫn Sử dụng Hash Chaining Visualizer

### UI Layout

- **Left Panel**: Nút điều khiển (Create, Insert, Delete, Search, Update, Clear)
- **Center Canvas**: Hiển thị Hash Table với 13 buckets (0-12)
  - Các node được sắp xếp theo stack từ trái sang phải
  - Gradient colors giúp phân biệt từng node
  - Highlight (màu đỏ) node đang được xem xét
- **Right Panel**: Pseudocode với dòng hiện tại được highlight
- **Bottom Panel**: Playback controls (Skip Back, Step, Play, Step Forward, Skip Forward) + Speed slider

### Các Phím Tắt

| Nút | Chức năng |
|-----|---------|
| **+ Create** | Tạo ngẫu nhiên 6 phần tử |
| **+ Insert** | Nhập giá trị và chèn vào hash table |
| **- Delete** | Nhập giá trị và xóa từ hash table |
| **🔍 Search** | Tìm kiếm giá trị trong hash table |
| **↻ Update** | Cập nhật giá trị (xóa cái cũ, thêm cái mới) |
| **Clear** | Xóa hết tất cả nodes |
| **▶ Play** | Chạy animation tự động |
| **⏩ Step Forward** | Tiến 1 bước |
| **⏪ Step Back** | Lùi 1 bước |
| **⏮ Skip Back** | Quay lại đầu |
| **⏭ Skip Forward** | Nhảy đến cuối |
| **Speed** | Điều chỉnh tốc độ animation (1-10x) |

### Ví dụ Quy Trình

1. Nhấn **+ Create** → Random sinh 6 phần tử
2. Nhấn **+ Insert** → Nhập `42` → Enter → Xem node được chèn vào bucket tương ứng
3. Nhấn **▶ Play** → Xem animation từng bước
4. Nhấn **🔍 Search** → Nhập `42` → Enter → Xem quá trình tìm kiếm

## Glassmorphism Design

Visualizer sử dụng Glassmorphism aesthetic:

- **Translucent Backgrounds**: Panel có độ trong suốt với alpha channel
- **Gradient Colors**: Mỗi node có gradient color khác nhau từ 10 pastel palettes
- **Outer Glow**: Viền trắng mảnh và glow effect giới thiệu tính chất "glass"
- **Smooth Animations**: Animation mượt khi node di chuyển hoặc highlight

## Cấu hình Màu Sắc

Các pastel color palettes (trong HashVisualizer.h):

```cpp
{188, 236, 255, 200}, {194, 187, 255, 200}  // Sky Blue → Lavender
{141, 223, 246, 200}, {247, 205, 225, 200}  // Light Cyan → Pink
// ... 8 pairs more
```

Các màu highlight:

- **Target Node**: Đỏ (255, 100, 100)
- **Target Bucket**: Vàng (255, 215, 0)
- **Status Text**: Xanh lục (150, 255, 150)
- **Input Text**: Cyan (100, 255, 255)

## Troubleshooting

### Build fails với lỗi SFML

**Lỗi**: `SFML not found`

**Giải pháp**: 
```bash
# Windows (MSYS2)
pacman -S mingw-w64-ucrt-x86_64-sfml

# Hoặc update CMakeLists.txt với đúng SFML path
```

### Fonts không load

**Lỗi**: "Failed to load font"

**Giải pháp**:
- Đảm bảo `assets/fonts/` folder tồn tại
- Hoặc copy fonts vào folder đó
- Fallback tự động sử dụng font khác nếu không tìm thấy

### Window không hiện lên

**Lỗi**: SFML window không hiện hoặc crash

**Giải pháp**:
- Kiểm tra video driver
- Update SFML library
- Chạy từ terminal để xem error message

## Architecture

### Core Components

1. **HashChaining** (`src/HashChaining/`)
   - Data structure backend
   - Tạo steps/animation từ operations

2. **HashVisualizer** (`src/HashVisualizer/`)  
   - UI rendering (Glassmorphism)
   - Event handling
   - Animation controller

3. **Animation** (`src/core/`)
   - Step-by-step execution engine
   - Timeline playback control

4. **UI Components** (`src/UI/components/`)
   - Button, Slider, v.v.
   - Reusable UI elements

## Mở rộng

Để thêm tính năng mới:

1. Thêm logic vào `HashChaining.cpp`
2. Cập nhật pseudocode trong `buildPseudocode()`
3. Implement render trong `HashVisualizer.cpp`
4. Thêm action handler nếu cần

## License

© 2026 HCMUS Data Structures Group Project

---

**Build Status**: ✅ Ready to compile and run

**Last Updated**: 2026-04-21
