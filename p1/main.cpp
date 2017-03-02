/*
The MIT License (MIT)
Copyright (c) 2016-2017 Avishek Parajuli.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// main.cpp

#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>

#include "VideoController.h"

using namespace std;
using namespace cv;

/* Class LaneFinder is a sub-class of the Model interface class; This class will implement 
  the required method process which takes input frame  as cv::Mat& src and performs some operation on it.
   The output of the algorithm is stored in the 2nd input argument cv:: Mat& out.
 */
class LaneFinder: public Model {
	public:
  void process(Mat & src, Mat & out){
	Mat img;
	out= src.clone();
	cvtColor(src,img,CV_BGR2GRAY);
	Mat im_edge(img.size(), CV_8U, Scalar::all(0));
	Canny(img, im_edge,100,300,3);
	//util::imdisp(im_edge,"canny");
	// Rect(x,y,w,h) w->width=cols;h->rows
	Mat roi= im_edge(Rect(0,im_edge.rows*0.6, im_edge.cols,im_edge.rows-im_edge.rows*0.6));
	
	vector<Vec4i> lines;
	// 1st best {30, 20, 20} > {30,10,20}>{40, 20, 10} 
 	HoughLinesP( roi, lines, 1,1*CV_PI/180, 30, 20, 20 );
	/* the detected hough lines are for the roi image 
	hence need to add the offset y to the detected lines for displaying in the full image */ 
	int y_offset = im_edge.rows*0.6;
    for( size_t i = 0; i < lines.size(); i++ ){
	   bool skipFlagSlope=false;
	   lines[i][1] = lines[i][1] +y_offset;
	   lines[i][3] = lines[i][3] +y_offset;
        // calculated slope is with reference to origin at botton left, aka mathematical orignal
	   float x1 = lines[i][0], y1= lines[i][1], x2 = lines[i][2], y2= lines[i][3];
	   float slope = (y2-y1)/(x2-x1);
	   slope = tan(slope) * 180 / CV_PI;
	   //cout<< lines[i]<<slope<<endl;
	   // lane lines are close to +/- 45 degree; horizontal lane lines have slope~0
	   if(abs(abs(slope)-45)>15.0)
		  skipFlagSlope =true; 
	   Scalar colorR = Scalar(0,0,255);
	   // do not draw lines if any of the flags are set
	   if (!skipFlagSlope)
         line( out, Point(lines[i][0], lines[i][1] ),
            Point(lines[i][2], lines[i][3]), colorR, 2, 8 );
    	}
     }
};
// class LaneFinder ends 


int main(){
	VideoController vid;
	string vidname ="../../CarND-LaneLines-P1-master/solidWhiteRight.mp4";
	vid.setInputVideo(vidname);
	vid.setOutputVideoName("myOutputCarLanesVideo.avi");
	vid.setOutputWindowName("processed frame");
	LaneFinder LD;
	vid.setAlgorithmModel(&LD);
	vid.run();
 	char ch;
	cout<<"Input any key to quit";
	cin>>ch;
	return 0;
	
}
