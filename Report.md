# Development and Testing Report of the C−− Language Compiler Front-End

## Abstract

This project implements a compiler front-end for the C−− language using C++, including a **lexical analyzer** and a **syntax analyzer**.

The lexical analysis is implemented through token classification, construction of a **non-deterministic finite automaton (NFA)**, **NFA determinization**, and **DFA minimization**.

The syntax analysis is implemented by constructing the **FIRST set**, **FOLLOW set**, and **predictive parsing table** of the C−− grammar, ultimately generating a **shift-reduce parsing sequence**.

After completing the project, when a C−− source code file is provided as input:

* The lexical analyzer produces the **token sequence** and **symbol table**.
* The syntax analyzer produces the **FIRST set**, **FOLLOW set**, and the **shift-reduce sequence** of the source code.

**Keywords:** lexical analysis, syntax analysis, compiler

---

# Chapter 1 Introduction

This chapter mainly introduces the overall description of the compiler principle course project, including **problem description**, **requirement analysis**, and the **general solution design**.

---

## 1.1 Problem Description

The project requires implementing a **compiler front-end for the C−− language**, including:

* lexical analyzer
* syntax analyzer
* semantic analysis
* intermediate code generation

The tasks include:

1. Implement a lexical analyzer using **automata theory**.
2. Implement a syntax analyzer using either **top-down** or **bottom-up parsing methods**.
3. Complete the implementation of **intermediate code generation**.

---

## 1.2 System Architecture Design

The workflow of the system is shown in Figure 1-1.

From the overall perspective:

* The **lexical analyzer** is at the top level.
* It reads **source code** as input and produces a **token sequence**.
* The token sequence is passed to the **syntax analyzer**, which generates the **syntax tree** and the **predictive parsing table**.
* The traversal sequence of the syntax tree is then passed to the **intermediate code generator**, which compiles it into **LLVM IR intermediate representation**.

---

## 1.3 Requirement Analysis

The requirements can be divided into three parts:

1. **Token types in the C−− language**
2. **Grammar operations supported by the language**
3. **Intermediate code generation**

---

### 1.3.1 C−− Token Types

#### Keywords (KW)

(case-insensitive)

* int
* void
* return
* const

#### Operators (OP)

* `+`
* `-`
* `*`
* `/`
* `%`
* `=`
* `>`
* `<`
* `==`
* `<=`
* `>=`
* `!=`
* `&&`
* `||`

#### Separators (SE)

* (
* )
* {
* }
* ;
* ,

#### Identifiers (IDN)

Identifiers follow the same rules as in C language:

* composed of letters, digits, and underscore `_`
* must **not begin with a digit**

#### Integers (INT)

Regular expression:

```
INT = -?(0 | [1-9][0-9]*)
```

---

### 1.3.2 Grammar Operations Supported

The language supports the following constructs:

1. Constant declaration
   `int a;`

2. Constant initialization
   `int a = 3;`

3. Constant type definition
   `const int a = 3;`

4. Variable assignment
   `a = 3;` or `a = b;`

5. Function definition
   `int add(...)`

6. Function parameter management
   `int add(int a, int b)`

7. Unary arithmetic expressions
   `int a = 3 + 4;`

8. Unary operators
   `+`, `-`, `!`

9. Compound expressions
   `a = (b + c) * d`

10. Relational operations
    `> >= < <=`

11. Binary logical operations
    `|| && == !=`

---

### 1.3.3 LLVM Code Generation

Semantic analysis is constructed during syntax analysis rules related to expressions.

Each expression represents a semantic rule.

The intermediate representation is generated using **quadruples** in **postfix form**.

The system:

* creates new temporary variables
* stores them in the symbol table
* performs symbol table lookup operations
* generates intermediate code

The compiler backend traversal functions are provided by the instructor on **Gitee**.

---


# Chapter 2 Design and Implementation

## 2.1 System Overview

The compiler is implemented using **C++**.

After analyzing the project requirements, the overall design workflow is shown in Figure 2-1.

---

