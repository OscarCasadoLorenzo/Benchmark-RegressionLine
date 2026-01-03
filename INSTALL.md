# Installation and Compilation Guide

This guide provides step-by-step instructions to compile and run the Benchmark-RegressionLine project on Windows **using only the terminal** (no IDE required).

## Prerequisites

This project uses **Microsoft Visual C++ (MSVC)** inline assembly syntax, which requires the Microsoft Visual C++ compiler. You can install just the command-line tools without the full Visual Studio IDE.

## Installation Steps

### Option A: Build Tools Only (Lightweight - ~2-3 GB, No IDE)

**Recommended if you only want to compile from terminal:**

1. **Download Build Tools for Visual Studio 2022**

   - Visit: https://visualstudio.microsoft.com/downloads/
   - Scroll down to "All Downloads" → "Tools for Visual Studio"
   - Download **"Build Tools for Visual Studio 2022"**
   - OR direct link: https://aka.ms/vs/17/release/vs_buildtools.exe

2. **Run the Installer**

   - Launch `vs_buildtools.exe`

3. **Select Components**

   - Check **"Desktop development with C++"**
   - This installs only:
     - MSVC compiler (cl.exe)
     - Windows SDK
     - Essential build tools
   - **No IDE, no extra bloat**

4. **Install**
   - Click "Install" (requires ~2-3 GB of disk space)
   - Wait for installation to complete (may take 10-15 minutes)

### Option B: Full Visual Studio (If you want the IDE too - ~7-10 GB)

1. **Download Visual Studio 2022 Community Edition** (Free)

   - Visit: https://visualstudio.microsoft.com/downloads/
   - Click on "Free download" under "Community 2022"

2. **Run the Installer**

   - Launch the downloaded `VisualStudioSetup.exe`

3. **Select Workload**

   - In the installer, check **"Desktop development with C++"**
   - This will install:
     - MSVC compiler (cl.exe)
     - Windows SDK
     - Visual Studio IDE
     - CMake tools
     - C++ core features

4. **Install**
   - Click "Install" (requires ~7-10 GB of disk space)
   - Wait for installation to complete (may take 15-30 minutes)

**Note:** You can use Option B and still compile from terminal without ever opening the IDE.

**Note:** You can use Option B and still compile from terminal without ever opening the IDE.

## Verify Installation

After installation, verify the compiler is available:

1. **Open "Developer Command Prompt for VS 2022"** or **"Developer PowerShell for VS 2022"**

   - Press `Windows Key` and search for "Developer Command Prompt" or "Developer PowerShell"
   - These terminals come with either installation option

2. **Test the compiler:**

   ```cmd
   cl
   ```

   You should see output like:

   ```
   Microsoft (R) C/C++ Optimizing Compiler Version 19.xx.xxxxx for x86
   ```

## Quick Start - Compile and Run

### IMPORTANT: This code uses 32-bit x86 assembly and MUST be compiled for x86 (32-bit) architecture!

### Method 1: Using Developer Command Prompt (Easiest)

1. **Open "x86 Native Tools Command Prompt for VS 2022"** from Start Menu

   - **NOT** the x64 version - must be x86!

2. **Navigate to project directory:**

   ```cmd
   cd "C:\Users\oscar\Desktop\Github\Benchmark-RegressionLine"
   ```

3. **Compile:**

   ```cmd
   cl /EHsc /O2 main.cpp /Fe:benchmark.exe
   ```

4. **Run:**
   ```cmd
   benchmark.exe
   ```

### Method 2: Using Developer PowerShell (Recommended for PowerShell users)

1. **Open your regular PowerShell**

2. **Navigate to project directory:**

   ```powershell
   cd C:\Users\oscar\Desktop\Github\Benchmark-RegressionLine
   ```

3. **Set up x86 (32-bit) environment and compile:**

   ```powershell
   cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" && cl /EHsc /O2 main.cpp /Fe:benchmark.exe'
   ```

   Or for full Visual Studio installation:

   ```powershell
   cmd /c '"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" && cl /EHsc /O2 main.cpp /Fe:benchmark.exe'
   ```

4. **Run:**

   ```powershell
   .\benchmark.exe
   ```

5. **Compile:**

   ```powershell
   cl /EHsc /O2 main.cpp /Fe:benchmark.exe
   ```

