#!/bin/bash

build: Quadtree.o functions.o Quadtree.h
	gcc -o quadtree Quadtree.o functions.o -g -lm
Quadtree.o: Quadtree.c
	gcc -c Quadtree.c
functions.o: functions.c
	gcc -c functions.c
clean:
	rm -r 'quadtree'
	rm -r *.o
