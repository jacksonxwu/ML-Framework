#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
#include "csvparser.h"
#include "bayes.h"
#include "predict.h"
#include "gendata.h"

using namespace std;



int main() {
    int i =  0;
    int numlines = 0;
    int nwords = 0;
    int npunc = 0;
    int numchars = 0;
    double wlen = 0;
    //                                   file, delimiter, first_line_is_header?
    CsvParser *csvparser = CsvParser_new("test.txt", " ", 0);
    CsvRow *row;

    while ((row = CsvParser_getRow(csvparser)) ) {
    	//printf("==NEW LINE==\n");
        numlines++;
        const char **rowFields = CsvParser_getFields(row);
        for (i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
            nwords++;
            int n = (int) strlen(rowFields[i]);
            for(int j = 0; j < n ; j++){
                numchars++;
                if(ispunct(rowFields[i][j])){
                    npunc++;
                }
            }


            //printf("FIELD: %s\n", rowFields[i]);
        }
		//printf("\n");
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);
//    cout << "-----Data for Input-----" << endl;
//    cout << endl;
//
//    wlen = (double)numchars / (double)nwords;
//    cout << "Number of lines: " << numlines << endl;
//    cout << "Number of Words: " << nwords << endl;
//    cout << "Number of Punctuation: " << npunc << endl;
//    cout << "Average Word Length: " << wlen << endl;
//    cout << endl;

    wlen = (double)numchars / (double)nwords;
    cout << "----Results----" << endl;
    result(numlines, nwords, npunc, wlen);

    return 0;
}