6. **Run:**
   ```powershell
   .\benchmark.exe
   ```

### Method 3: Using Your Regular Terminal (PowerShell/CMD)

**CRITICAL: Use vcvars32.bat (NOT vcvars64.bat) because the code uses 32-bit assembly!**

**For PowerShell:**

```powershell
# One-time setup per terminal session - USE vcvars32.bat for x86!
cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" && cl /EHsc /O2 main.cpp /Fe:benchmark.exe'

# OR if you installed full Visual Studio:
cmd /c '"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" && cl /EHsc /O2 main.cpp /Fe:benchmark.exe'

# Then run
.\benchmark.exe
```

**For CMD:**

```cmd
REM One-time setup per terminal session - USE vcvars32.bat for x86!
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"

REM OR if you installed full Visual Studio:
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"

REM Navigate to project
cd C:\Users\oscar\Desktop\Github\Benchmark-RegressionLine

REM Compile
cl /EHsc /O2 main.cpp /Fe:benchmark.exe

REM Run
benchmark.exe
```

## Compilation Flags Explained

- `/EHsc` - Enable C++ exception handling (standard)
- `/O2` - Optimize for maximum speed (recommended for benchmarking)
- `/Fe:benchmark.exe` - Specify output executable name
- `/W3` - Warning level 3 (optional, good practice)

## Troubleshooting

### Error: "cl is not recognized"

**Cause:** The compiler environment variables are not set.

**Solutions:**

1. Use "x86 Native Tools Command Prompt for VS 2022" from Start Menu (easiest)
2. OR run the vcvars32.bat script in your current terminal (see Method 3 above)
3. **Important:** Make sure you're using vcvars**32**.bat, not vcvars64.bat!

### Error: "\_\_asm keyword not supported on this architecture" or "illegal number of operands"

**Cause:** You're compiling for x64 (64-bit) instead of x86 (32-bit).

**Solution:**

- Use vcvars**32**.bat instead of vcvars**64**.bat
- OR open "**x86** Native Tools Command Prompt" (not x64)
- The inline assembly in this code only works with 32-bit (x86) compilation

### Error: "Cannot open include file"

**Cause:** C++ development tools not properly installed.

**Solution:**

- Rerun the Visual Studio installer
- Make sure "Desktop development with C++" is checked
- Click "Modify" to add it if missing

### Error: "vcvars32.bat not found"

**Cause:** Wrong installation path.

**Solution:** Find your actual installation path:

- Full VS: Usually `C:\Program Files\Microsoft Visual Studio\2022\Community\`
- Build Tools: Usually `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\`

Look for: `VC\Auxiliary\Build\vcvars32.bat`

### Compilation warnings about deprecated functions

The code uses standard C functions. If you see warnings, you can:

- Safely ignore them, OR
- Compile with `/W3` flag for fewer warnings

### Program crashes or shows incorrect results

**Cause:** The code uses 32-bit x86 assembly instructions.

**Solution:** Make sure you're compiling for x86 (32-bit) architecture:

```cmd
cl /EHsc /O2 /arch:IA32 main.cpp /Fe:benchmark.exe
```

## System Requirements

- **OS:** Windows 10 or Windows 11
- **Architecture:** x86 or x64 CPU (code uses 32-bit assembly instructions)
- **Disk Space:**
  - Build Tools only: ~2-3 GB
  - Full Visual Studio: ~7-10 GB
- **RAM:** 4 GB minimum (8 GB recommended)
- **No IDE required** - Everything works from the terminal!

## Summary - TL;DR

**For terminal-only users (no IDE):**

1. Install Build Tools for Visual Studio 2022 (2-3 GB)
2. Open "Developer Command Prompt for VS 2022"
3. Run:
   ```cmd
   cd C:\Users\oscar\Desktop\Github\Benchmark-RegressionLine
   cl /EHsc /O2 main.cpp /Fe:benchmark.exe
   benchmark.exe
   ```

**That's it! No Visual Studio IDE needed.**

## Notes

- The program uses **inline assembly** specific to MSVC (`__asm` blocks)
- It benchmarks three implementations: pure C++, x86 Assembly, and SSE instructions
- The benchmark measures regression line calculation performance
- Results will vary based on your CPU performance

## License

See [LICENSE](LICENSE) file for details.
