#!/bin/bash

# Copy the data files to the current directory
cp ../build/data* .

# And use octave to plot it
octave myplot.m
