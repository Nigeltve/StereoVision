#include "util.hpp"

using namespace std;
using namespace cv;


bool fexists(const string& filename){
	ifstream ifile(filename.c_str()); //make file object
	return (bool)ifile; // returns true if not NULL
}

//read in conf file
int readConf(string paramLook){
	//open the file to read
	ifstream inFile;
	inFile.open("Stereo.conf");
	
	//find the para
	string input, paramFound;
	if(inFile.is_open()){ //checks to see if opencv
		while(getline(inFile, input)){ //goes through whole file
			paramFound = input.substr(0, input.find(" ")); // find the right param
			
			if(paramFound == paramLook){ // if same as look up
				return atoi(input.substr(input.find(" ")+1, input.length()).c_str()); //return the value
			}
		}
		inFile.close(); //close when done
	}
	return -1; //returns -1 if nothing is found
}

void getDisplayImgParam(){
	//the paths to predefined images 
	string saveFileRight = "/home/pi/Desktop/Code/Thesis/images/right.jpg";
	string saveFileLeft = "/home/pi/Desktop/Code/Thesis/images/left.jpg";
	
	// stream from ip
	string ipVideoS = "http:/" + string("/192.168.10.20:8081");
	string ipVideoM = "http:/" + string("/192.168.10.10:8081");
	
	// sets up for live stream
	Mat rightFrame, leftFrame;
	while(true){
		//right stream
		VideoCapture rightStream(ipVideoS);
		rightStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		rightStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
			
			
		//left stream
		VideoCapture leftStream(ipVideoM);
		leftStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		leftStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		//puts stream into frames 
		rightStream >> rightFrame;
		leftStream >> leftFrame;
		
		//show what is being streamd
		imshow("rightWin", rightFrame);
		imshow("leftWin", leftFrame);
		
		//key options
		char c = waitKey(1000/readConf("FPS"));
		if(c =='q') exit(0); //Q quits program
		if(c =='s'){
			// S saves images at file path
			imwrite(saveFileRight, rightFrame);
			imwrite(saveFileLeft, leftFrame);
			break; // done and breaks out of function
		}
	}
}

Mat rightFrame, leftFrame, disp;

//all the variables sliders 
int dispar = 1, blckSz = 3, disp12 = 0, unqRtio = 3, spkSz =0;
int spkWinSz = 50, M = 0, prefilter = 0;

//max sizes for sliders 
int dispar_Max = 16, blckSz_Max = 10, disp12_Max = 3, prefilter_Max = 40;
int unqRtio_Max = 15, spkSz_Max = 20, spkWinSz_Max=200, M_Max= 3;


//object of Semi global block matching
static Ptr<StereoSGBM> stereo = StereoSGBM::create(-1*((16*dispar)/2), 16*dispar, blckSz);

void trackBarMode(int, void*){
	//sets parameters  sets new one for Mode
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//computes disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp); //shows updated version
}

