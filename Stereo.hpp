#ifndef STEREO_H
#define STEREO_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <getopt.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/ximgproc.hpp>
#include <vector>
#include <chrono>

/**
 * 
 * @brief: dispays the video stream highlighting with what is too close in red 
 * 
 * @param[in] winNameR: what is the right window name
 * 
 * @param[in] winNameL: name of left window
 * 
 * @param[in] calibFile: the file path to calibration file
 * 
 * 
 */
void stereoDispStream(std::string winNameR, std::string winNameL, const char* calibFile);

/**
 * @brief: saves the photo if Mode is 1 then it goes through all 
 *         disparity and mode types and saves them, if not just saves
 * 		   with optimal params
 * 		   
 * 
 * @param[in] imgleft: the left images to save
 * 
 * @param[in] imgRight: the right images to save
 * 
 * @param[in] disp: the highlighted images to save
 * 
 * @param[in] timings: vector of timings for how long steps take
 * 
 * @param[in] disp_num: what disparity number, only for mode 1
 * 
 * @param[in] Mode: what mode is for the blockMatching
 * 
 * @param[in] type: where image is taken 
 * 
 */
void savePhoto(cv::Mat& imgleft, cv::Mat& imgRight, cv::Mat& disp, 
			   std::vector<double> timings, int disp_num, int Mode, 
			   std::string type);


/**
 * @brief: shows rectify images 
 * 
 * @param[in] calibfile: the path to the calibration file
 * 
 * 
 */
void rectifyVis(const char* calibFile);

#endif
