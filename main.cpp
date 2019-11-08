#include "PhotoRetriever.hpp"
#include "Calibration.hpp"
#include "Stereo.hpp"
#include "util.hpp"
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	
	int  option; //the option for case handeling
	
	//goes throug hall options 
	while((option = getopt(argc,argv, "f:t:s:pvrh")) !=-1){
		switch (option) {
			case 'f': // case for doing full calibration
				printf("Doing full Calibration\n");
				takePhoto(atoi(optarg));
				Calibration(atoi(optarg),"/home/pi/Desktop/Code/Thesis/calib_photos/");
				break;
				
			case 't': // case for just taking photos
				printf("Taking photos\n");
				takePhoto(atoi(optarg));
				break;
				
			case 's': //case for calibrating on predefined images 
				printf("calibrating from photos\n");
				Calibration(atoi(optarg),"/home/pi/Desktop/Code/Thesis/calib_photos/");
				break;
				
			case 'v': // case for showing live video
				stereoDispStream("winR", "winL","/home/pi/Desktop/Code/Thesis/calibFiles/Calibration.yml");
				break;
			
			case 'r': // shows rectified images
				rectifyVis("/home/pi/Desktop/Code/Thesis/calibFiles/Calibration.yml");
			case 'p':
				// helper option for finding optimal parameters settigns for Block Matching
				if(!fexists("/home/pi/Desktop/Code/Thesis/images/left.jpg") || 
				   !fexists("/home/pi/Desktop/Code/Thesis/images/right.jpg")){
					   // if predefined images arnt there get them from stereo feed
					getDisplayImgParam();
				}
				setParameters();
				break;
				
			case 'h':
				//help option explains what each does
				printf("---- HELP ----\n");
				
				printf("\t -f: This is the Full calibration\n");
				printf("\t     so it takes photos and calibrates\n");
				printf("\t     and saves the calibration\n");
				printf("\n");

				printf("\t -t: This takes N photos for the\n");
				printf("\t     calibration where 'c' is to\n");
				printf("\t     capturer an image and 'q'\n");
				printf("\t     is to quit out of function\n");
				printf("\n");
				
				printf("\t -s: goes through images saved for\n");
				printf("\t     calibration and saves the camera\n");
				printf("\t     information in a yml file\n");
				printf("\n");
				
				printf("\t -v: does live stream Block Matching\n");
				printf("\t     needs number with it 1 being testing");
				printf("\t	   where goes through different modes \n");
				printf("\t 	   and disparities.\n");
				printf("\t     pressing Q will quit program and\n");
				printf("\t 	   pressing s will save an image\n");
				printf("\n");
				
				printf("\t -r: shows rectified stereo iamges\n");
				printf("\n");
				
				printf("\t -p: from a stereo pair shows disparity\n");
				printf("\t     map and has sliders to help find\n");
				printf("\t     optimal results\n");
				printf("\n");
				break;
				
			default:
				//incase no option was picked -> give warning
				printf("Error: Something went wrong\n");
				printf("Use the -h for help.\n");
				break;
		}
	}
	return 0;
}
