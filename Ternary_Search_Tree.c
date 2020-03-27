//ARDA BAYRAM - 150116029
//This program create a tst with given values after that search and remove its nodes

#include <stdio.h>
#include <stdlib.h>

#define square(x) x*x

struct terTree{
	int data;
	struct terTree * left, * mid, * right;
};
typedef struct terTree node;

int count;//Assign a global counter for depth level

void insert(node ** tree, int key)
{//This function insert nodes to tree
	node *temp = NULL;
	
	if(!(*tree))
	{//Check empty tree
		temp = (node *)malloc(sizeof(node));
		temp->left = temp->mid = temp->right = NULL;
		temp->data = key;
		*tree = temp;
		return;
	}
	
	//Check some conditions after that, insert the node recursively
	if(key < (*tree)->data)
	{
		insert(&(*tree)->left, key);
	}
	else if(key > (*tree)->data && key <= square((*tree)->data))
	{
		insert(&(*tree)->mid, key);
	}
	else if(key > square((*tree)->data))
	{
		insert(&(*tree)->right, key);
	}
	
}

node *findMin(node *p)
{//Find minimum data in given tree
	if(p != NULL)
	{
		while(p->left != NULL) p=p->left;
	}
	return p;
}

node *findMax(node *p)
{//Find maximum data in given tree
	if(p == NULL) return NULL;
	if(p->right == NULL)
		if(p->mid == NULL)
			return p;
	if(p->right != NULL)
		return findMax(p->right);
	else if(p->mid != NULL)
		return findMax(p->mid);
}

node* Remove(node *tree, int key)
{//Remove node and rebuild the tree recursively
	node *temp;
	
	if(!tree)
	{//Check empty tree
		printf("Empty tree!\n");
		return;
	}
	
	//Check conditions and remove the node. Then rebuild the tree
	if(key < tree->data)
	{
		tree->left = Remove(tree->left, key);
	}
	else if(key > tree->data && key <= square(tree->data))
	{
		tree->mid = Remove(tree->mid, key);
	}
	else if(key > square(tree->data))
	{
		tree->right = Remove(tree->right, key);
	}
	else if((tree->left && tree->mid && !(tree->right)) || (!(tree->left) && tree->mid && tree->right) || 
	(tree->left && tree->mid && tree->right) || (!(tree->left) && tree->mid && !(tree->right)))
	{
		temp = findMin(tree->mid);
		if(tree->right)
		{
			if(square(temp->data) > tree->right->data)
			{
				temp = findMax(tree->left);
				if(square(temp->data) < tree->mid->data)
				{
					tree->right->left = tree->mid;
					tree->mid = NULL;
				}
				tree->data = temp->data;
				tree->left = Remove(tree->left, tree->data);
			}
			else
			{
				tree->data = temp->data;
				tree->mid = Remove(tree->mid, tree->data);		
			}
		}
		else
		{
			tree->data = temp->data;
			tree->mid = Remove(tree->mid, tree->data);	
		}
	}
	else if((tree->left && !(tree->mid) && !(tree->right)) || (tree->left && !(tree->mid) && tree->right))
	{
		temp = findMax(tree->left);
		tree->data = temp->data;
		tree->left = Remove(tree->left, tree->data);
	}
	else if(!(tree->left) && !(tree->mid) && tree->right)
	{
		temp = findMin(tree->right);
		tree->data = temp->data;
		tree->right = Remove(tree->right, tree->data);
	}
	else
	{
		temp = tree;
		if(tree->left && !(tree->mid) && !(tree->right))
		{
			tree=tree->left;
		} 
		else if(!(tree->left) && tree->mid && !(tree->right))
		{
			tree=tree->mid;
		} 
		else if(!(tree->left) && !(tree->mid) && tree->right)
		{
			tree=tree->right;	
		}
		else
		{
			tree=NULL;
			free(temp);	
		} 
	}
	return tree;
}

void search(node **tree, int key, int cnt)
{//Search a node and count its depth level
	if(!(*tree))
	{//Check wrong values
		printf("There is no node with that value!\n");
		return;
	}
	
	//Check node recursively and print its informations
	if(key < (*tree)->data)
	{
		cnt++;
		search(&(*tree)->left, key, cnt);
	}
	else if(key > (*tree)->data && key <= square((*tree)->data))
	{
		cnt++;
		search(&(*tree)->mid, key, cnt);
	}
	else if(key > square((*tree)->data))
	{
		cnt++;
		search(&(*tree)->right, key, cnt);
	}
	else if(key == (*tree)->data)
	{
		printf("Your data is: %d \nDepth level: %d\n", (*tree)->data, cnt);
		if((*tree)->left) printf("Left child: %d\n", (*tree)->left->data);
		if((*tree)->mid) printf("Mid child: %d\n", (*tree)->mid->data);
		if((*tree)->right) printf("Right child: %d\n", (*tree)->right->data);
		return;
	}
}

void main()
{
	
	node *root;
	char op;
	int num;
	
	FILE* file = fopen ("input.txt", "r");
    int i = 0;

    while (!feof (file))
    {//Read the input file and create the tree  
        fscanf (file, "%d", &i);      
        insert(&root, i);
    }
    fclose (file);
    
    printf(">>r: remove; s: search; e: exit\n>>");
    do{//Check conditons and call functions
    	scanf("%c", &op);
    	switch(op)
    	{
    		case 'r': printf(">>"); scanf("%d", &num); Remove(root, num); printf("\n>>r: remove; s: search; e: exit\n>>");
    				break;
    		case 's': count=0; printf(">>"); scanf("%d", &num); search(&root, num, count); printf("\n>>r: remove; s: search; e: exit\n>>");
					break;		
    		case 'e': break;
		}
	} while(op != 'e');
	
}
