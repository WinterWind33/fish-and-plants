#!/bin/bash

# Copyright (C) 2022 Andrea Ballestrazzi

# This is the input file where the file changes are.
input_file="./tmp/$1_changes.txt"

# Now we load the files lines (the changed files) and if we find
# files whose path starts with src/$1 then we increment the software
# version.
changes_found=false
while IFS= read -r line
do
    if [ [["$line" == "src/$1/"*]] ]
    then
        echo "Found!"
        changes_found=true
        break
    fi
done < "$input_file"

if [ "$changes_found" = false ] ; then
    echo "[INFO] => No changes found for this component."
fi
