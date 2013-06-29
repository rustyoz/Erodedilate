
#include <string>
#include <sstream>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <chrono>


using namespace cv;
using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

int DILATE_MAX = 40;
int ERODE_MAX = 30;



void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed

	if(DILATE_MAX == 0) DILATE_MAX = 1;
	if(ERODE_MAX == 0) ERODE_MAX = 1;

}

void createTrackbars(){
	//create window for trackbars

	
    namedWindow("Trackbars",CV_WINDOW_AUTOSIZE);
	createTrackbar( "Erode", "Trackbars", &ERODE_MAX, ERODE_MAX, on_trackbar );
	createTrackbar("Dilate", "Trackbars", &DILATE_MAX, DILATE_MAX, on_trackbar );
}



int main( int argc, char** argv )
{
	int counter= 0;
	time_t start, end;
	Mat frames[5];
	Mat frame;
	VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	if(!capture.isOpened()) return -1;
	createTrackbars();
	namedWindow("combination", CV_WINDOW_KEEPRATIO);
	

	while(1)
	{
		auto start = chrono::high_resolution_clock::now();
		cout.clear();
		
		capture.read(frame);
		Mat erodeElement = getStructuringElement( MORPH_RECT,Size(ERODE_MAX,ERODE_MAX));
		Mat dilateElement = getStructuringElement( MORPH_RECT,Size(DILATE_MAX,DILATE_MAX));
		erode(frame, frames[0],erodeElement);
		dilate(frames[0],frames[1],dilateElement);
		/*
		imshow("Raw", frame);
		imshow("Erode", frames[0]);
		imshow("Dilate", frames[1]);
		*/

		Mat combination;
		hconcat(frame, frames[0], combination);
		hconcat(combination, frames[1], combination);
		
		waitKey(30);
		time(&end);
		counter++;
		auto end = chrono::high_resolution_clock::now();
		auto diff = end - start;
		float fps = 1000.0/chrono::duration_cast<chrono::milliseconds>(end - start).count();
		ostringstream fps_stringstream;
		fps_stringstream << "FPS: " << fps;
		putText(combination, fps_stringstream.str(), Point(10, 30), 
			CV_FONT_HERSHEY_PLAIN, 1,  CV_RGB(255, 255, 255));

		imshow("combination", combination);
		//cout << "FPS: " << fps <<endl;
	}


	                                          // Wait for a keystroke in the window
    return 0;
}