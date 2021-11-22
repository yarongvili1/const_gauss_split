#!/bin/bash

NAME="$1"
if [ -z "$NAME" ]; then NAME=combined.out; fi

Os=$(seq $(awk '$1==".o"{print $2 - 1}' inp/L) -1 0)
S=$(ls -q out/*.out | wc -l)

(
for i in $(seq 1 $S); do
	cat out/$i.out
	j=`expr $i - 1`	
	for o in $Os; do
		echo "out[$o]=(~disable_update&(out_t[$o]))|(disable_update&out[$o]);"
	done
	echo "//control and disable_update update--"
	echo "control=control&bit["$j"];"
	echo "disable_update=disable_update|(~control);"
	echo "//----------------"$i"--------------"
done
) | sed 's=[!]=~=g' > "$NAME"
