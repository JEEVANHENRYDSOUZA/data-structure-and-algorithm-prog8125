/*******************************************************************************
* File Name          : finalProg8130CircQueue1.cpp
* Description        : Starter Code for final programming question
*                      skelleton
*
* Author:            :  PROG8130 / Allan Smith
*                    :  jeevan dsouza 8716171
* Date:              :  Dec 11 2021
******************************************************************************
*/

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

DWORD WINAPI myReadThreadFunction(LPVOID lpParam);              // function used by read from queue thread
unsigned int putToCircularQueue(uint32_t inputBuffer);			// circular queue function to add data to queue
unsigned int getFromCircularQueue(uint32_t* ptrOutputBuffer);	// circular queue function to remove data from queue
unsigned int randomRange(unsigned int minVal, unsigned int maxVal);		// function to generate a random number between the two values given
uint32_t simulateDeviceRead(void);								// simulated read from a device of a 32 bit unsigned number
int numberOfReadsToDo(void);									// returns a random number of successive calls to simulateDeviceRead to perform

#define MAX_DATA_TO_READ	5					// mamimum number of values to read from queue
#define MIN_TIMEOUT_MS    500					// minimum time between put to queue and get from queue
#define MAX_TIMEOUT_MS    5000					// maximum time between put to queue and get from queue

#define CIRCULAR_QUEUE_SIZE 10					// size of the circular queue

// data structure used to keep track of circular queue
struct myQueueStruct {
	uint32_t* ptrBuffer = 0;                   // pointer to start of the circular queue buffer
	volatile unsigned int queueCount = 0;      // number of items currently in cirucular queue (optional)
	volatile uint32_t* ptrCircularHead = NULL;     // location where data is added to queue
	volatile uint32_t* ptrCircularTail = NULL;     // loation where data is removed from queue
};

struct myQueueStruct myQueue;                  // create an instance of the circular queue data structure

int main()
{
	HANDLE		hThread;            // handle to thread
	uint32_t	inputBufferData;	// last reading from the input simulator

	srand((unsigned)time(NULL));		// seed the random number generator

	myQueue.ptrBuffer = (uint32_t*)calloc(CIRCULAR_QUEUE_SIZE, sizeof(uint32_t));
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
		for (int i = numberOfReadsToDo(); i != 0; i--)
		{
			inputBufferData = simulateDeviceRead();

			printf("QUEUE: 0x%08x\n", inputBufferData);		// print out the data that is being queued

			// put the data into the circular queue but check if an error (marked by queue function returning 0) occurred
			if (putToCircularQueue(inputBufferData) == 0)
				printf("Error queuing data\n");
		}

		// random delay before putting more data into queue
		Sleep(randomRange(MIN_TIMEOUT_MS, MAX_TIMEOUT_MS));
	}
	return 0;
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
	uint32_t readBuffer;			// local buffer to put the data into when reading from queue and print
	unsigned int  readCount = 0;	// the number of characters read from queue

	while (1)
	{
		Sleep(randomRange(MIN_TIMEOUT_MS, MAX_TIMEOUT_MS));      // wait a random amount of time

		// check if data is available and if so print it out
		do {
			readCount = getFromCircularQueue(&readBuffer);
			if (readCount != 0)           // check for reads that did not get any data
				printf(" UNQUEUE: 0x%08x\n", readBuffer);
		} while (readCount != 0);
	}
	return 0;    // will never reach here
}

