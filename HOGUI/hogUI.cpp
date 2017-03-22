//hogUI.cpp

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
//#include <fstream>
#include <ctime>

#define WINDOW_NAME "WINDOW"

#define	IMAGE_SIZE Size(40,40)

#define ImageWindowName "image"
#define HOGWindowName "HOG"
#define ShowHOGButton "ShowHOG"



using namespace cv;
using namespace cv::ml;
using namespace std;

struct mousedata{
	Mat im;
};
Mat ROI,IMG,IMOUT;

bool RESET = true;
int SLIDINGWINDOWSIZE= 40;
char MSG[50];
//string Instruction(" 1) Select color, 2) Left click on the image, 3) Press 'showHOG' \n 4) Repeat(1-3) OR press ESC to quit");
string Instruction(" select color, left click, showHOG");

Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size);

void readImage(string imgname, Mat & im);
void showHOG(int, void * );
void dispHOG(Mat &);
void mouseHandler(int event, int x, int y, int , void* );
void printHOGParams(HOGDescriptor & hog);

void winsizeCallbackFcn1(int  value, void *)
{
SLIDINGWINDOWSIZE = 40 + (value *8);
sprintf(MSG, "winsize= %d; %s ", SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
return;
}

void radioCallBack1(int, void *)// gray
{
//Mat bgr[3];   //destination array
//split(IMG,bgr);//split source
sprintf(MSG, "winsize= %d,  ImageType = GRAY;\n %s ", SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
cvtColor(IMG, IMOUT, COLOR_BGR2GRAY);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack2(int, void *)
{
Mat bgr[3];   //destination array
split(IMG,bgr);//split source
IMOUT = bgr[2];//R
sprintf(MSG, "winsize= %d,  ImageType = R;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack3(int, void *)
{
Mat bgr[3];   //destination array
split(IMG,bgr);//split source
IMOUT = bgr[1];//G
sprintf(MSG, "winsize= %d,  ImageType = G;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack4(int, void *)
{
Mat bgr[3];   //destination array
split(IMG,bgr);//split source
IMOUT = bgr[0];//B
sprintf(MSG, "winsize= %d,  ImageType = B;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}
void radioCallBackHSV(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2HSV);
IMOUT = temp;//HSV
sprintf(MSG, "winsize= %d,  ImageType = HSV;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}
void radioCallBackLUV(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2Luv);
IMOUT = temp;//HSV
sprintf(MSG, "winsize= %d,  ImageType = LUV;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack5(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2HSV);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[0];//H_HSV
sprintf(MSG, "winsize= %d,  ImageType = H(HSV) ;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack6(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2HSV);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[1];//S_HSV
sprintf(MSG, "winsize= %d,  ImageType = S(HSV) ;\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack7(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2HSV);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[2];//S_HSV
sprintf(MSG, "winsize= %d,  ImageType = V(HSV);\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack8(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2Luv);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[0];//L_Luv
sprintf(MSG, "winsize= %d,  ImageType = L(Luv);\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack9(int, void *)
{Mat temp;
cvtColor(IMG, temp,COLOR_BGR2Luv);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[1];//u_Luv
sprintf(MSG, "winsize= %d,  ImageType = u(Luv);\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

void radioCallBack10(int, void *)
{ Mat temp;
cvtColor(IMG, temp,COLOR_BGR2Luv);
Mat bgr[3];   //destination array
split(temp,bgr);//split source
IMOUT = bgr[2];//v_Luv
sprintf(MSG, "winsize= %d,  ImageType = v(Luv);\n %s",SLIDINGWINDOWSIZE,Instruction.c_str());
displayOverlay(ImageWindowName, MSG);
imshow(ImageWindowName, IMOUT); // Shows original image
return;
}

int main(int argc, char** argv)
{

	//test_it(IMAGE_SIZE);
	string imgname("../test_images/test3.jpg");
	if (argc>1)
		imgname = argv[1];

	Mat im;
	readImage(imgname, im);
	cv::resize(im,im, Size(640,480));
    namedWindow(ImageWindowName,WINDOW_AUTOSIZE);
    imshow(ImageWindowName,im);
    IMG =im.clone();
    IMOUT = IMG.clone();
    mousedata mdata;
	Mat im_temp = im.clone();//clone src so thatcircles can be drawn on temp img
	mdata.im = im_temp;
	setMouseCallback(ImageWindowName, mouseHandler, &mdata );
    //only one radiobox button can be selected for a given window/program
    createButton("GRAY",radioCallBack1,NULL,CV_RADIOBOX);
    createButton("R",radioCallBack2,NULL,CV_RADIOBOX);
    createButton("G",radioCallBack3,NULL,CV_RADIOBOX);
    createButton("B",radioCallBack4,NULL,CV_RADIOBOX);
    //creating trackbar on window name NULL will put it into control panel
    //create trackbar for sliding windowsize
    int ntimes=5;
    createTrackbar("winsize(40+8*Times)", NULL, &ntimes, 10, winsizeCallbackFcn1);
    createButton(ShowHOGButton, showHOG,NULL, CV_PUSH_BUTTON,1);
    createButton("HSV",radioCallBackHSV,NULL,CV_RADIOBOX);
    createButton("H_HSV",radioCallBack5,NULL,CV_RADIOBOX);
    createButton("S_HSV",radioCallBack6,NULL,CV_RADIOBOX);
    createButton("V_HSV",radioCallBack7,NULL,CV_RADIOBOX);
    createButton("LUV",radioCallBackLUV,NULL,CV_RADIOBOX);
    createButton("L_LUV",radioCallBack8,NULL,CV_RADIOBOX);
    createButton("U_LUV",radioCallBack9,NULL,CV_RADIOBOX);
    createButton("V_LUV",radioCallBack10,NULL,CV_RADIOBOX);


    imshow(ImageWindowName,im);
    while(1)
    {   // wait infinitely until the users hit escape key.
          int key=waitKey(0);
          if(key==27) break;
    }
	return 0;
}
void dispHOG(Mat &img)
{

    HOGDescriptor hog;

	hog.winSize = Size(SLIDINGWINDOWSIZE,SLIDINGWINDOWSIZE);//IMAGE_SIZE;//Size(64,64);
	Mat gray;
    vector< float > descriptors;
    //cvtColor(img, gray, COLOR_BGR2GRAY);
	hog.compute(IMOUT, descriptors,Size(8, 8), Size(0, 0));
	printHOGParams(hog);
    imshow(HOGWindowName, get_hogdescriptor_visu(img.clone(), descriptors, hog.winSize));
	waitKey(10);
}

void printHOGParams(HOGDescriptor &hog )
{
cout<<
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

void mouseHandler(int event, int x, int y, int flags, void* data_ptr)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {   mousedata *data = ((mousedata *) data_ptr);
        if (RESET)
        {
            /*Mat temp = data->im.clone();
            circle(temp, Point(x,y),3,Scalar(0,255,255), 2, CV_AA);
            rectangle(temp,Rect(x-SLIDINGWINDOWSIZE/2-1,y-SLIDINGWINDOWSIZE/2-1,
                    SLIDINGWINDOWSIZE+2,SLIDINGWINDOWSIZE+2),Scalar(255,0,0), 1);//draw rectangle on outer edge
            imshow(ImageWindowName, temp);
            ROI = data->im(Rect(x-SLIDINGWINDOWSIZE/2,y-SLIDINGWINDOWSIZE/2,SLIDINGWINDOWSIZE,SLIDINGWINDOWSIZE));
            */
            Mat temp = IMOUT.clone();
            circle(temp, Point(x,y),3,Scalar(0,255,255), 2, CV_AA);
            rectangle(temp,Rect(x-SLIDINGWINDOWSIZE/2-1,y-SLIDINGWINDOWSIZE/2-1,
                    SLIDINGWINDOWSIZE+2,SLIDINGWINDOWSIZE+2),Scalar(255,0,0), 1);//draw rectangle on outer edge
            imshow(ImageWindowName, temp);
            ROI = IMOUT(Rect(x-SLIDINGWINDOWSIZE/2,y-SLIDINGWINDOWSIZE/2,SLIDINGWINDOWSIZE,SLIDINGWINDOWSIZE));

            namedWindow(HOGWindowName);
            imshow(HOGWindowName,ROI);
            RESET =false;
        }
    }
}
void showHOG(int, void * )
{
	std::cout<<"***************** pushbutton ->'ShowHOG' pressed **************** "<<endl;
	RESET = true;
	dispHOG(ROI);
}
void readImage(string imgname, Mat & im){

	im = imread(imgname.c_str(), IMREAD_COLOR);
	if (im.empty())
	{ cout<<" Invalid image read, imgname =argv[1] = "<<imgname<<endl;
	 CV_Assert(false);
	}
	cout<<"****** successfully image read, imgname = "<<imgname<<endl;
}

// From http://www.juergenwiki.de/work/wiki/doku.php?id=public:hog_descriptor_computation_and_visualization
Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size)
{
	const int DIMX = size.width;
	const int DIMY = size.height;
	float zoomFac = 3;
	Mat visu;
	resize(color_origImg, visu, Size((int)(color_origImg.cols*zoomFac), (int)(color_origImg.rows*zoomFac)));

	int cellSize = 8;
	int gradientBinSize = 9;
	float radRangeForOneBin = (float)(CV_PI / (float)gradientBinSize); // dividing 180° into 9 bins, how large (in rad) is one bin?

    // prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = DIMX / cellSize;
	int cells_in_y_dir = DIMY / cellSize;
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;

			for (int bin = 0; bin<gradientBinSize; bin++)
				gradientStrengths[y][x][bin] = 0.0;
		}
	}

	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;

	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;

	for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr = 0; cellNr<4; cellNr++)
			{
				// compute corresponding cell nr
				cellx = blockx;
				celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}

				for (int bin = 0; bin<gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;

					gradientStrengths[celly][cellx][bin] += gradientStrength;

				} // for (all bins)


				  // note: overlapping blocks lead to multiple updates of this sum!
				  // we therefore keep track how often a cell was updated,
				  // to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;

			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)


	  // compute average gradient strengths
	for (celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (cellx = 0; cellx<cells_in_x_dir; cellx++)
		{

			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

			// compute average gradient strenghts for each gradient bin direction
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}

	// draw cells
	for (celly = 0; celly<cells_in_y_dir; celly++)
	{
		for (cellx = 0; cellx<cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize;
			int drawY = celly * cellSize;

			int mx = drawX + cellSize / 2;
			int my = drawY + cellSize / 2;

			rectangle(visu, Point((int)(drawX*zoomFac), (int)(drawY*zoomFac)), Point((int)((drawX + cellSize)*zoomFac), (int)((drawY + cellSize)*zoomFac)), Scalar(100, 100, 100), 1);

			// draw in each cell all 9 gradient strengths
			for (int bin = 0; bin<gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];

				// no line to draw?
				if (currentGradStrength == 0)
					continue;

				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = (float)(cellSize / 2.f);
				float scale = 2.5; // just a visualization scale, to see the lines better

								   // compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

				// draw gradient visualization
				line(visu, Point((int)(x1*zoomFac), (int)(y1*zoomFac)), Point((int)(x2*zoomFac), (int)(y2*zoomFac)), Scalar(0, 255, 0), 1);

			} // for (all bins)

		} // for (cellx)
	} // for (celly)


	  // don't forget to free memory allocated by helper data structures!
	for (int y = 0; y<cells_in_y_dir; y++)
	{
		for (int x = 0; x<cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;

	return visu;

} // get_hogdescriptor_visu
