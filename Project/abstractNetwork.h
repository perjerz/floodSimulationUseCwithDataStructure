/*  
 *  abstractNetwork.h
 *
 *  Defines the necessary functions for a network, that is a 
 *  graph with weights on each edge
 *
 *  Created by Sally Goldin on 1 Feb 2012 for CPE 113
 *  Modified by Siwat Karwlung ID 56070503434 Jame 
 *
 */
#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

#define WHITE 0
#define GRAY  1
#define BLACK 2


/* List items for the adjacency list.
 * Each one is a reference to an existing vertex
 */
typedef struct _adjacent
{
    void * pVertex;           /* pointer to the VERTEX_T this 
                               * item refers to.
                               */
    struct _adjacent * next;  /* next item in the ajacency list */ 
} ADJACENT_T;

/* List items for the main vertex list.*/
typedef struct _vertex
{
    char * key;               /* key for this vertex */
    void * data;              /* ancillary data for this vertex */
    int color;                /* used to mark nodes as visited */
    struct _vertex * parent;  /* pointer to parent found in Dijkstra's algorithm */     
    struct _vertex * next;    /* next vertex in the list */
    ADJACENT_T * adjacentHead;    /* pointer to the head of the
		               * adjacent vertices list
                               */
    ADJACENT_T * adjacentTail;    /* pointer to the tail of the
			       * adjacent vertices list
                               */
}  VERTEX_T;


/* Initialize the graph.
 * Argument 
 *    maxVertices  - how many vertices can this graph
 *                   handle.
 *    bDirected    - If true this is a directed graph.
 *                   Otherwise undirected.
 * Returns 1 unless there is a memory allocation error,
 * in which case it returns zero.
 */
int initGraph(int maxVertices, int bDirected);


/* Free all memory associated with the graph and
 * reset all parameters.
 */
void clearGraph();


/* Add a vertex into the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex
 *     pData -   Additional information that can
 *               be associated with the vertex.
 *     pReturn -  Pointer to this vertex
 * Returns 1 unless there is an error, in which case
 * it returns a 0. An error could mean a memory allocation 
 * error or running out of space, depending on how the 
 * graph is implemented. Returns -1 if the caller tries
 * to add a vertex with a key that matches a vertex
 * already in the graph.
 */
int addVertex(char* key, void* pData,VERTEX_T* pReturn);


/* Remove a vertex from the graph.
 * Arguments
 *     key   -   Key value or label for the 
 *               vertex to remove
 * Returns a pointer to the data stored at that
 * vertex, or NULL if the vertex could not be 
 * found.
 */
void* removeVertex(char* key);


/* Add an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 *    weight - Weight for this edge. Constrained to be positive
 * Returns 1 if successful, 0 if failed due to
 * memory allocation error, or if either vertex
 * is not found. Returns -1 if an edge already
 * exists in this direction.
 */
int addEdge(char* key1, char* key2);


/* Remove an edge between two vertices
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns 1 if successful, 0 if failed 
 * because either vertex is not found or there
 * is no edge between these items.
 */
int removeEdge(char* key1, char* key2);


/* Find a vertex and return its data
 * Arguments
 *    key  -  Key for the vertex to find
 * Returns the data for the vertex or NULL
 * if not found.
 */
void* findVertex(char* key);

/* Find the edge between two vertices (if any) and return
 * its weight
 * Arguments
 *    key1  -  Key for the first vertex in the edge
 *    key2  -  Key for the second vertex
 * Returns weight if successful and edge exists.
 * Returns -1 if an edge is not found
 */
/*int findEdge(char* key1, char* key2);*/


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
char** getAdjacentVertices(char* key, int* pCount);

/* Color all vertices to the passed color.
 * Argument
 *    A color constant
 */
void colorAll(int color);

/*
* Use to print information of 
* river and send signal *** if that river is flood
* Argument : pVertex (which is the river)
* No Return
*/

void printRiverInfo(VERTEX_T* pVertex);
/*
* Use to get head of vertex in list
*/
VERTEX_T* getHeadVertex();

/* Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 * Arguments
 *       key    -  Key we are looking for
 *       pPred  -  used to return the predecessor if any
 * Returns pointer to the vertex structure if one is found       
 */
VERTEX_T * findVertexByKey(char* key, VERTEX_T** pPred);

#endif