// FUNCTION      : putToCircularQueue
// DESCRIPTION   :
//   Put the supplied uint32_t data into a circular queue
// PARAMETERS    :
//   inputData - an unsigned 32 bit value to be queued
//
// RETURNS       :
//   Number of characters successfully queued (1 on success, 0 on failure)
//reference for this function is taken frpm my submitted assignment nummber 3
unsigned int putToCircularQueue(uint32_t inputData)
{
	unsigned int numberOfCharsQueued = 0;  // number of characters placed in circular queue


	uint32_t* queueEndBuffer = myQueue.ptrBuffer + CIRCULAR_QUEUE_SIZE - 1;//refernce taken from https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-array-indexing/
	// add code to implement the adding to the circular queue using the data structures in myQueueStruct
	// and the passed in pointer and data to be added count
	if (myQueue.queueCount == 10) {
		printf("overflow\n");
		return 0;
	}
	*myQueue.ptrCircularTail = inputData;  //using relative index to add contents from input buffer 
	myQueue.queueCount++;
	numberOfCharsQueued++;
	if (myQueue.ptrCircularTail == queueEndBuffer) {    // when the tail is pointing to the end of the queue and there is space wrapping it around to add more content 
		myQueue.ptrCircularTail = myQueue.ptrBuffer;
	}
	else {
		myQueue.ptrCircularTail++;
	}
	return numberOfCharsQueued;
}

// FUNCTION      : getFromCircularQueue
// DESCRIPTION   :
//   Read as much data as there is room for from the circular queue
// PARAMETERS    :
//   ptrOutputBuffer - a pointer to a unsigned 32 bit number that was read from queue
//
// RETURNS       :
//   Number of characters successfully removed from queue (1 if successful, 0 if none are read)
////reference for this function is taken frpm my submitted assignment nummber 3
unsigned int getFromCircularQueue(uint32_t* ptrOutputBuffer)
{
	unsigned int readCount = 0;
	uint32_t* queueEndBuffer = myQueue.ptrBuffer + CIRCULAR_QUEUE_SIZE - 1;//reference taken fro https://www.learncpp.com/cpp-tutorial/pointer-arithmetic-and-array-indexing/
	// add code to implement the removing from the circular queue using the data structures in myQueueStruct
	// and place the data into the ptrOutputBuffer location (note this is a uint32_t )
	if (myQueue.queueCount == 0) {
		return 0;
	}
	*(ptrOutputBuffer) = *(myQueue.ptrCircularHead);
	myQueue.queueCount--;  //as we remove element from the queue this will decrease
	readCount++;
	ptrOutputBuffer++;

	if (queueEndBuffer == myQueue.ptrCircularHead) { //if head is pointing to the end of the queue and there are elements to be removed then wraping it around
		myQueue.ptrCircularHead = myQueue.ptrBuffer;
	}
	else {
		myQueue.ptrCircularHead++;//making it point to the next element to be dequeued
	}

	return readCount;
}

// FUNCTION      : randomRange
// DESCRIPTION   :
//   function that returns an unsigned int random number that is between the two minVal and maxVal
// PARAMETERS    :
//   minVal - minimum number that can be randomly returned
//   maxVal - maximum number that can be randomly returned
//
// RETURNS       :
//   A random number that meets the minVal/maxVal specification

unsigned int randomRange(unsigned int minVal, unsigned int maxVal)
{
	unsigned int randomVal = 0;

	randomVal = (unsigned int)((((double)rand() / ((uint32_t)RAND_MAX + 1)) * (maxVal - minVal)) + minVal);
	return randomVal;
}

// FUNCTION      : simulateDeviceRead
// DESCRIPTION   :
//   simulates a device returning a single unsigned 32 bit value
// PARAMETERS    :
//  none
//
// RETURNS       :
//   An unsigned 32 bit number that is meant to simulate reading the value from a device

uint32_t simulateDeviceRead(void)
{
	uint32_t randomVal = 0;

	randomVal = randomRange(1, 10000);
	return randomVal;
}

// FUNCTION      : numberOfReadsToDo
// DESCRIPTION   :
//   Returns a random number between 1 and 3 that represents how many reads from Device to do
// PARAMETERS    :
//  none
//
// RETURNS       :
//   A number between 1 and 3

int numberOfReadsToDo(void)
{
	int	retValue = 0;

	retValue = randomRange(1, 3);
	return retValue;
}

//  *		*  *

