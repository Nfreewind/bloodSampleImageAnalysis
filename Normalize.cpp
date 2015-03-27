#include <opencv2/photo/photo.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
using namespace cv;
using namespace std;

vector<double> Normalize(vector<double> vol)//scales the volume to [0,1] interval
{

    // The formula for normalization used is currently by scaling the vector to [0,1] interval
    // alternatively standard normalization can be used as well (for normally distributed data)
    // Scaling,
    //                      (originalVol[i] - minimumVolume)
    //      scaledVol[i] =  --------------------------------
    //                      (maximumVolume - minimumVolume)


    double max_vol=-1.0, min_vol=9999999;//initialize
    for(int i=0; i<vol.size(); i++)
    {
        max_vol = (max_vol < vol[i]) ? vol[i] : max_vol;//find the maximum volume
        min_vol = (min_vol > vol[i]) ? vol[i] : min_vol;//find the minimum volume
    }
    double diff = max_vol - min_vol;//difference between the maximum and minimum volumes
    for(int i=0; i<vol.size(); i++)
    {
        vol[i]=(vol[i]-min_vol)/(diff);//scale all the volumes to [0,1] interval
    }
    return vol;//return scaled volumes for relative comparison
}

