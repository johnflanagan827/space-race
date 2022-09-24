# John Flangan
# sp22-cse-20311.01
# Makefile

project: space.c
	gcc space.c gfx2.o -lX11 -lm -o project

clean:
	rm project
