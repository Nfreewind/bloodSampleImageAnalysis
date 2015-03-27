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
