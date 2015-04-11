/*
* River.h
*
* Header file, define necessary elements or items (river)
* to store information.
* by using multi tree algorithm and hash table to keep each data
* in it ex. name ,flow rate and water gate status
* 
*
*
*
* Created by Mr. Siwat Karwlung ID 56070503434
* 17 March 2014 21:45
*/


typedef struct _river
{
    char *key; /*key of river use for link river to river*/
    char *name; /*river name*/
    char *province; /*position in geographic*/
    double flowRate; /*water losing per period*/
    int bGate;/*gate status of water (optional)*/
    double maxCapacity; /*amount of water in river*/
    double currentCapacity; /*amount of water in river*/
    int bHead;/*show that it is head river*/
}RIVER_T;

/*har allRiverName[512];*//*to store all of river name in this simulation*/

/* Opens and reads a river text file file that is supposed to follow
 * the structure explained.
 * Firstly, open file riverlist.txt to find a name of river which is another which include each content of river
 * After read river name, it will open river file again to store information in graph (VERTEX)
 *The river list file name text file, its format is as follows
 *
 * Line 1              NumRivers     e.g.    "100" (not neccessary)
 * Line 2              Ping River
 * Line 3              Nan River
 *   .
 *   .
 *  so on
 * The river vertex file is a text file. Its format is as follows.
 * Following lines are each river information
 * A river line must have the form
 *     key|rivername|province|capacity|flowrate              e.g. "0|Chao Phaya river|Chiang Mai|500|200".
 * Arguments
 *    filename    -   filename to read, with path
 * Returns 1 if successful, -1 if any error occurs.
 */
int readRiver(char* filename);

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

int readLink(char* filename);

/*
* Ask user the river name until correct type input 
* Argument  : displayMessage - output message to user
*           : input - (output argument) correct river name
* No Return
*/

void getRiverNameString(char *displayMessage,char* input);

/*
* Ask user the number until correct type input 
* Argument  : displayMessage - output message to user
* Return correct number
*/

int getNumberString(char *displayMessage);

/*
* Ask user the menu until correct type input in between max and min value
* Argument  : displayMessage - output message to user
*           : min number of first choice menu
            : max last of firsh choice menu
* Return correct number
*/

int getNumberFromUser(char *displayMessage,unsigned int min,unsigned int max);

/*
* Ask user the number in double until correct type input in between max and min value
* Argument  : displayMessage - output message to user
*           : min number of first choice menu
            : max last of firsh choice menu
* Return correct number
*/

double getDoubleFromUser(char *displayMessage,double min,double max);
