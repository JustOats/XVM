# XVM

Anti-Tamper Software

## Requirements

- A C17 compiler
- CMake 3.16 or newer
- libsodium
- xxHash
- libyara
- Zydis

## Build

With CMake, clang, and vcpkg:

```powershell
cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build
```

With clang directly, point the include and library paths at your installed copies of libsodium,
Zydis, Zycore, xxHash, and libyara:

```powershell
clang -std=c17 -Wall -Wextra -Wpedantic -I include -I C:\path\to\deps\include src\xvm.c src\xvm_crypto.c src\xvm_disasm.c src\xvm_fast_hash.c src\xvm_yara.c src\main.c -L C:\path\to\deps\lib -lsodium -lZydis -lZycore -lxxhash -lyara -o xvm.exe
```

## Run

```powershell
.\xvm.exe
```

For CMake multi-config generators:

```powershell
.\build\Debug\xvm.exe
```

For single-config generators, the executable may be at `.\build\xvm.exe`.

## Project Layout

- `include/xvm/xvm.h` - public API
- `src/xvm.c` - core implementation
- `src/xvm_crypto.c` - libsodium integration
- `src/xvm_disasm.c` - Zydis integration
- `src/xvm_fast_hash.c` - xxHash integration
- `src/xvm_yara.c` - libyara integration
- `src/main.c` - command-line entrypoint
- `vcpkg.json` - external dependency manifest
