#include <opencv2/photo/photo.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
using namespace cv;
using namespace std;

vector<double> measureVolume(Mat);//detects blobs and calculates volume (in terms of pixel intensity)
pair<double, double> calcMuSigma(vector<double>);//return mean and std. deviation of given data (volumes)
vector<double> Normalize(vector<double>);//scales the volume to [0,1] interval
