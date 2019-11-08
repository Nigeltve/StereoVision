#include "Stereo.hpp"
#include "util.hpp"

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

int count_num = 0;

void savePhoto(Mat& imgleft, Mat& imgRight ,Mat& disp, vector<double> timings, int disp_num, int mode, string type){
	//sets the name for the file to be saved
	string dir = "images/results/";
	string name = type + "_D" + to_string(disp_num) + "_M" + to_string(mode) +"_"+ to_string(count_num);
	
	//writes the iamges to that path
	imwrite(dir + "SGBM_img_right_"+ name + ".jpg", imgRight);
	imwrite(dir + "SGBM_img_left_"+ name + ".jpg", imgleft);
	imwrite(dir + "SGBM_img_disp_"+ name + ".jpg", disp);
	
	// saves the timings in a txt file
	ofstream file(dir + "SGBM_time_" + name + ".txt");
	if(file.is_open()){
		file << "compute time: " << timings[0] << "\n";
		file << "stereo left: " << timings[1] << "\n";
		file << "stereo right: " << timings[2] << "\n";
		file << "filtering: " << timings[3] << "\n"; 
		file << "remap time: " << timings[4] << "\n";
		file.close();
	}
	count_num+=1;
}


void stereoDispStream(string winNameR, string winNameL, const char* calibFile){
	//ip's for the stream 
	string ipVideomaster = "http:/" + string("/192.168.10.10:8081");
	string ipVideo = "http:/" + string("/192.168.10.20:8081");

	//matrix for the remaping
	Mat MR1, MR2, ML1,ML2, Q;

	//readin the remapping matrix
	FileStorage fsi(calibFile, FileStorage::READ);
	fsi["MR1"] >> MR1;
	fsi["MR2"] >> MR2;
	fsi["ML1"] >> ML1;
	fsi["ML2"] >> ML2;
	fsi["Q"] >> Q;
	
	// for images and the timings
	Mat rightFrame, leftFrame, disp,dispR,M3D,filtered_disp,filtered_disp_vis, highlight;
	vector<Mat> splt;
	
	
	//SGBM
	//gets the information from param file 
	static Ptr<StereoSGBM> stereo = StereoSGBM::create(); // the stereo object
	stereo->setNumDisparities(readConf("disp"));
	stereo->setMode(readConf("mode"));
	stereo->setMinDisparity((-1*(readConf("disp")/2)));
	stereo->setBlockSize(readConf("blockSize"));
	stereo->setP1(8*rightFrame.channels()*readConf("blockSize")*readConf("blockSize"));
	stereo->setP2(64*rightFrame.channels()*readConf("blockSize")*readConf("blockSize"));
	stereo->setDisp12MaxDiff(readConf("disp12"));
	stereo->setPreFilterCap(readConf("prefilter"));
	stereo->setUniquenessRatio(readConf("uniqueRatio"));
	stereo->setSpeckleRange(readConf("speckleRange"));
	stereo->setSpeckleWindowSize(readConf("spackleWindowSize"));
	
	
	/*
	//BM
	static Ptr<StereoBM> stereo = StereoBM::create();
	stereo->setNumDisparities(readConf("disp"));
	stereo->setMinDisparity((-1*(readConf("disp")/2)));
	stereo->setBlockSize(readConf("blockSize"));
	stereo->setPreFilterType(StereoBM::PREFILTER_XSOBEL);
	stereo->setDisp12MaxDiff(readConf("disp12"));
	//stereo->setPreFilterCap(readConf("prefilter"));
	stereo->setUniquenessRatio(readConf("uniqueRatio"));
	stereo->setSpeckleRange(readConf("speckleRange"));
	stereo->setSpeckleWindowSize(readConf("spackleWindowSize"));
	*/
	
	static Ptr<StereoMatcher> stereoR = createRightMatcher(stereo);
	static Ptr<DisparityWLSFilter> wls_filter = createDisparityWLSFilter(stereo);
	
	//got from opencv example Disparity_filter.cpp
	wls_filter->setLambda(readConf("lambda"));
	wls_filter->setSigmaColor(readConf("sigma"));
	
	
	namedWindow("disparity");
	namedWindow("colorMap");
	moveWindow("disparity",0,0);
	moveWindow("colorMap", 640,0);
	
	
	
	//loop for live stream
	while(true){
		//right stream
		VideoCapture rightStream(ipVideo);
		rightStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		rightStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		//left stream
		VideoCapture leftStream(ipVideomaster);
		leftStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		leftStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		// puts stream into frames
		rightStream >> rightFrame;
		leftStream >> leftFrame;
		leftStream >> highlight;
		
		//converts color to gray to help processing
		cvtColor(rightFrame, rightFrame, COLOR_BGR2GRAY); 
		cvtColor(leftFrame, leftFrame, COLOR_BGR2GRAY);
		
		//remaps images and takes time
		auto remapTimeB = chrono::steady_clock::now();
		remap(rightFrame, rightFrame, MR1, MR2, INTER_CUBIC);
		remap(leftFrame, leftFrame, ML1, ML2, INTER_CUBIC);
		auto remapTimeE = chrono::steady_clock::now();
		remap(highlight, highlight, ML1,ML2, INTER_CUBIC);
		
		//computes disparity map and times it
		auto computeTimeBegin = chrono::steady_clock::now();
		
		auto stereoCompB1 = chrono::steady_clock::now();
		stereo->compute(leftFrame,rightFrame,disp);
		auto stereoCompE1 = chrono::steady_clock::now();
		
		auto stereoCompB2 = chrono::steady_clock::now();
		stereoR->compute(rightFrame,leftFrame,dispR);
		auto stereoCompE2 = chrono::steady_clock::now();
		
		auto filterCompB = chrono::steady_clock::now();
		wls_filter->filter(disp,leftFrame,filtered_disp,dispR,Rect(),rightFrame);
		auto filterCompE = chrono::steady_clock::now();
		
		getDisparityVis(filtered_disp, filtered_disp_vis, readConf("visNum"));
		auto computeTimeEnd = chrono::steady_clock::now();
		
		//colors disparity map  
		Mat colorMap;
		applyColorMap(filtered_disp_vis,colorMap,COLORMAP_JET);
		
		//gets the overlay for live feed
		Mat vis, mask;
		double min, max;
		minMaxLoc(filtered_disp_vis, &min,&max);
		inRange(filtered_disp_vis, Scalar(max - readConf("low_thresh")), Scalar(max), mask);
		cvtColor(mask,mask,CV_GRAY2BGR);
		
		//create the color the overlay should be (red is what is chosen)
		Mat color = Mat(rightFrame.rows, rightFrame.cols, CV_8UC3, Scalar(0,0,255));
		
		// does the overlay an resizes the imamge
		bitwise_and(mask, color, mask);
		addWeighted(highlight, 1, mask, 0.95, 0.0,vis);
		resize(vis, vis, Size(640,420));
		
		//shows the images 
		imshow("disparity",vis);  
		imshow("colorMap", colorMap);
		
		//puts timings into vector
		vector<double> timing;
		timing.push_back((double)chrono::duration_cast<chrono::milliseconds>(computeTimeEnd - computeTimeBegin).count()); //0
		timing.push_back((double)chrono::duration_cast<chrono::milliseconds>(stereoCompE1 - stereoCompB1).count()); //1
		timing.push_back((double)chrono::duration_cast<chrono::milliseconds>(stereoCompE2 - stereoCompB2).count()); //2
		timing.push_back((double)chrono::duration_cast<chrono::milliseconds>(filterCompE - filterCompB).count()); //3
		timing.push_back((double)chrono::duration_cast<chrono::milliseconds>(remapTimeE - remapTimeB).count()); //4
			
		//option for saving or quiting
		char c = waitKey(1000/readConf("FPS"));
		if(c == 'q'){ 
			exit(0); //quits when Q is pressed
		}
		
		if(c == 's'){ //saves image
			printf(" D%d_M%d_Room %d\n",readConf("disp"), readConf("mode"),count_num);
			savePhoto(leftFrame, rightFrame, filtered_disp_vis, timing, readConf("disp"), readConf("mode"), "Room");
		}
		if(c == 'r'){ //saves image
			printf(" D%d_M%d_Road %d\n",readConf("disp"), readConf("mode"), count_num);
			savePhoto(leftFrame, rightFrame, filtered_disp_vis, timing, readConf("disp"), readConf("mode"), "Road");
		}
	}
	
	//when dont quits program
	destroyAllWindows();	
}


