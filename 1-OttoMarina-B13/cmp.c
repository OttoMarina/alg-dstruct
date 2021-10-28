#define _CRT_SECURE_NO_WARNINGS

#include "memallocator.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MEM_BLOCK_SIZE 10000
#define MAX_BLOCK_SIZE 1000
#define MAX_ITER 3000

double mallocTestTime[MAX_ITER];
double memallocTestTime[MAX_ITER];


double CalcMallocTime(size_t iter)
{
	unsigned long start = clock();

	for (int i = 0; i < iter; i++)
	{
		void* p = malloc(rand() % MAX_BLOCK_SIZE + 1);
		free(p);
	}
	return (((double)clock() - start) / CLOCKS_PER_SEC);
}

double CalcMemallocTime(size_t iter)
{
	double start = clock();

	for (int i = 0; i < iter; i++)
	{
		void* p = memalloc(rand() % MAX_BLOCK_SIZE + 1);
		memfree(p);
	}
	return (((double)clock() - start) / CLOCKS_PER_SEC);

}

int main(void)
{
	srand(time(NULL));

	FILE* mFile = fopen("malloc.txt", "w");
	FILE* memFile = fopen("memalloc.txt", "w");

	void* pMemory = malloc(MEM_BLOCK_SIZE);

	meminit(pMemory, MEM_BLOCK_SIZE);

	for (int i = 0; i < MAX_ITER; i++)
	{
		mallocTestTime[i] = CalcMallocTime(i);
		memallocTestTime[i] = CalcMemallocTime(i);
		fprintf(mFile, "%lf, ", mallocTestTime[i]);
		fprintf(memFile, "%lf, ", memallocTestTime[i]);
	}

	fclose(mFile);
	fclose(memFile);

	return 0;
}