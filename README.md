# 📚 Linear Equations System Solver

## 📌 Overview
This program is written in **C++** and designed to read, parse, and process systems of linear equations. It allows the user to input a set of equations as text strings, then provides an interactive command-line interface to perform algebraic operations on them (like addition, subtraction, and substitution) and solve the entire system using **Cramer's Rule**.

## 🛠️ Dependencies
The code relies on standard C++ libraries:
* `<iostream>` / `<sstream>`: For input/output operations and string stream parsing.
* `<vector>`: For storing dynamic arrays.
* `<string>`: For handling equation texts and commands.
* `<map>`: To store the coefficients of the variables.
* `<set>`: To extract unique variable names and sort them alphabetically.
* `<cmath>` / `<cstdlib>` / `<cctype>`: For mathematical operations and string conversions.

## 🏗️ Core Architecture

### 1. The `Equation` Class
This class represents a single linear equation:
* **Attributes:**
  * `variables`: A `map` connecting each variable's name to its coefficient.
  * `constant_value`: The constant term on the right-hand side of the standard form equation.
* **Parsing Methods:** Processes strings to extract terms, coefficients, and variables while handling signs correctly.
* **Algebraic Methods:** Includes functions to `add`, `subtract`, `scale`, and `substitute` equations.

### 2. Math Functions
* `get_all_vars`: Returns a set of all unique variables across the system.
* `determinant`: A recursive function that calculates the determinant of a square matrix using Cofactor Expansion.

## 💻 Command Line Interface (CLI)

The program runs an interactive loop accepting the following commands:

| Command | Description |
|---|---|
| `num_vars` | Prints the total number of unique variables. |
| `equation i` | Prints equation number `i` in standard form. |
| `column varName` | Prints the coefficient of `varName` across all equations. |
| `add i j` | Adds equation `i` to equation `j`. |
| `subtract i j` | Subtracts equation `j` from equation `i`. |
| `substitute var i j` | Eliminates `var` from equation `i` using equation `j`. |
| `D` | Prints the main coefficient matrix. |
| `D varName` | Prints the coefficient matrix replacing the `varName` column with constants. |
| `D_value` | Prints the Determinant of the main matrix. |
| `solve` | Solves the system using Cramer's Rule. |
| `quit` | Exits the program. |

## 🧮 Solving Algorithm (Cramer's Rule)
The `solve` command relies entirely on **Cramer's Rule** to solve the linear system. The algorithmic steps are as follows:

1. It builds a square matrix containing all the variable coefficients, and a separate vector for the right-hand side constants.
2. It calculates the main determinant of the coefficient matrix ($D$).
3. If $D$ is extremely close to zero, the program prints `No Solution` because a unique solution does not exist.
4. To find the value of any variable, a modified matrix is created by replacing its column with the constants vector.
5. The determinant of this modified matrix ($D_{x_i}$) is calculated.
6. The variable's value is calculated as: 

$$x_i = \frac{D_{x_i}}{D}$$

The program prints the calculated value for each variable in alphabetical order.
