Tetra Parallel Programming Language
===================================

Tetra is a high-level parallel programming language that is designed to make
parallel programs easier to write, visualize, debug, and optimize. Its goals are
to make parallel programming easier for students and professionals programmers
alike.

It combines a friendly, Python-like syntax, static typing, powerful data
structures, and built-in parallelism. It comes with a graphical development
environment.

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

Installation
------------

Tetra has thus far only works on Linux systems.  To install:

1. Download Tetra, either by cloning the repository, or by downloading a zip file.
2. Install dependencies
```
sudo apt-get install build-essential clang bison qt5-default
```
3. Compile it:
```
cd tetra
./build
```

You should now be able to run the command line interpreter as bin/tetra or the
IDE as bin/tetraide.

More Information
----------------

Tetra is licensed under the MIT license.

See http://tetra-lang.org for more details.

