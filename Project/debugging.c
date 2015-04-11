/* 
* Debugging.c
* Get Input from user and ask user again if input is incorrect type.
* including river name or number
* Created by Mr. Siwat Karwlung ID 56070503434
* 17 March 2014 23:38
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "River.h"

/*
* Check string is river name using possibility
* Argument : string (input from user)
* Return : 0 if it's not river name
* 		 : 1 if it's river name
*/

int bRiverNameString(char *string)
	{
	unsigned i = 0;
	for(i=0;i<strlen(string);i++)
		{
		if(!(isalpha(string[i]) || isdigit(string[i]) || isspace(string[i]) ))
			return 0;
		}
	return 1;
	}

/*
* Check string is digit only
* Argument : string (input from user)
* Return : 0 if all of character is not digit
* 		 : 1 opposite also
*/

int bIntegerString(char *string)
	{
	unsigned i = 0;
	for(i=0;i<strlen(string);i++)
		{
		if(!isdigit(string[i]))
			return 0;
		}
	return 1;
	}

/*
* Ask user the river name until correct type input 
* Argument 	: displayMessage - output message to user
*		 	: input - (output argument) correct river name
* No Return
*/

void getRiverNameString(char *displayMessage,char* input)
	{
	char buffer[128];
	fprintf(stdout,"%s",displayMessage);
	fgets(buffer,sizeof(buffer),stdin);
	if(buffer[strlen(buffer)-1] == '\n')
		{
		buffer[strlen(buffer)-1] = 0;
		}
	while(!bRiverNameString(buffer) || strlen(buffer) == 0)
		{
		fprintf(stdout,"Wrong input, alphabetic only.\n%s",displayMessage);
		fgets(buffer,sizeof(buffer),stdin);
		if(buffer[strlen(buffer)-1] == '\n')
			{
			buffer[strlen(buffer)-1] = 0;
			}
		}
		strcpy(input,buffer);
	}

/*
* Ask user the number until correct type input 
* Argument 	: displayMessage - output message to user
* Return correct number
*/

int getNumberString(char *displayMessage)
	{
	char buffer[128];
	int number = 0;
	fprintf(stdout,"%s",displayMessage);
	fgets(buffer,sizeof(buffer),stdin);
	if(buffer[strlen(buffer)-1] == '\n')
		{
		buffer[strlen(buffer)-1] = 0;
		}
	while(!bIntegerString(buffer) || strlen(buffer) == 0)
		{
		fprintf(stdout,"Wrong input, digit only.\n%s",displayMessage);
		fgets(buffer,sizeof(buffer),stdin);
		if(buffer[strlen(buffer)-1] == '\n')
			{
			buffer[strlen(buffer)-1] = 0;
			}
		}
		sscanf(buffer,"%d",&number);
		return number;
	}

/*
* Ask user the menu until correct type input in between max and min value
* Argument 	: displayMessage - output message to user
*			: min number of first choice menu
			: max last of firsh choice menu
* Return correct number
*/

int getNumberFromUser(char *displayMessage,unsigned int min,unsigned int max)
	{
	int number = getNumberString(displayMessage);
	while(number > max || number < min)
		{
		fprintf(stdout,"Menu number must between %d-%d\n",min,max);
		number = getNumberString(displayMessage);
		}
	return number;
	}

/*
* get double number from user by correct input
* it will ask user until get correct double number
* Argument 	: displayMessage - to show user what he,she should input
*		 	: min - minimum number which user can input
*			: max - maximum number which user can input
* Return : double number
*/

double getDoubleFromUser(char *displayMessage,double min,double max)
	{
	char buffer[128];
	double number = min-1;
	fprintf(stdout,"%s",displayMessage);
	fgets(buffer,sizeof(buffer),stdin);
	if(buffer[strlen(buffer)-1] == '\n')
		{
		buffer[strlen(buffer)-1] = 0;
		}
	while(!bIntegerString(buffer))/*buffer should be in number only*/
		{
		fprintf(stdout,"Wrong input, digit only.\n%s",displayMessage);
		fgets(buffer,sizeof(buffer),stdin);
		if(buffer[strlen(buffer)-1] == '\n')
			{
			buffer[strlen(buffer)-1] = 0;
			}
		}
	sscanf(buffer,"%lf",&number);
	while(number < min || number > max)
	{
		fprintf(stdout,"Number must between %lf-%lf\n%s",min,max,displayMessage);
		fgets(buffer,sizeof(buffer),stdin);
		if(buffer[strlen(buffer)-1] == '\n')
			{
			buffer[strlen(buffer)-1] = 0;
			}
		while(!bIntegerString(buffer))/*buffer should be in number only*/
			{
			fprintf(stdout,"Wrong input, digit only.\n%s",displayMessage);
			fgets(buffer,sizeof(buffer),stdin);
			if(buffer[strlen(buffer)-1] == '\n')
				{
				buffer[strlen(buffer)-1] = 0;
				}
			}
		sscanf(buffer,"%lf",&number);
		}
	return number;	
	}