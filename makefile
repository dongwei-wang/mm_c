all:
	gcc-4.9 -std=gnu11 -fopenmp mm.c -o mm

clean:
	rm mm tags
