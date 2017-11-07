# dp-mincov
Dynamic Programming to Compute a Cover of Minimum Cost

### Introduction

This was a school project of the course Introduction Algorithms. I implemented a dynamic programming algorithm to find at lease one sensor set that covers all targets with the minimum cost in polynomial time. More detailed problem description is right below.

I did my best and was satisfied with my work. So I want to share it!

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

### My Solution

