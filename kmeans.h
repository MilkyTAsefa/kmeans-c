//Kmeans.h
//Milky Tefera Asefa
//k-means clustering

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

unsigned int kmDataSize;
unsigned int kmNumberOfClusters;

const char* kmDataFileName;
const char* kmClusterInitial;
const char* kmOutputFile;


typedef struct {
    float x;
    float y;
    char* clusterName;	
}kmCentroid;

typedef struct {
    float dist;
    char* clusterName;
}kmDist;

typedef struct {
    float x;
    float y;
    kmDist clusterInfo;
}kmData;

//Container to hold coordinates 
kmData *data;
//Container to hold centroids
kmCentroid *centroids;

kmDist *distFromCentroids;
bool converged;

void intializeBuffers();
void loadData();
void reCluster();
void reCalcMean();
void writeResults();
void cleanBuffers();
void intializeCentroids();
float euclidianDis(kmData coordinate, kmCentroid center);
kmDist minDistance();
void kmeans();
