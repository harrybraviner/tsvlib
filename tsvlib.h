// My library of functions for dealing with tab separated value files
#include <stdio.h>
#include <stdlib.h>

struct dataLine{
	int numRecords;
	char *charArray;
	long int *recordPos;
};

struct floatRecords{
	int numRecords;
	float *floatArray;
};

long int getNumLines(FILE *stream);

int isComment(struct dataLine *lineIn);	// Determine whether or not the line is a comment FIXME - write this

/* The following functions return and int to indicate whether or not the function call was successful */
//	0	Success
//	1	EOF encountered
//	-1	stream pointer was null
int gobbleLine(FILE *stream, struct dataLine *lineOut);	// Processes a line of tsv data into lineOut, advancing the stream position.

int skipLine(FILE *stream);	// Advances the file position to the next line

int dataLineToFloats(struct dataLine *lineIn, struct floatRecords *floatsOut);	// Processes a dataLine object into an array of floats
