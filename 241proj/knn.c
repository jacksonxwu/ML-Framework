#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "csvparser.h"


void *alloc(size_t size) {
    void *m = malloc(size);
    if (m == NULL) {
	printf("Malloc failed.\n");
	exit(3);
    }
    return m;
}

/* Calculates distance between a single unknown data item and known data item */
double calculateDistance(int length, float unknownData[], float knownItem[]) {
    double dist = 0;
    for (int i = 0; i < length; i++) {
	dist += pow((unknownData[i] - knownItem[i]), 2);
    }
    dist = sqrt(dist);
    return dist;
}


/* Get the number of features each data item has */
int getNumFeatures(const char *filename) {
    CsvParser *csvparser = CsvParser_new(filename, ",", 0);
    CsvRow *row = CsvParser_getRow(csvparser);
    int features = CsvParser_getNumFields(row);

    CsvParser_destroy_row(row);
    CsvParser_destroy(csvparser);
    return features;
}

/* Get the number of data items in a given list */
int getNumItems(const char *filename) {
    CsvParser *csvparser = CsvParser_new(filename, ",", 0);
    CsvRow *row;
    int numOfKnowns = 0;

    while ((row = CsvParser_getRow(csvparser))) {
	numOfKnowns++;
	if (row) {
	    CsvParser_destroy_row(row);
	}
    }
    CsvParser_destroy(csvparser);
    return numOfKnowns;
}

void printPredictions(char **predictions, int length) {
    for (int i = 0; i < length; i++) {
	printf("Item %d prediction: %s\n", i+1, predictions[i]);
    }
}


int main(int argc, char **argv) {
    /* CURRENT FUNCTIONALITY: user inputs dataset and list of unknowns as arguments */
   /* TODO: ideally, user could input a link to their dataset */ 

    char *dataset;
    char *unknowns;

    if (argc == 3) {
	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
	    printf("Usage error: empty input file.\n");
	    return 0;
	}
	fclose(fp);
	dataset = (char*) alloc(sizeof(argv[1]));
	strcpy(dataset, argv[1]);
	FILE *fp2 = fopen(argv[2], "r");
	if (!fp2) {
	    printf("Usage error: empty input file.\n");
	    return 0;
	}
	fclose(fp2);
	unknowns = (char*) alloc(sizeof(argv[2]));
	strcpy(unknowns, argv[2]);
    }
    else {
	printf("Usage error: invalid input files.\n");
	return 0;
    }

    int features = getNumFeatures(dataset);
    int numOfKnowns = getNumItems(dataset);

    /* Initialize arrays for dataset */
    char **datasetNames = (char**) alloc(numOfKnowns*sizeof(char*));
    float datasetValues[numOfKnowns][features];

    /* Fill out arrays */
    int i;
    int curr = 0;
    CsvParser *csvparser = CsvParser_new(dataset, ",", 0);
    CsvRow *row;
    while ((row = CsvParser_getRow(csvparser))) {
	const char **rowFields = CsvParser_getFields(row);	
	/* Fill in datasetValues */
	// TODO:
	// (currently we go up to features - 1 because the final feature is the name of the data item)
	// (however this may vary for different data sets)
	for (i = 0; i < features - 1; i++) {
	    datasetValues[curr][i] = strtof(rowFields[i], NULL);
	}
	/* Fill in datasetNames */
	datasetNames[curr] = (char*) alloc(sizeof(rowFields[i]));
	strcpy(datasetNames[curr], rowFields[i]);
	curr++;
	if (row) {
	    CsvParser_destroy_row(row);
	}
    }
    CsvParser_destroy(csvparser);

    int numOfUnknowns = getNumItems(unknowns);
    float unknownData[numOfUnknowns][features];
    
    /* Fill out unknown data items' array */
    i = 0;
    curr = 0;
    CsvParser *newparser = CsvParser_new(unknowns, ",", 0);
    CsvRow *newRow;
    while ((newRow = CsvParser_getRow(newparser))) {
	const char **rowFields = CsvParser_getFields(newRow);
	for (i = 0; i < features - 1; i++) {
	    unknownData[curr][i] = strtof(rowFields[i], NULL);
	}
	curr++;
	if (newRow) {
	    CsvParser_destroy_row(newRow);
	}
    }
    CsvParser_destroy(newparser);


    /* Make predictions */
    char **predictions = (char**) alloc(numOfUnknowns*sizeof(char*));
    double dist;
    for (i = 0; i < numOfUnknowns; i++) {
	double closestDistance = calculateDistance(features - 1, unknownData[i], datasetValues[i]);
	int closestIndex = 0;
	for (int j = 0; j < numOfKnowns; j++) {
	    dist = calculateDistance(features - 1, unknownData[i], datasetValues[j]);
	    if (dist < closestDistance) {
		closestDistance = dist;
		closestIndex = j;
	    }
	}
	predictions[i] = (char*) alloc(sizeof(datasetNames[closestIndex]));
	strcpy(predictions[i], datasetNames[closestIndex]);
    }

    printPredictions(predictions, numOfUnknowns);

    /* TODO: still getting a lot of valgrind errors */
    free(dataset);
    free(unknowns);
    for (int x = 0; x < numOfKnowns; x++) {
	free(datasetNames[x]);
    }
    free(datasetNames);
    for (int y = 0; y < numOfUnknowns; y++) {
	free(predictions[y]);
    }
    free(predictions);
}
