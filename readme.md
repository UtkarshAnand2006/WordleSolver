# Wordle Solver

A C++17 command-line assistant that recommends Wordle guesses using Shannon entropy. It combines a precomputed feedback table with candidate elimination to choose the next guess from 14,855 allowed words.

## Run locally

### 1. Prerequisites

- Git, or a downloaded and extracted copy of this repository.
- A C++17 compiler: GCC, Clang, or Microsoft Visual C++.
- CMake 3.10 or newer for the CMake build below. A direct GCC build is also provided.

No external C++ libraries, Python environment, API keys, or network connection are required at runtime. The word lists and precomputed table are included.

### 2. Get the source

```bash
git clone https://github.com/UtkarshAnand2006/WordleSolver.git
cd WordleSolver
```

Run all subsequent commands from this **repository root**, where `data/` and `CMakeLists.txt` are located.

### 3. Build and launch

Use a fresh directory named `build-local`. The repository's existing `build/` folder contains generated Windows/Visual Studio files and a machine-specific CMake cache; it should not be reused on another machine.

**Windows — PowerShell with Visual Studio C++ Build Tools installed**

Install the **Desktop development with C++** workload and CMake, then run:

```powershell
cmake -S . -B build-local
cmake --build build-local --config Release
.\build-local\Release\wordle.exe
```

These commands assume CMake selects a Visual Studio generator. If the compiler is not detected, use a Developer PowerShell for Visual Studio. With a single-configuration generator such as Ninja or MinGW Makefiles, configure with `-DCMAKE_BUILD_TYPE=Release` and launch `.\build-local\wordle.exe` instead.

**Linux / macOS — GCC or Clang with CMake**

```bash
cmake -S . -B build-local -DCMAKE_BUILD_TYPE=Release
cmake --build build-local --config Release
./build-local/wordle
```

**Alternative: build directly with GCC**

This avoids CMake and compiles the same five sources used by the CMake target:

```bash
g++ -O2 -std=c++17 -Iinclude src/main.cpp src/entropy.cpp src/solver.cpp src/loader.cpp src/encode.cpp -o wordle
./wordle
```

On Windows with MinGW, use `-o wordle.exe` and launch `.\wordle.exe`.

> Keep the working directory at the repository root when launching the executable. The program opens `data/guesses.txt`, `data/answers.txt`, and `data/patterns.bin` using relative paths. Do not change into the build directory before running it.

### 4. Use the solver

1. Enter the recommended word in your Wordle game, or choose another word from `data/guesses.txt`.
2. At `Enter your guess:`, type the exact five-letter **lowercase** word you played. The recommendation is not submitted automatically.
3. At `Enter feedback (g/y/x):`, type exactly five lowercase symbols, from left to right:

   | Symbol | Tile | Meaning |
   | --- | --- | --- |
   | `g` | Green | Correct letter in the correct position |
   | `y` | Yellow | Letter present, but in another position |
   | `x` | Gray | No remaining occurrence of that letter after green/yellow matches are accounted for |

4. Repeat with the updated recommendation. The program prints every remaining candidate after filtering.

When one candidate remains, the solver reports the answer and exits; you may still need to enter that word in the game. Entering `ggggg` also ends the session. Press `Ctrl+C` to stop and restart the executable for a new puzzle.

**Example using the included data, with `crane` as the hidden answer**

The candidate lists are omitted below for readability.

```text
Best guess: tarse
Enter your guess: tarse
Enter feedback (g/y/x): xyyxg
Remaining candidates: 22

Best guess: drack
Enter your guess: drack
Enter feedback (g/y/x): xggyx
Remaining candidates: 3

Best guess: anvil
Enter your guess: anvil
Enter feedback (g/y/x): yyxxx
Remaining candidates: 1
The answer is: crane
Congratulations! You've found the answer!
```

### Troubleshooting

| Problem | What to check |
| --- | --- |
| CMake or the compiler is not found | Install the prerequisite and ensure it is on `PATH`; reopen the terminal afterward. |
| CMake reports an old source path or a generator mismatch | Configure into a fresh build directory rather than the committed `build/` directory. |
| The executable cannot be found | The target is named `wordle`. Visual Studio Release builds normally place it in `build-local/Release/`; single-configuration builds place it in `build-local/`. |
| Startup fails, throws an allocation error, or crashes | Confirm the working directory is the repository root and all three data files are present. File-loading errors are not checked by the current implementation. |
| No candidates remain | Check the word and feedback, especially repeated letters. The hidden answer must also belong to the bundled answer list. Restart after correcting a mistaken turn. |
| A word is rejected by the game | The bundled dictionary may differ from the game's current accepted words. Choose another word present in `data/guesses.txt` and enter the feedback for that word. |

