#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <time.h>
#include <pthread.h>

int
compare_func(const void *a, const void *b) {
	int ia = *((int*)a);
	int ib = *((int*)b);
	if (ia < ib) return -1;
	if (ia > ib) return 1;
	return 0;
}

struct sorter_args {
	int *a;
	size_t start;
	size_t finish;
};

void *
sorter(void *varg) {
	struct sorter_args *args = varg;

	// Sort the vector "v" starting at position "start" and
	// and going to "finish" - 1.

	qsort(args->a + args->start, args->finish - args->start,
			sizeof(*args->a), compare_func);
	return NULL;
}

void
do_the_sort(int *a, size_t sz) {
	pthread_t thd;
	struct sorter_args arg;
	// This is what you need to modify. This is so slow...
	// It's single threaded, we can do better, right?

	// start a thread
	arg.a = a;
	arg.start = 0;
	arg.finish = sz;
	pthread_create(&thd, NULL, sorter, &arg);

	// wait for it to finish
	pthread_join(thd, NULL);

	// You should determine the number of cores available and
	// create a thread for each available core. Each thread
	// should be given a part of the array to work on.
	// C++, look at std::thread::hardware_concurrency()
	// C, it varies based on OS, but for Linux: sysconf(_SC_NPROCESSORS_ONLN)
	// and Windows: GetSystemInfo()
	// 
	// My recommendation is to get the algorithm to work for 2 threads
	// and only then modify it for N threads.

	// Your main thread will need to join the results together
	// to finish the sorting process. Note: you're welcome
	// to use std::sort() (C++) or the qsort() function (C) in
	// your thread. This isn't a data structures class.
}

bool
verifysorted(const int *a, size_t sz) {
	// Make sure the array really is sorted
	for (size_t i = 1; i < sz; i++) {
		if (a[i - 1] > a[i])
			return false;
	}
	return true;
}

double
convert_timespec(struct timespec *t) {
	double d = t->tv_nsec;
	d /= 1e9;
	d += t->tv_sec;
	return d;
}

int main()
{
	int *vec = NULL;
	size_t sz = 0;

	for (;;) {
		int *nextv, x;

		if (feof(stdin))
			break;
		scanf("%d", &x);

		nextv = realloc(vec, (sz + 1) * sizeof(*vec));
		if (nextv == NULL)
			err(1, "realloc");
		vec = nextv;
		vec[sz++] = x;
	}

	// start the clock
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);

	// do the sort
	do_the_sort(vec, sz);

	// stop the clock
	clock_gettime(CLOCK_MONOTONIC, &finish);

	// Verify the sort actually happened
	if (!verifysorted(vec, sz)) {
		printf("Your array isn't sorted. Fast and wrong is wrong.\n");
		return 1;
	}

	double t2 = convert_timespec(&finish), t1 = convert_timespec(&start);

	printf("Elapsed time: %f seconds\n", t2 - t1);
	return 0;
}

