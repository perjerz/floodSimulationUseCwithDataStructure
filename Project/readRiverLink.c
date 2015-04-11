/**
 *  networkBuilder.c from Lab 9 and dictionary.c from Lab 6 (adapted)
 *  readRiverLink.c (new)
 *  Reads a file that defines a network, and builds that network
 *  by calling functions in the abstractNetwork ADT.
 *
 *  Created by Sally Goldin, 1 February 2012 for CPE 113
 *  Modified by Siwat Karwlung, 2 March 2014 for Project
 *  Change format of database by use a word that relate to flod simulation
 *  such as RIVER. and add the content of each item of river in database which include
 *  capacity, province and so on. Next thing is removing unnecessary elements which is weight by using all weights are zero
 *  Recent update 11 May 2014 20:47 Add findRiverName function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"
/*#include "networkBuilder.h"*/
#include "River.h"
#include "linkedListUtil.h"


/*
* Convert all string to lower case
* Argument : string - string in any case
* No return
*/
void tolowerString(char *string)
  {
  int i=0;
  for(i=0;i<strlen(string);i++)
    {
    string[i] = tolower(string[i]);
    }
  }

/* Opens and reads a river text file file that is supposed to follow
 * the structure explained.
 * Firstly, open file riverlist.txt to find a name of river which is another which include each content of river
 * After read river name, it will open river file again to store information in graph (VERTEX)
 *The river list file name text file, its format is as follows
 *
 * Line 1              NumRivers     e.g.    "100"
 * Line 2              pingriver.txt
 * Line 3              nanriver.txt
 *   .
 *   .
 *  so on
 * The river vertex file is a text file. Its format is as follows.
 * Following lines are each river information
 * A river line must have the form
 *     key|rivername|province|capacity|flowrate|bheadstaus              e.g. "0|Chao Phaya river|Chiang Mai|500|200|0".
 * Arguments
 *    filename    -   filename to read, with path
 * Returns 1 if successful, -1 if any error occurs.
 */
int readRiver(char* filename)
  {
  int retval = 0;
  int status = 1;
  FILE* pFp = NULL;
  FILE* pFRiver = NULL;
  char bufferRiverName[64];
  char inputline[128];
  int vertexCount = 0;
  char *pToken = NULL;
  VERTEX_T* pReturn = NULL;
  RIVER_T * pRiver = NULL;
  VERTEX_T* pPred = NULL;
  pFp = fopen(filename,"r");
  LIST_HANDLE listRiver = NULL;
  if (pFp == NULL)
    {
    status = -1;
    fprintf(stderr,"Error - cannot open file %s\n", filename);
    }
  else
    {
    if (fgets(inputline,sizeof(inputline),pFp) != NULL)
      {
      sscanf(inputline,"%d",&vertexCount);
      if (vertexCount == 0)
        {
        status = -1;
        } 
      else if (initGraph(vertexCount,1))
        {
        while (fgets(inputline,sizeof(inputline),pFp) != NULL && status == 1)
          {
          inputline[strlen(inputline) - 1] = '\0';
          strcpy(bufferRiverName,inputline);
          /*strcat(allRiverName,bufferRiverName);
          strcat(allRiverName,"\n");*/
          pFRiver = fopen(inputline,"r");
          fprintf(stdout,"***************************************************\n");
          fprintf(stdout,"Open File %s.\n",inputline);
          fprintf(stdout,"***************************************************\n");
          if (pFRiver == NULL)
            {
            status = -1;
            fprintf(stderr,"Error - cannot open file %s\n", inputline);
            }
          else
            {
            listRiver = newList();
            }
          while ((fgets(inputline,sizeof(inputline),pFRiver)) != NULL && status == 1)
            {
			
            if((pRiver = (RIVER_T*)calloc(1,sizeof(RIVER_T))) != NULL)
              {
              inputline[strlen(inputline) - 1] = '\0';  
              pToken = strtok(inputline,"|");
              retval = addVertex(pToken,pRiver,pReturn);
              pReturn = findVertexByKey(pToken,&pPred);
              if (retval == 1) 
                {
                pRiver->key=strdup(pToken);
                pToken = strtok(NULL,"|");
                pRiver->name=strdup(pToken);
                pToken = strtok(NULL,"|");
                pRiver->province=strdup(pToken);
                pToken = strtok(NULL,"|");
                sscanf(pToken,"%lf",&pRiver->maxCapacity);
                pRiver->currentCapacity = 0.0;
                pToken = strtok(NULL,"|");
                sscanf(pToken,"%lf",&pRiver->flowRate);
                pToken = strtok(NULL,"|");
                sscanf(pToken,"%d",&pRiver->bHead);
                if(pRiver->bHead == 1)
                  {
                  listInsertEnd(headRiver,pReturn);                
                  }
                listInsertEnd(listRiver,pReturn);
                fprintf(stdout,"Key |%s| River |%s| Province |%s| added\n",pRiver->key,pRiver->name,pRiver->province);
                }
              else if (retval < 0)
                {
                fprintf(stderr,
                "Error: River with name |%s| already exists in graph\n", pRiver->key);
                free(pRiver);
                listDestroy(headRiver);
                listDestroy(listRiver);
                status = -1;
                }
              else 
                {
                fprintf(stderr,
                "Error: Trying to insert river with {%s|- allocation error or graph full\n",pRiver->key);
                free(pRiver);
                listDestroy(headRiver);
                listDestroy(listRiver);
                status = -1;
                }
              }
            else
              {
              listDestroy(headRiver);
              listDestroy(listRiver);
              status = -1;
              }
            }
          listInsertEnd(riverName[tolower(bufferRiverName[0])-'a'],listRiver);
          fclose(pFRiver);
          }
        }
      else
        {
        status = -1;
        }
      }
    }
  fclose(pFp);
  return status;
  } 

