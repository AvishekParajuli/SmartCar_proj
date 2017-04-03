//detectcar.cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

#include <unistd.h>
#include <memory>

// the util/imageutils.h is used for display, debugging and profiling purposes only
//#include "../../../../util/imageutils.h"

#define WINDOW_NAME "WINDOW"
//string vidname =".solidWhiteRight.mp4" or solidYellowLeft.mp4 or challenge.mp4
//#define TRAFFIC_VIDEO_FILE "../../CarND-LaneLines-P1-master/solidYellowLeft.mp4"
#define TRAFFIC_VIDEO_FILE "../project_video.mp4"
#define TRAINED_SVM "vehicle_detector.yml"
#define	IMAGE_SIZE Size(40,40)
#define save_video true
#define OUT_Video_File "march30_project_video.avi"

using namespace cv;
using namespace cv::ml;
using namespace std;

bool file_exists(const string &file);

void draw_locations(Mat & img, const vector< Rect > & locations, const Scalar & color);

void readImage(string imgname, Mat & im);
void test_image(Mat & img, const Size & size);
void test_video(const Size & size);
bool checkIfpatchIsVehicle(Mat & img2check);

void printHOGParams(HOGDescriptor &hog )
{
cout << "HOG descriptor size is " <<  hog.getDescriptorSize() << endl;
cout << "hog.windowSize: " << hog.winSize <<endl;
cout<< " cellsize " << hog.cellSize << endl;
cout<< " hog.nbins " << hog.nbins << endl;
cout<< " blockSize " << hog.blockSize << endl;
cout<< " blockStride " << hog.blockStride << endl;
cout<< " hog.nlevels " << hog.nlevels<< endl;
cout<< " hog.winSigma " << hog.winSigma<< endl;
cout<< " hog.free_coef  " << hog.free_coef << endl;
cout<< " hog.DEFAULT_NLEVELS " << hog.DEFAULT_NLEVELS << endl;

}


int main(int argc, char** argv)
{

	/*//test_it(IMAGE_SIZE);
	string imgname("../test_images/test3.jpg");
	if (argc>1)
		imgname = argv[1];

	Mat im;
	readImage(imgname, im);
	test_image(im,IMAGE_SIZE);
*/
    test_video(IMAGE_SIZE);
	return 0;
}
void readImage(string imgname, Mat & im){

	im = imread(imgname.c_str(), IMREAD_COLOR);
	if (im.empty())
	{ cout<<" Invalid image read, imgname =argv[1] = "<<imgname<<endl;
	 CV_Assert(false);
	}
	//cout<<"****** successfully image read, imgname = "<<imgname<<endl;
}

bool file_exists(const string &file)
{
	return access(file.c_str(), 0) == 0;
}




