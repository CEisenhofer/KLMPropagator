## KLM User-Propagator

This tool implements the semantics of KLM (Variants: C, CL, CM, P, and M) via user-propagation in the Z3 SMT solver.

## Build

The build process was tested successfully built on Windows (VC++ 2022) and Linux (gcc-11.4).

Requirements apart from a C++ 17 compiler:
- cmake
- [Z3](https://github.com/Z3Prover/z3/releases)
- Optional: [graphviz](https://graphviz.org/download/)

### Instructions

Open a console-window at some writeable location, clone the git-repository and navigate into the cloned repo 

```
git clone https://github.com/CEisenhofer/UP_KLM.git
cd UP_KLM
```

create a folder for the compiled output and run cmake (here for Linux)

```
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release -DZ3_INCLUDE_DIR="[path-to-z3]/include" -DZ3_LIBRARIES="[path-to-z3]/bin/libz3.so" ..
cmake --build .
```

Replace "[path-to-z3]" by the path to the downloaded z3 library.
The compiled program will be likely put in the sub-directory `src` of the directory where you invoked `cmake`

Note: The extension for Windows is ".lib" and for MAC is ".dylib". On Windows make sure that you download the correct version (x86 vs x64 - you probably want x64)

In case you want to also compile Z3 from source, make sure that you install it or you copy `z3++.h` and `z3_version.h` in the same directory as the other API-header files!
You do _not need_ to have yacc/flex installed. The parser files are precompiled and OS-independent. If you want to change the parser, you can install the two programs and use `gen_parser.sh` to update the files.

## Usage

You can run the program by calling it via command-line arguments.
```
KLM -l=[C|CL|CM|P|M] -t=ms --smtlib --visual path-to-input-file
```

This will solve the problem given in path `path-to-input-file`. The result will be output to console.
Additional arguments are:

- `-l` defines the KLM variant to be used. e.g., `-l=CM` makes the solver use the axioms of `CM`
- `-t` defines the timeout for solving the problem in milliseconds. e.g., `-t=1000` gives to solver 1000 milliseconds to solve the problem
- `--smtlib` makes the solver parse the input-file as an SMTLIB2 file rather than the custom input format (see below)
- `--visual` makes the solver output a graph witnessing the satisfiability of the input (see below)

### Model Visualization

In case the argument `--visual` is passed, the solver creates a file called `output-graph.dot` in the sanme directory as the tool.
You can compile this file into a visualization of the counterexample with graphviz.

Use via command-line
```
dot -Tpdf output-graph.dot -o output-graph.pdf
```
to generate a PDF `output-graph.pdf` showing the model.
Note that graphviz is a separate program that has to be installed separately.
On Linux this can be done for example via: `sudo apt-get install graphviz`.

### Input Language

You can represent problems in two ways. A custom input-language or SMTLIB.

#### Custom Input Language
In the custom syntax each line in the file is one pair `(F1, F2)` where `F1` and `F2` are propositional formulas.
Use `~` for negation, `&` for conjunction, `|` for disjunction, `=>` for material implication, `=` for material equivalence and brackets as usual.
There is no operator precedence, so use brackets!
Each line consists of one tuple `(F1, F2)` which is written as `F1 -> F2` in case the tuple exists and 
`F1 -/> F2` in case it does not exist.

Example
```
(A & B) => C -> B
~B -> C | A
B -/> A & C
```

This corresponds to the query if the two pairs
```
((A & B) => C, B)
(~B, C | A)
```
entail the pair
```
(B, A & C)
```
by refuting the existence of a counterexample.

#### SMTLIB2
We can represent the problem by "misusing" SMTLIB2.

Assume the existence of a predefined function `(declare-fun node (Bool Bool) Bool)`.
The problem before can be written as

```
(declare-const A Bool)
(declare-const B Bool)
(declare-const C Bool)

(assert 
    (and (node (or C (not (and A B))) B)
    (node (not B) (or C A))      
    (not (node B (and A C))))
)
```

Don't worry about the fact that `A`, `B`, and `C` are defined as Boolean constants.
The tool automatically transform the input into another form internally.
