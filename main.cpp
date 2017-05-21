#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include<iostream>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 73;
int points = 50;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void*);

int main( )
{
    src = imread( "1.jpg", 1 );

    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );

    namedWindow( "Wind", CV_WINDOW_AUTOSIZE ); imshow( "Wind", src );
    createTrackbar( " Threshold:", "Wind", &thresh, max_thresh, thresh_callback );

    namedWindow( "Wind1", CV_WINDOW_AUTOSIZE ); imshow( "Wind1", src );
    createTrackbar( " Threshold:", "Wind1", &points, max_thresh, thresh_callback );

    thresh_callback(0,0);

    waitKey(0);
    return(0);
}

void thresh_callback( int, void*)
{
    Mat threshold_output;
    Mat edge;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Canny( src_gray, edge, thresh, 3*thresh, 3);
    edge.convertTo(threshold_output, CV_8U);

    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    cout<<endl<<contours[1]<<endl;
    cout<<endl<<Mat(contours[1])<<endl;

    for( int i = 0; i < contours.size(); i++ )
    { minRect[i] = minAreaRect( Mat(contours[i]) );
        if( contours[i].size() > points )
        { minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
    }

    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {

        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        ellipse( drawing, minEllipse[i], color, 2, 8 );
        Point2f rect_points[4]; minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ )
            line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
    }
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}

