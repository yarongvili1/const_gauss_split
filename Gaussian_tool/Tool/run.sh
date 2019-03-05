#!/bin/bash

for i in {1..108}; do
	./espresso -Dso -S1 -o eqntott $i.in > ./out/$i.out
	echo "Done: " $i.in
done
