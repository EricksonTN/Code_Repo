#include <iostream>
#include <fstream>
#include <vector>
#include "pntvec.h"
#include <cmath>

// CS3505
// Professor Jenkins
// 1/25/21
// Author: Erickson Nguyen



void Determine_Candidate (std::vector<pntvec> candidates, std::vector<pntvec> cloud);
double Determine_Score (pntvec candidate, std::vector<pntvec> point_cloud);


// Handles the file input stream to receive the data needed to determine the best candidate against a point cloud.
//
// Return: returns 0 if passes with no compilation error.
int main() 
{
  std::vector<pntvec> possibleCandidates;
  std::vector<pntvec> pointCloud;
  

  //File Input stream that takes in the list of possible candidates.
  std::ifstream candidateReader("candidates.txt");

  //A loop that handles the input stream's data and puts it into a vector that contains all the candidates. It will break out of the while loop once the stream fails.
  while(true)
  {
    pntvec candidate;
    candidateReader >> candidate.x >> candidate.y >> candidate.z;
    if (candidateReader.fail())
    {
      break;
    }
    possibleCandidates.push_back(candidate);
  }  

  candidateReader.close();
  
  //File Input stream that takes in the list of points in the point cloud.
  std::ifstream pcReader ("point_cloud.txt");

  //A loop that handles the input stream's data and puts it inot a vector that contains all the points in the point cloud. It will break out of the while loop once the stream fails.
  while(true)
  {
    pntvec cloudPoint;
    pcReader >> cloudPoint.x >> cloudPoint.y >> cloudPoint.z;
    if (pcReader.fail())
    {
      break;
    }
    pointCloud.push_back(cloudPoint);
  }

  pcReader.close();

  Determine_Candidate(possibleCandidates, pointCloud);

  return 0;
}


// Handles the process of determining the candidate that best fits the point cloud. Takes in a 'candidates' vector as well as a 'point cloud' vector. utilizes both to find the lowest score pntvec before
// outputting the results to the console with the best candidate's score, followed by it's coordinates.
//
// Return: void
void Determine_Candidate (std::vector<pntvec> candidates, std::vector<pntvec> cloud)
{
  int bestIndex = 0;

  // Get's the initial best score
  double best = Determine_Score(candidates[0], cloud); 

  // Iterates through the list of candidates and compares their scores to determine the one with the lowest score.
  for (int i = 1; i < candidates.size(); i++)
  {
    double possibleBest = Determine_Score(candidates[i], cloud);
    if (possibleBest < best)
    {
      best = possibleBest;
      bestIndex = i;
    }
  }

  std::cout << best << std::endl;
  std::cout << candidates[bestIndex].x << " " << candidates[bestIndex].y << " " << candidates[bestIndex].z << std::endl;
}


// Determines the score of an individual candidate utilizing the point_cloud given. The method takes in a singular candidate, and a vector containing all the points in the point cloud.
// 
// Return: Method returns the score for the inputted candidate.
double Determine_Score (pntvec candidate, std::vector<pntvec> point_cloud)
{
  double distance = 0;
  double score = 0;

  // Handles the math of the current candidate's scoring against the entirety of the point_cloud vector.
  for (int i = 0; i < point_cloud.size(); i++)
  {
    pntvec currPoint = point_cloud[i];
    double x = candidate.x - currPoint.x;
    double y = candidate.y - currPoint.y;
    double z = candidate.z - currPoint.z;

    distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    score = score + (pow(distance, 2));
  }
  
  return score;
}




