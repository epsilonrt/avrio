#!/bin/bash

for f in $(find -type f -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp")
do
  echo "Indent $f..."
  indent $f
done

