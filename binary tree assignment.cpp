/*******************************************************************************
  * File Name          : assignment2.c
  * Description        : i am building a binary search tree and doing its inorder traversal so that the output data is printed sorted manner
  * Student Number     : 8716171
  * Author:            : Jeevan Henry Dsouza
  * Date:              :   18-10-2021
  ******************************************************************************
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //includeing this library fro strcmp function as it is available in this library
// structure defining a binary tree node.  Lower sorted values will go the the left, higher to the right.
struct nodeData {
	char name[30];				// character string of the data being stored in the node
	struct nodeData* left;		// pointer to the next node in the tree that is less than current node OR NULL if empty
	struct nodeData* right;		// pointer to the next node in the tree that is greater than current node OR NULL if empty
};


// data to be stored in tree.  In this case there is just a character string but this could have contained more

// replace inputData used for test put it in here and is created from Prog8130AssignDataCreator.exe 
const char* inputData[12] = {
		{ "pkwts"},
		{ "vkrvc"},
		{ "idhbx"},
		{ "qsrvr"},
		{ "lkhuq"},
		{ "qtufy"},
		{ "foocm"},
		{ "fvwra"},
		{ "rbpou"},
		{ "mbera"},
		{ "qlaxi"},
		{ "fkjke"}
};

struct nodeData* head = NULL;	// head of the binary tree
// The correct sorted order using a binary tree is:
//      fkjke
//      foocm
//      fvwra
//      idhbx
//      lkhuq
//      mbera
//      pkwts
//      qlaxi
//      qsrvr
//      qtufy
//      rbpou
//      vkrvc

// FUNCTION      : newnode
// DESCRIPTION   : creates the node of the tree with left,right and root
// PARAMETERS    :  newString - the string that is to be stored in the binary tree in alphabetica order 
// RETURNS       : head/root of the tree
struct nodeData* newnode(const char* newString) {
	struct nodeData* newNode = (struct nodeData*)malloc(sizeof(struct nodeData));
	if (newNode == NULL) { printf_s("malloc error"); }                             //here i am actually creating the node for the binary search tree
	else {
		strcpy_s(newNode->name, newString);
		newNode->right = NULL;
		newNode->left = NULL;
	}

	return newNode;
}


// FUNCTION      :  addToBinaryTree
// DESCRIPTION   :  This function will store the data in newNode into a binary tree according to alphabetical order
// PARAMETERS    :  newString - the string that is to be stored in the binary tree in alphabetica order 
// RETURNS       :  head/root  of the tree

void addToBinaryTree(const char* newString) {
	struct nodeData* current = NULL;             //refernce for this function is taken from https://www.geeksforgeeks.org/insert-a-node-in-binary-search-tree-iteratively/
	if (head == NULL) {
		head = newnode(newString);             //making the first element of the array as the root of the binary tree
		return;
	}
	current = head;
	while (1) {
		if (strcmp(newString, current->name) > 0) { //i am checking wether input string is greater than the current stored string
			if (current->right != NULL) {

				current = current->right;          //traversing the right tree in order to find the correct place to insert the newly created node
			}

			else {
				current->right = newnode(newString);   //inserting the newly created node
				break;

			}
		}
		else {

			if (current->left != NULL) {        //this is again traversing the left string if the string is less than the input array string

				current = current->left;
			}

			else {


				current->left = newnode(newString);  //inserting the newly creadted node in  the left subtree
				break;

			}
		}
	}
	return;  // for the next element in the input array
}



// FUNCTION      : printBinaryTree
// DESCRIPTION   : This function will print the entire binary tree out.You can choose to print.
// PARAMETERS    : None   
// RETURNS       : Nothing
void printBinaryTree(void) {      //i am doing morris in-order traversal
	struct nodeData* curr, * prev;    //refernce for this function is taken from https://www.educative.io/edpresso/what-is-morris-traversal

	curr = head;
	while (curr != NULL) {
		if (curr->left == NULL) {
			printf_s("%s\n", curr->name);
			curr = curr->right;
		}
		else {

			prev = curr->left;
			while (prev->right != NULL && prev->right != curr)     // Find the previous (prev) of curr 
				prev = prev->right;
			if (prev->right == NULL) {                        //make current as the right child of previous
				prev->right = curr;
				curr = curr->left;
			}

			else {
				prev->right = NULL;                       //fix the right child of previous
				printf_s("%s\n", curr->name);
				curr = curr->right;
			}
		}
	}

}


// FUNCTION      : printBinarytreerecusrsive
// DESCRIPTION   : This function will print the entire binary tree out.You can choose to print.
// PARAMETERS    : head- root/head of the binary tree     
// RETURNS       : Nothing
void printBinaryTreeRecursive(struct nodeData* head) {
	//refernce for this function is taken from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/

	if (head == NULL)                       //breaking condition of the recusrion
		return;


	printBinaryTreeRecursive(head->left);          //i am doing inorder traversal which  will give the sorted order hence first traversing to the left 

	printf_s("%s\n", head->name);


	printBinaryTreeRecursive(head->right);          //traversing the right

}




int main() {
	int i;
	for (i = 0; i < 12; i++)
		addToBinaryTree(inputData[i]);	// put all the data into the binary tree
	printf_s("iterative printing output\n");
	printBinaryTree();
	printf_s("\n");
	printf_s("\n");
	printf_s("recursive printing output\n");
	printBinaryTreeRecursive(head);
	// print the entire tree.  It should come out in alphabetical order because of the way it was stored in the tree
	return 0;
}
