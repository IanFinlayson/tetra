#!/bin/sh

make
./tetra -v test.ttr
./tetra -v < test.ttr
./tetra -vf test.ttr
./tetra -vf test.ttr test.ttr
