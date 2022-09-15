#!/bin/bash

# Copyright (C) 2022 Andrea Ballestrazzi

echo "[INFO] => Received first parameter: $1"
echo "[INFO] => Received second parameter: $2"

# This is the input file where the file changes are.
input_file="./tmp/$1_changes.txt"

# Now we load the files lines (the changed files) and if we find
# files whose path starts with src/$1 then we increment the software
# version.
changes_found=false
while IFS= read -r line
do
    if [[ $line =~ ^"src/$1" ]]
    then
        # We pass the configuration file and the component name to the python file.
        echo "[INFO] => Calling python script for incrementing the version number."
        python3 .github/scripts/increment-version.py $2 $1
        changes_found=true
        break
    fi
done < "$input_file"

if [ "$changes_found" = false ] ; then
    echo "[INFO] => No changes found for this component."
else
    # Here we commit the new changes.
    echo "[INFO] => Committing and pushing changes."
    git add "$2"
    git commit -m "[$1](INCREMENT VERSION) Automatic version increment."
    git push
fi
