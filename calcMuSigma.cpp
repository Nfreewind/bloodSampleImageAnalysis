#include <opencv2/photo/photo.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
using namespace cv;
using namespace std;

pair<double, double> calcMuSigma(vector<double> vol)//return mean and std. deviation of given data (volumes)
{
    // Calculates mean and standard deviation of given vector <double>,
    //
    // Mean (mu) = summation for all i (vol[i])
    //             ----------------------------
    //                 number of observations
    //
    // Variance (sigma^2) = summation for all i (vol[i] - mean)^2
    //                      -------------------------------------
    //                              number of observations
    //
    // Standard deviation (sigma) = sqrt(sigma^2)


    pair<double, double> retval;//returning pair retval.first <= mean, retval.second <= std. deviation
    retval.first=retval.second=0.0;//initialize the retval

    for(int i=0; i<vol.size(); i++)
    {
        retval.first=retval.first+vol[i];//sum all the volumes
    }

    retval.first=retval.first/vol.size();//calculate and store the mean volume

    for(int i=0;i<vol.size();i++)
    {
        retval.second=retval.second+(retval.first-vol[i])*(retval.first-vol[i]);//store the squared distance between each volume vol[i] and the mean
    }

    retval.second=retval.second/vol.size();//find sigmaSquared (=variance)
    retval.second=sqrt(retval.second);//stddeviation = sqrt(variance)

    return retval;//returns the ordered pair: (mu, sigma)
}
