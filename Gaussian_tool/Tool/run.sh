#!/bin/bash

N=$(ls -1 inp/*.in | wc -l)

for i in $(seq 1 $N); do
	./espresso -Dso -S1 -o eqntott inp/$i.in | awk -v IN=inp/$i.in '
	BEGIN {
		while (0 < (getline line < IN)) {
			if (substr(line,1,1) == "-") {
				split(line,x," ")
				out = x[2]
			}
		}
	}
	{
		n = length($0)
		if (substr($0,n,1) == ";") {
			++c
			if (substr($0,n-3,4) == " = ;") {
				$0 = substr($0,1,n-1) (substr(out,c,1) == "0" ? "bzero" : "bones") ";"
			}
		}
		print $0
	}' > ./out/$i.out
	echo "Done: " inp/$i.in
done