void trackBarSpklWinSz(int, void *){
	//sets parameters  sets new one for speckle window size
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//computes disparity 
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackBarSpklRng(int, void*){
	//sets parameters  sets new one for speckle range
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//computes disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackbarPrefilter(int,void*){
	//sets parameters sets new one for prefilter
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz);
	stereo->setP1(64*rightFrame.channels()*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//computes stereo map
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackBarUnqRtio(int,void*){
	
	//sets parameters  sets new one for unique ratio
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//computes new disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackbardisp12(int, void*){
	
	//sets parameters  sets new one for disp12
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//updates disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackBarBkSz(int, void*){
	if(blckSz % 2 == 0) blckSz + 1; // block size can only be odd
	
	//sets parameters  sets new one for blockSize
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//updates disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
}

void trackBarDisp(int, void*){
	
	dispar = 16 * dispar; // can only be multiples of 16. 
	if(blckSz % 2 == 0) blckSz + 1; // blocksize can only be odd
	
	//sets parameters  sets new one for disparity
	stereo->setMinDisparity(-1*(dispar)/2);
	stereo->setNumDisparities(dispar);
	stereo->setBlockSize(blckSz);
	stereo->setP1(8*rightFrame.channels()*blckSz*blckSz);
	stereo->setP1(32*rightFrame.channels()*blckSz*blckSz);
	stereo->setDisp12MaxDiff(disp12);
	stereo->setPreFilterCap(prefilter);
	stereo->setUniquenessRatio(unqRtio);
	stereo->setSpeckleRange(spkSz);
	stereo->setSpeckleWindowSize(spkWinSz);
	stereo->setMode(M);
	
	//updates disparity
	stereo->compute(leftFrame,rightFrame,disp);
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	imshow("disp", disp);
	
}

void setParameters(){
	//make windows 
	namedWindow("rightWin");
	namedWindow("leftWin");
	namedWindow("disp");
	namedWindow("scales"); // used for sliders 
	
	//read in images 
	rightFrame = imread("/home/pi/Desktop/Code/Thesis/images/right.jpg", 1);
	leftFrame = imread("/home/pi/Desktop/Code/Thesis/images/left.jpg",1);
	
	// read in remap matrix
	Mat MR1, MR2, ML1, ML2;
	FileStorage fsi("/home/pi/Desktop/Code/Thesis/calibFiles/Calibration.yml", FileStorage::READ);
	fsi["MR1"] >> MR1;
	fsi["MR2"] >> MR2;
	fsi["ML1"] >> ML1;
	fsi["ML2"] >> ML2;
	
	//remaps
	remap(rightFrame, rightFrame, MR1, MR2, INTER_CUBIC);
	remap(leftFrame, leftFrame, ML1, ML2, INTER_CUBIC);
	
	//computes disparity
	stereo->compute(leftFrame,rightFrame,disp);
	
	//normalize to actually see results
	normalize(disp,disp,0, 255,NORM_MINMAX, CV_8U);
	
	// shows images 
	imshow("rightWin", rightFrame);
	imshow("leftWin", leftFrame);
	imshow("disp", disp);
	
	//create trackbar and sets limits to them (cant set steps :/)
	createTrackbar("disparity", "scales", &dispar, dispar_Max, trackBarDisp);
	setTrackbarMin("disparity", "scales",1);
	setTrackbarMax("disparity", "scales", dispar_Max);
	
	createTrackbar("blockSize", "scales", &blckSz, blckSz_Max, trackBarBkSz);
	setTrackbarMin("blockSize", "scales",2);
	setTrackbarMax("blockSize", "scales", blckSz_Max);
	
	createTrackbar("disp12", "scales", &disp12, disp12_Max, trackbardisp12);
	
	createTrackbar("prefilter", "scales", &prefilter, prefilter_Max, trackbarPrefilter);
	
	createTrackbar("unqRtio", "scales", &unqRtio, unqRtio_Max, trackBarUnqRtio);
	setTrackbarMin("unqRtio", "scales", 0);
	setTrackbarMax("unqRtio", "scales", unqRtio_Max);
	
	createTrackbar("spkrange", "scales", &spkSz, spkSz_Max, trackBarSpklRng);
	
	createTrackbar("spkWinRng", "scales", &spkWinSz, spkWinSz_Max, trackBarSpklWinSz);
	setTrackbarMin("spkWinRng", "scales", 20);
	setTrackbarMax("spkWinRng", "scales", spkSz_Max);
	
	createTrackbar("Mode", "scales", &M, M_Max, trackBarMode);
	
	//initilises bars 
	trackBarDisp(dispar,0);
	trackBarBkSz(blckSz,0);
	trackbardisp12(disp12,0);
	trackbarPrefilter(prefilter,0);
	trackBarUnqRtio(unqRtio,0);
	trackBarSpklRng(spkSz,0);
	trackBarSpklWinSz(spkWinSz,0);
	trackBarMode(M,0);
	
	//when key is pressed quites program
	waitKey(0);
	//writes image
	imwrite("/home/pi/Desktop/Code/Thesis/images/disp.jpg",disp);
	destroyAllWindows();
}
