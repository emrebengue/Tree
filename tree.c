#include "tree.h"
#include <fcntl.h>

Tree root = { .n = {
	.tag = (TagRoot | TagNode),
	.up = (Node*)&root,
	.right = 0,
	.left = 0,
	.path = "/"
}};

void printTree(int fd, Tree *t){
	static char buf[256];
	int16 indentation;
	int16 size;
	Node *n;
	Leaf *l;
	
	indentation = 0;
	for(n = (Node*)t; n; n = n->left){
		Print(indent(indentation++));
		Print(n->path);
		Print("\n");
		if(n->right != NULL){
			for(l = n->right; l; l = l->right){
				Print(indent(indentation));
				Print(n->path);
				Print("/");
				Print(l->key);
				Print(" -> '");
				(void)!write(fd, l->value, l->size);
				Print("'\n");
			}
		}
	}

	return;

}

char *indent(int16 s){
	static char buf[256];
	char *p;
	int16 i;
	
	if(s < 1){
		return "";
	}
	assert(s < 120);
	zero((char*)buf, 256);
	
	for(i=0, p=(char*)buf; i<s; i++, p+=2){
		strncpy((char*)p, "  ", 2);
	}
	return (char*)buf;
}

void zero(char* str, int16 size){
	memset(str, 0, size);
}

Node* create_node(Node* parent, char* path){
	Node* n;
	int16 size;

	assert(parent);
	size = sizeof(struct node);
	n = (Node*)malloc((int)size);
	zero((char*)n, size);

	parent->left = n;
	n->tag = TagNode;
	n->up = parent;
	strncpy((char*)n->path, (char*)path, 255);
	
	return n;
}
Node *find_node_linear(char* path){
	Node *p, *ret;
	
	for(ret=(Node*)0,p=(Node*)&root; p; p = p->left){
		if(!strcmp(p->path, path)){
			ret = p;
			break;
		}
	}
	return ret;
}

Leaf* find_leaf_linear(char *path, char *key){
	Node *n;
	Leaf* l, *ret;

	n=find_node_linear(path);
	if(!n){
		return (Leaf*)0;
	}
	for(ret=(Leaf*)0, l=n->right; l; l=l->right){
		if(strcmp(l->key, key) == 0){
			ret = l;
			break;
		}
	}
	return ret;
}

char *lookup_leaf_value_linear(char *path, char *key){
	Leaf *p;

	p = find_leaf_linear(path,key);

	return (p) ? p->value : 0;
}

Leaf* find_last_linear(Node* parent){
	assert(parent);
	Leaf* l;
	
	errno = 0;
	if(!parent->right){
		return NULL;
	}
	for(l = parent->right; l->right; l = l->right);

	assert(l);
	return l;

}

Leaf* create_leaf(Node* parent, char* key, char* value, int16 count){
	Leaf *l, *newLeaf;
	int16 size;
	
	assert(parent);
	l = find_last_linear(parent);

	size = sizeof(struct leaf);
	newLeaf = (Leaf*)malloc(size);
	assert(newLeaf);
	if(l == NULL){
		parent->right = newLeaf;
	}
	else{	
		l->right = newLeaf;
	}

	zero((char*)newLeaf, size);
	newLeaf->tag = TagLeaf;
	newLeaf->back = (!l) ? (Tree*)parent : (Tree*)l;

	strncpy((char*)newLeaf->key, key, 127);
	newLeaf->value = (char*)malloc(count);
	zero(newLeaf->value, count);
	assert(newLeaf->value);

	strncpy((char*)newLeaf->value, value, count);

	newLeaf->size = count;

	return newLeaf;

}

Tree *example_tree(){
	char c;
	Node *n, *p;
	char path[256];
	int32 x;

	zero(path,256);
	//strcpy(path,"/");
	x = 0;
	for(n= (Node*)&root, c='a'; c<= 'z'; c++){
		x = (int32)strlen(path);
		path[x++] = '/';
		path[x++] = c;
		p = n;
		n = create_node(p, path);
	}
	return (Tree*)&root;

}
char *example_path(char path){
	static char buf[256];
	char c;
	int32 x;
	zero(buf, 256);
	
	for(c = 'a'; c <= path; c++){
		x = (int32)strlen(buf);
		*(buf + x++) = '/';
		*(buf + x) = c;
	}
	
	return buf;
}

int32 *example_leaves(){
	int fd;
	char c;
	char buf[256];
	char path;
	Leaf *l;
	Node *n;

	fd = open(file,O_RDONLY);
	assert(fd);
	

}

int main(){
	Tree *example;

	printf("%s\n", example_path('j'));

	example = example_tree();
	printTree(1, example);

	return 0;
}

