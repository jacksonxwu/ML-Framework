#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "predict.h"

using namespace std;

double calculateProbability(double x, double mean, double stdev){
    double exponent = exp(-(pow(x-mean,2)/(2*pow(stdev,2))));
    return (1/(sqrt(2*M_PI) * stdev)) * exponent;
    }


double calculateClassProbability( double *features, double *means , double *stdev , int nfeatures){
    double probability = 1;
    for (int i = 0; i < nfeatures; i++){
        probability *= calculateProbability(features[i], means[i], stdev[i]);
    }
    return probability;
}

void predictClass( double *features, double **emaildata , double **messdata, int nfeatures){
    double probEmail;
    double probMess;
    probEmail = calculateClassProbability(features, emaildata[0] , emaildata[1], nfeatures);
    probMess = calculateClassProbability(features, messdata[0], messdata[1], nfeatures);
    cout << "Probability of being an Email is: " << probEmail << endl;
    cout << "Probability of being a Text Message is: " << probMess << endl;
    if( probEmail > probMess){
        cout << "This must be an Email!" << endl;
    }else{
         cout << "This must be a Text Message!" << endl;

    }

}


//int main(){
//  double emaildata[2][1] = {{1.0},{0.5}};
//  double messdata[2][1] = {{20.0}, {5.0}};
//  double test[] = {1.1};
//  predictClass(test,emaildata,messdata,1);
//
//}
