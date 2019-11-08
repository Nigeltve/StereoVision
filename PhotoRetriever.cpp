#include "PhotoRetriever.hpp"
#include "util.hpp"
using namespace std;
using namespace cv;

//global varables
const string PATH = "/home/pi/Desktop/Code/Thesis/calib_photos/";
string ipVideo = "http:/" + string("/192.168.10.20:8081");
string ipVideoM = "http:/" + string("/192.168.10.10:8081");

void saveFileStereo(cv::Mat& rightFrame, cv::Mat& leftFrame, int i){
	//saves images from stream and saves them
	printf("saving images %d\n", i);
	string rightPhotoName = PATH + "rightBoard/right" + std::to_string(i) + ".jpg";
	string leftPhotoName = PATH + "leftBoard/left" + std::to_string(i) + ".jpg";
	imwrite(rightPhotoName, rightFrame);
	imwrite(leftPhotoName, leftFrame);
	printf("saved\n");
	
}


void vid(int i){
	Mat rightFrame, leftFrame;
	namedWindow("rightWin",CV_WINDOW_AUTOSIZE);
	namedWindow("leftWin",CV_WINDOW_AUTOSIZE);
	
	while(true){
		// grab images froms stream at the same time 
		VideoCapture rightStream(ipVideo);
		rightStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		rightStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		VideoCapture leftStream(ipVideoM);
		leftStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		leftStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		rightStream >> rightFrame;
		leftStream >> leftFrame;
		
		//displays stream
		imshow("rightWin", rightFrame);
		imshow("leftWin", leftFrame);
		
		// options
		char c = waitKey(1000/readConf("FPS"));
		if(c == 'q') exit(0); //exits
		if(c == 'c') {
			//saves photo
			saveFileStereo(rightFrame, leftFrame, i);
			break;	
		}
		
	}
}

void takePhoto(int numberOfPhotos){
	//goes through the number of photos user wants
	for(int i = 0; i < numberOfPhotos; i++){
		vid(i);
	}
}

