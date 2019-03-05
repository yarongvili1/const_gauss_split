#!/bin/bash

b=44
c=11
for i in {1..116}; do
	cat $i.out >> combined.out
	j=`expr $i - 1`	
	if [ $i -lt $b ]
	then
		echo "out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);">> combined.out
		echo "out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);">> combined.out
		echo "out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);">> combined.out
		echo "out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);">> combined.out
		echo "//control and disable_update update--">> combined.out
		echo "control=control&bit["$j"];">> combined.out
		echo "disable_update=disable_update|(~control);">> combined.out
		#echo "disable_update=disable_update|(control&~bit["$j"]);">> combined.out
		#echo "bit_counter=bit_counter+1;">> combined.out
		echo "//----------------"$i"--------------">> combined.out
	else
		echo "out[4]=(~disable_update&(out_t[4]))|(disable_update&out[4]);">> combined.out
		echo "out[3]=(~disable_update&(out_t[3]))|(disable_update&out[3]);">> combined.out
		echo "out[2]=(~disable_update&(out_t[2]))|(disable_update&out[2]);">> combined.out
		echo "out[1]=(~disable_update&(out_t[1]))|(disable_update&out[1]);">> combined.out
		echo "out[0]=(~disable_update&(out_t[0]))|(disable_update&out[0]);">> combined.out
		echo "//control and disable_update update--">> combined.out
		echo "control=control&bit["$j"];">> combined.out
		echo "disable_update=disable_update|(~control);">> combined.out
		#echo "disable_update=disable_update|(control&~bit["$j"]);">> combined.out
		#echo "control=control&bit["$j"];">> combined.out
		#echo "bit_counter=bit_counter+1;">> combined.out
		echo "//----------------"$i"--------------">> combined.out
	fi
done
