#ifndef PHOTORETRIEVER_H
#define PHOTORETRIEVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

/**
 * @brief: goes through the number of photos user wants
 * 
 * @param[in] numberOfPhotos: how many photos user wants to take 
 */
void takePhoto(int numberOfPhotos);

/**
 * @brief: is the stream to get the stereo pairs from
 *  
 * @param[in] i: which photo is being taken. 
 * 
 */
void vid(int i);
					   
/**
 * 
 * @brief: saves the pair from the stream
 * 
 * @param[in] rightFrame: right camera from stream
 * 
 * @param[in] leftFrame: left camera from stream
 * 
 * @param[in] i: photo number. used for the photo name
 * 
 */
void saveFileStereo(cv::Mat& rightFrame, cv::Mat& leftFrame, int i);

#endif
