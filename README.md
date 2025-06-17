# Description
SMPL will be compiled down to C, which is then again compiled down to machine code by clang.
This project has not been finished yet and won't be supporting OOP until the language is capable of doing tasks using the FP paradigm.

> The compiler parser's print() function is pretty ugly, I am focusing on parsing for now and will later make printing more pretty

# Goal
SMPL aims to be as simple as possible, much like C but with more type-safety and other safety features.
# Example Code
``` C++
// Function definition describes a function with
// two parameters of type i32, and should also return
// a value of type i32
defn pow(base: i32, exp: i32) -> i32 {
    let result: i32 = 1;

    // Range-based for loop
    for i in 0..exp {
        result *= base;
    }

    return result;
}

defn main() {
    let x: i32 = 12;
    let y: i32 = x = 64;
    let z: i32 = pow(x, y);  // FIXME: Overflow alert. But it's okay, we're not executing anything

    // While loop
    while true {
        let some: i32 = true; // Nope. Won't compile. bool and i32 are not assignable
    }

    if false == not true {
        // Should be true, this does not evaluate anything yet though
    }

    // Expression statement
    x;
    true;
    pow(x, y);
    not true; // False
}
```
# Progress
- 🟢 Lexical analysis done (I think). It's just a matter of expanding now.
- Parser can now parse:
    - if statements
    - while statements
    - for statements
    - return statements
    - function definitions
    - assignment statements
    - expression statements
    - unary expression (only `not` `<expr>` and -`<expr>`)
    - binary expression
    - function calls
    - boolean expressions (including relational expressions)
-❌ Semantic Analysis

# Primitive Types
An integer's name can be composed of its signedness and its bit-width. For example, i32 means it is a signed 32-bit integer and u32 means it is an unsigned 32-bit integer.
Floating-point types are also named this way, without the signedness.

**Here are the primitive types for SMPL:**
- i8
- u8
- i16
- u16
- i32
- u32
- i64
- u64
- int (signed integer with a bit-width that depends on your machine's architecture)
- uint (unsigned integer with a bit-width that depends on the architecture, primarily used for array indexing)
- f16
- f32
- f64
- f128
- char (only supports ASCII)
- bool
# Composite types
**SMPL composite types will be composed of:**
- Class
- Arrays
- Lists (Dynamic Arrays)
- Hashmaps
- Strings
- Enums
# Prerequisites
- [CMake](https://cmake.org/download/) (Optional if you want to build the code yourself)
- clang (or any C++23 compiler)
- C++23
- [Magic enums](https://github.com/Neargye/magic_enum) (Optional, for printing the token kinds)
# Build
To build, execute `cmake .` under the same directory as CMakeLists.txt
# Run
Run the executable with the command
> The current state of the compiler can only be able to parse expressions! If your source file contains anything other than just expressions, it will not parse the program and throw an error.
```
./<executable> source_file.spl
```
