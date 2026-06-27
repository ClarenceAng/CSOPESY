# CSOPESY MO1

## CSOPESY S09 Group 10

### Members

- Adiong, Nathaniel Irvin
- Ang, Clarence Ivan
- Go, Kenneth D.
- Trocino, Job D.

## Build and Run (Windows)

### Prerequisites
- CMake
- MinGW (GCC) installed and added to your `PATH`

### Instructions
1. Open a terminal in the project root.
2. Configure the project:
   ```sh
   cmake -S . -B build -G "MinGW Makefiles"
   ```
3. Build the project:
   ```sh
   cmake --build build
   ```
4. Run the executable:
   ```sh
   .\build\app.exe
   ```

## Build and Run (Linux)

### Prerequisites
- CMake

### Instructions
1. Open a terminal in the project root.
2. Build the project:
   ```sh
   cmake --build build
   ```
3. Run the executable:
   ```sh
   ./build/app
   ```