#pragma once

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#ifdef __cplusplus
extern "C"
{
#endif

#define N 50
#define Error -1
#define Add_to_begining 1
#define Add_not_to_begining 2

	typedef struct Node_t {
		char* word;
		int key;
		struct Node_t* next;
	}Node_t;

	typedef struct {
		Node_t* start;
	}List_t;

	List_t* Create_List(char* Word, int Key);
	Node_t* Create_Element(char* Word, int Key);
	void Destroy_List(List_t* roster);
	Node_t* Search_Need(List_t* roster, int DataKey);
	int Add(List_t* roster, Node_t* new_element);
	int Search_Word(List_t* roster, int DataKey);
	void Output_To_The_Screen(List_t* roster);
	void MemoryLeaks(void);

#ifdef __cplusplus
}
#endif


