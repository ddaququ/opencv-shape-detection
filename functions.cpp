#include "functions.h"

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;
string objectType;

void color_detection() { //import image
	string path = "Resources/shapes.png";
	Mat img = imread(path);  //n-dimensional dense array class, whitch handels all kinds images
	//imread: image read from a path/file name

	//把img轉成HSV( Hue(色調), Saturation(飽和), Value)
	//用意:找尋顏色較簡單in HSV space

	//可透過namewwindow創建trackbar 調整bar內的欄位直到某個圖形的顏色出現為止,方可確認此顏色的HSV
	namedWindow("TrackBars", (640, 200));
	createTrackbar("Hue min", "TrackBars", &hmin, 179);
	createTrackbar("Hue max", "TrackBars", &hmax, 179);
	createTrackbar("Sat min", "TrackBars", &smin, 255);
	createTrackbar("Sat max", "TrackBars", &smax, 255);
	createTrackbar("Val min", "TrackBars", &vmin, 255);
	createTrackbar("Val max", "TrackBars", &vmax, 255);
	while (1) {
		Mat imgHSV;
		Mat mask;

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, lower, upper, mask);  //蒐集從自訂的下限到上限的顏色

		imshow("Hi", img);
		imshow("imgHSV", imgHSV);
		imshow("imgMask", mask);

		waitKey(0);
	}
}


void getContours(Mat img_after_dialtion, Mat img_proecessed) { //取得img的輪廓

	vector<vector<Point>> contours; //vector<point>中放了一系列points,這些points用另一個vector存取
	vector<Vec4i> hierarchy; //opencv內定義的datatype,一個元素有四個int值
	//e.g hierarchy[0]={1,2,3,4};
	findContours(img_after_dialtion, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img_proecessed, contours, -1, Scalar(255, 0, 255), 2);

	//---------以下為找躁點的方法------------	
	vector<vector<Point>> contoursPolygun(contours.size());
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++) {

		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1000) { //小於1000 則視為躁點

			//find bounding box  around these objects / shapes	 
			float perimeter = arcLength(contours[i], true);
			/*
				The function arcLength computes a curve length or a closed contour perimeter.
				@param curve Input vector of 2D points, stored in std::vector or Mat.
				@param closed Flag indicating whether the curve is closed or not.
			*/

			//find # of curves/ corner points
			/*e.g.
				3:三角形
				很多:圓形
			*/
			approxPolyDP(contours[i], contoursPolygun[i], 0.02 * perimeter, true);

			drawContours(img_proecessed, contoursPolygun, i, Scalar(255, 0, 255), 2); //紫色部分為>1000的輪廓部分

			cout << contoursPolygun[i].size() << endl;
			boundRect[i] = boundingRect(contoursPolygun[i]);
			rectangle(img_proecessed, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			int objCor = (int)contoursPolygun[i].size();

			if (objCor == 3) objectType = "tri";
			else if (objCor == 4) {
				float aspect_Ratio = (float)boundRect[i].width / boundRect[i].height;
				if (aspect_Ratio > 0.95 && aspect_Ratio < 1.05)
					objectType = "squ";
				else
					objectType = "rect";
			}
			else objectType = "cri";
			putText(img_proecessed, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 69, 255), 1);
		}
		else {
			drawContours(img_proecessed, contours, i, Scalar(255, 255, 255), 5); //剩下黑色的部分 則是<1000(過小)
		}
	}
}


void shape_detction() {

	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgGray; //Def 改黑白的img
	Mat imgBlur;//Def 模糊過的img
	Mat imgCanny; //Def 邊緣檢測後的img 會把圖片上的邊描繪出來
	Mat imgDialation, imgErode; //Dialation:把canny的邊給加粗, Erode:把canny的邊用細
	//用意:canny的邊有類似毛邊的效果,透過dilation後可吧毛邊給加粗,加強邊緣檢測的效果

	cvtColor(img, imgGray, COLOR_BGR2GRAY);  //convert color from the origin color(RGB) -> 灰色

	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0); //將img做模糊化

	Canny(imgBlur, imgCanny, 25, 75); //邊緣檢測 threshold:臨界點

	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(imgCanny, imgDialation, kernal);  //把canny出來的邊緣檢測線條放大

	erode(imgDialation, imgErode, kernal);

	//---------------以上為Preprocessing----------------

	getContours(imgDialation, img);

	imshow("Hi", img);
	/*imshow("Hi gray", imgGray);
	imshow("Hi blur", imgBlur);
	imshow("Hi canny", imgCanny);
	imshow("Hi Dilation", imgDialation);
	imshow("Hi Erode", imgErode);*/


	waitKey(0);
}

