/*
 * main function
 * flood simulation
 * Eve FC 
 * Boonyakorn Phimpoklang 56070503421
 * Sarun Kongsungnern 56070503433
 * 27/03/2014
 * Last modified 11 May 2014
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "River.h"
#include "abstractNetwork.h"
#include "linkedListUtil.h"

/*
* Print Menu Flood Simulation
*
*/
void printMenu()
	{
	printf("-------------------------------------------------------\n");
	printf("|              FLOOD SIMULATION                        |\n");
	printf("-------------------------------------------------------\n");
	printf("* Please select the option below.\n");
	printf("1.Start the simulation\n");
	printf("2.Show the database of the rivers\n");
	printf("3.Exit\n");
	printf("-------------------------------------------------------\n");
	}

/*
* This function is to give memory back to computer
* to make this program completed the rule
* After use allocation, we must give it back a memory.
*/
void deallocationMemory()
	{
	int i =0;
	LIST_HANDLE destroyRiverList = NULL;
	VERTEX_T* pVertex = getHeadVertex();
	while(pVertex != NULL)
		{
		free(((RIVER_T*)(pVertex->data))->key);
		free(((RIVER_T*)(pVertex->data))->name);
		free(((RIVER_T*)(pVertex->data))->province);
		free(pVertex->data);
		pVertex = pVertex->next;
		}
	clearGraph();
	listDestroy(headRiver);
	for(i=0;i<26;i++)
		{
		listReset(riverName[i]);
		while((destroyRiverList = listGetNext(riverName[i])) != NULL)
			{
			listDestroy(destroyRiverList);
			}
		listDestroy(riverName[i]);
		}
	}


/*
* To find Vertex of river by use only river name and acces to chain hash table. 
* the key is the first letter of input which is river name.
* After that we change first letter to index of array to direct accessing then use linear search in list which river name matches but it's list.
* Now we linear search again which province name matches then return the vertex of graph to manipulate respectively
*
* Argument : inputRiver - riverName
* Return : Vertex in graph which match the river name and selected by province
*/
VERTEX_T * findRiverName(char* inputRiver)
  {
  if(tolower(inputRiver[0])-'a' < 0 || tolower(inputRiver[0])-'a' > 25)
    return NULL;
  int i = 0,count = 0,bFound = 0;
  LIST_HANDLE test = NULL;
  RIVER_T* river = NULL;
  VERTEX_T* pVertex = NULL;
  listReset(riverName[tolower(inputRiver[0])-'a']);

  while((test = listGetNext(riverName[tolower(inputRiver[0])-'a'])) != NULL) 
    {
    i=0;
    listReset(test);
    if(strcmp(inputRiver,(river = (RIVER_T*)(pVertex = (VERTEX_T*)listGetNext(test))->data)->name) == 0)
      {
      fprintf(stdout,"*************** %s ***************\n",river->name);
      fprintf(stdout,"Please select province.\n");
      fprintf(stdout,"%d %s\n",i+1,river->province);
      i++;
      while((pVertex = (VERTEX_T*)listGetNext(test)) != NULL)
        {
        river = (RIVER_T*)(pVertex->data);
        fprintf(stdout,"%d %s\n",i+1,river->province);
        i++;
        }
      count = getNumberFromUser("Please enter number of province.\n",1,i);
      listReset(test);
      for(i=0;i<count;i++)
        {
        pVertex = (VERTEX_T*)listGetNext(test);
        }
      bFound = 1;
      test = NULL;
      }
    }
    if(bFound == 0)
      {
      pVertex = NULL;
      }
    return pVertex;
  }
