#include<stdio.h>
#include<stdlib.h>
#define H 5

typedef struct b_tree {
    int keysNum;
    int leaf;
    int keys[2 * H];
    struct b_tree* right;
    struct b_tree* childs[2 * H + 1];
    struct b_tree* parent;
    struct b_tree* left;

} b_tree;
void NMerge(b_tree* tree, b_tree* node, int key);
void DelFromNode(b_tree* tree, b_tree* node, int key);
int FindMin(b_tree* tree) {
    for (int i = 0; i < 2 * H; i++)
        if (tree->childs[i])
            return FindMin(tree->childs[i]);
    return tree->keys[0];
}
void UpdKeys(b_tree* node, int key) {
    b_tree* upd = node->parent;
    int i = 0;
    while (upd) {
        while (i < upd->keysNum && upd->keys[i] < key)
            i++;
        if (i < upd->keysNum)
            if (upd->keys[i] != FindMin(upd->childs[i + 1]))
                upd->keys[i] = FindMin(upd->childs[i + 1]);
        upd = upd->parent;
    }

    return;
}
b_tree* InitTree() {
    b_tree* tree = (b_tree*)(malloc(sizeof(b_tree)));
    if (!tree)
        return NULL;
    tree->leaf = 0;
    tree->keysNum = 0;
    tree->parent = NULL;
    tree->left = NULL;
    tree->right = NULL;
    for (int i = 0; i < 2 * H + 1; i++)
        tree->childs[i] = NULL;
    return tree;
}
int DelTree(b_tree* tree) {
    if (!tree)
        return 0;
    if (tree->leaf == 1) {
        for (int i = 0; i <= tree->keysNum; i++)
            DelTree(tree->childs[i]);
    }
    free(tree);
    return 0;
}
b_tree* FindTree(b_tree* tree, int key) {
    if (!tree)
        return NULL;
    b_tree* currentNode = tree;
    while (currentNode->leaf == 1)
        for (int i = 0; i <= currentNode->keysNum; i++) {
            if (i == currentNode->keysNum || key < currentNode->keys[i]) {
                currentNode = currentNode->childs[i];
                break;
            }
        }

    return currentNode;
}
int FindByKey(b_tree* tree, int key) {
    b_tree* node = FindTree(tree, key);
    if (node) {
        for (int i = 0; i < node->keysNum; i++) {
            if (node->keys[i] == key)
                return 1;
        }
    }

    return 0;
}
void NSplit(b_tree** root, b_tree* node) {
    int i = 0, key = 0, pos = 0;

    b_tree* newNode = InitTree();
    newNode->leaf = 1;


    if (node->right != NULL) {
        newNode->right = node->right;
        node->right->left = newNode;
        node->right = newNode;
        newNode->left = node;
    }
    else {
        newNode->right = node->right;
        node->right = newNode;
        newNode->left = node;
    }

    key = node->keys[H];
    newNode->keysNum = H - 1;
    node->keysNum = H;
    for (i = 0; i < newNode->keysNum; i++) {
        newNode->keys[i] = node->keys[i + H + 1];
        newNode->childs[i] = node->childs[i + H + 1];
        node->childs[i + H + 1] = NULL;
    }
    newNode->childs[newNode->keysNum] = node->childs[2 * H];
    node->childs[2 * H] = NULL;

    if (node->leaf == 0) {
        newNode->keysNum++;
        newNode->leaf = 0;
        for (i = newNode->keysNum - 1; i >= 1; i--)
            newNode->keys[i] = newNode->keys[i - 1];
        newNode->keys[0] = node->keys[H];
    }

    if (node == *(root)) {
        (*root) = (b_tree*)malloc(sizeof(b_tree));
        if (!(*root))
            return;

        (*root)->keys[0] = key;
        (*root)->childs[0] = node;
        (*root)->childs[1] = newNode;
        for (i = 0; i < 2 * H + 1; i++) {
            if (newNode->childs[i] != NULL)
                newNode->childs[i]->parent = newNode;
            if (node->childs[i] != NULL)
                node->childs[i]->parent = node;
        }
        (*root)->leaf = 1;
        (*root)->parent = NULL;
        (*root)->left = NULL;
        (*root)->right = NULL;
        for (i = 2; i < 2 * H + 1; i++)
            (*root)->childs[i] = NULL;
        (*root)->keysNum = 1;
        node->parent = (*root);
        newNode->parent = (*root);
    }
    else {
        newNode->parent = node->parent;
        while (pos < node->parent->keysNum && node->parent->keys[pos] < key)
            pos++;

        for (i = node->parent->keysNum; i >= pos; i--)
            node->parent->keys[i] = node->parent->keys[i - 1];
        for (i = node->parent->keysNum + 1; i >= pos + 2; i--)
            node->parent->childs[i] = node->parent->childs[i - 1];
        for (i = 0; i < 2 * H; i++)
            if (newNode->childs[i] != NULL)
                newNode->childs[i]->parent = newNode;
        node->parent->keys[pos] = key;
        node->parent->childs[pos + 1] = newNode;
        node->parent->keysNum++;

        if (node->parent->keysNum == 2 * H)
            NSplit(root, node->parent);
    }

    return;
}
void NMerge(b_tree* tree, b_tree* node, int key) {

    b_tree* right = node->right;
    b_tree* left = node->left;

    int i = 0;

    if (right != NULL && right->parent == node->parent) {
        for (i = 0; i < right->keysNum; i++) {
            node->keys[node->keysNum] = right->keys[i];
            node->childs[node->keysNum + 1] = right->childs[i];
            if (right->childs[i] != NULL)
                right->childs[i]->parent = node;
            node->keysNum++;
        }
        node->childs[node->keysNum + 1] = right->childs[right->keysNum];
        if (right->childs[right->keysNum])
            right->childs[right->keysNum]->parent = node;
        if (right->right)
            right->right->left = node;

        node->right = right->right;

        DelFromNode(tree, node->parent, FindMin(right));
        UpdKeys(node, key);
    }
    else {
        for (i = 0; i < node->keysNum; i++) {
            left->keys[left->keysNum] = node->keys[i];
            left->childs[left->keysNum + 1] = node->childs[i];
            if (node->childs[i] != NULL)
                node->childs[i]->parent = left;
            left->keysNum++;
        }
        left->childs[left->keysNum + 1] = node->childs[node->keysNum];
        if (node->childs[node->keysNum])
            node->childs[node->keysNum]->parent = left;
        left->right = node->right;
        if (node->right != NULL)
            node->right->left = left;

        DelFromNode(tree, left->parent, FindMin(node));
        UpdKeys(left, key);
    }
    return;
}
int InsertKey(b_tree** root, int key) {
    b_tree* leaf = FindTree((*root), key);
    if (!leaf)
        return 0;

    int i = 0, pos = 0;
    for (i = 0; i < leaf->keysNum; i++) {
        if (leaf->keys[i] == key)
            return 0;
    }

    while (pos < leaf->keysNum && leaf->keys[pos] < key)
        ++pos;

    for (int i = leaf->keysNum; i >= pos + 1; i--)
        leaf->keys[i] = leaf->keys[i - 1];

    leaf->keys[pos] = key;
    leaf->keysNum++;
    if (leaf->keysNum == 2 * H)
        NSplit(root, leaf);

    return 0;
}
void DelFromNode(b_tree* tree, b_tree* node, int key) {
    int pos = 0;
    while (pos < node->keysNum && node->keys[pos] < key)
        pos++;
    for (int i = pos; i < node->keysNum; i++)
        node->keys[i] = node->keys[i + 1];
    for (int i = pos + 1; i <= node->keysNum; i++)
        node->childs[i] = node->childs[i + 1];
    for (int i = node->keysNum + 1; i < 2 * H + 1; i++)
        *(node->childs + i) = NULL;
    node->keysNum--;
    if (node == tree) {
        for (int i = 0; i < 2 * H + 1; i++)
            if (node->childs[i] != NULL)
                if (node->childs[i]->keysNum == 0)
                    node->childs[i] = NULL;
        if (tree->keysNum == 0 && tree->childs[0] != NULL)
            tree = tree->childs[0];
        return;
    }
    if (node->keysNum >= 1)
        UpdKeys(node, key);
    else
        if (node->keysNum < H - 1) {
            b_tree* right = node->right;
            b_tree* left = node->left;
            if (left != NULL && left->keysNum > (H - 1) && left->parent == node->parent) {
                left->keysNum--;
                node->keysNum++;
                for (int i = 1; i < node->keysNum; i++) {
                    node->keys[i] = node->keys[i - 1];
                    node->childs[i] = node->childs[i - 1];
                }
                node->childs[node->keysNum] = node->childs[node->keysNum - 1];
                node->keys[0] = left->keys[left->keysNum];
                node->childs[0] = left->childs[left->keysNum + 1];
                left->childs[left->keysNum + 1] = NULL;
                if (node->childs[0] != NULL)
                    node->childs[0]->parent = node;
                UpdKeys(node, key);
            }
            else
                if (right != NULL && right->keysNum > (H - 1) && right->parent == node->parent) {
                    node->keysNum++;
                    node->keys[node->keysNum - 1] = right->keys[0];
                    node->childs[node->keysNum] = right->childs[0];
                    if (node->childs[node->keysNum] != NULL)
                        node->childs[node->keysNum]->parent = node;
                    for (int i = 0; i < right->keysNum; i++) {
                        right->keys[i] = right->keys[i + 1];
                        right->childs[i] = right->childs[i + 1];
                    }
                    for (int i = 0; i < 2 * H; i++)
                        if (right->childs[i] == right->childs[i + 1] && right->childs[i] != NULL) {
                            right->childs[i + 1] = NULL;
                            break;
                        }
                    right->keysNum--;
                    UpdKeys(node, key);
                }
                else
                    NMerge(tree, node, key);
        }
    return;
}
int DelByKey(b_tree* tree, int key) {
    if (!tree)
        return 0;

    if (!FindByKey(tree, key))
        return 0;

    b_tree* leaf = FindTree(tree, key);
    DelFromNode(tree, leaf, key);
    return 0;
}
int main(void) {
    b_tree* root = InitTree();
    int key;
    char command = getchar();
    while (command != EOF) {
        scanf("%d", &key);

        switch (command) {
        case 'a':
            InsertKey(&root, key);
            break;
        case 'r':
            DelByKey(root, key);
            break;
        case 'f':
            if (FindByKey(root, key)) {
                printf("yes\n");
            }
            else
                printf("no\n");
            break;

        }

        command = getchar();

    }
    DelTree(root);
    return 0;
}