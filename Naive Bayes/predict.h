#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>


double calculateProbability(double x, double mean, double stdev);

double calculateClassProbability( double *features, double *means , double *stdev , int nfeatures);

void predictClass( double *features, double **emaildata , double **messdata, int nfeatures);



