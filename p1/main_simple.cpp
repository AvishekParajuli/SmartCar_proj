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

// main_simple.cpp

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>


using namespace std;
using namespace cv;


void testImage(string imgname);
void testVideo(string vidname);
void processFrame(Mat &, Mat &);
float calcSlope( Vec4i );

int main(int argc, char ** argv)
{
	//solidWhiteRight.jpg
	//solidYellowLeft.jpg
	//whiteCarLaneSwitch.jpg
	//solidWhiteCurve.jpg
	//solidYellowCurve2.jpg
	//solidYellowCurve.jpg
	string imgname("../CarND-LaneLines-P1-master/test_images/whiteCarLaneSwitch.jpg");
	//testImage(imgname);	
	/* list of video files
        1. solidWhiteRight.mp4
	2. solidYellowLeft.mp4
	3. challenge.mp4 */
	string vidname ="../CarND-LaneLines-P1-master/solidWhiteRight.mp4";
	testVideo(vidname);
	waitKey(0);
	return 0;
}

void testImage(string imgname){	
	cout<<" testing image : "<<imgname<<endl;
	Mat im = imread(imgname.c_str());
	if (im.empty()){
	 cout<<" Invalid image read "<<endl; 
	 CV_Assert(false);
	}
	
	namedWindow("detected lanes",WINDOW_AUTOSIZE);
	moveWindow("detected lanes",0,0);
	Mat out;
	processFrame(im, out);
	imshow("detected lanes",out);
	
}
void processFrame(Mat & src, Mat & out){
	Mat img;
	out= src.clone();
	cvtColor(src,img,CV_BGR2GRAY);
	Mat im_edge(img.size(), CV_8U, Scalar::all(0));
	Canny(img, im_edge,100,300,3);
	//imshow("canny",im_edge);
	// Rect(x,y,w,h) w->width=cols;h->rows
	Mat roi= im_edge(Rect(0,im_edge.rows*0.6, im_edge.cols,im_edge.rows-im_edge.rows*0.6));
	
	vector<Vec4i> lines;
	// tune the houghline prameters
 	HoughLinesP( roi, lines, 1,1*CV_PI/180, 30, 20, 20 );
	/* the detected hough lines are for the roi image 
	hence need to add the offset y to the detected lines for displaying in the full image */ 
	int y_offset = im_edge.rows*0.6;

    	for( size_t i = 0; i < lines.size(); i++ )
    	{
	bool skipFlagSlope=false;
	lines[i][1] = lines[i][1] +y_offset;
	lines[i][3] = lines[i][3] +y_offset;
// calculated slope is with reference to origin at botton left, aka mathematical orignal
	float slope =calcSlope(lines[i]);
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

float calcSlope( Vec4i lines){
	//retunr the slope in degrees; input is Vec4i (x1,y1,x2,y2)
	float x1 = lines[0], y1= lines[1], x2 = lines[2], y2= lines[3];
	float slope = (y2-y1)/(x2-x1);
	return tan(slope) * 180 / CV_PI;
}

void testVideo(string vidname){
	
	cv::VideoCapture reader(vidname.c_str());
	// assert if the video is not opened.
	if (!reader.isOpened()){
		cout<<" Invalid video file read "<<endl; 
	 CV_Assert(false);
	}
	// Get the frame rate
	double fps = reader.get(CV_CAP_PROP_FPS);
	cout<<" Frame rate : "<< fps<<endl;
	cout<<" Input video codec :"<<reader.get(CV_CAP_PROP_FOURCC);	
	// initilaize the video writer object to write the video output
	std::string outputFile("out.avi");
	VideoWriter writer;
	int codec = static_cast<int>(reader.get(CV_CAP_PROP_FOURCC));
	//int codec = CV_FOURCC('M', 'J', 'P', 'G'); 
	bool isWriterInitialized=false;
	bool stop(false);
	cv::Mat frame,out; // current video frame
	//cv::namedWindow("Extracted Frame");
	cv::namedWindow("processed frame");
	// Delay between each frame in ms
	// corresponds to video frame rate
	int delay= 1000/rate;
	std::vector<double> frameProcessingTime;
	// for all frames in video
	while (!stop) {
		// read next frame if any
		if (!reader.read(frame))
			break;
		//imshow("Extracted Frame",frame);
		if (!isWriterInitialized){
		//execute only once
			isWriterInitialized=true;
		/*writer.open(outputFile, 
		capture.get(CV_CAP_PROP_FOURCC),
        	capture.get(CV_CAP_PROP_FPS),
        	Size(capture.get(CV_CAP_PROP_FRAME_WIDTH),capture.get(CV_CAP_PROP_FRAME_HEIGHT)),
		true);*/
		writer.open(outputFile, codec,fps,frame.size(), true);
		}
		double initialTime= (double)getTickCount();
		processFrame(frame, out);
		cv::imshow("processed frame",out);
		writer.write(out);
		double elaspedTime= (((double)getTickCount() - initialTime)/getTickFrequency())*1000;
		frameProcessingTime.push_back(elaspedTime);
		double remainingTime = delay -(elaspedTime); //converting all time to ms
		//cout<<"Remaining time : "<<remainingTime<<endl;
		if (remainingTime) 
		// if positive wait for the remaining time
			if (cv::waitKey(remainingTime)>=0)
				stop= true;		
		else
		// delay for 1 ms and wait for any key to stop
			if (cv::waitKey(1)>=0)
				stop= true;
		
	}
	//cout<<" instances of all elaspedTimes"<<endl<<Mat(frameProcessingTime);
	Scalar m = mean(Mat(frameProcessingTime));
	cout<<endl<<" mean processing time "<<sum(sum(m))<<endl;
	// Close the video file.
	// Not required since called by destructor
	writer.release();
	reader.release();
}

