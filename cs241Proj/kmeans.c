#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "csvparser.h"
#define sqr(x) ((x)*(x))
#define DATA_LENGTH 1000
#define BIG (1000*1000)
#define MAX 16

int *output;

void *alloc(size_t size){
  void *m = malloc(size);
  if(m == NULL){
    printf("%s\n", "Malloc Failed");
    exit(1);
  }
  return m;
}

double distance(int dim, double *x, double *y){
  double sum_sq = 0;

  for(int i = 0; i < dim; i++){
    sum_sq += sqr(x[i] - y[i]);
  }

  return sum_sq;
  
}

double all_distance(int dim, int n, int k, double *X, double *centroid, double *dist){
  int i;
  int j;

  for(i = 0; i<n; i++){ //for each point
    for(j = 0; j < k; j++){ //for each cluster
      dist[i*k + j] = distance(dim, &X[i*dim], &centroid[j*dim]);
    }
  }
}


void choose_cluster(int dim, int n, int k, double *dist_array, int *cluster_member_index){

  for(int i=0; i<n; i++){ //for each point
    
    int best = -1; //intial best index
    double closest_dist = BIG;
    
    for(int j=0; j<k; j++){ // for each cluster
      double current_dist = dist_array[i*k + j]; //the distance between a point and centroid
      if(current_dist < closest_dist){
	best = j; //set the best index to equal the j cluster
	closest_dist = current_dist; // set the current distance to be the new closest distance
      }
    }
    cluster_member_index[i] = best; //put that point into the cluster
  }
}



void update_clusters(int dim, int n, int k, double *X, int *cluster_member_index, double *new_centroid){
  int member_count[MAX]; //initalize member count

  //initalize centroids
  for(int i=0; i<k; i++){
    member_count[i] = 0;

    for(int j=0; j<dim; j++){
      new_centroid[i*dim + j] = 0;
    }
  }
  
  //add up all the points in each cluster
  for(int i = 0; i<n; i++){
    int curr_cluster = cluster_member_index[i];
    member_count[curr_cluster]++;//update the number of members in the cluster
    // printf("%d\n", member_count[curr_cluster]);
	   
    for(int j=0; j < dim; j++){
      new_centroid[curr_cluster*dim + j] += X[i*dim +j]; //coordinate for finding centroid
    }

    //each cluster
    for(int i=0; i<k; i++){
      printf("%d\n", member_count[i]);
      if(member_count[i] == 0){
	printf("Empty Cluster %d \n", i);
	break;
      }

      //each dimension
      for(int j=0; j<dim; j++){
	new_centroid[i*dim +j] /= member_count[i]; //new centroids
	printf("%.2f\n", new_centroid[i*dim+j]);
      }
    }
  }
}

//the diagrams used to print the output
void diagram(int dim, int n, int k, double *X, int *cluster_member_index, double *centroid){
  int member_count[MAX];
  int i;
  
  printf("Final Clusters: \n");
  for(i=0; i<k; i++){
    printf("cluster %d:     members: %8d,  centroid (%.1f %.1f) \n", i+1, member_count[i],
	   centroid[i*dim +0], centroid[i*dim +1]);
  }
}

//copies kmeans results into an array
void copy(int n, int *old, int *new){
  int i;

  for(i=0; i<n; i++){
    old[i] = new[i];
  }
}

//checks to see if the cluster means changed
int change_count(int n, int x[], int y[]){
  int count = 0;
  int i;

  for(i=0; i<n; i++){
    if(x[i] != y[i]){
      count++;
    }
  }
  return count;
}
      
//dim = dimension of data
//*X = pointer to the data
//n = number of elements
//k = number of clusters
void kmeans(int dim, int k, int n, double *X,  int *output_cluster, double *centroid){
  double prev_total = BIG;
  int iter = 0; //number of iterations

  //dynamically allocate all arrays needed
  double *dist = (double *)alloc(sizeof(double) * (n*k));
  int *curr_cluster = (int *)alloc(sizeof(int) *n);
  int *prev_cluster = (int *)alloc(sizeof(int)*n);

  //initalize
  all_distance(dim, n, k, X, centroid, dist);
  choose_cluster(dim, n, k, dist, curr_cluster);
  copy(n, curr_cluster, prev_cluster);

  while(iter < MAX){

    //calculate new centroids
    update_clusters(dim, n, k, X, curr_cluster, centroid);

    //save the cluster
    copy(n , curr_cluster, prev_cluster);

    //move all points to the nearest cluster
    all_distance(dim, n, k, X, centroid, dist);
    choose_cluster(dim, n, k, dist, curr_cluster);

    //sees if the clusters changed or not
    int count = change_count(n, curr_cluster, prev_cluster);

    //if they didn't change, you're done
    if(count == 0){
      printf("Done \n");
      break;
    }

    iter++;
  }

  diagram(dim, n, k, X, curr_cluster, centroid);
  copy(n, curr_cluster, output_cluster);

  free(dist);
  free(curr_cluster);
  free(prev_cluster);
}
  
int main(){
  /*double inital_centroids[100];
  int arr[100];
  output = (int *)malloc(sizeof(int)*BIG);
  for(int i = 0;  i < 21; i++){
    arr[i] =  rand() % 10;
  }
  int count = 0;
  double *arrD;
  arrD = (double *)malloc(sizeof(double)*100);
  for(int i = 0; i < 20; i ++){
    if(arr[i] == 0){
      continue;
    }
    else{
      arrD[i] = arr[i] * 1.0;
      count++;
    }
    printf("%d\n", count);
    printf("%.2f\n", arrD[i]);
  }
  
  for(int i = 0; i < 2; i++){
    int random = rand() % 18;
    double dub = (double) random;
    inital_centroids[i] = dub;
  }
  for(int j = 0; j < 2; j++){
    printf("%f\n", inital_centroids[j]);
  }
  int i =  0;
  //                                   file, delimiter, first_line_is_header?
  CsvParser *csvparser = CsvParser_new(example_file.csv, ",", 0);
  CsvRow *row;
  
  while ((row = CsvParser_getRow(csvparser)) ) {
    printf("==NEW LINE==\n");
    const char **rowFields = CsvParser_getFields(row);
    for (i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
      printf("FIELD: %s\n", rowFields[i]);
    }
    printf("\n");
      CsvParser_destroy_row(row);
  }
  CsvParser_destroy(csvparser);
  
  return 0;*/
  double arr[33] = {0.2, 0.2, 0.4, 2.3, 1.0, 1.4, 0.2, 2.5, 1.9, 1.1, 1.5, 0.2, 0.3, 2.4, 0.2, 0.3, 2.4, 0.2, 1.5, 0.4, 1.5, 1.1, 1.3, 2.3, 1.4, 0.2, 1.4, 2.4, 0.2, 1.8, 1.0, 1.3, 1.8};
  double arr2[33] = {1.4, 1.4, 1.3, 1.7, 1.5, 1.1, 1.6, 4.6, 4.5, 4.7, 4.1, 4.3, 4.9, 4.8, 5.1, 4.7, 5.2, 5.3, 6.0, 5.9, 5.5, 1.1, 1.2, 3.9, 4.4, 1.9, 5.1, 4.1, 4.6, 4.8, 5.3, 1.3, 4.8};
  double arr3[33];

  
  for(int i = 0; i < 33; i++){
   arr3[i] = (arr[i] + arr2[i])/2;
   printf("%f\n", arr3[i]);
  }

  double initial_centroids[3] = {0.4, 1.5,2.3};
  kmeans(1, 3, 33, arr3, output, initial_centroids);
}
