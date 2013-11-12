#!/bin/sh

make
./tetra -v tst.tetra
./tetra -v < tst.tetra
./tetra -vf tst.tetra
./tetra -vf tst.tetra tst.tetra
