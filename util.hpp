#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>

/**
 * @brief: checks to see if file exists
 * 
 * @param[in] fileName: the file to check; 
 * 
 * @returns: a bool value weather the file exists
 */
bool fexists(const std::string& filename);

/**
 * @brief: reads the config file
 * 
 * @param[in] paramLook: what parameters to look up
 * 
 * @returns: returns the integer read from that file
 */
int readConf(std::string paramLook);

/**
 * @brief: takes picture from stream if predetemined pictures arnt there 
 *  
 */
void getDisplayImgParam();

/**
 * @brief: trackbar implementation for the prefilter parameter
 * 
 */
 
 /**
 * @brief: trackbar implementation for disp12 param
 * 
 */
void trackbarPrefilter(int, void*);

 /**
 * @brief: trackbar implementation for disp parameter
 * 
 */
void trackbardisp12(int, void*);

 /**
 * @brief: trackbar implementation for blockSize parameter
 * 
 */
void trackBarBkSz(int, void*);

 /**
 * @brief: trackbar implementation for  disparity parameter
 * 
 */
void trackBarDisp(int, void*);

 /**
 * @brief: trackbar implementation for track bar for unique ratio param
 * 
 */
void trackBarUnqRtio(int,void*);

 /**
 * @brief: trackbar implementation for speckle range parameter
 * 
 */
void trackBarSpklRng(int, void *);

 /**
 * @brief: trackbar implementation for speckle window range parameter
 * 
 */
void trackBarSpklWinSz(int, void *);

 /**
 * @brief: trackbar implementation for Mode for computation
 * 
 */
void trackBarMode(int, void*);

 /**
 * @brief: from two images computes despairity map, parameters of 
 * 		   disparity map can be changed using sliders.
 * 
 */
void setParameters();
#endif