/*
* Print all information of river such as name, key, province, flow rate, capacity and so on
* and managed that suit to the table
* Argument : pVertex - Node that include river info
* No Return 
*/
void printRiverRow(VERTEX_T* pVertex)
{
    int i =0;
    char buffer[64];
    fprintf(stdout,"#\t%s",((RIVER_T*)(pVertex->data))->key);
    for(i=0;i<8-strlen(((RIVER_T*)(pVertex->data))->key);i++)/*all of managed column came from my experiment*/
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\t%s",((RIVER_T*)(pVertex->data))->name);
    for(i=0;i<16-strlen(((RIVER_T*)(pVertex->data))->name);i++)
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\t%s",((RIVER_T*)(pVertex->data))->province);
    for(i=0;i<16-strlen(((RIVER_T*)(pVertex->data))->province);i++)
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\t%.2lf",((RIVER_T*)(pVertex->data))->flowRate);
    sprintf(buffer,"%.2lf",((RIVER_T*)(pVertex->data))->flowRate);
    for(i=0;i<16-strlen(buffer);i++)
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\t%.2lf",((RIVER_T*)(pVertex->data))->maxCapacity);
    sprintf(buffer,"%.2lf",((RIVER_T*)(pVertex->data))->maxCapacity);
    for(i=0;i<16-strlen(buffer);i++)
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\t\t%.2lf",((RIVER_T*)(pVertex->data))->currentCapacity);
    sprintf(buffer,"%.2lf",((RIVER_T*)(pVertex->data))->currentCapacity);
    for(i=0;i<39-strlen(buffer);i++)
      {
      fprintf(stdout," ");
      }
    fprintf(stdout,"#\n");
}

/*
* Show all river in table by name
* Go though hash table and all list find all river and show
* and get flood river in list
* Finally, show flood river in the table again
* No Argument
* No Return
*/
void showRiverInTable()
	{
	char buffer[512];
	memset(buffer,0,sizeof(buffer));
	LIST_HANDLE listRiver = NULL;
	LIST_HANDLE listFloodRiver = newList();
	listReset(listFloodRiver);
	VERTEX_T* printVertex = NULL;
	int i = 0;
	for(i=0;i<26;i++)
		{
		listReset(riverName[i]);
		while((listRiver = listGetNext(riverName[i])) != NULL)
			{
			listReset(listRiver);
			printVertex = (VERTEX_T*)listGetNext(listRiver);
			if(((RIVER_T*)(printVertex->data))->currentCapacity > ((RIVER_T*)(printVertex->data))->maxCapacity)
				{
				listInsertEnd(listFloodRiver,printVertex);
				}
			fprintf(stdout,"########################################################################################################################################################################\n");
			fprintf(stdout,"#\t\t\t\t\t\t\t\t\t\t%s\t\t\t\t\t\t\t\t\t\t#\n",((RIVER_T*)printVertex->data)->name);
			fprintf(stdout,"########################################################################################################################################################################\n");
			fprintf(stdout,"#\tKey\t#\tRiver\t\t#\tProvince\t#\tFlow rate\t#\tMax capacity\t#\t\tCurrent capacity\t\t\t#\n");
    		fprintf(stdout,"########################################################################################################################################################################\n");
			printRiverRow(printVertex);
			while((printVertex = (VERTEX_T*)listGetNext(listRiver)) != NULL)/*go though the list of river to find vertex*/
				{
				if(((RIVER_T*)(printVertex->data))->currentCapacity > ((RIVER_T*)(printVertex->data))->maxCapacity)/*check flood*/
					{
					listInsertEnd(listFloodRiver,printVertex);/*get flood river to flood river list*/
					}
				printRiverRow(printVertex);
				}
			fprintf(stdout,"########################################################################################################################################################################\n");
			}
		}
	listReset(listFloodRiver);
	if((VERTEX_T*)listGetNext(listFloodRiver) != NULL)/*any flood ? river to print*/
		{
		fprintf(stdout,"\n########################################################################################################################################################################\n");
		fprintf(stdout,"#\t\t\t\t\t\t\t\t\t\tFlooded provinces\t\t\t\t\t\t\t\t\t\t#\n");
		fprintf(stdout,"########################################################################################################################################################################\n");
		fprintf(stdout,"#\tKey\t#\tRiver\t\t#\tProvince\t#\tFlow rate\t#\tMax capacity\t#\t\tCurrent capacity\t\t\t#\n");
    	fprintf(stdout,"########################################################################################################################################################################\n");
		listReset(listFloodRiver);	
		while((printVertex = (VERTEX_T*)listGetNext(listFloodRiver)) != NULL)
			{
			printRiverRow(printVertex);
			}
		fprintf(stdout,"########################################################################################################################################################################\n");
		}
	listDestroy(listFloodRiver);
	}


