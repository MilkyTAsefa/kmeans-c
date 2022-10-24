//Kmeans.c
//Milky Tefera Asefa
//k-means clustering

#include "kmeans.h"

void loadData(){
    
    //Read data from file
    FILE *fileHandle;
    fileHandle=fopen(kmDataFileName,"r");
    if(fileHandle==NULL){
        printf("Error opening : %s Please check and rerun\n",kmDataFileName);
        exit(-1);
    }

    //Read file line by line from file
    char* readLine;
    readLine=(char*)malloc(2*sizeof(float)*sizeof(char));
    unsigned int ii=0;
    while(fscanf(fileHandle,"%s",readLine)!=EOF && ii<kmDataSize){

        //Strip newline char "\n"
        readLine=strtok(readLine,"\n");

        //Split the coordinates by "," delimiter
        data[ii].x=strtof(strtok(readLine, ","),NULL);
        data[ii].y=strtof(strtok(NULL, ","),NULL); 
        ii++;
    }
    fclose(fileHandle);

    if(ii!=kmDataSize){
        printf("Missmatch in the number of data given %i and number of data expected %i\n",ii,kmDataSize);
        exit(-1); 
    }
}


void intializeCentroids(){
    //Read cluster intial file
    FILE *fileHandle;
    fileHandle=fopen(kmClusterInitial,"r");
    if(fileHandle==NULL){
        exit(-1);
    }

    for(unsigned int ii=0;ii<kmNumberOfClusters;ii++){
        //Read file line by line from file
        char* readLine;
        readLine=(char*)malloc(10);

        fscanf(fileHandle,"%s",readLine);

        //Strip newline char "\n"
        readLine=strtok(readLine,"\n");

        centroids[ii].x=strtof(strtok(readLine, ","),NULL);
        centroids[ii].y=strtof(strtok(NULL, ","),NULL);
        centroids[ii].clusterName=strtok(NULL, ",");
    }
    fclose(fileHandle);
}


void intialize(){
    //Allocate memory for data
    data=(kmData*)malloc(sizeof(kmData)*kmDataSize);
    
    //convergence indicator
    converged=false;

    //Allocate memory for cluster centroids
    centroids=(kmCentroid*)malloc(sizeof(kmCentroid)*kmNumberOfClusters);

    //Buffer to keep tally of current diastance of coordinate from centriods
    distFromCentroids=(kmDist*)malloc(sizeof(kmDist)*kmNumberOfClusters);

    intializeCentroids();
    loadData();
}


float euclidianDis(kmData coordinate, kmCentroid center){
    float xdis = coordinate.x-center.x;
    float ydis = coordinate.y-center.y;
    float dist = sqrt((xdis*xdis)+(ydis*ydis));
    return(dist);
}


kmDist minDistance(){
     kmDist minDist=distFromCentroids[0];
     for(unsigned int kk=0;kk<kmNumberOfClusters;kk++){
         if(distFromCentroids[kk].dist < minDist.dist)
             minDist = distFromCentroids[kk];
     }
     return(minDist);
}


void reCluster(){
     int countOfReclusteredPoints = 0;
     for(unsigned int ii = 0; ii < kmDataSize; ii++){
         for(unsigned int kk=0;kk<kmNumberOfClusters;kk++){
             distFromCentroids[kk].dist = euclidianDis(data[ii],centroids[kk]);
             distFromCentroids[kk].clusterName = centroids[kk].clusterName;
         }
         char *previousCluster = data[ii].clusterInfo.clusterName;
         data[ii].clusterInfo=minDistance();

         if(previousCluster != data[ii].clusterInfo.clusterName)
             countOfReclusteredPoints++;
     }

     //Check if convergence achived
     if(countOfReclusteredPoints==0)
         converged=true;
}


void reCalcMean(){
     for(unsigned int kk=0;kk<kmNumberOfClusters;kk++){
         int count=0;
         float xSum=0;
         float ySum=0;
         for(unsigned int ii = 0; ii < kmDataSize; ii++){
             if(data[ii].clusterInfo.clusterName == centroids[kk].clusterName){
                 count++;
                 xSum += data[ii].x;
                 ySum += data[ii].y;
             }
         }
         centroids[kk].x = xSum/count;
         centroids[kk].y = ySum/count;
     }
}


void cleanBuffers(){
    //Free memory
    free(data);
    free(centroids);
    free(distFromCentroids);
}


void kmeans(){
    while(!converged){
        reCluster();
        if(!converged)
            reCalcMean();
    }
}


void writeResults(){
    //Read data from file
    FILE *fileHandle;
    fileHandle=fopen(kmOutputFile,"w");
    float errorSum=0.000;
    for(unsigned int ii = 0; ii < kmDataSize; ii++)
        errorSum +=  data[ii].clusterInfo.dist;
    fprintf(fileHandle,"error=%.3f\n",errorSum);

    for(unsigned int ii = 0; ii < kmDataSize; ii++)
        fprintf(fileHandle, "%s\n", data[ii].clusterInfo.clusterName);
   
    fclose(fileHandle);
}


int main(int argc, const char *argv[]){
    kmDataFileName="input.csv";//argv[1];
    kmClusterInitial="clusterInti.csv";//argv[2];
    kmOutputFile="output.csv";
    kmDataSize=10000;//atoi(argv[3]);
    kmNumberOfClusters=5;//atoi(argv[4]);
    intialize();
    kmeans();
    writeResults();
    cleanBuffers();
    
    return 0;
}