## 2.2 Lexical Analyzer Design

The lexical analyzer is based on:

* **NFA**
* **DFA**
* **Symbol Table**

---

### 2.2.1 NFA

The purpose of the **NFA** is to determine the category of tokens.

It distinguishes:

* INT
* IDN
* UNDEFINED

Important notes:

1. Keywords and operators are detected directly in lexical analysis and **not passed to the automaton**.
2. Negative numbers involve the operator "-" and therefore are handled in **syntax analysis instead of the NFA**.

---

### 2.2.2 DFA

#### Deterministic DFA

After **NFA determinization**, the DFA contains:

| Node  | Meaning                       |
| ----- | ----------------------------- |
| Node0 | start state                   |
| Node1 | integer 0                     |
| Node2 | integers 1–9                  |
| Node3 | identifier starting character |
| Node4 | multi-character identifier    |
| Node5 | integer ≥10                   |

#### Minimized DFA

After minimization, the DFA contains:

| Node  | Meaning     |
| ----- | ----------- |
| Node0 | start state |
| Node1 | integer 0   |
| Node2 | integer ≠0  |
| Node3 | identifier  |

---

### 2.2.3 Symbol Table

The symbol table stores:

* identifier name
* memory location

Each record contains:

* **name**
* **value (address)**

Implementation:

```
unordered_map
```

---

## 2.3 Lexical Analyzer Implementation

Key components include:

* FSM_Node
* FSM
* NFA creation
* NFA → DFA conversion
* DFA minimization

---

## 2.4 Syntax Analyzer Design

The syntax analyzer uses the **LL(1) predictive parsing method**.

LL(1) parsing is a **top-down recursive descent parser**.

---

### 2.4.1 LL(1) Grammar Conditions

A grammar must satisfy:

1. No left recursion
2. No ambiguity
3. No common left factor

---

### 2.4.2 Parsing Process

Steps include:

1. Build grammar rules
2. Construct parsing table
3. Read input tokens
4. Match terminal symbols
5. Expand non-terminal symbols
6. Continue until stack is empty

---

## 2.5 Syntax Analyzer Implementation

Major functions:

* getVnVt()
* getFirst()
* getFollow()
* getTable()
* doAnalysis()

---

### FIRST Set

FIRST(X) contains all terminals that can appear at the beginning of strings derived from X.

---

### FOLLOW Set

FOLLOW(X) contains terminals that may appear immediately after X.

---

### Predictive Parsing Table

Constructed using:

* FIRST sets
* FOLLOW sets

Each table entry corresponds to a production rule.

---

# Chapter 3 Testing and Evaluation

Test cases include:

* four valid programs
* two invalid programs

Testing covers:

* lexical analysis
* automata correctness
* syntax analysis

---

## 3.1 Automata Verification

The following structures are printed:

* NFA
* DFA
* minimized DFA

Results match the theoretical models.

---

## 3.2 Functionality Tests

The lexical analyzer successfully recognizes:

* keywords
* operators
* separators
* identifiers
* integers
* negative numbers

---

## 3.3 Syntax Analysis Tests

Outputs include:

* FIRST sets
* FOLLOW sets
* predictive parsing tables
* shift-reduce sequences

Correct programs produce:

```
accept
```

Incorrect programs produce:

```
error
```

---

# Chapter 4 Conclusion

The completed tasks include:

| Task                     | Status    |
| ------------------------ | --------- |
| Lexical analysis         | Completed |
| NFA determinization      | Completed |
| DFA minimization         | Completed |
| Syntax analysis          | Completed |
| Intermediate code design | Completed |

---

## Experience and Insights

Through the implementation of the C−− compiler front-end, we gained a deeper understanding of:

* lexical analysis
* syntax analysis
* compiler architecture

The project also improved:

* programming ability
* system design skills
* understanding of compiler theory

Using C++ and object-oriented design allowed us to build modular components such as:

* lexical analyzer
* syntax analyzer
* symbol table

Additionally, robust **error handling mechanisms** were implemented to improve compiler reliability.




