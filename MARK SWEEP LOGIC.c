// Mark Sweep Logic 
//BT20CSE071
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
typedef enum{FALSE,TRUE} boolean;
struct node {
 int value;
 struct node *left;
 struct node *right;
 bool mark;
 struct node *next;
 };
 typedef struct node Node;

 struct root {
 Node *start;
 struct root *next;
 };
 typedef struct root Root;

struct alloc {
Node *nodes;
Root *roots;
};
typedef struct alloc Alloc; // An allocator Alloc * holds the head of the lists of nodes and roots

Alloc *make_allocator(void) {
Alloc *a = malloc(sizeof(Alloc));
a->roots = NULL; // Creates a fresh allocator with empty set of roots and nodes
a->nodes = NULL;
 return a;
 }
 // Creating a node 
 Node *node(int value,Node *left,Node *right,Alloc *a) {
 Node *r = malloc(sizeof(Node)); // allocating 
 r->value = value; // data 
 r->left = left;
 r->right = right;
 r->mark = false; //initializes mark to false
 r->next = a->nodes; // adds a new node 
 a->nodes = r;
 return r;
 // creating a root 
 Root *root(Node *node,Alloc *a) {
     Root *g = malloc(sizeof(Root));
 g->start = node;
 g->next = a->roots;
 a->roots = g;
 return g;
 }
 // Marking 
 void mark_node(Node *node) { 
if (node != NULL && !node->mark) { // function marks a node if unmarked, and then recursively marks subnodes
 node->mark = true;
 mark_node(node->left);
 mark_node(node->right);
 }
 }

 void mark(Alloc *a) { // procedure iterates over the roots, marking the nodes reachable from it.
 Root *g = a->roots; 
 while (g != NULL) {
 mark_node(g->start);
 g = g->next;
 }
}
// Sweeping 
void sweep(Alloc *a) {
Node *n = a->nodes; // get a pointer to all allocated nodes
 Node *live = NULL; //  create a new empty list of live nodes
 while (n != NULL) {
 Node *tl = n->next;
 if (!(n->mark)) {
 free(n); // if unmarked free 
 } else {
 n->mark = false; //If marked, reset the mark bit and add it to the live list
 n->next = live;
 live = n;
 }
 n = tl;
 }// iterates over each allocated node 
 a->nodes = live; // upadating to live nodes. 
 }
 