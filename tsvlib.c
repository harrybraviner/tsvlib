// My library of functions for dealing with tab separated value files
#include "./tsvlib.h"

long int getNumLines(FILE * stream){
	// FIXME - error handling
	long int initialPos = ftell(stream);

	long int linecount = 1;
	char char1, char2;
	fseek(stream, 0, SEEK_SET);
	char1 = '\n';
	while(char1 != EOF){
		char2 = char1;
		char1 = fgetc(stream);
		if(char1 == '\n' && char2 != '\n') linecount ++;
	}
	if(char2 == '\n') linecount --;

	fseek(stream, initialPos, SEEK_SET);	// Reset the stream to the position it was in when passed to this function
	return linecount;
}

int gobbleLine(FILE *stream, struct dataLine *lineOut){
	// FIXME - error handling
	long int initialPos = ftell(stream);
	long int numChars = 0, numRecords = 1;

	// First read through - count the number of characters and the number of tabs in the line
	char char1 = fgetc(stream);
	while(char1 != '\n' && char1 != EOF){
		if(char1 == '\t') numRecords++;
		else numChars ++;
		char1 = fgetc(stream);
	}

	lineOut->numRecords = numRecords;
	if(lineOut->charArray != NULL) free(lineOut->charArray);
	lineOut->charArray = malloc(sizeof(char)*(numChars+1));
	if(lineOut->recordPos != NULL) free(lineOut->recordPos);
	lineOut->recordPos = malloc(sizeof(long int)*numRecords);

	// Second read through - locate the start of each record and gobble the characters in the charArray
	fseek(stream, initialPos, SEEK_SET);
	numRecords = 0;
	numChars = 0;
	char1 = fgetc(stream);
	while(char1 != '\n' && char1 != EOF){
		if(char1 == '\t'){
			lineOut->recordPos[numRecords] = numChars;
			numRecords++;
		} else {
			lineOut->charArray[numChars] = char1;
			numChars++;
		}
		char1 = fgetc(stream);
	}
	lineOut->charArray[numChars] = '\0';	// Null-terminate this
	lineOut->recordPos[numRecords] = numChars;
	
	return 0;	// Success
}

int skipLine(FILE *stream){
	if(stream==NULL) return -1;
	// Gobble characters from stream until we hit a newline or end of file
	char char1 = fgetc(stream);
	while(char1!='\n' && char1!=EOF) fgetc(stream);

	if(char1=='\n') return 0;
	else return 1;
}

int dataLineToFloats(struct dataLine *lineIn, struct floatRecords *floatsOut){
	// FIXME - error handling
	int i,j;
	long int maxRecordLength;
	char *recordNullTerminated;

	// Find out what the longest record in lineIn is
	maxRecordLength = lineIn->recordPos[0];
	for(i=0; i<lineIn->numRecords-1; i++) if((lineIn->recordPos[i+1]-lineIn->recordPos[i])>maxRecordLength) maxRecordLength = (lineIn->recordPos[i+1]-lineIn->recordPos[i]);
	recordNullTerminated = malloc(sizeof(char)*(maxRecordLength+1));

	floatsOut->numRecords = lineIn->numRecords;
	if(floatsOut->floatArray != NULL) free(floatsOut->floatArray);
	floatsOut->floatArray = malloc(sizeof(float)*floatsOut->numRecords);

	// Build recordNullTerminated, and then use the strtof function to populate floatArray
	// Deal with the zero'th record
	for(j=0; j<lineIn->recordPos[0]; j++){	// Loop over chars within a record
		recordNullTerminated[j] = lineIn->charArray[j];
	}
	recordNullTerminated[j] = '\0';
	floatsOut->floatArray[0] = strtof(recordNullTerminated, NULL);
	for(i=1; i<floatsOut->numRecords; i++){	// Loop over records
		for(j=0; j<(lineIn->recordPos[i]-lineIn->recordPos[i-1]); j++){	// Loop over chars within a record
			recordNullTerminated[j] = lineIn->charArray[j+lineIn->recordPos[i-1]];
		}
		recordNullTerminated[j] = '\0';
		floatsOut->floatArray[i] = strtof(recordNullTerminated, NULL);
	}

	free(recordNullTerminated);
	return 0;
}
