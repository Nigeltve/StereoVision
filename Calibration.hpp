#ifndef CALIBRATION_H
#define CALIBRATION_H


#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

/**
 * 
 * @brief: calibrates cameras using the Mouguet method using a chessboard
 * 		   pattern
 * 
 * @param[in] numImgs: how many images are for calibration
 * 
 * @param[in] Path: path to stereo images
 * 
 */
void Calibration(int numImgs, const char* Path);

/**
 * 
 * @brief: calibrates camears based on the Hartley method using a chessboard
 * 		   pattern
 * 
 * @param[in] numImgs: how many images are for calibration
 * 
 * @param[in] Path: path to stereo images
 * 
 */
void Hartley(int numImgs, const char* Path);
#endif
