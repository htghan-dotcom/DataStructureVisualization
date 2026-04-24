@echo off
echo Checking SFML installation...
echo.

if exist "C:\SFML\lib\cmake\SFML\SFMLConfig.cmake" (
    echo ✓ SFML found at C:\SFML
    echo.
    echo In VS Code settings.json, set:
    echo "cmake.configureSettings": {
    echo     "SFML_DIR": "C:/SFML/lib/cmake/SFML"
    echo }
) else if exist "C:\msys64\ucrt64\lib\cmake\SFML\SFMLConfig.cmake" (
    echo ✓ SFML found in MSYS2 ucrt64
    echo.
    echo In VS Code settings.json, set:
    echo "cmake.configureSettings": {
    echo     "SFML_DIR": "C:/msys64/ucrt64/lib/cmake/SFML"
    echo }
) else (
    echo ✗ SFML not found!
    echo.
    echo Please install SFML first:
    echo 1. Download from: https://www.sfml-dev.org/download/sfml/2.6.1/
    echo 2. Choose: GCC 7.3.0 MinGW (DW2) - 32-bit
    echo 3. Extract to C:\SFML
    echo 4. Run this script again
)

echo.
pause