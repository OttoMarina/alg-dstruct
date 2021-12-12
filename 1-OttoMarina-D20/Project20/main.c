#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

enum {
	FILE_ERROR = -100,
	READING_ERROR,
	ALLOCATION_ERROR,
	INITIALIZATION_ERROR
} errors;

typedef char boolean;
typedef unsigned int uint;

typedef struct {
	uint size;
	boolean isUsed;
	boolean isTouched;
} plentyElem_t;

typedef struct {
	uint size;
	int index;
} stackElem_t;

typedef struct {
	int cur;
	stackElem_t* arr;
} stack_t;

void PrintSolution(stack_t* stack, FILE* fp) {
	if (stack->cur == -1) {
		fprintf(fp, "0 ");
		return;
	}
	for (int i = 0; i <= stack->cur; i++)
		fprintf(fp, "%u ", stack->arr[i].size);
}

stack_t* StackInit(uint sizeStack) {
	stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
	if (stack == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		return NULL;
	}
	stack->arr = (stackElem_t*)malloc(sizeStack * sizeof(stackElem_t));
	if (stack->arr == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		return NULL;
	}
	for (uint i = 0; i < sizeStack; i++) {
		stack->arr[i].size = 0;
		stack->arr[i].index = -1;
	}
	stack->cur = -1;
	return stack;
}

void StackPush(stack_t* stack, uint sizeElem, int indexElem) {
	stack->cur++;
	stack->arr[stack->cur].size = sizeElem;
	stack->arr[stack->cur].index = indexElem;
}

void StackPop(stack_t* stack) {
	if (stack->cur == -1)
		return;
	stack->arr[stack->cur].size = 0;
	stack->arr[stack->cur].index = -1;
	stack->cur--;
}

stackElem_t StackTop(stack_t* stack) {
	stackElem_t result = { 0, -1 };
	if (stack->cur == -1)
		return result;
	return stack->arr[stack->cur];
}

void FreeStack(stack_t* stack) {
	free(stack->arr);
	free(stack);
}

int FindSubset(plentyElem_t* plenty, stack_t* curSolution, uint N, uint requiredAmount, uint curIndex) {
	static int returnRes = 0;
	for (uint i = curIndex; i < N; i++) {
		if (plenty[i].isTouched == FALSE)
			plenty[i].isTouched = TRUE;
		if (plenty[i].isUsed == FALSE) {
			if (plenty[i].size < requiredAmount) {
				StackPush(curSolution, plenty[i].size, i);
				plenty[i].isUsed = TRUE;
				requiredAmount -= plenty[i].size;
				returnRes = FindSubset(plenty, curSolution, N, requiredAmount, i + 1);
				if (returnRes == 0)
					return 0;
				requiredAmount += returnRes;
			}
			if (plenty[i].size == requiredAmount) {
				StackPush(curSolution, plenty[i].size, i);
				plenty[i].isUsed = TRUE;
				requiredAmount -= plenty[i].size;
				return 0;
			}
		}
	}
	if (requiredAmount == 0)
		return 0;
	stackElem_t delElem = StackTop(curSolution);
	requiredAmount += delElem.size;
	plenty[delElem.index].isUsed = FALSE;
	StackPop(curSolution);
	return delElem.size;
}


int main(void) {

	FILE* file = fopen("input.txt", "rt");
	if (file == NULL) {
		fprintf(stderr, "Opening file error!\n");
		return FILE_ERROR;
	}

	uint N, B;
	if (fscanf(file, "%u\n%u", &B, &N) != 2) {
		fprintf(stderr, "Reading B and N error!\n");
		fclose(file);
		return READING_ERROR;
	}

	plentyElem_t* plenty = (plentyElem_t*)malloc(N * sizeof(plentyElem_t));
	if (plenty == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		free(plenty);
		fclose(file);
		return ALLOCATION_ERROR;
	}
	for (uint i = 0; i < N; i++) {
		if (fscanf(file, "%u ", &plenty[i].size) != 1) {
			fprintf(stderr, "Reading plenty error!\n");
			free(plenty);
			fclose(file);
			return READING_ERROR;
		}
		plenty[i].isUsed = FALSE;
		plenty[i].isTouched = FALSE;
	}

	int checkEndFile;
	if (fscanf(file, "%d", &checkEndFile) != -1) {
		fprintf(stderr, "Plenty hasn't been initialized completely\n");
		free(plenty);
		fclose(file);
		return INITIALIZATION_ERROR;
	}

	fclose(file);

	stack_t* curSolution = StackInit(N);
	FindSubset(plenty, curSolution, N, B, 0);

	file = fopen("output.txt", "wt");
	if (file == NULL) {
		fprintf(stderr, "Opening file error!\n");
		free(plenty);
		FreeStack(curSolution);
		return FILE_ERROR;
	}

	PrintSolution(curSolution, file);
	fclose(file);
	free(plenty);
	FreeStack(curSolution);

	return 0;
}