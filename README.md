# RegressionLine-Performance-Study-x86

A performance benchmarking study comparing three implementations of linear regression line calculation: **C++**, **x86 Assembly**, and **SSE (SIMD)** instructions.

---

## 📖 Project Overview

This project implements the statistical **least squares regression line** algorithm using three different approaches to measure and compare their execution performance on x86 architecture systems.

The regression line formula is:

$$y = mx + b$$

Where:

- $m = \frac{\text{covariance}(X,Y)}{\text{variance}(X)}$
- $b$ is calculated to ensure the line passes through the mean point

Each implementation processes configurable random data points (**default: 10,000**, **maximum tested: 50,000**) using **dynamic memory allocation** for scalability. Benchmarks run across multiple iterations (default 1,000 and 3,000) to measure execution time differences.

---

## 🔬 Implementation Details

### The Regression Line in C++

Standard high-level C++ implementation using:

- Dynamic arrays for X and Y coordinates (heap-allocated for large datasets)
- Sequential calculation of means, variance, and covariance
- Clock-based timing for performance measurement
- Proper memory cleanup with `delete[]`

### The Regression Line in Assembly (x8086)

Low-level x86 assembly implementation featuring:

- Direct register manipulation (`eax`, `ebx`, `ecx`, `edx`, `esi`, `edi`)
- Integer arithmetic for summations
- x87 FPU (Floating-Point Unit) instructions for division and statistical calculations
- Manual loop control and conditional jumps
- Pointer-based array access compatible with dynamic allocation

### The Regression Line with SSE Instructions

SIMD (Single Instruction Multiple Data) optimized implementation using:

- SSE registers (`xmm0`-`xmm5`) for parallel data processing
- Vectorized operations processing 4 integers simultaneously
- Horizontal addition (`phaddd`) for efficient summation reduction
- Combined with FPU instructions for final statistical calculations
- Dynamic memory allocation for scalable dataset sizes

---

## ✅ Verification

Algorithm accuracy is verified by comparing results with **SPSS** (Statistical Package for the Social Sciences), ensuring all three implementations produce mathematically correct regression lines.

---

## 🖥️ System Requirements

### Platform

- **Operating System**: Windows (MSVC compiler required)
- **Architecture**: x86 or x64 (Intel/AMD processors)
- **Compiler**: Microsoft Visual Studio (MSVC) with inline assembly support

### Why Not Cross-Platform?

This code uses:

- `__asm { }` syntax (MSVC-specific)
- x86 assembly instructions
- Architecture-specific registers

It will **not compile** on macOS, Linux (without modifications), or ARM-based systems.

---

## 🧪 Benchmarking Methodology

### SPEC CPU2000 Integration

The project references the **SPEC CPU2000** benchmark suite:

- **CINT2000**: Integer performance testing
- **CFP2000**: Floating-point performance testing

### Test Hardware

Performance tests were conducted across multiple machines with varying specifications:

- CPU models and clock speeds
- RAM capacity
- Cache sizes (L1, L2, L3)
- GPU configurations

_(Detailed specifications documented in original study)_

---

## 📊 Results Summary

### Reduced Benchmark Comparison

Execution time comparison across implementations:

| Implementation | Relative Performance |
| -------------- | -------------------- |
| **C++**        | Baseline (1x)        |
| **Assembly**   | ~4x faster           |
| **SSE (SIMD)** | Further improvement  |

### Key Findings

1. **Assembly (ASM)** achieves approximately **4x speedup** over standard C++
2. **SSE instructions** provide additional performance gains through parallel processing
3. Performance improvements vary across different hardware configurations
4. Trade-off: Significantly increased code complexity for speed gains

---

## 🎓 Academic Context

This project was developed as a performance study exploring:

- The relationship between code abstraction and execution speed
- Practical applications of low-level optimization techniques
- SIMD parallel processing paradigms
- Hardware-software performance boundaries

---

## 🚀 Usage

### Compilation

Compile with **Microsoft Visual Studio** (C++ project with inline assembly enabled):

```bash
# Open main.cpp in Visual Studio
# Build in Release mode for accurate benchmarks
```

### Execution

Run the compiled executable:

```bash
.\benchmark.exe
```

The program will:

1. Run iterations of all three implementations (first test)
2. Run iterations of all three implementations (second test)
3. Display a comparison table of execution times
4. Show regression line statistics (slope, intercept, equation)

---

## 📄 License

MIT License - See [LICENSE](LICENSE) file for details.

---

## 🔗 Related Concepts

- **Linear Regression**: Statistical method for modeling relationships between variables
- **Least Squares Method**: Minimizes the sum of squared residuals between observed and predicted values
- **SIMD (Single Instruction Multiple Data)**: Parallel computing technique processing multiple data points simultaneously
- **x87 FPU**: Floating-point coprocessor for mathematical operations
- **SSE (Streaming SIMD Extensions)**: Intel's SIMD instruction set extension

---

**Note**: This is an educational/research project demonstrating low-level optimization techniques. For production code, modern compilers with optimization flags (`-O3`, `/O2`) often produce highly efficient code without manual assembly.
