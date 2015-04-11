/*
 * initialRiver.c
 * Use to initialize all data required for the program
 * Sarun Kongsungnern 56070503433
 * 02/04/2014
 */
#include <stdio.h>
#include "River.h"
#include "abstractNetwork.h"

/*
 * getInitialData
 * Function that will get the initial data from the user for further uses
 *
 */
int getInitialData ()
	{
		printf("Initializing Data\n");
		printf("-------------------------------------------------------\n");
		int initWater = getNumberFromUser("Enter initial water level (in percent) : ",0,100);
		VERTEX_T * current = getHeadVertex();
		while(current != NULL)
			{
			((RIVER_T*)(current->data))->currentCapacity=((RIVER_T*)(current->data))->maxCapacity/100*initWater;
			current=current->next;
			}
	}
	
/*
 * insertIntialData
 * Function that will get data to calculate the initial data such as water level
 * in the simulation
 * 
 */
void insertIntialData (RIVER_T* section,int initWater)
	{
		while (section != NULL)
		{
			section->currentCapacity = section->maxCapacity*initWater/100;
		}
	}
