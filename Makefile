all: kmeans.c
	gcc -std=gnu99 kmeans.c -o kmeans -lm
clean:
	rm kmeans
