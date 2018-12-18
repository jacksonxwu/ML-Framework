#ifndef LINREGRESS_H
#define LINREGRESS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

    double *lrTrain(int iterations, double learn, int count, double *x, double *y, FILE *out){

	double a_0, a_1;
	a_0 = a_1 = 1.0;	
	double estim, total0, total1, mean_sq_er, error;
        estim = total0 = total1 = mean_sq_er = error = 0.0;	
	int epoch = 0;
	double *result;

	if (!(result = malloc(2 * sizeof(double)))){
	    fprintf(stderr,"Malloc failed\n");
	    exit(2);
	}
	
	if (!x || !y){
	    fprintf(stderr,"Please enter two non-null arrays\n");
	    exit(2);
	}

	for (epoch = 0; epoch < iterations; epoch++){
	    int i;
	    for (i = 0; i<count; i++) {
		//finds total error
		estim = a_0 + x[i] * a_1;
		error = estim - y[i];
		total0 += error; 
		total1 += error * x[i];
		mean_sq_er += error*error;
	    }

	    mean_sq_er = mean_sq_er / (double) count;	
	    a_0 = a_0 - learn * total0 / count;
	    a_1 = a_1 - learn * total1 / count;

	    if (epoch % 10 == 0){
		if (out){	
		fprintf(out,"MSE: %f\n",mean_sq_er);
		fprintf(out,"a's: %f\t%f\n",a_0,a_1);
		}
	    }
	    mean_sq_er = total0 = total1 = 0;
	    
	}
	result[0] = a_0;
	result[1] = a_1;
	return result;
}

void lrTest(double *x,double *y, double *ab, int l, FILE *out){
    //use a and b as predictors for test data set 
    int i = 0;
    for (i = 0; i < l; i++){
	
	fprintf(out, "Input: %f\t", x[i]);
	fprintf(out, "Predicted: %f\t", ab[0] + x[i] * ab[1]);
	fprintf(out, "Actual: %f\n", y[i]);
	
	}
    fprintf(out,"%f\t%f",ab[0],ab[1]);
    
    return;
}
#endif
