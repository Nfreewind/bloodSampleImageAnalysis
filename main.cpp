#include <opencv2/photo/photo.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include "header.h"
using namespace cv;
using namespace std;


int main()
{
    Mat image;//initialize a matrix to store the image
    image = imread("C:/Users/student/Desktop/adc/VIT/GSOC/helikar/COW5.jpg", 1);//read the image

    resize(image, image, Size(320, 480));//resize the image

    namedWindow( "Input", 2 );
    imshow( "Input", image );

    cvtColor(image, image, CV_BGR2GRAY);//convert to gray scale

    vector<double> vol = measureVolume(image);//vol stores the volumes of detected blobs

    pair<double, double> mean_sd = calcMuSigma(vol);//stores ordered pair (mean, standard deviation) of vector passed (vol)

    vol = Normalize(vol);//normalize the volume to [0,1] interval

    waitKey(0);//press space to end program

    return 0;

}


