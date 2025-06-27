# Description
SMPL will be compiled down to C, which is then again compiled down to machine code by clang.
This project has not been finished yet and won't be supporting OOP until the language is capable of doing tasks using the FP paradigm.

# Goal
SMPL aims to be as simple as possible, much like C but with more type-safety and other safety features.
# Prerequisites
- [CMake](https://cmake.org/download/) (Optional if you want to build the code yourself)
- clang (or any C++23 compiler)
# Build
To build, execute `cmake .` under the same directory as CMakeLists.txt
# Run
Run the executable with the command
```
./<executable> source_file.spl
```
# Example Code
``` C++
// Function definition describes a function with
// two parameters of type i32, and should also return
// a value of type i32
defn pow(base: i32, exp: i32) -> i32 {
    let result: i32 = 0;

    // Range-based for loop
    for i in 0..exp {
        result *= base;
    }

    return result;
}

// Main implicitly has a return type of int
defn main() {
    let x: i32 = 1;
    let y: i32 = x = 64;
    let z: i32 = pow(x, y);
    let explicit_type_cast: u64 = z as u64; // Type-cast with 'as'

    // While loop
    while true {
    }

    if false == not true {
        // True
    }

    // Expression statement
    x;
    true;
    pow(x, y);
    not true; // False

    return 0;
}
```
# Progress
- 🟢 Lexical analysis done (Lexing). It's just a matter of expanding now.
- 🟢 Syntax analysis (Parsing) done for now. Will expand later.
- 🟡 Semantic Analysis done, but needs better error messages.
- Can now generate code!

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
- f32
- f64
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
# Semantic rules
### Declaration Rules

- Identifiers must be defined before use.
Any usage of a variable or function before its declaration is an error.

- Every variable must be initialized. SMPL disallows declaration of variables without values. (e.g., `let x: i32;`)

- Redefinition in the same scope is not allowed.
A variable or function name can only be declared once within the same scope.

- Primitive type names are reserved and cannot be redefined.
Declaring a variable or function with a name matching a built-in type (e.g., i32, bool) is prohibited.


### Typing Rules

- Static and explicitly typed.
Every expression and variable has a type known at compile time.

- Type inference for literals.
Integer literals default to untyped integer (UntypedInt), and float literals to UntypedFloat. They must be assigned to a concrete type via declaration or context.

- Operations must use compatible types.
Binary and unary operators require operands of the correct type (e.g., arithmetic on integers or floats, logical operators on booleans).

- Assignment requires compatible types.
You can only assign an expression to a variable if their types are compatible. Implicit narrowing conversions are not allowed.


### Type-Casting

- `as` is the explicit cast operator.

#### Allowed:

- Numeric to numeric (e.g., i64 as f64)

- Numeric to boolean (e.g., f32 as bool)
Non-zero value evaluates to true, false if 0 or 0.0

- Boolean to numeric (e.g., bool as u8)
True evaluates to 1 or 1.0, false evaluates to 0 or 0.0

#### Not allowed:

- Any non-numeric casting (e.g., array as string)

**Bit-width demotion is allowed but unsafe**.

**It is the programmer’s responsibility to avoid overflows when casting from a larger bit-width to a smaller one.**

### Function Rules

- Functions must be declared before use.

- Parameter names must be unique within the parameter list.
 
- The return type must match the type of the returned expression.

- Return type can be omitted for main().
When omitted, main defaults to returning int.

### Control Flow

- Condition expressions must evaluate to bool.
This includes conditions for if, while, and for.

- Variable bindings in for loops are scoped within the loop block.


### Scope

- Blocks introduce a new scope.
Variables declared in a block (e.g., {}) are not accessible outside that block.
