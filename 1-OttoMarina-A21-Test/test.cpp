#include "pch.h"
#include "header.h"

Node_t new_element;
List_t Test_List;
Node_t start;

TEST(Create_Element, ElementCreated_ExpectedEqualityofkey) {
	new_element.word = "test";
	new_element.key = 1;
	new_element.next = NULL;
	Node_t* element2 = Create_Element("test", 1);
	EXPECT_TRUE(!strcmp(new_element.word, element2->word));
	EXPECT_EQ(new_element.key, element2->key);
	EXPECT_EQ(new_element.next, element2->next);
	free(element2->word);
	free(element2);
}

TEST(Create_List, ListCreated_ExpectedListstartnotNULLanditskeyisRight) {
	List_t* Test_List = Create_List("start", 1);
	EXPECT_TRUE(Test_List->start != NULL);
	EXPECT_TRUE(!strcmp(Test_List->start->word, "start"));
	EXPECT_EQ(Test_List->start->key, 1);
	Destroy_List(Test_List);
}

TEST(Search_Need, TheSmallestKeyNumber_ExpectedreturnNULL) {
	start.word = "start";
	start.key = 1;
	start.next = NULL;
	Test_List.start = &start;
	int TestKey = 0;
	EXPECT_TRUE(Search_Need(&Test_List, TestKey) == NULL);
}

TEST(Search_Need, UsualKeyNumber_ExpectedreturnNotNULL) {
	start.word = "start";
	start.key = 1;
	start.next = NULL;
	Test_List.start = &start;
	int TestKey = 2;
	EXPECT_TRUE(Search_Need(&Test_List, TestKey) != NULL);
}

TEST(Add, AddingToTheBegining_ExpectedNewElementAsstartofList) {
	start.word = "start";
	start.key = 1;
	start.next = NULL;
	Test_List.start = &start;
	new_element.word = "zero";
	new_element.key = 0;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Add_to_begining);
	EXPECT_TRUE(Test_List.start == &new_element);
}

TEST(Add, AddingNotToTheBegining_ExpectedNewElementAsstartNext) {
	start.word = "start";
	start.key = 1;
	start.next = NULL;
	Test_List.start = &start;
	new_element.word = "second";
	new_element.key = 2;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Add_not_to_begining);
	EXPECT_TRUE(Test_List.start->next == &new_element);
}

TEST(Add, AddingElement_ExpectedAddingElementtotheRightPlace) {
	start.word = "start";
	start.key = 1;
	start.next = NULL;
	Test_List.start = &start;
	Node_t new_element1, new_element2, new_element3;
	new_element1.word = "second";
	new_element1.key = 2;
	new_element1.next = NULL;
	new_element2.word = "5th";
	new_element2.key = 5;
	new_element2.next = NULL;
	new_element3.word = "zero";
	new_element3.key = 0;
	new_element3.next = NULL;
	new_element.word = "third";
	new_element.key = 3;
	new_element.next = NULL;
	EXPECT_EQ(Add(&Test_List, &new_element), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element1), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element2), Add_not_to_begining);
	EXPECT_EQ(Add(&Test_List, &new_element3), Add_to_begining);
	EXPECT_TRUE(Test_List.start->next->next->next == &new_element);
}