void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector)
{
	// get the support vectors
	Mat sv = svm->getSupportVectors();
	const int sv_total = sv.rows;
	// get the decision function
	Mat alpha, svidx;
	double rho = svm->getDecisionFunction(0, alpha, svidx);
    cout<<"alpha = "<<alpha<<endl;
	CV_Assert(alpha.total() == 1 && svidx.total() == 1 && sv_total == 1);
	CV_Assert((alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
		(alpha.type() == CV_32F && alpha.at<float>(0) == 1.f));
	CV_Assert(sv.type() == CV_32F);
	hog_detector.clear();

	hog_detector.resize(sv.cols + 1);
	memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
	hog_detector[sv.cols] = (float)-rho;
}

void draw_locations(Mat & img, const vector< Rect > & locations, const Scalar & color)
{
	if (!locations.empty())
	{
		vector< Rect >::const_iterator loc = locations.begin();
		vector< Rect >::const_iterator end = locations.end();
		for (; loc != end; ++loc)
		{
			rectangle(img, *loc, color, 2);
		}
	}
}

void test_video(const Size & size)
{
	char key = 27;
	Mat img, draw;
	Ptr<SVM> svm;
	HOGDescriptor hog;
	hog.winSize = size;
	vector< Rect > locations;

	// Load the trained SVM.
	svm = StatModel::load<SVM>(TRAINED_SVM);
	// Set the trained svm to my_hog
	vector< float > hog_detector;
	get_svm_detector(svm, hog_detector);
	hog.setSVMDetector(hog_detector);
	printHOGParams(hog);

	VideoCapture video;
	// Open the video file.
	video.open(TRAFFIC_VIDEO_FILE);
	if (!video.isOpened())
	{
		cerr << "Unable to open the device" << endl;
		exit(-1);
	}
        // Get the frame rate
	double rate= video.get(CV_CAP_PROP_FPS);
	cout<<" Frame rate : "<< rate<<endl;
	cout<<" Input video codec :"<<video.get(CV_CAP_PROP_FOURCC);
	// initilaize the video writer object to write the video output
	std::string outputFile(OUT_Video_File);
	VideoWriter writer;
	int codec = static_cast<int>(video.get(CV_CAP_PROP_FOURCC));
	//int codec = CV_FOURCC('M', 'J', 'P', 'G');
	bool isWriterInitialized=false;

	int num_of_vehicles = 0;
	bool end_of_process = false;
	while (!end_of_process)
	{
        video >> img;
		if (img.empty())
			break;


		draw = img.clone();
        Mat cropped;
        cv::resize(draw,cropped, Size(720,560));

        Mat temp,temp3;
        cvtColor(cropped, temp, COLOR_BGR2GRAY);
        /*Mat bgr[3];   //destination array
        split(temp3,bgr);//split source
        temp = bgr[0]+bgr[2];
        */
        if (!isWriterInitialized){
		//execute only once
			isWriterInitialized=true;
		/*writer.open(outputFile,
		capture.get(CV_CAP_PROP_FOURCC),
        	capture.get(CV_CAP_PROP_FPS),
        	Size(capture.get(CV_CAP_PROP_FRAME_WIDTH),capture.get(CV_CAP_PROP_FRAME_HEIGHT)),
		true);*/
		writer.open(outputFile, codec,rate,cropped.size(), true);
		}


		locations.clear();
		// Rect(x,y,w,h) w->width=cols;h->rows
		// first remove the upper 50% from height  Original Cropped =size(720,560)=(cols,rows)
		Mat roi = temp(Rect(0,temp.rows*0.5, temp.cols,temp.rows-temp.rows*0.5));
		//size(roi) = size(720,280)
		//cout<<"roi.size() = "<<roi.size()<<endl;
		int y_offset = temp.rows*0.5;
		//again crop the lower 10 % to remove the images near dashboard-> remove false positives
        roi = roi(Rect(0,0, roi.cols,roi.rows-100));
        //cout<<"roi.size() = "<<roi.size()<<endl;
        //no offset required as this is the lower row colums.

		//hog.detectMultiScale(roi, locations);
		hog.detectMultiScale(roi, locations, 0.00, Size(4,8), Size(0,0), 1.05, 2);//less false positive
		//hog.detectMultiScale(roi, locations, 0.00, Size(8,8), Size(0,0), 1.05, 2);// less true negative(missed)
		// add the offset
		std::vector<Rect>::iterator it = locations.begin();
		std::vector<Rect>::iterator itend = locations.end();
		vector<Rect> actuallocations;
		bool isVehicle= false;
		for (;it!=itend;it++)
		  {
            Rect current = *it;
            //cout<<" Rect current = "<< current<<endl;
            //cout<<" roi size= "<<roi.size()<<endl;
            Mat roi2Check = roi(Rect(current.x,current.y,current.width,current.height));//define a roi of 50x50
            //cout<<" roi2Check size= "<<roi2Check.size()<<endl;
            isVehicle = checkIfpatchIsVehicle(roi2Check);
            if (isVehicle)
                actuallocations.push_back(Rect(current.x, current.y+y_offset,current.width,current.height));
		  }
		   if (0!=actuallocations.size())
            draw_locations(cropped, actuallocations, Scalar(0, 255, 0));

		imshow(WINDOW_NAME, cropped);
		if (save_video)
            writer.write(cropped);
		//wait  infinite fro keypress
		key = (char)waitKey(10);
		if (27 == key)
			end_of_process = true;
    }
    // Close the video file.
	// Not required since called by destructor
	writer.release();
	video.release();
}

void test_image(Mat & img, const Size & size)
{
	char key = 27;
	Mat draw;
	Ptr<SVM> svm;
	HOGDescriptor hog;
	hog.winSize = size;
	vector< Rect > locations;


	// Load the trained SVM.
	svm = StatModel::load<SVM>(TRAINED_SVM);
	// Set the trained svm to my_hog
	vector< float > hog_detector;
	get_svm_detector(svm, hog_detector);
	hog.setSVMDetector(hog_detector);
	int num_of_vehicles = 0;
	bool end_of_process = false;
	while (!end_of_process)
	{

		draw = img.clone();
        Mat cropped;
        cv::resize(draw,cropped, Size(720,560));

		/*// Eliminate ingoing traffic
		for (int pi = 0; pi < img.rows; ++pi)
			for (int pj = 0; pj < img.cols; ++pj)
				if (pj > img.cols / 2) {
					img.at<Vec3b>(pi, pj)[0] = 0;
					img.at<Vec3b>(pi, pj)[1] = 0;
					img.at<Vec3b>(pi, pj)[2] = 0;
				}
		*/
		locations.clear();
		// Rect(x,y,w,h) w->width=cols;h->rows
		// first remove the upper 50% from height  Original Cropped =size(720,560)=(cols,rows)
		Mat roi = cropped(Rect(0,cropped.rows*0.5, cropped.cols,cropped.rows-cropped.rows*0.5));
		//size(roi) = size(720,280)
		cout<<"roi.size() = "<<roi.size()<<endl;
		int y_offset = cropped.rows*0.5;
		//again crop the lower 10 % to remove the images near dashboard-> remove false positives
        roi = roi(Rect(0,0, roi.cols,roi.rows-80));
        cout<<"roi.size() = "<<roi.size()<<endl;
        //no offset required as this is the lower row colums.

		//hog.detectMultiScale(roi, locations);
		hog.detectMultiScale(roi, locations, 0.00, Size(4,4), Size(0,0), 1.05, 2);
		// add the offset
		std::vector<Rect>::iterator it = locations.begin();
		std::vector<Rect>::iterator itend = locations.end();
		vector<Rect> actuallocations;
		bool isVehicle;
		for (;it!=itend;it++)
		  {
            Rect current = *it;
            Mat roi2Check = roi(Rect(current.x,current.y,50,50));//define a roi of 50x50
            isVehicle = checkIfpatchIsVehicle(roi2Check);
            if (isVehicle)
                actuallocations.push_back(Rect(current.x, current.y+y_offset,current.width,current.height));
		  }
		  if (0!=actuallocations.size())
            draw_locations(cropped, actuallocations, Scalar(0, 255, 0));
		//draw_locations(roi,locations, Scalar(0, 255, 0));
        //Mat temp (roi, locations[0] );
		//dispHOG(temp);

		/*for_each(Rect r in locations) {

			// Center point of the vehicle
			Point center(r.x + r.width / 2, r.y + r.height / 2);

			if (abs(center.y - img.rows * 2 / 3) < 2) {
				++num_of_vehicles;
				line(draw, Point(0, img.rows * 2 / 3), Point(img.cols / 2, img.rows * 2 / 3), Scalar(0, 255, 0), 3);
				imshow(WINDOW_NAME, draw);
				waitKey(50);
			}
			else
				line(draw, Point(0, img.rows * 2 / 3), Point(img.cols / 2, img.rows * 2 / 3), Scalar(0, 0, 255), 3);

		} */

		//putText(draw, "Detected vehicles: " + to_string(num_of_vehicles), Point(50, 50), 1, 1, Scalar(0, 0, 255), 2);

		imshow(WINDOW_NAME, cropped);
		waitKey();
		key = (char)waitKey(10);
		if (27 == key)
			end_of_process = true;
    }
}


bool checkIfpatchIsVehicle(Mat & img2check)
{
//cout<<"******inside function checkIfPatchIsVehicle "<<endl;
bool isVehicle =false;
//util::imdisp(img2check,"im2check");
vector<string> templateList;
templateList.push_back("temp1.png");templateList.push_back("temp2.png");
templateList.push_back("temp3.png");templateList.push_back("temp4.png");
templateList.push_back("temp5.png");templateList.push_back("temp6.png");
templateList.push_back("temp7.png");
//templateList.push_back("temp8.png");
//templateList.push_back("temp9.png");templateList.push_back("temp10.png");

Mat matchScore = Mat::zeros(7,1,CV_32FC1);

for (int ii=0; ii<templateList.size(); ii++){
    Mat result;

    string imgname = templateList.at(ii);
    Mat templ,img2check_copy;
    readImage(imgname, templ);
    cvtColor(templ,templ,COLOR_BGR2GRAY);
    cv::resize(img2check,img2check, Size(50,50));
    cv::resize(templ,templ, Size(50,50));
    int result_cols =  img2check.cols - templ.cols + 1;
    int result_rows = img2check.rows - templ.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );

	Canny(img2check,img2check_copy, 50,150);

	//util::imdisp(img2check_copy,"canny");


  /// Do the Matching and Normalize
  matchTemplate(img2check_copy, templ, result, CV_TM_CCORR_NORMED );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
  matchScore.at<float>(ii,0)= maxVal;
  if (maxVal>0.15)
    isVehicle =true;
  }
  //cout<<"MatchScore = "<<endl<<matchScore<<endl;
    float total =sum( matchScore )[0];
  cout << "sum(Matchscore) = " << cv::sum( matchScore )[0] << std::endl;
    if (total>0.13*templateList.size())
        isVehicle =true;
    //waitKey();
    return isVehicle;
}