void rectifyVis(const char* calibFile){
	//IP of streams
	string ipVideomaster = "http:/" + string("/192.168.10.10:8081");
	string ipVideo = "http:/" + string("/192.168.10.20:8081");
	
	Mat left, right, rect;
	Mat MR1, MR2, ML1,ML2;
	
	namedWindow("rect");
	 //load files 
	FileStorage fsi(calibFile, FileStorage::READ);
	fsi["MR1"] >> MR1;
	fsi["MR2"] >> MR2;
	fsi["ML1"] >> ML1;
	fsi["ML2"] >> ML2;
	
	//the color of line
	Scalar Color =  Scalar(rand() % 255 + 100, rand() % 255 + 100, rand() % 255 + 100);
	while(true){
		// right stream
		VideoCapture rightStream(ipVideo);
		rightStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		rightStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		//left Stream
		VideoCapture leftStream(ipVideomaster);
		leftStream.set(CAP_PROP_FRAME_HEIGHT, readConf("IMGHEIGHT"));
		leftStream.set(CAP_PROP_FRAME_WIDTH, readConf("IMGWIDTH"));
		
		//puts stream into frames
		rightStream >> right;
		leftStream >> left;
		
		//remaps iamges 
		remap(right, right, MR1, MR2, INTER_LINEAR);
		remap(left, left, ML1, ML2, INTER_LINEAR);
		
		//puts images side by side
		hconcat(left,right,rect);
		
		//draw lines
		for(int i = 0; i <= readConf("IMGHEIGHT"); i+=48){	
			line(rect, Point(0,i), Point(rect.cols, i),Color, 1);
		}
		
		imshow("rect", rect); //show
		
		// keypresses.
		char c = waitKey(1000/readConf("FPS"));
		if(c == 'q') exit(0);
		if(c == 'c'){
			imwrite("images/rectified.jpg",rect);
			exit(0);
		}
	}
}
