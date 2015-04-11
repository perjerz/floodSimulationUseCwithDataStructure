/*
 *  linkedListNetwork.c
 *
 *  Implements an abstractNetwork using adjacency lists (linked lists).
 *  This is a structure with two levels of list. There is a master
 *  list of vertices, linked in series. Each vertex points to a subsidiary
 *  linked list with references to all the other vertices to which it
 *  is connected.
 *
 *  Each vertex has an integer weight and a pointer to a parent vertex 
 *  which can be used for route finding and spanning tree algorithms
 *
 *  Key values are strings and are copied when vertices are inserted into
 *  the graph. Every vertex has a void* pointer to ancillary data which
 *  is simply stored. 
 *
 *  Created by Sally Goldin, 1 February 2012 for CPE 113
 *  Modified 18 March 2013 to improve naming.
 *  Modified by Siwat Karwlung JaMe 56070503434, 14 March 2014
 *  Add printShortPath debugging if 2 vertices are not reachable
 *  use recursive to print backward
 *  Modified by Siwat Karwlung Jame 56070503434, 3 April 2014
 *  Delete unnecessary function and element such as weight and printshortestpah
 *  Last Modified 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"

VERTEX_T * vListHead = NULL;  /* head of the vertex list */
VERTEX_T * vListTail = NULL;  /* tail of the vertex list */
int bGraphDirected = 0;       /* if true, this is a directed graph */
/** Private functions */

/* Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 * Arguments
 *       key    -  Key we are looking for
 *       pPred  -  used to return the predecessor if any
 * Returns pointer to the vertex structure if one is found       
 */
VERTEX_T * findVertexByKey(char* key, VERTEX_T** pPred) 
{
    VERTEX_T * pFoundVtx = NULL;
    VERTEX_T * pCurVertex = vListHead;
    *pPred = NULL;
    /* while there are vertices left and we haven't found
     * the one we want.
     */
    while ((pCurVertex != NULL) && (pFoundVtx == NULL))
       {
       if (strcmp(pCurVertex->key,key) == 0)
          {
	  pFoundVtx = pCurVertex;
	  }
       else
          {
	  *pPred = pCurVertex;
          pCurVertex = pCurVertex->next;
          }
       }
    return pFoundVtx;
}

/* Free the adjacencyList for a vertex 
 * Argument
 *   pVertex    - vertex whose edges we want to delete 
 */
void freeAdjacencyList(VERTEX_T *pVertex)
{
   ADJACENT_T * pCurRef = pVertex->adjacentHead;
   while (pCurRef != NULL)
      {
      ADJACENT_T * pDelRef = pCurRef;
      pCurRef = pCurRef->next;
      free(pDelRef);
      }
   pVertex->adjacentHead = NULL;
   pVertex->adjacentTail = NULL;
}

/* Check if there is already an edge between
 * two vertices. We do not want to add a duplicate.
 * Arguments
 *   pFrom        -  Start point of edge
 *   pTo          -  End point of edge
 * Return 1 if an edge already exists, 0 if it does
 * not.
 */
int edgeExists(VERTEX_T* pFrom, VERTEX_T* pTo)
{
    int bEdgeExists = 0;
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
       {
       if (pCurRef->pVertex == pTo)
          {
	  bEdgeExists = 1;  /* the 'To' vertex is already in the
                             * 'From' vertex's adjacency list */ 
          }
       else
          {
	  pCurRef = pCurRef->next;
          }
       } 
    return bEdgeExists;
}

/* Component of removeVertex. Removes all references
 * to this vertex as the end point of edges in other
 * vertices' adjacency lists.
 */
