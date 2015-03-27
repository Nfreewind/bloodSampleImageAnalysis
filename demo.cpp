#include <opencv2/photo/photo.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
using namespace cv;
using namespace std;

vector<double> measureVolume(Mat image)//detects blobs and calculates volume (in terms of pixel intensity)
{
    //Basic structure of the function:
    //1. First, a blurring filter is applied to reduce any unwanted noise to the resized image.
    //2. The image is then converted to gray-scale and spots/blobs are detected using thresholding.
    //3. The volume of the spots/blobs is calculated by summing over the intensity of pixels in a given detected spot in the original image.
    //   The volume of the spots/blobs is calculated in units of pixel intensity.

    Mat original = image.clone();//copy image
    Mat thresh = image.clone();//copy image
    namedWindow("gray", 2);//create window to show image
    imshow("gray",image);//show image
    medianBlur(image, thresh, 5);//blur image to reduce noise

    threshold(thresh, thresh, 140, 255, THRESH_BINARY);//threshold the image to obtain spots
    namedWindow("thresh", 2);
    imshow("thresh",thresh);

    Mat cont = thresh.clone();//copy image to obtain contours
    vector<vector<Point> > contours;//stores contour boundary
    vector<Vec4i> hierarchy;//contour hierarchy
    findContours(cont, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0,0));//finds contours in thresholded image

    vector<vector<Point> > contours_poly(contours.size());//contour outline
    vector<Rect> boundRect(contours.size());//stores bounding rectangles

    bitwise_not(thresh, thresh);//invert image to perform AND operation
    bitwise_and(original, thresh, original);//mask the image (only the blobs/spots remain)

    Mat drawing = Mat::zeros( cont.size(), CV_8UC3 );//display the blobs/spots found
    Mat temp;//stores the cropped image (for each detected blob in the image)
    namedWindow("temp", 2);

    namedWindow("contours", 2);
    RNG rng(235);//random number generator for coloring on Mat drawing


    vector<double> retval;//stores volumes (in terms of pixel intensity) to be returned
    for( int i = 0; i < contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));//select a random color (for display purpose)
        drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());//draw the outline of detected blob on Mat drawing

        int area = contourArea(contours[i], true);//finds the area of the blob (in terms of pixels)

        if(area > 0)//consider contours with value over threshold (i.e. the detected blobs only; ignores the background)
        {
            cout<<"Approximate area(in pixels): "<<area<<endl;//display area of the blob in pixels
            imshow("contours", drawing);//display the drawn outlines

            approxPolyDP( Mat(contours[i]), contours_poly[i], 0.05, true );//fit a polynomial on pixels
            boundRect[i] = boundingRect(Mat(contours_poly[i]));//find the bounding rectangle

            temp=original(boundRect[i]).clone();//crop the masked image (temp contains only a single blob with surroundings masked to 0)
            Scalar s = sum(temp);//calculate the sum of intensities of pixels (the volume)
            cout<<"Approximate volume(in units of pixel intensity): "<<2*s[0]<<endl;//display the volume of the sphere
            retval.push_back(2*s[0]);//add the new volume into the retval
            imshow("temp",temp);
            cout<<"\n*Press space key to see next detected (masked) blob*\n";
            waitKey(0);
        }

    }
    return retval;//return back the volumes of detected blobs
}

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

int main()
{
    Mat image;//initialize a matrix to store the image
    image = imread("testImage.jpg", 1);//read the image

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


