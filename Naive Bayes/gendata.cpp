#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "csvparser.h"
#include <iostream>
#include <fstream>
#include "bayes.h"
#include "predict.h"
#include "gendata.h"

using namespace std;



 double Enlines[1000], Enwords[1000], Enpunc[1000] , Ewlen[1000];
 double Mnlines[1000], Mnwords[1000], Mnpunc[1000] , Mwlen[1000];


double emailMeans[4];
double emailStdevs[4];
double messMeans[4];
double messStdevs[4];

void result(double nlines, double nwords, double npunc, double wlen) {
    CsvParser *csvparser = CsvParser_new("nbayes.data", ",", 0); //change back to comma
    CsvRow *row;
    int i =  0;
    int mcount = 0;
    int ecount = 0;
   //                                                file, delimiter, first_line_is_header?
    while ((row = CsvParser_getRow(csvparser)) ) {
    	  //printf("==NEW LINE==\n");
        const char **rowFields = CsvParser_getFields(row);
        if ((string) rowFields[0] == "1"){
           Mnlines[i] = atof(rowFields[1]);
           Mnwords[i] = atof(rowFields[2]);
           Mnpunc[i] = atof(rowFields[3]);
           Mwlen[i] = atof(rowFields[4]);
           mcount++;

        }else{
           Enlines[i] = atof(rowFields[1]);
           Enwords[i] = atof(rowFields[2]);
           Enpunc[i] = atof(rowFields[3]);
           Ewlen[i] = atof(rowFields[4]);
           ecount++;

        }

		//cout << Enlines[i] << endl;
        CsvParser_destroy_row(row);
        i++;
    }
    CsvParser_destroy(csvparser);

    emailMeans[0] = mean(Enlines,ecount);
    emailMeans[1] = mean(Enwords,ecount);
    emailMeans[2] = mean(Enpunc,ecount);
    emailMeans[3] = mean(Ewlen,ecount);
    emailStdevs[0] = stdev(Enlines,ecount);
    emailStdevs[1] = stdev(Enwords,ecount);
    emailStdevs[2] = stdev(Enpunc,ecount);
    emailStdevs[3] = stdev(Ewlen,ecount);
    messMeans[0] = mean(Mnlines,mcount);
    messMeans[1] = mean(Mnwords,mcount);
    messMeans[2] = mean(Mnpunc,mcount);
    messMeans[3] = mean(Mwlen,mcount);
    messStdevs[0] = stdev(Mnlines,mcount);
    messStdevs[1] = stdev(Mnwords,mcount);
    messStdevs[2] = stdev(Mnpunc,mcount);
    messStdevs[3] = stdev(Mwlen,mcount);

    double **emaildata = (double**) malloc(2*sizeof(emailMeans));
    double **messdata = (double**) malloc(2*sizeof(messMeans));
    emaildata[0] = (double*)malloc(sizeof(emailMeans));
    emaildata[1] = (double*)malloc(sizeof(emailStdevs));
    emaildata[0] = emailMeans;
    emaildata[1] = emailStdevs;
    messdata[0] = (double*)malloc(sizeof(messMeans));
    messdata[1] = (double*)malloc(sizeof(messStdevs));
    messdata[0] = messMeans;
    messdata[1] = messStdevs;

    double test[4] = {};
//    test[0] = atof(argv[0]);
//    test[1] = atof(argv[1]);
//    test[2] = atof(argv[2]);
//    test[3] = atof(argv[3]);
    test[0] = nlines;
    test[1] = nwords;
    test[2] = npunc;
    test[3] = wlen;
    predictClass(test, emaildata, messdata, 4);



}