The current interface does not validate guesses or feedback. An unknown guess can cause an out-of-bounds lookup, and malformed feedback can produce incorrect results. Use exact dictionary words and five symbols from `g`, `y`, and `x`.

## Contents

- [How it works](#how-it-works)
- [Feedback encoding and repeated letters](#feedback-encoding-and-repeated-letters)
- [Implementation](#implementation)
- [Data and precomputation](#data-and-precomputation)
- [Complexity and tradeoffs](#complexity-and-tradeoffs)
- [Current scope](#current-scope)

## How it works

Wordle can be viewed as a sequence of experiments: each guess produces feedback that partitions the possible answers into groups. A useful guess distinguishes many of those answers, even when the guess itself is not a possible solution.

Let $C$ be the remaining answer set, and let $C_r(w)$ contain the candidates that produce feedback pattern $r$ for guess $w$. The solver treats each remaining candidate as equally likely:

$$
p_r(w) = \frac{|C_r(w)|}{|C|}.
$$

It scores the guess using Shannon entropy, measured in bits:

$$
H(w) = -\sum_{r:\,p_r(w)>0} p_r(w)\log_2 p_r(w).
$$

Higher entropy means a more informative distribution of possible responses. For example, a split of eight candidates into groups of four and four gives one bit of information; a split into groups of seven and one gives about 0.544 bits.

With a uniform candidate prior and deterministic feedback, maximizing this entropy is equivalent to minimizing the expected logarithm of the remaining candidate count:

$$
H(w)=\log_2|C|-\sum_r p_r(w)\log_2|C_r(w)|.
$$

After observing feedback $r_{\mathrm{obs}}$, the update is:

$$
C \leftarrow \{a\in C:\operatorname{pattern}(w,a)=r_{\mathrm{obs}}\}.
$$

The implementation evaluates **all allowed guesses** at each turn. It uses a greedy, one-step information objective: it does not search a complete decision tree or directly minimize the total number of guesses. An informative probe may therefore be recommended even when it cannot be the answer.

## Feedback encoding and repeated letters

Each tile is a ternary digit: `x = 0`, `y = 1`, and `g = 2`. For digits $d_0,\ldots,d_4$ in left-to-right order:

$$
\operatorname{code}=\sum_{i=0}^{4}d_i3^{4-i}.
$$

There are $3^5=243$ possible codes, from 0 to 242, so one `uint8_t` stores a complete feedback pattern.

| Feedback | Base-3 digits | Decimal code |
| --- | --- | --- |
| `xxxxx` | `00000` | 0 |
| `ggyxy` | `22101` | 226 |
| `gyxgx` | `21020` | 195 |
| `ggggg` | `22222` | 242 |

For example, guessing `crane` against `cream` produces `ggyxy`.

The precomputation code handles repeated letters in two passes:

1. Count letters in the answer, mark exact matches green, and consume those occurrences.
2. For each unmatched position, mark yellow only when an unused occurrence remains; consume it as well. Otherwise, leave the tile gray.

This prevents extra copies of a letter from receiving yellow tiles after all available occurrences have already been matched.

## Implementation

| File | Responsibility |
| --- | --- |
| `CMakeLists.txt` | Builds the C++17 executable `wordle`. |
| `include/constants.hpp` | Defines 14,855 guesses, 2,315 answers, and 243 feedback patterns. |
| `src/main.cpp` | Loads data, initializes candidates, prompts for input, and controls the solving loop. |
| `src/loader.cpp`, `include/loader.hpp` | Load whitespace-separated word lists and the binary pattern table. |
| `src/entropy.cpp`, `include/entropy.hpp` | Count response buckets and calculate Shannon entropy for a guess. |
| `src/solver.cpp`, `include/solver.hpp` | Select the highest-entropy guess and filter candidates by observed feedback. |
| `src/encode.cpp`, `include/encode.hpp` | Convert user feedback to the corresponding ternary code. |
| `src/precomputation.cpp` | Standalone program that generates the binary table; excluded from the main CMake target. |
| `data/guesses.txt` | Ordered dictionary of allowed guesses. |
| `data/answers.txt` | Ordered list of possible hidden answers. |
| `data/patterns.bin` | One precomputed byte for each guess–answer pair. |

### Runtime flow

1. Load both dictionaries and the pattern table into memory.
2. Initialize candidates to every answer index.
3. If no candidates remain, report an inconsistency and stop. If one remains, report it and stop.
4. Score all guesses against the current candidates and recommend the highest-entropy word.
5. Read the user's actual guess and feedback.
6. Stop on `ggggg`; otherwise, retain only candidates whose cached pattern matches the feedback, print them, and repeat.

`bestGuess` updates its selection only when a score is strictly greater than the current maximum. Equal computed scores therefore retain the earliest word in dictionary order; there is no explicit preference for a remaining answer.

## Data and precomputation

The included files have the following dimensions:

| Item | Size |
| --- | --- |
| Allowed guesses ($G$) | 14,855 words |
| Possible answers ($A$) | 2,315 words |
| Pattern table ($G\times A$) | 34,389,325 entries |
| Binary table storage | 34,389,325 bytes, approximately 32.80 MiB |

The table is a flat, guess-major array. The pattern for guess index `g` and answer index `a` is read as:

```cpp
patterns[g * NUM_ANSWERS + a]
```

The word-list order is part of the file format: reordering either dictionary invalidates the table. `patterns.bin` contains raw bytes with no header, version, dimensions, or checksum.

The bundled files define the vocabulary used by this solver. They should not be assumed to track the current Wordle answer list or accepted-guess dictionary.

### Regenerate the table

**This is optional for an unchanged checkout:** `data/patterns.bin` is already included.

The generator has its own `main()` and must be compiled separately. From the repository root, using GCC:

```bash
g++ -O2 -std=c++17 src/precomputation.cpp -o precompute
./precompute
```

On Windows with MinGW, use `-o precompute.exe` and run `.\precompute.exe`. The generator prints progress every 100 guesses and overwrites `data/patterns.bin`.

If you change the dictionaries:

1. Keep every word five letters long and lowercase (`a`–`z`), and ensure every possible answer is also an allowed guess.
2. Update `NUM_GUESSES` and `NUM_ANSWERS` in `include/constants.hpp` and the matching `G` and `A` values in `src/precomputation.cpp`.
3. Recompile and run the generator to regenerate the table using the new list order.
4. Rebuild the solver.

Both programs assume valid input files with the configured dimensions. Keep the dictionaries, constants, and binary table synchronized.

## Complexity and tradeoffs

Let $G$ be the number of allowed guesses, $A$ the original answer count, $N$ the remaining candidate count, and $P=243$ the number of possible patterns.

| Operation | Time | Additional working space |
| --- | --- | --- |
| Generate all patterns | $O(GA)$ for fixed five-letter words | $O(GA)$ for the table |
| Read one cached pattern | $O(1)$ | $O(1)$ |
| Score one guess | $O(N+P)$ | $O(P)$ |
| Choose the next guess | $O(G(N+P))$ | $O(P)$ for scoring |
| Filter candidates | $O(N)$ | $O(N)$ for the filtered set |

The interactive program also performs a linear dictionary search to locate the word entered by the user, costing up to $O(G)$ comparisons for fixed-length words. Loading the binary table takes $O(GA)$ time and memory; word lists and candidates require additional storage.

Precomputation trades disk space and startup memory for constant-time feedback lookup. As candidates are eliminated, entropy scoring becomes cheaper. Release builds or compiler optimization such as `-O2` are useful because every recommendation evaluates the full guess dictionary.

No exhaustive solve-depth benchmark is included. Average guesses, worst-case guesses, and wall-clock timings are therefore not claimed here.

## Current scope

- Interactive terminal assistant with manually supplied feedback; no browser automation or graphical interface.
- Fixed five-letter words, fixed dictionary dimensions, and a uniform prior over remaining answers.
- Recommendations may fall outside the remaining answer set, so Wordle hard-mode restrictions are not enforced.
- No six-turn limit, multi-step lookahead, or guarantee of solving within six guesses.
- No input validation, file-integrity checks, undo command, or explicit end-of-input handling.

Potential extensions include validated input, automatic dimension discovery, cache metadata, candidate-aware tie-breaking, hard-mode support, and an exhaustive benchmark over the bundled answers.
