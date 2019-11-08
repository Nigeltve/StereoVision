#include "Calibration.hpp"
#include "util.hpp"

using namespace std;
using namespace cv;

const float SQUAREDIMS = 2.f; //size in centemeters
const Size CHESSBOARDDIMS = Size(6,9);

//init of everythign needed for calibration
vector<vector<Point3f>> objectPoints;
vector<vector<Point2f>> rightImgPoints, leftImgPoints;
vector<Point2f> rightCorners, leftCorners;

Mat KL, DL, KR, DR, R, E, F, Q, PL, PR, RL, RR, HL, HR;
Vec3d T;

void Calibration(int numImgs, const char* Path){
	Mat imgLeft, imgRight; 
	for(int i = 0; i < numImgs; i++){
		//sets paths for loading
		char leftImgPath[100], rightImgPath[100];
		sprintf(leftImgPath, "%sleftBoard/left%d.jpg",Path,i);
		sprintf(rightImgPath, "%srightBoard/right%d.jpg",Path,i);
		
		printf("%s\n",leftImgPath);
		printf("%s\n",rightImgPath);
		cout << endl;
		
		// loads in images 
		imgLeft = imread(leftImgPath, CV_LOAD_IMAGE_COLOR);
		imgRight = imread(rightImgPath, CV_LOAD_IMAGE_COLOR);
		
		//vars for when chessboard is found
		bool leftFound = false, rightFound = false;
		
		// finds chesboards
		rightFound = findChessboardCorners(imgRight, CHESSBOARDDIMS, rightCorners, 
										   CV_CALIB_CB_ADAPTIVE_THRESH | 
										   CV_CALIB_CB_FILTER_QUADS);
		
		leftFound = findChessboardCorners(imgLeft, CHESSBOARDDIMS, leftCorners,
									      CV_CALIB_CB_ADAPTIVE_THRESH | 
									      CV_CALIB_CB_FILTER_QUADS);
									      
		if(leftFound && rightFound){ // if both are found 
			
			//draws patters on right chessboard images 
			Mat rightDraw;
			imgRight.copyTo(rightDraw);
			
			drawChessboardCorners(rightDraw, CHESSBOARDDIMS, rightCorners, rightFound);
			
			imshow("right",rightDraw);
	
			//draws patters on left chessboard images 
			Mat leftDraw;
			imgLeft.copyTo(leftDraw);
			
			drawChessboardCorners(leftDraw, CHESSBOARDDIMS, leftCorners, leftFound);
			
			imshow("left",leftDraw);
						  
			// waits for user input
			waitKey(0);
			destroyAllWindows();
			
			// converts points to realspace
			vector<Point3f> object;
			for(int i = 0; i < CHESSBOARDDIMS.height;i++){
				for(int j = 0; j < CHESSBOARDDIMS.width; j++){
						object.push_back(Point3f((float)i * SQUAREDIMS, 
												 (float)j * SQUAREDIMS, 0.f));
				}
			}
			
			//pushs back into global variables
			rightImgPoints.push_back(rightCorners);
			leftImgPoints.push_back(leftCorners);
			objectPoints.push_back(object);
		}
	}
	
	//inits the intrinsic matrix
	KL = initCameraMatrix2D(objectPoints, leftImgPoints, imgLeft.size(),0);
	KR = initCameraMatrix2D(objectPoints, rightImgPoints, imgRight.size(),0);
	
	//finds other camera matricies
	printf("calibrating Stereo\n");
	stereoCalibrate(objectPoints, leftImgPoints, rightImgPoints, 
					KL, DL, KR, DR, imgLeft.size(), R, T, E, F,
					CV_CALIB_FIX_ASPECT_RATIO | CALIB_ZERO_TANGENT_DIST |
					CV_CALIB_FIX_INTRINSIC | CV_CALIB_SAME_FOCAL_LENGTH);
					
	
	// finds the rotation and translation for rectification
	printf("rectification\n");
	stereoRectify(KL, DL, KR, DR, imgLeft.size(), R,T, RL, RR, PL, PR,
				  Q, CALIB_ZERO_DISPARITY);
	
	// finds the remap matrix for left camera
	printf("finding remap Matrix left\n");
	Mat ML1, ML2;
	initUndistortRectifyMap(KL, DL, RL, PL, imgLeft.size(), CV_32FC1, ML1, ML2);
	
	// finds the remap matrix for right camera
	printf("finding remap Matrix right\n");
	Mat MR1, MR2;
	initUndistortRectifyMap(KR, DR, RR, PR, imgLeft.size(), CV_32FC1, MR1, MR2);
	
	//saves into a yml file for later use
	printf("savin\n");
	FileStorage fsi("/home/pi/Desktop/Code/Thesis/calibFiles/Calibration.yml", FileStorage::WRITE);
	fsi << "KL" << KL;
	fsi << "KR" << KR;
	fsi << "DL" << DL;
	fsi << "DR" << DR;
	fsi << "R" << R;
	fsi << "E" << E;
	fsi << "F" << F;
	fsi << "Q" << Q;
	fsi << "PL" << PL;
	fsi << "PR" << PR;
	fsi << "RL" << RL;
	fsi << "RR" << RR;
	fsi << "T" << T;
	fsi << "MR1" << MR1;
	fsi << "MR2" << MR2;
	fsi << "ML1" << ML1;
	fsi << "ML2" << ML2;
	
	printf("done calibration\n");
}


