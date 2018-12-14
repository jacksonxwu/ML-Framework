#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_LENGTH 1000
#define BIG (1000*1000)
#define MAX 16

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
    sum_sq = sqr(x[i] - y[i]);
  }

  return sum_sq;
  
}

double all_distance(int dim, int n, int k, double *X, double *centroid, double *dist){
  int i;
  int j;

  for(i = 0; i<n; i++){ //for each point
    for(j = 0; j < k, j++){ //for each cluster
      dist[i*k + j] = distance(dim, &X[i*dim], &centroid[j*dim]);
    }
  }
}


void choose_cluster(int dim, int n, int k, double *dist_array, int *cluster_member_index){
  int i, j;

  for(i=0; i<n; i++){ //for each point
    
    int best = -1; //intial best index
    double closest_dist = BIG;
    
    for(j=0; j<k; j++){ // for each cluster
      double current_dist = dist_array[i*k + j]; //the distance between a point and centroid
      if(current_dist < closest_dist){
	best = j; //set the best index to equal the j cluster
	closest_dist = current_dist; // set the current distance to be the new closest distance
      }
    }
    cluster_member_index[i] = best_index; //put that point into the cluster
  }
}



void update_clusters(int dim, int n, int k, double *X, int *cluster_member_index, double *new_centriod){
  int member_count[MAX]; //initalize member count
  int i, j;

  //initalize centroids
  for(i=0; i<k; i++){
    member_count[i] = 0;

    for(j=0; j<dim; j++){
      new_centroid[i*dim + j] = 0;
    }
  }
  
  //add up all the points in each cluster
  for(i = 0; i<n; i++){
    int curr_cluster = cluster_member_index[i];
    member_count[current_cluster]++; //update the number of members in the cluster

    for(j=0; j < dim, j++){
      new_centroid[curr_cluster*dim + j] += X[i*dim +j]; //coordinate for finding centroid
    }

    //each cluster
    for(i=0; i<k; i++){
      if(member_count[i] == 0){
	printf("Empty Cluster %d \n", i);
      }

      //each dimension
      for(j=0; j<dim; j++){
	new_centroid[i*dim +j] /= member_count[i]; //new centroids
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
    printf("cluster %d:     members: %8d,  centroid (%.1f %.1f) \n", i, member_count[i],
	   centroid[i*dim +0], centroid[i*dim +1]);
  }
}

//copies kmeans results into an array
void copy(int n, int *old, int *new){
  int i;

  for(i=0; i<n, i++){
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

void kmeans(int dim, int k, int n, double *X, int *output_cluster, double *centroid){
  double prev_total = BIG;
  int iter = 0; //number of iterations

  //dynamically allocate all arrays needed
  double *dist = (double *)alloc(sizeof(double) * (n*k));
  int *curr_cluster = (int *)alloc(sizeof(int) *n);
  int *prev_cluster = (int *)alloc(sizeof(int)*n);

  //initalize
  all_distances(dim, n, k, X, centroid, dist);
  choose_clusters(dim, n, k, dist, curr_cluster);
  copy(n, curr_cluster, prev_cluster);

  while(iter < MAX){

    //calculate new centroids
    update_clusters(dim, n, k, X, curr_cluster, centroid);

    //save the cluster
    copy(n , curr_cluster, prev_cluster);

    //move all points to the nearest cluster
    all_distances(dim, n, k, X, centroid, dist);
    choose_clusters(dim, n, k, dist, curr_cluster);

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
  
