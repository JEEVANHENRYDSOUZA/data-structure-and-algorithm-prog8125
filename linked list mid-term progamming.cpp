/*******************************************************************************************
* File Name   :	midtermprogramming.cpp
* Description : removeDuplicates - which will delete only the nodes that have the same data in it
*			    removeLastNode - will delete the last node from list
*Author       : Jeevan Henry Dsouza
*Date         : 20-10-2021
* Student Number: 8716171
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_DATA_ITEMS	12					// the number of data items to be used in list

// definition of linked list data nodes
typedef struct LinkedListNode {
	struct LinkedListNode* ptrNextNode;		// pointer to next node in list (NULL if the end)
	int						NumberData;			// the number stored in node
} LinkedListNodeDef;

// prototypes
LinkedListNodeDef* AddToLinkedList(LinkedListNodeDef* ptrHead, int NameData);
void PrintLinkedList(LinkedListNodeDef* ptrHead);
LinkedListNodeDef* removeDuplicates(LinkedListNodeDef* ptrHead);
void removeLastNode(LinkedListNodeDef* ptrHead);


// data to be used for this programming question
int iData[NUMBER_OF_DATA_ITEMS] = {
	6,
	3,
	19,
	9,
	93,
	19,
	86,
	72,
	40,
	48,
	3,
	59
};

// your output from the program should look like this when working correctly:
//Data before Linked List Operations is :
//1.      6
//2.      3
//3.      19
//4.      9
//5.      93
//6.      19
//7.      86
//8.      72
//9.      40
//10.     48
//11.     3
//12.     59
//
//Data after inserted into linked list in numeric order :
//1.      3
//2.      3
//3.      6
//4.      9
//5.      19
//6.      19
//7.      40
//8.      48
//9.      59
//10.     72
//11.     86
//12.     93
//
//Data after processing :
//1.      3
//2.      6
//3.      9
//4.      19
//5.      40
//6.      48
//7.      59
//8.      72
//9.      86


// START OF CODE YOU ARE ALLOWED TO CHANGE

// FUNCTION      : removeDuplicates()
// DESCRIPTION   : This function will iterate through a linked list and remove all duplicate entries 
// PARAMETERS    : LinkedListNodeDef *ptrHead	- variable to store address of ptrHead
// RETURNS       : possibly updated pointer to the head of the list
LinkedListNodeDef* removeDuplicates(LinkedListNodeDef* ptrHead) {  //refernce taken from https://leetcode.com/problems/remove-duplicates-from-sorted-list/submissions/
	LinkedListNodeDef* currentNode = ptrHead;
	LinkedListNodeDef* temp;  //to used to store the node present after the duplicate node 
	LinkedListNodeDef* temp2;//used to store the currenNode 
	while (currentNode->ptrNextNode != NULL && currentNode != NULL) {          //in order to check i should have minimum of two nodes because if there is only one node then it will be sorted by itself and i am traversing till i reach the last node of the list
		if (currentNode->ptrNextNode->NumberData == currentNode->NumberData) {
			temp = currentNode->ptrNextNode->ptrNextNode; //since the list is sorted in nature the duplicate nodes will be consecutive hence here i am storing it in a variable 
			temp2 = currentNode;
			free(currentNode->ptrNextNode);          //deallocating memory
			currentNode = temp2;                     //i am again re-intializing it 
			currentNode->ptrNextNode = temp;         

		}
		else {

			currentNode = currentNode->ptrNextNode;   //if the nodes are not duplicate then i am just traversing the list


		}

	}

	return ptrHead;
}

// FUNCTION      : removeLastNode()
// DESCRIPTION   : This function will iterate through a linked list and remove/free the last entry
// PARAMETERS    : LinkedListNodeDef *ptrHead	- variable to store address of ptrHead
// RETURNS       : Nothing
void removeLastNode(LinkedListNodeDef* ptrHead) {
	LinkedListNodeDef* temp = ptrHead;         //refernced from https://www.alphacodingskills.com/ds/notes/linked-list-delete-the-last-node.php
	if (ptrHead != NULL) {            

		if (ptrHead->ptrNextNode == NULL) {         //checking for wether the list contains only one element
			ptrHead = NULL;
		}
		else {
			while (temp->ptrNextNode->ptrNextNode != NULL) { temp = temp->ptrNextNode; }  //going to the node before the last node
			LinkedListNodeDef* last = temp->ptrNextNode;
			temp->ptrNextNode = NULL;
			free(last);   //deleting the last element from memory
		}
	}
	return;
}

// END OF CODE YOU ARE ALLOWED TO CHANGE

// DO NOT CHANGE ANY OF THE CODE BELOW!  

int main(void) {
	int i;

	// define linked list head
	struct LinkedListNode* ptrHead = NULL;

	// Print out the input data
	printf("Data before Linked List Operations is:\n");
	for (i = 0; i < NUMBER_OF_DATA_ITEMS; i++) {
		printf("%d.\t%d\n", i + 1, iData[i]);
		ptrHead = AddToLinkedList(ptrHead, iData[i]);
	}

	// now print out the list in order it is stored starting at head
	printf("\nData after inserted into linked list in numeric order:\n");
	PrintLinkedList(ptrHead);
	removeDuplicates(ptrHead);

	removeLastNode(ptrHead);

	// now print out the list in order it is stored starting at head after required operations are complete
	printf("Data after processing:\n");
	PrintLinkedList(ptrHead);

	return 0;
}

// FUNCTION      : AddToLinkedList()
// DESCRIPTION   : This function takes an input of Data of 30 Char word and an index and 
//				   places them in a linked list organized numerically
// PARAMETERS    : LinkedListNodeDef *ptrHead	- variable to store address of ptrHead
//				   int	NumberData				- Number to add to linked list 
// RETURNS       : possibly updated pointer to the head of the list
LinkedListNodeDef* AddToLinkedList(LinkedListNodeDef* ptrHead, int NumberData) {
	LinkedListNodeDef* ptrCurrent = ptrHead;
	LinkedListNodeDef* ptrNew = NULL;

	// allocate a new node to be added to linked list
	ptrNew = (LinkedListNodeDef*)calloc(1, sizeof LinkedListNode);
	if (ptrNew == NULL) {
		printf("calloc failure!!\n");
		exit(-1);
	}

	// load new node with data
	ptrNew->ptrNextNode = NULL;
	ptrNew->NumberData = NumberData;

	if (ptrCurrent == NULL) {		// if list is empty simply change head to be new node
		ptrHead = ptrNew;
	}
	else {
		LinkedListNodeDef* ptrLast = NULL;

		// stop traversing if we reach the end of the list
		while (ptrCurrent != NULL) {
			// check for numeric order
			if (ptrCurrent->NumberData > NumberData)
			{
				// we found where new node should go so need to link in to list
				if (ptrLast == NULL)		// if the last pointer is NULL this means no traversing happened so put on beginning
					ptrHead = ptrNew;
				else						// otherwise link it into chain between last node and current
					ptrLast->ptrNextNode = ptrNew;
				ptrNew->ptrNextNode = ptrCurrent;
				return ptrHead;
			}
			ptrLast = ptrCurrent;			// keep track of last node because sometimes it is needed to insert new data
			ptrCurrent = ptrCurrent->ptrNextNode;	// traverse to next node
		}
		if (ptrCurrent == NULL) {			// if the current pointer is NULL that means we reached the end of the list so put it there
			ptrLast->ptrNextNode = ptrNew;
		}
	}
	return ptrHead;
}

// FUNCTION      : PrintLinkedList()
// DESCRIPTION   : This function takes an input of the head of the linked list, then loops through
//				   and prints each word in the linked list until the ptrNextNode is NULL.
// PARAMETERS    : LinkedListNodeDef *ptrHead 
// RETURNS       : void
void PrintLinkedList(LinkedListNodeDef* ptrHead) {
	int i = 0;
	LinkedListNodeDef* ptrCurrent = ptrHead;

	// check for empty list
	if (ptrCurrent != NULL) {
		// traverse list looking for the end printing the items out as we go
		for (i = 1; ptrCurrent != NULL; i++)
		{
			printf("%i.\t%d\n", i, ptrCurrent->NumberData);
			ptrCurrent = ptrCurrent->ptrNextNode;
		}
	}
	printf("\n");
	return;
}