void Hartley(int numImgs, const char* Path){
	Mat imgLeft, imgRight;
	//goes through all image pairs
	for(int i = 0; i < numImgs; i++){
		//gets path for loading
		char leftImgPath[100], rightImgPath[100];
		sprintf(leftImgPath, "%sleftBoard/left%d.jpg",Path,i);
		sprintf(rightImgPath, "%srightBoard/right%d.jpg",Path,i);
		
		printf("%s\n",leftImgPath);
		printf("%s\n",rightImgPath);
		cout << endl;
		
		//load in images 
		imgLeft = imread(leftImgPath, CV_LOAD_IMAGE_COLOR);
		imgRight = imread(rightImgPath, CV_LOAD_IMAGE_COLOR);
		
		//values for when chessboard is found
		bool leftFound = false, rightFound = false;
		
		//finds right chessboard
		rightFound = findChessboardCorners(imgRight, CHESSBOARDDIMS, rightCorners, 
										   CV_CALIB_CB_ADAPTIVE_THRESH | 
										   CV_CALIB_CB_FILTER_QUADS);
		
		//finds right chessboard
		leftFound = findChessboardCorners(imgLeft, CHESSBOARDDIMS, leftCorners,
									      CV_CALIB_CB_ADAPTIVE_THRESH | 
									      CV_CALIB_CB_FILTER_QUADS);
									      
		if(leftFound && rightFound){ // if both are found save
			
			//draws points of chessboard on right image
			Mat rightDraw;
			imgRight.copyTo(rightDraw);
			
			drawChessboardCorners(rightDraw, CHESSBOARDDIMS, rightCorners, rightFound);
			
			imshow("right",rightDraw);
	
			//draws points of chessboard on left image
			Mat leftDraw;
			imgLeft.copyTo(leftDraw);
			
			drawChessboardCorners(leftDraw, CHESSBOARDDIMS, leftCorners, leftFound);
			
			imshow("left",leftDraw);
						  
			//waits for user input
			waitKey(0);
			destroyAllWindows();
			
			//converts points to realspace
			vector<Point3f> object;
			for(int i = 0; i < CHESSBOARDDIMS.height;i++){
				for(int j = 0; j < CHESSBOARDDIMS.width; j++){
						object.push_back(Point3f((float)i * SQUAREDIMS, 
												 (float)j * SQUAREDIMS, 0.f));
				}
			}
			
			//stores corners and points
			rightImgPoints.push_back(rightCorners);
			leftImgPoints.push_back(leftCorners);
			objectPoints.push_back(object);
		}
	}
	
	
	// flattens out the left and right points. only need it to be in one
	// vector
	vector<Point2f> leftPoints, rightPoints;
	for(int i = 0; i < rightImgPoints.size(); i++){	
		copy(leftImgPoints[i].begin(), leftImgPoints[i].end(),back_inserter(leftPoints));
		copy(rightImgPoints[i].begin(), rightImgPoints[i].end(), back_inserter(rightPoints));
	}
	
	// calibrates 
	printf("calibrating Stereo\n");
	stereoCalibrate(objectPoints, leftImgPoints, rightImgPoints, 
					KL, DL, KR, DR, imgLeft.size(), R, T, E, F, 
					CV_CALIB_FIX_ASPECT_RATIO | 
					CV_CALIB_ZERO_TANGENT_DIST |
					CV_CALIB_SAME_FOCAL_LENGTH|
					CV_CALIB_FIX_INTRINSIC);
	
	// find fundamental matrix
	printf("finding F\n");
	F = findFundamentalMat(leftPoints,rightPoints, CV_FM_8POINT);
	
	// finds how to finds HL and HR
	printf("rectify Uncalib\n");
	stereoRectifyUncalibrated(leftPoints, rightPoints, F, imgLeft.size(), HL, HR,3);
	
	// finds RL and RR
	printf("find RL RR\n");
	RL = KL.inv()*HL*KL;
	RR = KR.inv()*HR*KR;
	
	
	// finds remap matrix for left camear
	printf("remap L\n");
	Mat ML1, ML2;
	initUndistortRectifyMap(KL,DL,RL,PL, imgLeft.size(),CV_16SC2, ML1, ML2);
	
	// finds remap matrix for right camear
	printf("remap R\n");
	Mat  MR1, MR2;
	initUndistortRectifyMap(KR,DR,RR,PR, imgLeft.size(),CV_16SC2, MR1, MR2);
	
	//saves in files for later use 
	printf("saveing\n");
	FileStorage fsi("/home/pi/Desktop/Code/Thesis/calibFiles/Calibration_Hartley.yml", FileStorage::WRITE);
	
	fsi << "MR1" << MR1;
	fsi << "MR2" << MR2;
	fsi << "ML1" << ML1;
	fsi << "ML2" << ML2;
	
	printf("done\n");
}
