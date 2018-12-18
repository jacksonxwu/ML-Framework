#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include "bayes.h"
using namespace std;


double mean(double *numbers, int length){
  double sum = 0.0;
  for (int i = 0;  i < length ; i++){
    sum += numbers[i];
  }
  return sum/length;
}

double var(double *numbers, int length){
  double avg = mean(numbers, length);
  //size_t length = sizeof(numbers)/sizeof(float);
  double sum = 0.0;
  for (int i = 0; i < length ; i++){
    sum += pow(numbers[i]-avg,2);
  }
  return sum/(length-1);
}

double stdev(double *numbers, int length){
  double variance = var(numbers, length);
  return sqrt(variance);
}



//int main(){
//  double numbers[] = {1.0,2.0,3.0,4.0,5.0};
//  cout << "Mean: " << mean(numbers,5) << endl;
//  cout << "Standard Deviation: " << stdev(numbers, 5) << endl;
//}
