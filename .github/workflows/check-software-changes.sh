#!/bin/bash

# Copyright (C) 2022 Andrea Ballestrazzi

# This is the input file where the file changes are.
input_file="./bin/$1_changes.txt"

# Now we load the files lines (the changed files) and if we find
# files whose path starts with src/$1 then we increment the software
# version.
while IFS= read -r line
do
    echo "$line"
done < "$input_file"
