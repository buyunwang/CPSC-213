#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//
// A binary tree node with value and left and right children
struct Node {
  int value;
  struct Node *left;
  struct Node *right;
};

//
// Insert node n into tree rooted at toNode
//
void insertNode (struct Node* toNode, struct Node* n) {
  if (n->value <= toNode->value) {
    if (toNode->left == NULL) {
      toNode->left = n;
    }
    else {
      insertNode(toNode->left, n);
    }
  }
  else {
    if (toNode->right == NULL) {
      toNode->right = n;
    }
    else {
      insertNode(toNode->right, n);
    }
  }
}

//
// Insert new node with specified value into tree rooted at toNode
//
void insert (struct Node* toNode, int value) {
  struct Node *newNode = malloc(sizeof(struct Node));
  newNode->value = value;
  insertNode(toNode, newNode);
}


//
// Print values of tree rooted at node in ascending order
//
void printInOrder (struct Node* node) {
  if (node->left != NULL) {
    printInOrder(node->left);
  }
  printf("%d\n", node->value);
  if (node->right != NULL) {
    printInOrder(node->right);
  }
}

//
// Create root node, insert some values, and print tree in order
//
int main (void) {
  struct Node root = {100, NULL, NULL};
  insert(&root, 10);
  insert(&root, 120);
  insert(&root, 130);
  insert(&root, 90);
  insert(&root, 5);
  insert(&root, 95);
  insert(&root, 121);
  insert(&root, 131);
  insert(&root, 1);
  printInOrder(&root);
  return 0;
}
