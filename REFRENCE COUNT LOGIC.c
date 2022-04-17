// refrence counting logic 
//BT20CSE071
#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
typedef enum{FALSE,TRUE} boolean;
struct node {
int value;
struct node *left;
struct node *right;
}; // A Tree Data Type

typedef struct node Tree;

Tree *node(int value, Tree *left, Tree *right) {//Allocate a pointer to a tree struct
 Tree *t = malloc(sizeof(tree));
 t->value = value;
t->right = right;
t->left = left;
return t;// Return the initialized pointer!
 }
Tree *n = node(2, NULL, NULL); // we allocate n on 
Tree *n2 =node(1, n, n); //n2->left and n2->right are said to alias – they are two pointers aimed at the same block of memory.
// refrence counting 
struct node {
 unsigned int rc; // rc to keep track of the references.
 int value;
 struct node *left;
 struct node *right;
};
typedef struct node Node;
const Node *empty = NULL;
 Node *node(int value, Node *left, Node *right); // We keep the same node 
void inc_ref(Node *node);// To increment reference count of node  
void dec_ref(Node *node);// decrement refrence count of a node 
Node *node(int value,
 Node *left,
 Node *right) {
Node *r = malloc(sizeof(Node));
r->rc = 1;
r->value = value;
r->left = left;// , we set the left field, and increment the reference count of  node.
 inc_ref(left);
 r->right = right;// same we set right and increse 
 inc_ref(right);
return r;
 }
 void inc_ref(Node *node) {
 if (node != NULL) {
 node->rc += 1;
}
}
void dec_ref(Node *node) {
 if (node != NULL) {
if (node->rc > 1) { // here we are checking whether it is last refernce or not 
node->rc -= 1; // if not then we just decrese the refrence by 1 .
} else {
 dec_ref(node->left); // then decrement the reference counts of thechildren 
 dec_ref(node->right);
 free(node); // free the current object 
}
 }
}
