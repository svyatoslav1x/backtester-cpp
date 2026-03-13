# Backtester C++ — Windows Setup for CLion

This guide explains how to download, build, and run **backtester-cpp** on **Windows** using **CLion**.

Repository:

- https://github.com/svyatoslav1x/backtester-cpp

This version of the guide is for people who already use **CLion** and want to build the project there without being
pushed into a Visual Studio setup. It also works for a **Qt + MinGW** setup such as **Qt 6.10.2 MinGW**, as long as your
compiler and Qt kit match.

The project is a **CMake-based Qt 6 C++ application** with tests.

---

## 1. What you need

To build this project in CLion on Windows, you need:

- **CLion**
- **Qt 6**
- a working **compiler toolchain**
- **CMake**
- **Git** if you want to clone the repo

You do **not** need Visual Studio if your current setup already works in CLion.

### Supported practical setup

A working example is:

- **CLion**
- **Qt 6.10.2 MinGW**
- **MinGW compiler**
- **CMake** bundled with CLion or installed separately

The important rule is simple:

- **MinGW Qt** must be used with a **MinGW compiler**
- **MSVC Qt** must be used with **MSVC**

Do **not** mix them.

---

## 2. Official download links

### Project

- GitHub repository: https://github.com/svyatoslav1x/backtester-cpp
- ZIP download: https://github.com/svyatoslav1x/backtester-cpp/archive/refs/heads/master.zip

### Tools

- CLion: https://www.jetbrains.com/clion/download/
- Qt open-source download page: https://www.qt.io/download-open-source
- Git for Windows: https://git-scm.com/download/win
- CMake: https://cmake.org/download/

---

## 3. Download the project

### Option A — Clone with Git

Open a terminal and run:

```bash
git clone https://github.com/svyatoslav1x/backtester-cpp.git
cd backtester-cpp
```

### Option B — Download ZIP

1. Open the repository page:
   `https://github.com/svyatoslav1x/backtester-cpp`
2. Click **Code**
3. Click **Download ZIP**
4. Extract it
5. Open the extracted folder in CLion

---

## 4. Open the project in CLion

1. Start **CLion**
2. Choose **Open**
3. Select the project folder that contains `CMakeLists.txt`
4. Let CLion load the CMake project

If CLion asks about the toolchain or CMake profile, configure it as described below.

---

## 5. Configure CLion for Qt 6.10.2 MinGW

If you already have **Qt 6.10.2 MinGW** installed and it works on your machine, keep using that setup.

### Step 1 — Check your Qt path

A typical path looks like this:

```text
C:\Qt\6.10.2\mingw_64
```

Your exact folder name may differ slightly depending on the Qt package you installed.

### Step 2 — Check your MinGW compiler path

Common examples:

```text
C:\Qt\Tools\mingw...
```

or another MinGW installation already configured in CLion.

### Step 3 — Set the CLion toolchain

In **CLion**:

1. Open **File -> Settings -> Build, Execution, Deployment -> Toolchains**
2. Make sure the toolchain uses your **MinGW** compiler
3. Check that **C Compiler** and **C++ Compiler** point to the MinGW installation you actually use
4. Make sure **CMake** is available
5. Apply the changes

### Step 4 — Set the CMake profile

In **CLion**:

1. Open **File -> Settings -> Build, Execution, Deployment -> CMake**
2. In **CMake options**, add:

```text
-DCMAKE_PREFIX_PATH=C:\Qt\6.10.2\mingw_64
```

If your Qt folder is different, use your real path instead.

That tells CMake where to find Qt.

---

## 6. Build the project in CLion

After the toolchain and CMake profile are set:

1. Reload the CMake project if needed
2. Select the main executable target
3. Click **Build**

The main target is usually:

- `backtester-cpp`

If CMake configures correctly, CLion should detect the targets automatically.

---

## 7. Run from terminal

If you want to build from the terminal with your MinGW Qt setup:

```bash
cmake -S . -B cmake-build-debug -DBUILD_TESTS=ON -DCMAKE_PREFIX_PATH="C:\Qt\6.10.2\mingw_64"
cmake --build cmake-build-debug
```

