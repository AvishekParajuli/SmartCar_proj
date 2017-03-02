# Project 1.0: Finding road lanes

## Overview
Do you feel the buzz of self-driving smart cars around? Are you interested to know some of the basic tasks and operations involved in smart cars such as lane detection and tracking, lane keeping and departure detection, vehicle/pedestrian detection and tracking,  road signs detection ...? I do not claim to be an expert, but i have acquired few knowledge in this field that i can share and i beleive that it would help some curious minds somwhere someday!
This is my attempt to give something back to the online community and share the knowledge, experience.

I plan to provide a tutorial on the basics of road lane detection. 
I would start with basic algorithm and move to complex alorithms for tackling challenging conditions.
I do plan to present one of the state of the art algorithms in this series where i will describe/present one of my previous work.
But for now let's get started with one of the basic algorithms. I will be using Opencv/C++ for performing these tasks.

## Acknowledgements
  A big thanks to Udacity and the self-driving car Nano-degree(CARND) program for those advertistment that created the vibe and inspired me to publish some of my previous work and attempt the CARND projects. I am not a CARND student but i would definitely like to give credit whenever it is due. Making these projects and challenges open source have must have helped a lot of alike minds. Kudos. The video data used herein is from udacity's CARND project.
  
  Secondly, i would like to thank my professor, Dr. Bryan Riley, for influencing and injecting the interest of self-driving smart cars into me. 

## Basic method description
The basic lane detection algorithm isn't that tough. Here are the basic steps

1.  Open video file or live camera
2.  Extract individual frames from the video and to get an image
3.  Convert the color image into grayscale (Feel free to try out different color spaces channels)
4.  Filter the image with gaussian filter
4.  Apply canny operator to detect edges(binarization)
5.  Apply hough transform to detect lines
6.  Process the detected lines to filter unwanted lines
7.  Draw the detected lane lines.

The opencv based code for C++ is in this repository *simple_main.cpp* for this operation. 
The central part is the function *processFrame*. Let us look it in detail.

First, it converts the input image *src* to a grayscale and applies the canny filter to extract the edge image.
``` cpp
cvtColor(src,img,CV_BGR2GRAY);
Canny(img, im_edge,100,300,3);
```
Since the video is taken from the camera dashboard, the image area of interest is usually in the lower half. we will define this as our region of interest and feed into the hough tranform algorithm.
``` cpp
  // Rect(x,y,w,h) w->width=cols;h->rows
	Mat roi= im_edge(Rect(0,im_edge.rows*0.6, im_edge.cols,im_edge.rows-im_edge.rows*0.6));
	vector<Vec4i> lines;
	// tune the houghline prameters
 	HoughLinesP( roi, lines, 1,1*CV_PI/180, 30, 20, 20 );
```
Now for each of these line vectors filter the lines that are irrelevant.
Let us step back and think which of these lines should be filtered out.Hmm,we can definitely filter the horizontal lines. These horizontal lines have slope~= 0.We will take a leaf out of our linear algebra classes and think of the correct lane lines w.r.t. heir slopes. we find that almost all lane lines have their slope ~ 45 degree. Bingo! now lets find the slope and filter unwanted lines.
``` cpp
	// lane lines are close to +/- 45 degree; horizontal lane lines have slope~0
	if(abs(abs(slope)-45)>15.0)
		skipFlagSlope =true; 
	Scalar colorR = Scalar(0,0,255);
	// do not draw lines if any of the flags are set
	 if (!skipFlagSlope)
         line( out, Point(lines[i][0], lines[i][1] ),
            Point(lines[i][2], lines[i][3]), colorR, 2, 8 );
    	}
```
We're done with the basic description. Feel free to try the code *main_simple.cpp*.

## OOAD
The above code is not architectured well. Meaning, if i have to do a different operation, say Vehicle detection, then i cannot re-use the code *main_simple.cpp* as it is. There are certains portions such as video read, and video write which can be re-used in many other appications. However, one would most likely end up either copying the file and then trying to decipher the code flow and then plug in their algorithm. Thats a pain and it will grow bigger with the increase in complexity.
Instead, lets look into the following diffferent files which have been well designed.

1. VideoController.cpp -> source code implementation details of video reading and writing operation. No need to change.
2. VideoController.h  -> Header file to be included to use so the video read write operation for free. No need to change.
3. main.cpp  -> user specific operation.

The *videoController.h* file defines an interface class "Model" with a virtual function.
This function is the one that will get called for processing the individual frames of the video.
The users is required to sub-class from this interface class that has pre-defined signature for the
the Virtual function i.e. "process(cv:: Mat &input, cv:: Mat &output)".
In this way the users can choose which algorithm they want to apply on the individual frames. They can change few parameters for the VideoController and will get the functionality for free without the need to know how it is working or changing it.
This COntroller woule be used in future series of tutorials as well.
Here is a snippet of main.
``` cpp
VideoController vid;
	string vidname ="../../CarND-LaneLines-P1-master/solidWhiteRight.mp4";
	vid.setInputVideo(vidname);
	vid.setOutputVideoName("myOutputCarLanesVideo.avi");
	vid.setOutputWindowName("processed frame");
  /* LaneFinder is a subclass of interface class which defines the process function
	LaneFinder LD;
	vid.setAlgorithmModel(&LD);
	vid.run();
 	
```

## summary
The resulting video is present in this repository. On my machine which is pretty average with intel corei5 and no dedicated GPU,  i did measure mean processing time of ~6ms for each image frame of the video *myOutputCarLanesVideo.avi*. For more complex algorithms, the mean processign time will definitely go higher. This is a basic algorithm and i know it will not yield good results in presence, high shadows, highly curvy roads, lane occlusions by vehicles, high dynamic range videos, and unfavourable weather conditions such as rain and snow. We will deal with them later in next series. But for now it is working great and fast on these input datasets. 
We have completed our first operation. Voila!

Good bye.


 was able to get 