/* Opens and reads a link river together text file file that is supposed to follow
 * the structure explained.
 *The link river file is a text file. Its format is as follows.
 * Following lines are each river link or connect together
 * A link line must have the form
 *     key1|key2         e.g. "0|2".
 * Arguments
 *    filename    -   filename to read, with path
 * Returns 1 if successful, -1 if any error occurs.
 */

int readLink(char* filename)
  {
  int retval = 0;
  int status = 1;
  FILE* pFp = NULL;
  char inputline[128];
  char *pToken = NULL;
  char key1[64];
  char key2[64];
  pFp = fopen(filename,"r");
  if (pFp == NULL)
    {
    status = -1;
    fprintf(stderr,"Error - cannot open file %s\n", filename);
    }
  else
    {
    while (fgets(inputline,sizeof(inputline),pFp) != NULL && status == 1)
      {
      inputline[strlen(inputline) - 1] = '\0';
      pToken = strtok(inputline,"|");
      strcpy(key1,pToken);
      pToken = strtok(NULL,"|");
      strcpy(key2,pToken);
      retval = addEdge(key1,key2);  
      if (retval == 1) 
        {
        fprintf(stdout,
          "Added link from |%s| to |%s|\n",
        key1,key2);
        }
      else if (retval < 0)
        {
        fprintf(stderr,"Error: link from |%s| to |%s| already exists\n",
        key1,key2);
        status = -1;
        }
      else 
        {
        fprintf(stderr,
        "Error: At least one vertex doesn't exist or memory allocation error\n");
        status = -1;
        }
      }
    }
  return status;
  }

/* simulation flow at pVertex (that river) and go down the graph 
 * to manipulation such as change capacity all of the rivers
 * Argument : pVertex (Vertex in graph which is each river)
 *            water (amount of water to add up)
 * the logic of water flow is increasing from upper river flow rate to current 
 *  river then decreasing from lower river (its flow rate give to lower). We start all of this at all of the head river.
 * it is a rule of flow rate the problem is heads of river doesn't recieve any water because their don't have upper river
 * Then we increase all of head river current capacity by its flowrate. It means that head river current is constant.
 * No Return
 */
