#pragma GCC diagnostic ignored "-Wstringop-truncation"
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>
#include <errno.h>

#include <fcntl.h>

#define file	"./wl.txt"

#define TagRoot 1 /* 00 01 */
#define TagNode 2 /* 00 10 */
#define TagLeaf 3 /* 01 00 */

#define Print(x) \
		zero((char*)buf, 256); \
		strncpy((char*)buf, (x),255); \
		size = (int16)strlen((char*)buf); \
		if(size){ \
			(void)!write(fd, (char*)buf, size); \
		}


typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char Tag;


struct node{
	Tag tag;
	struct node* up;
	struct node* left;
	struct leaf* right;
	char path[256];

};
typedef struct node Node;

struct leaf{
	Tag tag;
	union tree* back;
	struct leaf* right;
	char key[128];
	char* value;
	int16 size;
};
typedef struct leaf Leaf;

union tree{
	Node n;
	Leaf l;
};
typedef union tree Tree;

void printTree(int, Tree*);
char* indent(int16);
void zero(char*, int16);
Leaf *find_leaf_linear(char*,char*);
char *lookup_leaf_value_linear(char*, char*);
Node *find_node_linear(char*);
Node *create_node(Node*, char*);
Leaf *find_last_linear(Node*);
Leaf *create_leaf(Node*, char*, char*, int16);
Tree *example_tree(void);
int32 *example_leaves(void);
char *example_path(char);
int main(void);
