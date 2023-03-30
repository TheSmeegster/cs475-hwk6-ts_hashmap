
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ts_hashmap.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage: %s <num threads> <hashmap capacity>\n", argv[0]);
		return 1;
	}

  srand(time(NULL));
	int num_threads = atoi(argv[1]);
	int capacity = (unsigned int) atoi(argv[2]);

	if(num_threads == 0 || capacity == 0){
		printf("num threads and hashmap capacity must both be greater than 0");
		return 1;
	}

	
	// TODO: Write your test
	ts_hashmap_t *map = initmap(capacity);
	put(map, 5, 2);
	put(map, 6, 10);
	put(map, 3, 7);
	del(map, 5);
	put(map, 5, 4);
	printf("%i\n", get(map, 5));
	printmap(map);


	return 0;
}

