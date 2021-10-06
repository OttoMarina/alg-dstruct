#include "header.h"

List_t* Create_List(char* word, int key) {
	Node_t* start = Create_Element(word, key);
	List_t* roster = (List_t*)malloc(sizeof(List_t));
	if (!roster)
	{
		free(start->word);
		free(start);
		return 0;
	}
	roster->start = start;
	return roster;
}

Node_t* Create_Element(char* word, int key) {
	Node_t* new_node = (Node_t*)malloc(sizeof(Node_t));
	if (!new_node)
		return NULL;
	new_node->word = (char*)malloc(sizeof(char) * (strlen(word) + 1));
	if (!new_node->word)
	{
		free(new_node);
		return NULL;
	}
	strcpy(new_node->word, word);
	new_node->key = key;
	new_node->next = NULL;
	return new_node;
}

void Destroy_List(List_t* roster) {
	Node_t* tmp = roster->start;
	while (tmp)
	{
		Node_t* removed = tmp;
		tmp = tmp->next;
		free(removed->word);
		free(removed);
	}
	free(roster);
}

Node_t* Search_Need(List_t* roster, int key) {
	Node_t* pointer = roster->start;
	if (pointer->key > key)
		return NULL;
	while (pointer->next && pointer->next->key < key)
	{
		pointer = pointer->next;
	}
	return pointer;
}

int Add(List_t* roster, Node_t* new_node) {
	Node_t* need = Search_Need(roster, new_node->key);
	if (!need)
	{
		if (new_node->key == roster->start->key)
			return Error;
		new_node->next = roster->start;
		roster->start = new_node;
		return Add_to_begining;
	}
	else
	{
		if (need->key == new_node->key)
			return Error;
		if (need->next != NULL && need->next->key == new_node->key)
			return Error;
		new_node->next = need->next;
		need->next = new_node;
		return Add_not_to_begining;
	}
}

int Search_Word(List_t* roster, int key) {
	Node_t* pointer = roster->start;
	while (pointer)
	{
		if (pointer->key == key)
			return 1;
		pointer = pointer->next;
	}
	return 0;
}

void Output_To_The_Screen(List_t* roster) {
	Node_t* tmp = roster->start;
	while (tmp)
	{
		printf("%s %i\n", tmp->word, tmp->key);
		tmp = tmp->next;
	}
	free(tmp);
	return;
}

void MemoryLeaks(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}