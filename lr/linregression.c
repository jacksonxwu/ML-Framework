#include"csvparser.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"linregress.h"
double * makeCol(int len){
    double *result;
    if (!(result = malloc(len * sizeof(double)))){
	fprintf(stderr,"Malloc Failed\n");
	exit(2);
    }
    return result;
}

int main (int argc, char * argv[]){
    FILE* f;
    int fsize = 0;
    char *fname = "";
    int rowcount = 0;
    //int testrows= 0;
    //alpha is learning rate
    double alpha = 0.0001;
    int epoch = 1000000;
    double *testx;
    double *testy;
    double *trainx;
    double *trainy;
    double *ab;
    int count = 0;

    if (argc == 2){
	printf("Please enter # of rows to be trained (the rest will be tested): ");
	if (scanf("%d", &rowcount) != 1){
	    fprintf(stderr,"Failed to read number\n");
	    exit(2);
	}
	printf("\n");
	
    }
    if (argc == 3){
	fname = argv[1];
	if (sscanf(argv[2],"%d", &rowcount) != 1){
	    fprintf(stderr,"Please enter a file and number of rows to train: \n");
	    exit(2);
	}
    }
    if ((f = fopen(fname,"r"))){
	int c = 0;
	while ((c = fgetc(f)) != EOF){
	    if (c == '\n'){
		fsize++;
	    }
	}
	//to ignore header 
	fsize--;
	fclose(f);
    } else { 
	fprintf(stderr,"Failed to open file");
	exit(2);
    }

    CsvParser *csvparser = CsvParser_new(fname,",",1);
    CsvRow *row;
   
    trainx  = makeCol(rowcount);
    trainy = makeCol(rowcount);
    testx = makeCol(fsize - rowcount);
    testy = makeCol(fsize - rowcount);

    //Parses input file
    
    while ((row  = CsvParser_getRow(csvparser))){
	const char **fields = CsvParser_getFields(row);
	if (CsvParser_getNumFields(row) < 2){
	    fprintf(stderr,"Not enough fields in row. Every row must have at least two fields\n");
	    exit(2);
	}
	if (count < rowcount){
	    trainx[count] = atof(fields[0]);
	    trainy[count] = atof(fields[1]);
	} else {
	    testx[count-rowcount] = atof(fields[0]);
	    testy[count-rowcount] = atof(fields[1]);
	}
	count++;
	CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);

    //Trains algorithm
    ab = makeCol(2);
    ab = lrTrain(epoch,alpha,rowcount,trainx,trainy,NULL); 
    
    //Compares predictions to real results for rest of data
    lrTest(testx,testy,ab,count - rowcount, stdout);
    
    free(ab);
    free(trainx);
    free(trainy);
    free(testx);
    free(testy);

    return 1;

}