void manipulation(VERTEX_T* pVertex, double water)/*traversePostOrder*/
{
  ((RIVER_T*)(pVertex->data))->currentCapacity += water;
  if(pVertex->color == WHITE)
    {
    double tempWater = 0.0;
    tempWater = ((RIVER_T*)(pVertex->data))->currentCapacity;
    ((RIVER_T*)(pVertex->data))->currentCapacity -= ((RIVER_T*)(pVertex->data))->flowRate;
    if(((RIVER_T*)(pVertex->data))->currentCapacity < 0)/*if current capacity low than flow rate,we use current capacity to flow simultaion*/
      {
      ((RIVER_T*)(pVertex->data))->currentCapacity = 0;
      }

    VERTEX_T * pAdjVertex = NULL;    
    ADJACENT_T* pAdjacent = pVertex->adjacentHead;
    while (pAdjacent != NULL)
      {
      water = ((RIVER_T*)(pVertex->data))->flowRate/countAdjacent(pVertex);
      if(((RIVER_T*)(pVertex->data))->currentCapacity == 0)
        {/*if current capacity low than flow rate,we use current capacity to flow simultaion seperate by amount of flow way*/
        water = tempWater/countAdjacent(pVertex);
        }
      pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
      manipulation(pAdjVertex,water);
      pAdjacent = pAdjacent->next;  
      }
    pVertex->color = BLACK; 
  }
}

/* Execute a post order traverse from a single vertex,
 * calling the function (*vFunction) on the lowest level
 * vertex we visit, and coloring it black.
 * Arguments
 *    pVertex    -  starting vertex for traversal
 */
void traversePostOrder(VERTEX_T* pVertex, void (*vFunction)(VERTEX_T*))
{
    VERTEX_T * pAdjVertex = NULL;    
    ADJACENT_T* pAdjacent = pVertex->adjacentHead;
    (*vFunction)(pVertex);
    while (pAdjacent != NULL)
      {
      pAdjVertex = (VERTEX_T*) pAdjacent->pVertex;
      if (pAdjVertex->color == WHITE)
        {
        pAdjVertex->color = GRAY;
        traversePostOrder(pAdjVertex,vFunction);
        }
      pAdjacent = pAdjacent->next;  
      } /* end while queue has data */
    /* when we return from the bottom, call the 
     * function and color this node black.
     */
    pVertex->color = BLACK;
}
/*
* Use to print information of 
* river and send signal *** if that river is flood
* Argument : pVertex (which is the river)
* No Return
*/
void printRiverInfo(VERTEX_T* pVertex)
{
    int i =0;
    char buffer[64];
    fprintf(stdout,"______________________________________________________________________\n");
    if(((RIVER_T*)(pVertex->data))->currentCapacity>((RIVER_T*)(pVertex->data))->maxCapacity)
      {
      fprintf(stdout,"******************************\n");
      fprintf(stdout,"*** Warning Flooding River ***\n");
      fprintf(stdout,"******************************\n");
      }
    fprintf(stdout,"Key |%s| River |%s| Province |%s| Flow rate |%.2lf|\n\tMax capacity |%.2lf| Current capacity |%.2lf| \n",
    ((RIVER_T*)(pVertex->data))->key,((RIVER_T*)(pVertex->data))->name,
    ((RIVER_T*)(pVertex->data))->province,((RIVER_T*)(pVertex->data))->flowRate,
    ((RIVER_T*)(pVertex->data))->maxCapacity,((RIVER_T*)(pVertex->data))->currentCapacity);
    fprintf(stdout,"______________________________________________________________________\n");
}

/*
* Use for print the river 
* No Argument
* No return
*/
void printPostFirst()
{
   VERTEX_T* pVertex = NULL;
   if (pVertex == NULL)
      {
      printf("The graph is empty\n");
      }
   else
      {
      colorAll(WHITE);
      while (pVertex != NULL)
        {
        if (pVertex->color == WHITE)
          {
          printf("\nStarting new traversal river from |%s %s|\n",((RIVER_T*)(pVertex->data))->name,((RIVER_T*)(pVertex->data))->province);
          pVertex->color = GRAY;
          traversePostOrder(pVertex,&printRiverInfo);
          }
        pVertex = pVertex->next;
        }
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
/*VERTEX_T * findRiverName(char* inputRiver)
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
  }*/



