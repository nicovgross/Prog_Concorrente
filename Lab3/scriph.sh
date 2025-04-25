#!/bin/bash

threads=(1 2 3 4)
repeticoes=(1 2 3 4 5 6 7 8 9 10)

for i in ${threads[@]}
do
    for j in ${repeticoes[@]}
    do
	./a.out mat2000 vet2000 $i
    done
    echo ""
done
