/*
 * k-Nearest Neighbor algorithm
 *
 * Author: Thomas Maher
 * Updated: 1-17-2020
 *
 *
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <cstdint>
#include <cmath>
#include <utility>
#include <algorithm>

inline bool sort_pair(const std::pair<uint32_t,float>& Lpair, const std::pair<uint32_t,float>& Rpair) {
  /*
    Used to sort the pairs based on the float value (Euclidean distance).

    @param Lpair: Left pair to compare.
    @param Lpair: Left pair to compare.

    @return: Whether the left-hand distance is less than the right-hand distance.
  */
  return Lpair.second < Rpair.second;
}

float euclidean_distance(float *point1, float *point2, uint32_t N) {
  /*
    Find the Euclidean distance between 2 vectors.
    Euclidean distance is equal to:
      sqrt(sum i to N (x1_i - x2_i)^2)

    @param point1: The first point.
    @param point2: The first point.
    @param N: Domentionality of the points.

    @return: The distance between the points.
  */
  float distance = 0;

  for(int i=0; i<N; ++i)
    distance += std::pow((point1[i] - point2[i]), 2);

  return std::sqrt(distance);
}


float** get_neighbors(float **dataset, float *point, uint32_t k, uint32_t numPoints, uint32_t space) {
  /*
    Calculate the k nearest neighbors to the given point using the Euclidean distance calculation.

    @param dataset: The set of points to find neighbors from.
    @param point: The point we want to find the neighbors of.
    @param k: The number of neighbors to return.
    @param numPoints: The number of points in the dataset.
    @param space: What dimension of space we are working in.

    @return: A list of neighbor points for the given point.
  */
  float **neighbors = new float *[k];
  std::pair<uint32_t,float> *distances = new std::pair<uint32_t,float>[numPoints];
  for(int i=0; i<numPoints; ++i)
    distances[i] = std::pair<uint32_t,float>(i, euclidean_distance(point, dataset[i], space=2));
  std::sort(distances, distances+numPoints, sort_pair);

  for(int i=0; i<k; ++i)
    neighbors[i] = dataset[distances[i].first];

  return neighbors;
}

uint32_t predict_classification(float **dataset, float *point, uint32_t k, uint32_t numPoints, uint32_t space=2) {
  /*
    Entry point of the kNN algorithm. Calls get_neighbors and finds which class has the most neighbors for our point.
    Currently just picks the class with the majority of neighbors.

    @param dataset: The set of points to find neighbors from.
    @param point: The point we want to find the neighbors of.
    @param k: The number of neighbors to return.
    @param numPoints: The number of points in the dataset.
    @param space: What dimension of space we are working in.

    @return: Class that we predict the point is in.
  */
  uint32_t class_prediction;

  std::cout << "Predicting class for point: [";
  for(int i=0; i<space; ++i) {
    std::cout << point[i];
    if(i != space-1)
      std::cout << ", ";
    else
      std::cout << "]\n";
  }

  float **neighbors = get_neighbors(dataset, point, k, numPoints, space);

  uint32_t most = 0;
  uint32_t count;
  for(int i=0; i<k; ++i) {
    count = 1;
    for(int j=i+1; j<k; ++j)
      if(neighbors[j][space] == neighbors[i][space])
        count++;
    if(count > most) {
      class_prediction = (uint32_t) neighbors[i][space];
      most = count;
    }
  }

  std::cout << "  Prediction: " << class_prediction << std::endl;
  std::cout << "  Confidence: " << most*100/k << std::endl;
  return class_prediction;
}

void init_dataset(float **dataset, float *point, std::string dataFile, std::string pointFile, uint32_t space=2) {
  /*
    Data should be set so that the [0:N-1] columns are coordinate data, and the last column is the classification.

    @param dataset: The set of points to find neighbors from.
    @param point: The point we want to find the neighbors of.
    @param dataFile: File to get the training points from.
    @param pointFIle: File to get the point to classify from.
    @param space: What dimension of space we are working in.
  */
  uint32_t numPoints;
  std::string line;
  uint32_t i = 0;
  std::ifstream dataStream(dataFile), pointStream(pointFile);
  while(std::getline(dataStream, line)) {
    std::stringstream ssFile(line);
    dataset[i] = new float[space+1]; // To load a point and its classification
    uint32_t j = 0;
    for(float nextNum; ssFile >> nextNum; ) {
      dataset[i][j] = nextNum;
      if(ssFile.peek() == ',')
        ssFile.ignore();
      ++j;
    }
    ++i;
  }

  std::getline(pointStream, line);
  std::stringstream ssPoint(line);
  i = 0;
  for(float nextNum; ssPoint >> nextNum; ) {
    point[i] = nextNum;
    if(ssPoint.peek() == ',')
      ssPoint.ignore();
    ++i;
  }
  return;
}

int main(int argc, char **argv) {
  /*
    How to run:
      ./k-nearest <n-space> <k> <dataset-csv> <point-csv>

    @param space: What dimensional space we are working in.
    @param k: Number of neighbors we will be looking for.
    @param dataFile: The file storing all training points.
    @param pointFIle: The file storing the point we want to classify.
  */
  uint32_t space, k;
  std::string dataFile, pointFile;
  if(argc != 5) {
    std::cout << "How to run:\n\t./k-nearest <n-space> <k> <dataset-csv> <point-csv>\n\n";
    return 1;
  }
  else {
    space = std::atoi(argv[1]);
    k = std::atoi(argv[2]);
    dataFile = argv[3];
    pointFile = argv[4];
  }

  std::ifstream dataStream(dataFile);
  uint32_t numPoints = std::count(std::istreambuf_iterator<char>(dataStream), std::istreambuf_iterator<char>(), '\n');

  float **dataset = new float *[numPoints]; // Need to allocate arrays outside of init function otherwise we won't
  float *point = new float[space];          // have reference to the arrays

  init_dataset(dataset, point, dataFile, pointFile, space);
  predict_classification(dataset, point, k, numPoints, space);

  return 0;
}
