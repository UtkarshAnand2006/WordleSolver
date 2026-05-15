# 🧠 Wordle Solver — Information Theory Meets Search Optimization

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg"/>
  <img src="https://img.shields.io/badge/Algorithm-Entropy%20Optimization-success"/>
  <img src="https://img.shields.io/badge/Game%20Theory-Wordle-orange"/>
  <img src="https://img.shields.io/badge/Complexity-Precomputed-purple"/>
  <img src="https://img.shields.io/badge/Architecture-Modular-red"/>
</p>

---

# 🚀 Overview

This project is a **high-performance Wordle solving engine** built completely from scratch in modern C++.

Unlike naive frequency-based solvers, this system treats Wordle as an **information extraction problem** and uses concepts from:

- 📊 **Shannon Entropy**
- 🌳 **Decision Tree Optimization**
- ⚡ **State Space Reduction**
- 🧮 **Pattern Encoding**
- 💾 **Precomputation & Compression**
- 🎯 **Expected Information Gain Maximization**

The solver continuously chooses guesses that maximize the amount of information extracted from the hidden answer space.

---

# 🎯 Core Idea

Every Wordle guess partitions the remaining candidate set into groups.

A _good guess_ is one that creates the **most balanced partitioning**, because balanced splits maximize information gain.

This project models that mathematically using:

\[
Entropy = -\sum p_i \log_2(p_i)
\]

Where:

- \(p_i\) = probability of receiving a particular response pattern
- Higher entropy ⇒ more information gained
- More information ⇒ faster convergence to the answer

---

# 🧠 Pattern Encoding System

Instead of using Wordle’s traditional:

- 🟩 Green
- 🟨 Yellow
- ⬛ Black

this solver internally uses:

| Symbol | Meaning                          |
| ------ | -------------------------------- |
| `g`    | Correct letter, correct position |
| `y`    | Correct letter, wrong position   |
| `x`    | Letter absent                    |

Example:

```txt
Guess  : CRANE
Answer : CREAM

Pattern: ggyxy
```

This encoding is intentionally chosen because:

- Faster to serialize
- Easier to debug
- Compact for storage
- Cleaner for bitwise/integer conversion

---

# ⚡ Massive Precomputation

The computational bottleneck in Wordle solving is repeatedly computing:

```txt
guess × answer → pattern
```

This project eliminates runtime recomputation by generating a full:

# 📦 Pattern Lookup Table

```txt
(patterns.bin)
```

containing:

```txt
ALL_GUESSES × ALL_ANSWERS
```

precomputed pattern values.

---

## 🔢 Scale

| Metric            | Value       |
| ----------------- | ----------- |
| Allowed guesses   | ~14,855     |
| Possible answers  | ~2,315      |
| Total evaluations | ~34 million |
| Runtime lookup    | O(1)        |

---

# 🏗️ Architecture

```txt
wordleSolver/
│
├── include/
│   ├── entropy.hpp
│   ├── pattern.hpp
│   ├── solver.hpp
│   └── utils.hpp
│
├── src/
│   ├── entropy.cpp
│   ├── pattern.cpp
│   ├── solver.cpp
│   └── main.cpp
│
├── data/
│   ├── answers.txt
│   ├── words.txt
│   └── patterns.bin
│
├── scripts/
│   └── generatePatterns.cpp
│
├── CMakeLists.txt
└── README.md
```

---

# 🔥 Solving Strategy

For every turn:

1. Maintain a valid candidate set
2. Simulate every possible guess
3. Compute response distributions
4. Calculate entropy of each guess
5. Select guess with maximum expected information gain
6. Filter candidates
7. Repeat until solved

---

# 🧮 Entropy Computation

For a guess:

```txt
SLATE
```

against remaining candidate answers:

```txt
{CRANE, PLANE, FLAME, ...}
```

the solver computes all possible response patterns:

```txt
ggxxx
gyxxy
xxxxx
...
```

Then builds a probability distribution:

\[
p(pattern) = \frac{\text{count(pattern)}}{\text{remaining candidates}}
\]

Finally:

\[
H = -\sum p \log_2 p
\]

The guess with maximum entropy is selected.

---

# 🧠 Key Insight

The best Wordle guess is not necessarily the one most likely to be correct.

It is the one that maximally reduces uncertainty.

This solver therefore prioritizes:

> Information gain over immediate correctness.

A high-entropy guess may itself be unlikely to be the answer,
yet still be mathematically optimal because it partitions the search space most efficiently.

---

# ⚙️ Performance Optimizations

## ✅ Precomputed Pattern Cache

Avoids repeated expensive comparisons.

---

## ✅ Integer Pattern Encoding

Patterns are compressed into base-3 integers:

| State | Value |
| ----- | ----- |
| x     | 0     |
| y     | 1     |
| g     | 2     |

Example:

```txt
gyxgx
↓
21020(base3)
```

This enables:

- compact storage
- cache-friendly access
- fast comparisons

---

## ✅ Candidate Pruning

After each guess:

```txt
candidate survives iff:

pattern(guess, candidate)
==
observed_pattern
```

This rapidly shrinks the search space.

---

# 📈 Expected Performance

| Metric              | Value                 |
| ------------------- | --------------------- |
| Average solve depth | Very low              |
| Worst-case depth    | Typically ≤ 6         |
| Lookup complexity   | O(1)                  |
| Solver strategy     | Information-theoretic |

---

# 🧪 Example Run

```txt
Best Guess : slate
Pattern    : xgyxx

Remaining Candidates : 42

Best Guess : crone
Pattern    : gggxg

Remaining Candidates : 2

Answer : crane
```

---

# 💡 Why This Project Is Interesting

This is NOT just a game solver.

It demonstrates understanding of:

- Information Theory
- Search Space Optimization
- Efficient Data Representation
- Precomputation Tradeoffs
- Algorithmic Design
- Probabilistic Decision Making
- High-performance C++ Architecture

---

# 🔬 Concepts Explored

- Shannon Entropy
- Greedy Information Gain
- Pattern Compression
- Lookup Table Design
- Decision Trees
- State Reduction
- Branch Partitioning
- Candidate Elimination
- Cache-Friendly Computing

---

# 🛠️ Build Instructions

## Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

Run:

```bash
./wordleSolver
```

---

# 📂 Dataset

This project uses:

- Official Wordle answer list
- Extended valid guess dictionary

---

# ⭐ Final Note

This solver treats Wordle not as a vocabulary game…

…but as an **information extraction engine**.

Every guess is a carefully optimized experiment designed to maximally reduce uncertainty.
