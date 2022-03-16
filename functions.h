#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace cv;


void color_detection();
void getContours(Mat img_after_dialtion, Mat img_proecessed);
void shape_detction();