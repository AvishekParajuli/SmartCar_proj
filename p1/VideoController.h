//VideoController.h
/*
The MIT License (MIT)
Copyright (c) 2016-2017 Avishek Parajuli.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation of the rights
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


#ifndef _VIDEOCONTROLLER_H_
#define  _VIDEOCONTROLLER_H_

#include "VideoController.h"

/*********************************
Define an interface class "Model" with a virtual function 
 This function will get called for processing the individual frames of the video
 The users is required to sub-class from this interface class that has defined 
 the Virtual function "process(cv:: Mat &input, cv:: Mat &output)".
 In this way the users can choose to apply different algorithm on the individual video frames.
**********************/
class Model {
public:
// processing method
virtual void process(cv:: Mat &input, cv:: Mat &output)= 0;
};

class VideoController
{
private:
	//define opencv video capture object, videoreader.
	cv::VideoCapture videoreader;
	//define video writer object
	cv::VideoWriter writer;
	/* define a pointer of type Model. 
	 This is to be provided by the user and the virtual function should have the following 		arguments list signature, i.e.(const cv::Mat & src, cv::Mat & dest) */
	Model * modelPtr;
	//define a property to hold the input video rate in frame per sec.
	int fps;
	//output window name for displaying the processed video output;default: "Output"
	std::string outputWindowName;

	//define output video name; The o/p video will be saved only if this property is specified.
	// The output video will be saved at the same fps, and same size as the input
	std::string outputVideoName;
	//define a boolean variable that is set for stoping the program
	bool stop;

	//derrived property delay(ms) =1000/fps;
	double delay;
	bool isVideoOpened;
	bool isWriterInitialized;
	// define a property that is used for storing frameprocessing times(in ms)
	std::vector<double> frameProcessingTime;
	//define a property used for intial testing purpose to process the first frame only.
	bool testFirstFrameOnly;
public:
	VideoController();
	~VideoController();
	void setInputVideo(std::string);
	void setOutputVideoName(std::string);
	void setOutputWindowName(std::string);
	void setAlgorithmModel( Model * m);
	void run();
//define private functions
private:
	
	bool isOutputVideoSaveReqd();
	void initWriter();
	
};

#endif
