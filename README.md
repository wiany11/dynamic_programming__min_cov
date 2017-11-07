# dp-mincov
Dynamic Programming: Computing a Cover of Minimum Cost

### Introduction

This was a school project of the course Introduction Algorithms. I implemented a dynamic programming algorithm to find at least one sensor sets that cover all targets with the minimum cost in polynomial time. More detailed problem description is right below.

I did my best and was satisfied with the result. So I want to share it!

### Problem Description

Consider a straight line **L** in the plane.
A finite set **T** of targets are located above the line **L**, and a finite set **S** of wireless sensors are located below the line **L**.
A sensor **s** can monitor a target **t** if and only if the Euclidean distance between **s** and **t** is at most one.
Suppose that each sensor **s**∈**S** has a positive cost **c(s)** and each target **t**∈**T** can be monitored by at least
one sensor in **S**.
Consider a subset **S'** of sensors in **S**. **S'** is said to be a cover if each target in **T** is covered by at least one
sensor in **S'**.
The cost of **S'** is the total costs of the sensors in **S'**.
The objective is to compute a cover **S'** of minimum cost.
Please develop a polynomial time algorithm and write a program to implement it.

![prob-desc](https://github.com/wiany11/dp-mincov/blob/master/img/01.JPG)

In the picture above, for example, target (green dots) can be covered by both sets {1, 2, 3, 4} and {1, 3, 4}. Then the algorithm should take the set {1, 3, 4} because it obviously costs less. (Cost of sensors are given in parentheses.)

### My Solution

I designed a table like the below:

![sol-01](https://github.com/wiany11/dp-mincov/blob/master/img/02.JPG)

Rows represent targets ordered by x-coordinates and columns represent sensors also ordered by x-coordinates. More information about how to fill the table is described in the pdf file. After filling the table, covers (sensor sets which can monitor all targets) can be found like the following:

![sol-02](https://github.com/wiany11/dp-mincov/blob/master/img/02-2.JPG)![sol-03](https://github.com/wiany11/dp-mincov/blob/master/img/02-3.JPG)

Unless there are exponentially many covers the algorithm takes polynomial time. So it takes exactly polnomial time to find only one cover.

### Implementation

```
$ g++ -o Mincov Mincov.h Mincov.cpp Main.cpp
$ ./Mincov [input file]
```

There are 6 sample input files in *input* directory.
