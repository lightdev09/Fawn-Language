<p align="center">
  <img src="res/fawn_icon.ico" alt="Fawn logo" width="96" height="96">
</p>
<h1 align="center">Fawn</h1>
<p align="center">
  A small, beginner-friendly scripting language built from scratch in C++.
</p>
<p align="center">
  <img alt="version" src="https://img.shields.io/badge/version-1.5.0-blue">
  <img alt="language" src="https://img.shields.io/badge/language-C%2B%2B20-informational">
  <img alt="build" src="https://img.shields.io/badge/build-CMake-orange">
  <img alt="license" src="https://img.shields.io/badge/license-MIT-green">
</p>

---

## Table of Contents
- [Overview](#overview)
- [Architecture](#architecture)
- [The Story](#the-story)
- [Getting Started](#getting-started)
  - [Windows installer](#windows-installer)
  - [Build from source](#build-from-source-linux)
- [CLI Reference](#cli-reference)
- [Cheatsheet](#cheatsheet)
  - [Variables](#variables)
  - [Operators](#operators)
  - [Output & Input](#output--input)
  - [Control Flow](#control-flow)
  - [Loops](#loops)
  - [Functions](#functions)
  - [Lists](#lists)
  - [Strings](#strings)
  - [Native Functions](#native-functions)
  - [Ending a Program](#ending-a-program)
- [Full Native Function Reference](#full-native-function-reference)
- [Full String Method Reference](#full-string-method-reference)
- [Full List Method Reference](#full-list-method-reference)
- [Project Structure](#project-structure)
- [Examples](#examples)
- [Extension](#extension)
- [License](#license)

---

## Overview

Fawn is a tree-walking interpreted language with:

- **Optional static typing** — declare variables as `int`, `float`, `string`, `bool` for enforced type-locking, or use `var` for dynamic typing. Mixing is allowed within the same script.
- **No indentation pitfall** — I found myself struggling with Python's indentation rules, so I decided to go with the classical brace-based approach instead.
- **C++-flavored I/O** — the language is heavily inspired by C++, and I've always found `cout` useful for chaining output easily, so I borrowed `out << value << end` and `in >> variable` from `cout`/`cin`. If you prefer good old `print`, that's included too.
- **Functions with optional return-type enforcement** — declare `: int` and the function is locked to that type at runtime; leave it off and it can return anything. Default parameter values are supported.

## Architecture

```
source (.fw) → Lexer → Tokens → Parser → AST → Interpreter (tree-walking)
```

- **Lexer**: single-pass, maximal-munch tokenizer. Handles string escapes, comments, and multi-character operators (`==`, `===`, `<<`, `>>`, `->`, etc.).
- **Parser**: recursive-descent with precedence climbing for expressions (`or → and → equality → comparison → term → factor → unary → call → primary`). Produces a plain-data AST (`Expr`/`Stmt` hierarchies).
- **Interpreter**: a separate class that walks the AST via `dynamic_cast` dispatch (because I only learned about the visitor pattern after most of the interpreter was already written 🙂).
- **Environment**: a chained scope model, backing block scoping, function-call scoping, and loop-variable scoping.
- **Value**: a `std::variant`-based dynamically-tagged runtime value, with type-promotion rules for mixed arithmetic (`int + float → float`) and both loose (`==`) and strict, type-aware (`===`) equality.
- **Control-flow unwinding** (`return`, `break`) implemented via lightweight C++ exceptions caught at the appropriate boundary (loop body vs. function call site).

Currently a tree-walking interpreter (not yet compiled to bytecode). A bytecode VM is a possible future direction if I ever decide to keep developing this language.

## The Story

*Before moving on, I want to share how the project got started. If you don't want to read this, skip to* [Getting Started](#getting-started).

So it all started when I was learning Vulkan.

As anyone who has worked with Vulkan knows, the amount of boilerplate and complexity can be pretty exhausting, especially when you're still new to it. So I decided to take a break from Vulkan. While taking that break one day, I ended up watching a YouTube video of a guy trying out some joke scripting languages.

At the time, I'd been coding in C++ for around two and a half years, but I had barely ever used C++'s filesystem features, because I'd never really needed them. In Vulkan, though, you often need to read shader files and work with their contents, so I was still pretty new to working with the filesystem.

That got me thinking: *If I can read a file, can I use that file to produce some kind of output?*

So I opened a new project and started experimenting. And after juggling with some `if` statements, I finally managed to print **"Hello World"** in the console.

Then I thought, *Well, if I can print "Hello World", can I make a multiline statement? And if I can do that, can I make my own scripting language?*

And that's basically how it started.

I'd say around 85% of the project was written with my bare hands (the other 15% went to generating the logo, making this README overly emotional -_-, and debugging some stuff). I literally spent about an hour and a half going through a dictionary trying to find a name that wasn't already taken (+1 for my dedication :) ) — well, the dictionary finally got some use.

The project went through a lot of changes along the way. At first, I wanted to make a completely ridiculous joke language. Later, I decided to take it in a different direction and build something simple, readable, and actually usable.

I don't know why I made this — maybe I'm stupid as hell — but hey, I had a lot of fun building it. Seeing everything finally come together and behave the way I imagined was incredibly satisfying.

To be honest, the language is ridiculously slow, so just pretend you don't care about speed for now :0 (if you do, just wait a while — I'll get around to making the VM 🥀).

If you enjoy the language or just like the idea behind it, consider leaving a star on the repository. It would mean a lot.

---

## Getting Started

### Windows installer

**If you're using a Windows machine:**

1. Head to the [Releases](../../releases) section and download the latest Windows installer.
2. Run the installer and follow the setup wizard.

The installer automatically adds Fawn to your system `PATH`. Open a new terminal and verify the installation:

```bash
fawn --v
```

If for some reason it shows `fawn` isn't recognized, add it to `PATH` manually:

1. Open **Windows Settings** → search for **"Environment Variables"** → **Edit the system environment variables**.
2. Click **Environment Variables**.
3. Under **User variables** (or **System variables**, if installed for all users), select **Path** → **Edit**.
4. Click **New** and add the folder where `fawn.exe` was installed (e.g. `C:\Program Files\Fawn`).
5. Click **OK** on all windows, then open a new terminal for the change to take effect.

### Build from source (Linux)

> Pre-built Linux binaries aren't available yet — build from source using CMake.

#### Prerequisites

- CMake 3.20+
- A C++20-capable compiler (GCC or Clang)

#### Build

```bash
git clone https://github.com/AnkanDey05/Fawn-Language.git
cd Fawn-Language
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Verify

```bash
./build/fawn --v
```

#### Install (optional)

To use `fawn` from anywhere without typing the full path:

```bash
sudo cp build/fawn /usr/local/bin/
```

## CLI Reference

| Command | Effect |
|---|---|
| `fawn` | Launch the REPL |
| `fawn <file>.fw` | Run a script |
| `fawn --check <file>.fw` | Parse and validate without executing |
| `fawn --token <file>.fw` | Print the lexer's token stream |
| `fawn --ast <file>.fw` | Print the parsed AST |
| `fawn --version` / `--v` | Print the interpreter version |
| `fawn --help` | Print CLI usage |

---

## Cheatsheet

### Variables

```fawn
int age = 20              // statically typed — locked to int
float hp = 200.05
string name = "Ankan"
bool alive = true

var thing = 10             // dynamically typed — can change type later
thing = "now a string"     // legal, because it's var

int score                  // uninitialized, defaults to Null, still locked to int
score = 100                // fine now

const float PI = 3.14159   // cannot be reassigned
const string author = "Ankan"
```

### Operators

```fawn
+  -  *  /  %              // arithmetic
==  !=  ===                // equal, not equal, strict (value AND type) equal
<  >  <=  >=                // comparison
and  or  not                // logical
++  --                      // increment / decrement
```

### Output & Input

```fawn
out << "Hello" << " " << "world" << end   // stream-style output, 'end' = newline
out << "no newline here"                   // 'end' is optional
print("Hello World")                     // also supported

int x
in >> x                                     // reads a line into an existing variable
println("Variable is: ", x)              // add a new line
```

### Control Flow

```fawn
if age >= 18 {
    out << "adult" << end
} elif age >= 12 {
    out << "teen" << end
} else {
    out << "child" << end
}

// one-liner form
if x > 10 then out << "big" << end

// ternary
var status = age >= 18 ? "adult" : "minor"

// if as an expression
var label = if age > 21 then "senior" else "junior"
```

### Loops

```fawn
while count < 10 {
    count++
}

for i in 0 -> 10 step 2 {
    if i == 4 then continue
    if i == 8 then break
    out << i << " "
}
```

### Functions

```fawn
fn add(int a, int b) : int {
    return a + b
}

fn greet(string name, string greeting = "Hello") {
    out << greeting << " " << name << end
}

fn loose() {
    return "no declared return type, anything goes"
}
```

* Declaring a return type (`: int`) locks the function to that type — returning anything else is a runtime error.
* No return type declared means the function can return any type.
* Default parameter values are supported (once a parameter has a default, every parameter after it must too).
* Lists passed into functions share the same underlying data — mutating a list inside a function is visible to the caller.

### Lists

```fawn
list arr = {3, 1, 4, 1, 5}
list typed : int = {1, 2, 3}      // element-type locked
list fixed[10] = {1, 2, 3}         // fixed size, rest filled with Null

arr.put(9)                 // append
arr.place(0, 99)           // insert at index
arr.pull()                 // remove & return last (or by index)
arr.strip(4)               // remove first matching value
arr.flush()                // clear
arr.clone()                // copy
arr.find(4)                // index of first match, -1 if none
arr.freq(1)                // count of matching elements
arr.flip()                 // reverse in place
arr.sort()                 // sort in place
arr.size()                 // length

arr[0]                     // index read
arr[0] = 99                // index assign
```

### Strings

```fawn
"hello".caps()              // "HELLO"
"HELLO".small()              // "hello"
"a,b,c".slice(",")           // {"a", "b", "c"}
"  hi  ".strip()             // "hi"
"hello world".has("world")   // true
"hello".swap("l", "L")        // "heLLo"
"hello".size()                // 5
```

### Native Functions

```fawn
abs(-5)            least(3, 7)         most(3, 7)
exp(2, 3)          sqrt(16)             random(1, 100)
round(3.7)         floor(3.7)           ceil(3.2)
type_of(x)         len(x)               clock()
int(x)  float(x)  string(x)  bool(x)    // type conversion
```

### Ending a Program

```fawn
exit 0        // terminates immediately, with an exit code
```

---

## Full Native Function Reference

| Function | Description |
|---|---|
| `abs(x)` | Absolute value of an `int` or `float` |
| `least(a, b)` | Smaller of two values |
| `most(a, b)` | Larger of two values |
| `exp(base, power)` | Exponentiation |
| `sqrt(x)` | Square root |
| `round(x)` | Round to nearest integer |
| `floor(x)` | Round down |
| `ceil(x)` | Round up |
| `random(min, max)` | Random integer in range |
| `type_of(x)` | Returns the value's type as a string |
| `len(x)` | Length of a string or list |
| `int(x)` / `float(x)` / `string(x)` / `bool(x)` | Explicit type conversion (including string→number parsing) |
| `clock()` | Milliseconds since epoch |
| `clock("time" \| "date" \| "full")` | Formatted time, date, or datetime string |
| `print(...)` | Write one or more values to stdout, no trailing newline |
| `println(...)` | Write one or more values to stdout, with a trailing newline |
| `keyPressed()` | Non-blocking check for a key press |
| `getKey()` | Reads the currently pressed key |

## Full String Method Reference

Called as `value.method(...)`:

| Method | Description |
|---|---|
| `caps()` | Uppercase the whole string |
| `small()` | Lowercase the whole string |
| `upper()` / `lower()` | Uppercase / lowercase (alias pair to `caps`/`small`) |
| `empty()` | `true` if the string has zero length |
| `size()` | Character count |
| `at(i)` | Character at index `i` |
| `has(sub)` | `true` if `sub` occurs anywhere in the string |
| `begins(sub)` | `true` if the string starts with `sub` |
| `ends(sub)` | `true` if the string ends with `sub` |
| `slice(sep)` / `split_by(sep)` | Split on `sep`, returns a `list` |
| `strip()` | Trim leading/trailing whitespace |
| `trim()` | Trim leading/trailing whitespace (alias of `strip`) |
| `swap(old, new)` | Replace every occurrence of `old` with `new` |
| `cut(start, count)` | Substring of `count` characters starting at `start` |
| `locate(sub)` | Index of the first occurrence of `sub`, or `-1` |
| `append(str)` | Concatenate `str` onto the end |
| `insert_at(i, str)` | Insert `str` at index `i` |
| `erase_at(i, count)` | Remove `count` characters starting at index `i` |
| `repeat(n)` | Repeat the string `n` times |
| `pad_left(width [, fill])` | Left-pad to `width` (default fill: space) |
| `pad_right(width [, fill])` | Right-pad to `width` |
| `pad_center(width [, fill])` | Center-pad to `width` |

## Full List Method Reference

Called as `value.method(...)`:

| Method | Description |
|---|---|
| `put(x)` | Append `x` |
| `place(i, x)` | Insert `x` at index `i` |
| `pull([i])` | Remove and return the last element, or the element at index `i` |
| `strip(x)` | Remove the first element equal to `x` |
| `flush()` | Clear the list |
| `clone()` | Shallow copy |
| `freq(x)` | Count occurrences of `x` |
| `find(x)` | Index of the first element equal to `x`, or `-1` |
| `flip()` | Reverse in place |
| `sort()` | Sort in place |
| `size()` | Element count |

Indexing (`arr[i]`, `arr[i] = x`) is also supported directly.

---

## Project Structure

```
Fawn/
├── src/
│   ├── lexer/        # Tokenizer
│   ├── parser/         # Recursive-descent parser
│   ├── ast/             # AST node definitions (Expr, Stmt, Node)
│   ├── interpreter/     # Tree-walking evaluator + native stdlib
│   ├── runtime/         # Environment / variable scoping
│   ├── common/          # Value type, Token, version config
│   ├── io/               # File reading, keyboard input
│   ├── cli/              # Argument parsing
│   ├── utils/            # Error reporting
│   └── CodeRunner/      # Orchestrates REPL / file / debug modes
├── examples/             # Sample .fw scripts
├── res/                   # Windows icon & resource file
└── CMakeLists.txt
```

## Examples

The `examples/` directory contains runnable sample scripts, including:

- `blackjack.fw` — a card game implementation
- `snake.fw` — the classic snake game (might not work depending on your terminal, since it relies on ASCII escape sequences)

Run any of them with:

```bash
fawn examples/blackjack.fw
```

## Extension

If you're using the VS Code extension and something looks off — that part was mostly AI-generated, so no promises. Feel free to open an issue anyway.

## License

Fawn is released under the [MIT License](LICENSE.md).