#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define FAIL -1
#define SUCCESS 1
#define Min(a,b) (((a) < (b)) ? (a) : (b)) 

typedef enum
{
    False,
    True
} Bool;

typedef struct NODE
{
    int data;
    int level;
    struct NODE* l;
    struct NODE* r;
} Node;

void Merge(Node** head)
{
    Node* tmp, * tmp1;
    if ((*head) == NULL || (*head)->l == NULL)
    {
        return;
    }

    int tmpElem;
    if ((*head)->l->level == (*head)->level)
    {
        tmp = (*head);
        tmp1 = (*head)->l->r;
        (*head) = (*head)->l;
        (*head)->r = tmp;
        (*head)->r->l = tmp1;
    }
}

void Split(Node** head)
{
    Node* tmp, * tmp1;
    if ((*head) == NULL || (*head)->r == NULL || (*head)->r->r == NULL)
    {
        return;
    }

    if ((*head)->level == (*head)->r->r->level)
    {
        tmp = (*head);
        (*head)->r->level += 1;
        (*head) = (*head)->r;
        tmp1 = (*head)->l;
        (*head)->l = tmp;
        (*head)->l->r = tmp1;
    }
}

int Add(Node** head, int elem)
{
    if ((*head) == NULL)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));

        if (newNode == NULL)
        {
            return FAIL;
        }

        newNode->data = elem;
        newNode->level = 1;
        newNode->l = NULL;
        newNode->r = NULL;

        (*head) = newNode;
        return SUCCESS;
    }

    if (elem < (*head)->data)
    {
        Add(&((*head)->l), elem);
    }
    else
    {
        if (elem > (*head)->data)
        {
            Add(&((*head)->r), elem);
        }
    }

    Merge(head);
    Split(head);

    return SUCCESS;
}

int Find(Node* head, const int elem)
{
    int itExist = False;
    if (head == NULL)
    {
        itExist = False;
        return itExist;
    }

    if (elem < head->data)
    {
        itExist = Find(head->l, elem);
    }
    else
    {
        if (elem > head->data)
        {
            itExist = Find(head->r, elem);
        }
        else
        {
            itExist = True;
        }
    }

    return itExist;

}

void ChangeLevel(Node** head)
{
    int newLevel = 1,
        leftNodeLevel = 0,
        rightNodeLevel = 0;

    if ((*head)->l != NULL)
    {
        leftNodeLevel = (*head)->l->level;
    }
    if ((*head)->r != NULL)
    {
        rightNodeLevel = (*head)->r->level;
    }

    newLevel += Min(leftNodeLevel, rightNodeLevel);

    if (newLevel < (*head)->level)
    {
        (*head)->level = newLevel;
        if (newLevel < rightNodeLevel)
        {
            (*head)->r->level = newLevel;
        }
    }
}

void Delete(Node** head, int elem)
{
    Node** tmpNode1,
        * tmpNode2;
    int tmpVal;

    if ((*head) == NULL)
        return;
    if ((*head)->data < elem)
    {
        Delete(&((*head)->r), elem);
    }
    else
    {
        if ((*head)->data > elem)
        {
            Delete(&((*head)->l), elem);
        }
        else
        {
            if ((*head)->l == NULL && (*head)->r == NULL)
            {
                free((*head));
                (*head) = NULL;
                return;
            }
            if ((*head)->l == NULL)
            {
                tmpNode2 = (*head);
                (*head) = (*head)->r;
                free(tmpNode2);
                return;
            }
            if ((*head)->r == NULL)
            {
                tmpNode2 = (*head);
                (*head) = (*head)->l;
                free(tmpNode2);
                return;
            }
            else
            {
                tmpNode1 = &((*head)->l);
                while ((*tmpNode1)->r != NULL)
                {
                    tmpNode1 = &(*tmpNode1)->r;
                }
                tmpVal = (*tmpNode1)->data;
                (*tmpNode1)->data = (*head)->data;
                (*head)->data = tmpVal;
                Delete(&((*head)->l), elem);
            }
        }
    }

    ChangeLevel(head);
    Merge(head);
    Merge(&((*head)->r));
    if ((*head)->r != NULL)
    {
        Merge(&((*head)->r->r));
    }
    Split(head);
    Split(&((*head)->r));
}
void Freee(Node* head)
{
    if (head == NULL)
    {
        return;
    }
    Freee(head->l);
    Freee(head->r);
    free(head);
}
void Process(Node** tree)
{
    char c, operation, number[1024];
    int index = 0, data;

    c = getchar();
    while (c != EOF)
    {
        operation = c;
        c = getchar();
        while (isspace(c))
        {
            c = getchar();
        }
        while (isdigit(c))
        {
            number[index++] = c;
            c = getchar();
        }
        number[index] = '\0';
        data = atoi(number);
        index = 0;
        switch (operation)
        {
        case 'a':
        {
            Add(tree, data);
            break;
        }
        case 'r':
        {
            Delete(tree, data);
            break;
        }
        case 'f':
        {
            if (Find(*tree, data))
            {
                printf("yes\n");
            }
            else
            {
                printf("no\n");
            }
            break;
        }
        }
        if (c == '\n')
        {
            c = getchar();
        }
    }
}

int main()
{
    Node* tree = NULL;
    Process(&tree);
    Freee(tree);
    return 0;
}