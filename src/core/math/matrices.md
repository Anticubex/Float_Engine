# Implementation details for matrix operations

#### I noticed I was creating massive comment blocks explaining the algorithms, so I'm moving them into here

#### It's both easier and latex is prettier than comment ascii

<br>

# Table of contents

1. [Multiplication](#multiplication)

<br>
<br>
<br>

# Multiplication <a name="multiplication"></a>

### The multiple algorithms I've been experimenting with

[Matrix Multiplication (Wikipedia)](https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm)

---

### Basic Iterative Algorithm

#### Commonly refered to as "naive" or "bruteforce"

```C++
Matrix Matrix::naive_Iter_mul(const Matrix &A, const Matrix &B);
```

Basic iterative algorithm:

$${\displaystyle c_{ij}=\sum _{k=1}^{m}a_{ik}b_{kj}.}$$

---

### Strassen's Decomposition

#### Was unbeaten for like 40 years or smth

```C++
Matrix Matrix::square_strassens_mul(const Matrix &A, const Matrix &B);
```

Core idea:

$$
C = {\begin{pmatrix}c_{1} & c_{2}\\c_{3}&c_{4}\\\end{pmatrix}},\,
A = {\begin{pmatrix} a & b \\ c & d \\\end{pmatrix}},\,
B = {\begin{pmatrix} e & f \\ g & h \\\end{pmatrix}},
$$

$$
{\begin{pmatrix}c_{1} & c_{2}\\c_{3}&c_{4}\\\end{pmatrix}}=
{\begin{pmatrix} a & b \\ c & d \\\end{pmatrix}}
{\begin{pmatrix} e & f \\ g & h \\\end{pmatrix}}=
{\begin{pmatrix} ae+bg & af+bh \\ ce+dg & cf+dh \\\end{pmatrix}}
$$

Both when a,b,c,d,e,f,g,h are matrices _and_ scalars.
Thus, one can divide the matrices into submatrices and solve recursively,
given that the matrices are square and powers of two (or less effectively simply even numbers).

A second possible optimization can be achieved like so:

$ ae + bg = (a + d)(e + h) + d(g - e) - (a + b)h + (b - d)(g + h) $ <br>
$ af + bh = a(f - h) + (a + b)h $ <br>
$ ce + dg = (c + d)e + d(g - e) $ <br>
$ cf + dh = a(f - h) + (a + d)(e + h) - (c + d)e - (a - c)(e + f) $ <br>

This allows use to use only 7 multiplications:

$p_1 = (a + d)(e + h)$ <br>
$p_2 = d(g - e)$ <br>
$p_3 = (a + b)h$ <br>
$p_4 = (b - d)(g + h)$ <br>
$p_5 = a(f - h)$ <br>
$p_6 = (c + d)e$ <br>
$p_7 = (a - c)(e + f)$ <br>

And of course, the extra additions are trivial compared to matrix multiplication.

Especially with the 1-d array mapping, the addition alg used is $\Theta(n×m)$ for an $n×m$ matrix

---

### Non-square Divide and Conquer

#### A generalization of Strassens, faster in practive

```C++
Matrix Matrix::general_DaC_mul(const Matrix &A, const Matrix &B);
```

Splitting matrices in two instead of four allows for non-square matrices

Uses three recursive cases:

- Inputs: matrices $A$ of size $n × m$, $B$ of size $m × p$.
- Base case: if $max(n, m, p)$ is below some threshold, use an unrolled version of the iterative algorithm.
- Recursive Cases:

     1. If $max(n, m, p) = n$, split $A$ horizontally: <br>
        $C={\begin{pmatrix}A_{1}\\A_{2}\end{pmatrix}}{B}={\begin{pmatrix}A_{1}B\\A_{2}B\end{pmatrix}}$

     2. Else, if f $max(n, m, p) = p$, split $B$ vertically: <br>
        $C=A{\begin{pmatrix}B_{1}&B_{2}\end{pmatrix}}={\begin{pmatrix}AB_{1}&AB_{2}\end{pmatrix}}$

     3. Otherwise, $max(n, m, p) = m$. Split $A$ vertically and $B$ horizontally <br>
        $C={\begin{pmatrix}A_{1}&A_{2}\end{pmatrix}}{\begin{pmatrix}B_{1}\\B_{2}\end{pmatrix}}=A_{1}B_{1}+A_{2}B_{2}$

---