void removeReferences(VERTEX_T * pTarget)
{
   VERTEX_T * pCurrentVtx = vListHead;
   while (pCurrentVtx != NULL)
      {
      if (pCurrentVtx != pTarget)
         {
	 /* skip the target vertex */
	 ADJACENT_T* pAdjacent = pCurrentVtx->adjacentHead;
	 ADJACENT_T* pPrevAdjacent = NULL;
	 while (pAdjacent != NULL)
	    {
	    if (pAdjacent->pVertex == pTarget)  /* if this edge involves the target*/
	       {
	       if (pPrevAdjacent != NULL)
	          {
		  pPrevAdjacent->next = pAdjacent->next;
	          }
               else
	          {
		  pCurrentVtx->adjacentHead = pAdjacent->next;   
	          }
               if (pAdjacent == pCurrentVtx->adjacentTail)
	          {
		  pCurrentVtx->adjacentTail = NULL;
		  }
               free(pAdjacent);
               pAdjacent = NULL;
	       break;    /* can only show up once in the adjacency list*/
	       }
            else
	       {
	       pPrevAdjacent = pAdjacent;
               pAdjacent = pAdjacent->next;
	       }  
	    }
	 }
      pCurrentVtx = pCurrentVtx->next;      
      } 
}

/* Count adjacent vertices to a vertex.
 * Argument
 *    pVertex   -   Vertex whose adjacent nodes we want to count
 * Returns integer value for count (could be zero)
 */
int countAdjacent(VERTEX_T * pVertex)
{
    int count = 0;
    ADJACENT_T * pAdjacent = pVertex->adjacentHead;
    while (pAdjacent != NULL)
       {
       count += 1;
       pAdjacent = pAdjacent->next;
       }
    return count;
}


/* Function to print the information about a vertex
 * Argument  
 *   pVertex   -   vertex we want to print
 */
void doVertexInfo(VERTEX_T* pVertex,unsigned int water)
{

    /*printf("== Vertex key |%s| - data |%s|\n",
	   pVertex->key, pVertex->data);*/
}


/********************************/
/** Public functions start here */
/********************************/

/* Initialize or reintialize the graph.
 * Argument 
 *    maxVertices  - how many vertices can this graph
 *                   handle.
 *    bDirected    - If true this is a directed graph.
 *                   Otherwise undirected.
 * Returns 1 unless there is a memory allocation error,
 * in which case it returns zero.
 */
int initGraph(int maxVertices, int bDirected)
{ 
    /* for a linked list graph, we call
     * clearGraph and then initialize bGraphDirected
     */
    clearGraph();
    bGraphDirected = bDirected;
    return 1;  /* this implementation of initGraph can never fail */ 
}


/* Free all memory associated with the graph and
 * reset all parameters.
 */
void clearGraph()
{
    VERTEX_T * pCurVertex = vListHead;
    while (pCurVertex != NULL)
       {
       freeAdjacencyList(pCurVertex);
       VERTEX_T * pDelVtx = pCurVertex;
       pCurVertex = pCurVertex->next;
       free(pDelVtx->key);
       free(pDelVtx);
       }

    vListHead = NULL;  
    vListTail = NULL; 
    bGraphDirected = 0;

}

/* Add a vertex into the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex
 *     pData -   Additional information that can
 *               be associated with the vertex.
 * Returns 1 unless there is an error, in which case
 * it returns a 0. An error could mean a memory allocation 
 * error or running out of space, depending on how the 
 * graph is implemented. Returns -1 if the caller tries
 * to add a vertex with a key that matches a vertex
 * already in the graph.
 */
int addVertex(char* key, void* pData,VERTEX_T* pReturn)
{
    int bOk = 1;
    VERTEX_T * pPred;
    VERTEX_T * pFound = findVertexByKey(key, &pPred);
    if (pFound != NULL)  /* key is already in the graph */
       {
       bOk = -1;
       }
    else
       {
       VERTEX_T * pNewVtx = (VERTEX_T *) calloc(1,sizeof(VERTEX_T));
       char * pKeyval = strdup(key);
       if ((pNewVtx == NULL) || (pKeyval == NULL))
          {
	  bOk = 0;  /* allocation error */
	  }
       else
          {
          pReturn = pNewVtx;
    //      printf("%p\n",pReturn);
	  pNewVtx->key = pKeyval;
          pNewVtx->data = pData;
	  if (vListHead == NULL)  /* first vertex */
	     {
	     vListHead = pNewVtx;
	     }
	  else
	     {
	     vListTail->next = pNewVtx; 
	     }
	  vListTail = pNewVtx;
	  }
       }
    return bOk;
}


/* Remove a vertex from the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex to remove
 * Returns a pointer to the data stored at that
 * vertex, or NULL if the vertex could not be 
 * found.
 */
