#!/bin/bash

gawk -v IN=inp/L '
BEGIN {
	h = ""
	for (i=1;i<=4;i++) {
		getline line < IN
		h = h line "\n"
	}
	while (0 < (getline line < IN)) {
		split(line,x," ")
		bits = x[1]
		ones = 0
		for (j=length(bits);j>0;j--) {
			if (substr(bits,j,1) == "1") {
				ones++;
			} else {
				break;
			}
		}
		key = sprintf("%04d", ones)
		logic[key] = logic[key] line "\n"
	}
	asorti(logic,ilogic)
	nlogic = length(logic)
	for (i=1;i<=nlogic;i++) {
		printf("%s%s.e\n", h, logic[ilogic[i]]) > "inp/" i ".in"
	}
}' /dev/null
