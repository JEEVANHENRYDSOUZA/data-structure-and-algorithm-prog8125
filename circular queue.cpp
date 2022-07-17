/*******************************************************************************
* File Name          : circularQueueV2021.cpp
* Description        : Simple example of an implemenation of a circular Queue
*                      skelleton
*
* Author:            : Jeevan Dsouza
* Date:              :  Nov 3 2021
* Student Number     :  8716171
******************************************************************************
*/

#include <windows.h>
#include <stdio.h>

DWORD WINAPI myReadThreadFunction(LPVOID lpParam);                                   // function used by read from queue thread
unsigned int putToCircularQueue(char* ptrInputBuffer, unsigned int bufferLength);    // circular queue function to add data to queue
unsigned int getFromCircularQueue(char* ptrOutputBuffer, unsigned int bufferLength); // circular queue function to remove data from queue


#define BUFFER_SIZE         200                // local buffer used for sending or receiving data to/from queue in main and worker thread

#define INPUT_TIMEOUT_MS    5000               // dequeue every 5 seconds

#define CIRCULAR_QUEUE_SIZE 10                 // size of the circular queue


// data structure used to keep track of circular queue
struct myQueueStruct {
	char* ptrBuffer = NULL;                    // pointer to start of the circular queue buffer
	volatile unsigned int queueCount = 0;      // number of characters currently in cirucular queue
	volatile char* ptrCircularHead = NULL;     // location where data is added to queue
	volatile char* ptrCircularTail = NULL;     // loation where data is removed from queue
};

struct myQueueStruct myQueue;                  // create an instance of the circular queue data structure

int main()
{
	HANDLE  hThread;            // handle to thread
	char    inputBuffer[BUFFER_SIZE];

	myQueue.ptrBuffer = (char*)calloc(CIRCULAR_QUEUE_SIZE, sizeof(char));
	if (myQueue.ptrBuffer == NULL)
	{
		printf("Error unable to allocate memory for buffer\n");
		exit(-1);
	}

	myQueue.ptrCircularHead = myQueue.ptrCircularTail = myQueue.ptrBuffer;

	// create a thread that will consume the data we type in to demonstrate dequeing the data
	hThread = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		myReadThreadFunction,   // thread function name
		NULL,                   // argument to thread function (not being used)
		0,                      // use default creation flags 
		NULL);                  // returns the thread identifier (not being used)

	printf("length of circular queue is %d\n", CIRCULAR_QUEUE_SIZE);
	// get a string from the console and queue it to circular queue
	while (1)
	{
		scanf_s("%199s", inputBuffer, BUFFER_SIZE - 1);     // get data from console (note this is the input from console not the circular queue yet)
		inputBuffer[BUFFER_SIZE - 1] = '\0';			// ensure the read string has a nul char on end of string

		// put the data into the circular queue but check if an error (marked by queue function returning 0) occurred

		if (putToCircularQueue(inputBuffer, strlen(inputBuffer)) == 0)
			printf("Error queuing data\n");
	}
	return 0;
}

// FUNCTION      :  putToCircularQueue
// DESCRIPTION   :  put data into queue
// PARAMETERS    :  ptrInputBuffer - a pointer to the buffer to be put into the queue
// bufferLength- length of the input string
// RETURNS       :  Number of characters successfully put in the queue and zero on failure
// REFERENCE     :  Refrence for this code is class lecture on queue and also https://www.programiz.com/dsa/circular-queue here it is given with index and i have coverted it into pointer based code

unsigned int putToCircularQueue(char* ptrInputBuffer, unsigned int bufferLength)
{
	unsigned int numberOfCharsQueued = 0;  // number of characters placed in circular queue
	char* queueEndBuffer = myQueue.ptrBuffer + (CIRCULAR_QUEUE_SIZE * sizeof(char)) - 1;

	if (bufferLength > 10) { // the circular queue length is fixed that is 10 and if any string enetered is greater than 10 them i am displaying error message

		return 0;

	}
	else {
		for (unsigned int i = 0;i < bufferLength;i++) {
			if (myQueue.queueCount == 10) {
				printf("overflow\n"); // if there is no space to add the new string hence overflow
				break;
			}

			*myQueue.ptrCircularTail = *(ptrInputBuffer + i);  //using relative index to add contents from input buffer 
			myQueue.queueCount++;
			numberOfCharsQueued++;

			if (myQueue.ptrCircularTail == queueEndBuffer) {    // when the tail is pointing to the end of the queue and there is space wrapping it around to add more content 
				myQueue.ptrCircularTail = myQueue.ptrBuffer;
			}
			else {
				myQueue.ptrCircularTail++;
			}


		}
	}


	return numberOfCharsQueued;
}





// FUNCTION      : getFromCircularQueue
// DESCRIPTION   : Read as much data as there is room for from the circular queue
// PARAMETERS    : ptrOutputBuffer - a pointer to the buffer to place the data read from queue 
//               : bufferLength- maxiumum number of characters that can be read (IE the size of the output buffer)
// RETURNS       : Number of characters successfully queued (0 if none are read)
//REFERNCE       : https://www.programiz.com/dsa/deque
unsigned int getFromCircularQueue(char* ptrOutputBuffer, unsigned int bufferLength)
{
	unsigned int readCount = 0;
	char* queueEndBuffer = myQueue.ptrBuffer + (CIRCULAR_QUEUE_SIZE * sizeof(char)) - 1; // making a pointer point to the end of the queue
	for (unsigned int i = 0;i < bufferLength;i++) {
		if (myQueue.queueCount == 0) {              //if there are no elements in the queue tha is case of underflow then breaking the dequeue function
			break;
		}
		*(ptrOutputBuffer) = *(myQueue.ptrCircularHead); //mnormal 
		myQueue.queueCount--;  //as we remove element from the queue this will decrease
		readCount++;
		ptrOutputBuffer++;

		if (queueEndBuffer == myQueue.ptrCircularHead) { //if head is pointing to the end of the queue and there are elements to be removed then wraping it around
			myQueue.ptrCircularHead = myQueue.ptrBuffer;
		}
		else {
			myQueue.ptrCircularHead++;//making it point to the next element to be dequeued
		}
	}

	*ptrOutputBuffer = '\0'; //the output string needs to be null terminated at the nedd else it will give garbage values
	return readCount;
}


// FUNCTION      : myReadThreadFunction
// DESCRIPTION   :
//   A seperate thread from the main program that will independently check for data in queue and print it out
// PARAMETERS    :
//   lpParam - arguement passed into thread (not used in this example)
//
// RETURNS       :
//   Will never return so no effective return value

DWORD WINAPI myReadThreadFunction(LPVOID lpParam)
{
	char readBuffer[BUFFER_SIZE];     // local buffer to put the data into when reading from queue and print
	unsigned int  readCount = 0;      // the number of characters read from queue

	while (1)
	{
		Sleep(INPUT_TIMEOUT_MS);      // wait for some data to be received

		// check if data is available and if so print it out
		readCount = getFromCircularQueue(readBuffer, BUFFER_SIZE);
		if (readCount != 0)           // check for reads that did not get any data
			printf("UNQUEUE: %s\n", readBuffer);
	}
	return 0;    // will never reach here
}