void* removeVertex(char* key)
{
   void * pData = NULL; /* data to return */
   VERTEX_T * pPredVtx = NULL;
   VERTEX_T * pRemoveVtx = findVertexByKey(key,&pPredVtx);
   if (pRemoveVtx != NULL)
      {
      removeReferences(pRemoveVtx);
      freeAdjacencyList(pRemoveVtx);
      if (pPredVtx != NULL)
         {
	 pPredVtx->next = pRemoveVtx->next;
         }
      else /* if there is no predecessor that means this was the head */
         {
         vListHead = pRemoveVtx->next;
         }   
      if (pRemoveVtx == vListTail)
	 vListTail = pPredVtx;
      free(pRemoveVtx->key);
      pData = pRemoveVtx->data;
      free(pRemoveVtx);
      } 
   return pData;
}


/* Add an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 *    weight - weight for this edge
 * Returns 1 if successful, 0 if failed due to
 * memory allocation error, or if either vertex
 * is not found. Returns -1 if an edge already
 * exists in this direction.
 */
int addEdge(char* key1, char* key2)
{
    int bOk = 1;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFromVtx = findVertexByKey(key1,&pDummy);
    VERTEX_T * pToVtx = findVertexByKey(key2,&pDummy);
    if ((pFromVtx == NULL) || (pToVtx == NULL))
       {
       bOk = 0;
       }
    else if (edgeExists(pFromVtx,pToVtx))
       {
       bOk = -1;	   
       }
    else
       {
       ADJACENT_T * pNewRef = (ADJACENT_T*) calloc(1,sizeof(ADJACENT_T));
       if (pNewRef == NULL)
          {
	  bOk = 0;
          }
       else
          {
	  pNewRef->pVertex = pToVtx;
	  if (pFromVtx->adjacentTail != NULL)
              {
	      pFromVtx->adjacentTail->next = pNewRef;
	      }
          else
	      {
	      pFromVtx->adjacentHead = pNewRef;
	      }
	  pFromVtx->adjacentTail = pNewRef;
          } 
       } 
    /* If undirected, add an edge in the other direction */
    if ((bOk) && (!bGraphDirected))
       {
       ADJACENT_T * pNewRef2 = (ADJACENT_T*) calloc(1,sizeof(ADJACENT_T));
       if (pNewRef2 == NULL)
          {
	  bOk = 0;
          }
       else
          {
	  pNewRef2->pVertex = pFromVtx;
	  if (pToVtx->adjacentTail != NULL)
              {
	      pToVtx->adjacentTail->next = pNewRef2;
	      }
          else
	      {
	      pToVtx->adjacentHead = pNewRef2;
	      }
	  pToVtx->adjacentTail = pNewRef2;
          } 
       } 
    return bOk;
}


/* Remove an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns 1 if successful, 0 if failed 
 * because either vertex is not found or there
 * is no edge between these items.
 */
int removeEdge(char* key1, char* key2)
{
   int bOk = 1;
   VERTEX_T * pDummy = NULL;
   VERTEX_T * pFromVtx = findVertexByKey(key1,&pDummy);
   VERTEX_T * pToVtx = findVertexByKey(key2,&pDummy);
   if ((pFromVtx == NULL) || (pToVtx == NULL))
       {
       bOk = 0;
       }
   else if (!edgeExists(pFromVtx,pToVtx))
       {
       bOk = 0;
       }
   else
       {
       ADJACENT_T* pAdjacent = pFromVtx->adjacentHead;
       ADJACENT_T* pPrevAdjacent = NULL;
       while (pAdjacent != NULL)
          {  
	  if (pAdjacent->pVertex == pToVtx)  /* if this edge involves the target*/
	     {
	     if (pPrevAdjacent != NULL)
	        {
	        pPrevAdjacent->next = pAdjacent->next;
		}
             else
	        {
		pFromVtx->adjacentHead = pAdjacent->next;   
	        }
             if (pAdjacent == pFromVtx->adjacentTail)
	        {
		pFromVtx->adjacentTail = NULL;
		}
	     free(pAdjacent);
	     break;    /* can only show up once in the adjacency list*/
	     }
	  else
	     {
	     pPrevAdjacent = pAdjacent;
	     pAdjacent = pAdjacent->next;
	     }  
	  }
       /* If undirected, remove edge in the other direction */
       if ((bOk) && (!bGraphDirected))
          {
          ADJACENT_T* pAdjacent2 = pToVtx->adjacentHead;
          ADJACENT_T* pPrevAdjacent2 = NULL;
	  while (pAdjacent2 != NULL)
             {   
	     if (pAdjacent2->pVertex == pFromVtx)  
	        {
	        if (pPrevAdjacent2 != NULL)
	           {
	           pPrevAdjacent2->next = pAdjacent2->next;
		   }
		else
	           {
		   pToVtx->adjacentHead = pAdjacent2->next;   
		   }
		if (pAdjacent2 == pToVtx->adjacentTail)
	           {
		   pToVtx->adjacentTail = NULL;
		   }
		free(pAdjacent2);
		break;    /* can only show up once in the adjacency list*/
		}
	     else
	        {
		pPrevAdjacent2 = pAdjacent2;
		pAdjacent2 = pAdjacent2->next;
		}  
	     }
	  }
       }
}

