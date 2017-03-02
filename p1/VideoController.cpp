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

// VideoController.cpp

#include <opencv2/opencv.hpp>
#include<iostream>
#include "VideoController.h"

using namespace std;
using namespace cv;
/*******List of properties for reference ***************
	cv::VideoCapture videoreader;
	cv::VideoWriter writer;
	Model *modelPtr;
	int fps;
	std::string outputWindowName;
	std::string outputVideoName;
	bool stop;
	double delay;
	bool isVideoOpened;
	bool isWriterInitialized;
	std::vector<double> frameProcessingTime;
	bool testFirstFrameOnly;
****************/

VideoController::VideoController():delay(0),stop(false),outputWindowName("Output"),
				isWriterInitialized(false),isVideoOpened(false){}
VideoController:: ~VideoController(){
	// Close the video file.
	writer.release();
	videoreader.release();
}

void VideoController::setInputVideo(std::string ipVideoName){
	// It is important to release the any previous instances of class videoCapture 
	videoreader.release();
	videoreader.open(ipVideoName.c_str());
	// check if video is opened successfully
	if (!videoreader.isOpened()){
		cout<<" Invalid video file read "<<endl; 
		videoreader.release();
	 	CV_Assert(false);
	}
	// we are here only if the video is succesfully opened
	isVideoOpened = true;
	fps = videoreader.get(CV_CAP_PROP_FPS);
}

void VideoController::setOutputVideoName(std::string name){
	outputVideoName = name;
}

void VideoController::setOutputWindowName(std::string name){
	outputWindowName = name;
}

void VideoController:: setAlgorithmModel( Model * m){
	modelPtr = m;
}

bool VideoController::isOutputVideoSaveReqd(){
	// return true only if the outputWindowName is specified
	return(!outputVideoName.empty());
}

void VideoController::initWriter(){
	if (isOutputVideoSaveReqd()){
	    // release any previous instance of writer object
	    writer.release();
	    int codec = static_cast<int>(videoreader.get(CV_CAP_PROP_FOURCC));
	    Size sz = Size(videoreader.get(CV_CAP_PROP_FRAME_WIDTH),
			  videoreader.get(CV_CAP_PROP_FRAME_HEIGHT));
	    writer.open(outputVideoName.c_str(), codec,fps,sz, true);
	    if (!writer.isOpened()){
		cout<<" Error while calling the cv::VideoWriter.open("<<outputVideoName<<")"<<endl; 
		writer.release();
	 	CV_Assert(false);
	    }
	   isWriterInitialized= true;
	}
}

void VideoController::run(){
	// assert if the video is not opened.
	CV_Assert(isVideoOpened);
	// define Mat for currentFrame and outFrame
	Mat currentFrame, outputFrame;
	namedWindow(outputWindowName);
	// initialize the videowriter object; this will set/unset 'isWriterInitialized'
	initWriter();
	// delay in ms
	int delay = 1000/fps; 
	// read each frame in video
	while (!stop) {
		// read next frame
		if (!videoreader.read(currentFrame))
			break;
		double initialTime= (double)getTickCount();
		// call the function pointer
			modelPtr->process(currentFrame, outputFrame);
	double frameProcessTime = (((double)getTickCount() - initialTime)/getTickFrequency())*1000;
		frameProcessingTime.push_back(frameProcessTime);
		imshow(outputWindowName,outputFrame);
		//write only if the writer is initialized
		if (isWriterInitialized)
			writer.write(outputFrame);
		//get elasped time in ms since the video frame read
		double elaspedTime= (((double)getTickCount() - initialTime)/getTickFrequency())*1000;
		// find the remaining delay as the framew processing+imshow+write took elasped time
		double remainingTime = delay -(elaspedTime); 
		if (remainingTime) 
			// if positive wait for the remaining time
		   waitKey(remainingTime);		
		else
		   waitKey(1);// delay for 1 ms if elaspedtime>delay
	}
	Scalar m = mean(Mat(frameProcessingTime));
	cout<<endl<<" mean frame processing time "<<sum(sum(m))<<endl;
	writer.release();
	videoreader.release();
}
