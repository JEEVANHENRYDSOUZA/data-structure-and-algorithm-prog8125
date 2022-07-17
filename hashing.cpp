/*******************************************************************************
* File Name          : hashAssignmentV3.cpp
* Description        : Implemenation of a hash skelleton
*
* Author:            :  jeevan dsouza
* Date               : Nov 24, 2021
* student number     : 8716171
******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int putIntoHashTable(char* ptrInputData, unsigned int bufferLength);    // function to add to hash table
unsigned int getFromHashTable(char* ptrOutputData, unsigned int bufferLength);   // function to retrieve data from hash table

#define INPUT_BUFFER_SIZE         200             // local buffer used for adding data to the hash table (there is no reason in this assignment to change this value)

#define HASH_SIZE    10                          // size of hash table to be used (for testing I suggest making this number much lower)

												  // data structure used to keep track of hashed data
struct myHashStruct {
	char* ptrBuffer = NULL;                       // pointer to data stored in hash (you will need to malloc space for string to be stored)
	struct myHashStruct* ptrNextHashData = NULL;  // pointer to next item in this hash bucket (or NULL if no more)
};

struct myHashStruct* myHash[HASH_SIZE];           // create an empty hash table structure (note this is basically an arrary of linked list heads)

int main()
{
	char    inputBuffer[INPUT_BUFFER_SIZE];

	// initialize the hash table to empty one
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if ((myHash[i] = (struct myHashStruct*)calloc(1, sizeof(struct myHashStruct))) == NULL)
		{
			printf("calloc failed!\n");
			return(-1);
		}
	}

	// add to hash table loop
	while (1)
	{
		printf("enter data to be added to hash table or exit when done\n");

		// get strings from the console and place in hash until nothing entered
		scanf_s("%199s", inputBuffer, INPUT_BUFFER_SIZE);
		inputBuffer[INPUT_BUFFER_SIZE - 1] = '\0';

		// stop adding data into hash table when "exit" is entered
		if (strcmp(inputBuffer, "exit") == 0)
			break;

		if (putIntoHashTable(inputBuffer, strlen(inputBuffer)) == HASH_SIZE)
			printf("Error putting into hash table\n");
	}

	// check if data is in hash table
	while (1)
	{
		unsigned int hashIndexNumber = 0;

		printf("Enter data to find, done when complete\n");

		// get strings from the console and check if in hash table
		scanf_s("%199s", inputBuffer, INPUT_BUFFER_SIZE);
		inputBuffer[INPUT_BUFFER_SIZE - 1] = '\0';

		// stop adding data into hash table when "done" is entered
		if (strcmp(inputBuffer, "done") == 0)
			break;

		if ((hashIndexNumber = getFromHashTable(inputBuffer, strlen(inputBuffer))) == HASH_SIZE)
			printf("%s not found in hash table\n", inputBuffer);
		else
			printf("%s found in hash table at %u\n", inputBuffer, hashIndexNumber);
	}

	return 0;
}

// FUNCTION      : myHashFunction
// DESCRIPTION   :
//   Calculate a hash value to use in storing the data into the hash table
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//
// RETURNS       :
//   Hash value calculated ( HASH_SIZE on failure)
// reference for this function is taken from class notes
unsigned int myHashFunction(char* ptrInputBuffer)
{
	unsigned int calculatedHashValue = 0;

	// add code to create a hashed value here!!!
	int c;
	while (c = *ptrInputBuffer++) {
		calculatedHashValue = c + (calculatedHashValue << 6) + (calculatedHashValue << 16) - calculatedHashValue;

	}

	// make sure if hash value is bigger than the table size, the value wraps
	return calculatedHashValue % HASH_SIZE;
}

// FUNCTION      : putIntoHashTable
// DESCRIPTION   :
//   Put the supplied data into a hash table
// PARAMETERS    :
//   ptrInputBuffer - a pointer to the buffer to be put into the hash table
//   bufferLength   - how many characters are in the buffer being put into the hash table
//
// RETURNS       :
//   Hash value used ( HASH_SIZE on failure)

unsigned int putIntoHashTable(char* ptrInputData, unsigned int bufferLength)
{

	unsigned int hashValue = myHashFunction(ptrInputData);
	printf("hashVal=%d\n", hashValue);// printing it for testing purposes 
	struct myHashStruct* newNode;
	if ((newNode = (myHashStruct*)malloc(sizeof(myHashStruct))) == NULL) { //creating  a newnode
		printf("memory allocation failed!\n");
		return HASH_SIZE;
	}
	newNode->ptrNextHashData = NULL;


	newNode->ptrBuffer = (char*)malloc(bufferLength + 1);    //creating the character array
	if (newNode->ptrBuffer == NULL) {

		printf("memory allocation failed!\n");
		return HASH_SIZE;

	}

	strncpy_s(newNode->ptrBuffer, bufferLength + 1, ptrInputData, strlen(ptrInputData));
	if (myHash[hashValue]->ptrBuffer == NULL) {
		myHash[hashValue] = newNode;             //inserting the node at the specific position equivalent to the hashValue
		myHash[hashValue]->ptrNextHashData = NULL;
		return hashValue;
	}
	struct myHashStruct* temp = myHash[hashValue];
	while (temp->ptrNextHashData != NULL) {           //refernce for this traversal is taken from linked list assignment
		temp = temp->ptrNextHashData;
	}
	temp->ptrNextHashData = newNode;

	// add code to put data into the hash table!!!

	return hashValue;
}

// FUNCTION      : getFromHashTable
// DESCRIPTION   :
//   Read as much data as there is room for from the hash table
// PARAMETERS    :
//   ptrOutputBuffer - a pointer to the buffer to place the data read from hash table
//   bufferLength    - maxiumum number of characters that can be read
//
// RETURNS       :
//   Hash value used ( return HASH_SIZE value on failure)

unsigned int getFromHashTable(char* ptrOutputData, unsigned int bufferLength)
{
	unsigned int hashValue = myHashFunction(ptrOutputData);

	if (myHash[hashValue]->ptrBuffer != NULL && ptrOutputData != NULL) {

		struct myHashStruct* temp = myHash[hashValue];
		while (temp != NULL && temp->ptrBuffer != NULL) { //traverseing through and checking for match returning back if not found or
			if (strncmp(temp->ptrBuffer, ptrOutputData, strlen(ptrOutputData)) == 0) {  //comparing to get a match
				return hashValue;
			}
			temp = temp->ptrNextHashData;     //again traversing
		}
	}
	return HASH_SIZE;
}

// end code *     *     *  