/*
* Main program
* 3 Menu
* 1. Start Simulation
* 2. Look up the all of river
* 3. Exit program
* - Start Simulation
*/
void main (int argc,char* argv)
	{
		//declare variable
		char input[32];
		int choice = 0;
		int count = 1;
		VERTEX_T* section = NULL;
		//initial
		int i =0;
		for(i=0;i<26;i++)
			{
			riverName[i] = newList();/*create chain hash table*/
			}
		headRiver = newList();
		readRiver("riverList.txt");/*read file and allocation in memory*/
		readLink("link.txt");/*link river together*/
    	listReset(headRiver);
		//program
		printMenu();
		choice = getNumberFromUser("Enter : ",1,3);
		while (choice != 3)
			{
			switch (choice)
				{
				case 1:
					count = 1;
					getInitialData();/*use to set current capacity of all river in percent of max capacity*/
					printf("\n************************************************************************Day %d (Before simulation)*********************************************************************\n",count-1);
					/*listReset(headRiver);
					traversePostOrder((VERTEX_T*)listGetNext(headRiver),&printRiverInfo);*//*show the river*/
					showRiverInTable();
					while(count>0)
						{
						printf("\n**********************************************************************************Day %d*******************************************************************************\n",count);
						colorAll(WHITE);
						listReset(headRiver);
						section = (VERTEX_T*)listGetNext(headRiver);
						while (section != NULL)
							{/*the logic of water flow is increasing from upper river flow rate to current river then decreasing from lower river (its flow rate give to lower)*/
							((RIVER_T*)(section->data))->currentCapacity += ((RIVER_T*)(section->data))->flowRate;/*set head river capacity to constant*/
							manipulation(section,0.0);
							section = (VERTEX_T*)listGetNext(headRiver);
							}
						/*listReset(headRiver);*//*restart at the beginning of head river to traverse*/
						/*colorAll(WHITE);
						traversePostOrder((VERTEX_T*)listGetNext(headRiver),&printRiverInfo);*/
						showRiverInTable();
						choice = getNumberFromUser("\n\n\n\nPlease enter number.\n1)Stop simulation\n2)Continue simulation\n",1,2);
						if(choice == 1)
							{
							count = -1;/*exit simulation*/
							}
						else
							{
							choice = getNumberFromUser("Please enter number.\n1)Increase/Decrease water\n2)Continue simulation\n",1,2);	
							while(choice == 1)/*Ask until start new simulation round*/
								{
								/*fprintf(stdout,"*************** List of rivers ***************\n%s",allRiverName);*/
								getRiverNameString("Please enter river name\n",input);
								section = findRiverName(input);
								while(section == NULL)/*Ask until found the river*/
									{
									fprintf(stdout,"%s doesn't exist in database\n\n",input);
									/*fprintf(stdout,"*************** List of rivers ***************\n%s",allRiverName);*/
									getRiverNameString("Please enter river name\n",input);
									section = findRiverName(input);
									}
								printRiverInfo(section);
								((RIVER_T*)(section->data))->currentCapacity = getDoubleFromUser("Set current capacity, Please enter water:\n",0.0,1000000000.0);
								choice = getNumberFromUser("Do you want to set current water again ?\nPlease enter number.\n1)Increase/Decrease water\n2)Continue simulation\n",1,2);
								}
							count ++;
							}
						}		
					break;
				case 2:
					/*listReset(headRiver);
					colorAll(WHITE);
					traversePostOrder((VERTEX_T*)listGetNext(headRiver),&printRiverInfo);*/
					showRiverInTable();	
					break;
				}
			printMenu();
			choice = getNumberFromUser("Enter : ",1,3);
			}
		printf("Exit.\n");
		deallocationMemory();
	}
			