/* Find a vertex and return its data
 * Arguments
 *    key  -  Key for the vertex to find
 * Returns the data for the vertex or NULL
 * if not found.
 */
void* findVertex(char* key)
{
    void* pData = NULL;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFoundVtx = findVertexByKey(key,&pDummy);
    if (pFoundVtx != NULL)
       {
       pData = pFoundVtx->data;
       }
    return pData; 
}

/* Find the edge between two vertices (if any) and return
 * its weight
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns weight if successful and edge exists.
 * Returns -1 if an edge is not found
 */
/*int findEdge(char* key1, char* key2)
{
    int weight = -1;
    int bEdgeExists = 0;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFrom = findVertexByKey(key1,&pDummy);	
    ADJACENT_T * pCurRef = pFrom->adjacentHead;
    while ((pCurRef != NULL) && (!bEdgeExists))
       {
       VERTEX_T * pFrom = (VERTEX_T*) pCurRef->pVertex;
       if (strcmp(pFrom->key,key2) == 0)
          {
	  weight = pCurRef->weight;  
          bEdgeExists = 1;
          }
       else
          {
	  pCurRef = pCurRef->next;
          }
       } 
    return weight;
}
*/

/* Return an array of copies of the keys for all nodes
 * adjacent to a node. The array and its
 * contents should be freed by the caller when it 
 * is no longer needed.
 * Arguments
 *    key   -  Key for the node whose adjacents we want
 *    pCount - Return number of elements in the array
 * Returns array of char* which are the keys of adjacent
 * nodes. Returns number of adjacent vertices in pCount.
 * If pCount holds -1, the vertex does not exist.
 */
char** getAdjacentVertices(char* key, int* pCount)
{
    char** keyArray = NULL;
    VERTEX_T * pDummy = NULL;
    VERTEX_T * pFoundVtx = findVertexByKey(key,&pDummy);
    if (pFoundVtx != NULL)
       {
       *pCount = countAdjacent(pFoundVtx);
       if (*pCount > 0)
          {
	  int i = 0;
	  keyArray = (char**) calloc(*pCount, sizeof(char*));
          if (keyArray != NULL)
	     {
	     ADJACENT_T * pAdjacent = pFoundVtx->adjacentHead;
	     while (pAdjacent != NULL)
	        {
		VERTEX_T* pVertex = (VERTEX_T*) pAdjacent->pVertex;
		keyArray[i] = strdup(pVertex->key);
		pAdjacent = pAdjacent->next;
		i += 1;
	        }
	     }
          } 
       } 
    else
       {
       *pCount = -1;
       }
    return keyArray;
}


/* Color all vertices to the passed color.
 * Argument
 *    A color constant
 */
void colorAll(int color)
{
    VERTEX_T* pVertex = vListHead;
    while (pVertex != NULL)
       {
       pVertex->color = color;
       pVertex = pVertex->next;
       }
}

/* Find Head Vertex
 *
 */
VERTEX_T * getHeadVertex()
{
  return vListHead;
}




