Tetra Parallel Programming Language
===================================

Tetra is a high-level parallel programming language thatis designed to make
parallel programs easier to write, visualize, debug, and optimize. Its goals
are to make parallel programming easier for students and professionals alike.

It combines a friendly, Python-like syntax, static typing, powerful data
structures, and built-in parallelism. It comes with a graphical development
environment with several unique features.

About
-----
Tetra is still a work in progress, but will combine the following components:

- A programming language geared towards making parallel programming as easy as
possible.  The syntax is based on Python, but it includes static typing with
local type inference, and garbage collection.

- A development environment for writing Tetra programs including an integrated
simulator. The simulator will provide advanced tools for visualizing and
debugging programs including the ability to see code executing on multiple
(virtual) cores simultaneously, pause cores, set the relative speed of cores,
and the ability to save and replay execution traces. These abilities will aid
the user in being able to discover and correct subtle bugs that would otherwise
be non-deterministic and thus hard to find.

- A compiler that compiles Tetra code down to efficient low-level parallel code
that can be run on a variety of parallel architectures.

Requirements
------------

1. Bison.
2. A C++ compiler (clang++ is default).
3. make
4. graphviz for automatic tree diagrams.

Tetra is licensed under the MIT license.

See http://tetra-lang.org for more details.
