#include "functions.h"

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;
string objectType;

void color_detection() { //import image
	string path = "Resources/shapes.png";
	Mat img = imread(path);  //n-dimensional dense array class, whitch handels all kinds images
	//imread: image read from a path/file name

	//��img�নHSV( Hue(���), Saturation(���M), Value)
	//�ηN:��M�C���²��in HSV space

	//�i�z�Lnamewwindow�Ы�trackbar �վ�bar������쪽��Y�ӹϧΪ��C��X�{����,��i�T�{���C�⪺HSV
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
		inRange(imgHSV, lower, upper, mask);  //�`���q�ۭq���U����W�����C��

		imshow("Hi", img);
		imshow("imgHSV", imgHSV);
		imshow("imgMask", mask);

		waitKey(0);
	}
}


void getContours(Mat img_after_dialtion, Mat img_proecessed) { //���oimg������

	vector<vector<Point>> contours; //vector<point>����F�@�t�Cpoints,�o��points�Υt�@��vector�s��
	vector<Vec4i> hierarchy; //opencv���w�q��datatype,�@�Ӥ������|��int��
	//e.g hierarchy[0]={1,2,3,4};
	findContours(img_after_dialtion, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img_proecessed, contours, -1, Scalar(255, 0, 255), 2);

	//---------�H�U����ļ�I����k------------	
	vector<vector<Point>> contoursPolygun(contours.size());
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++) {

		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1000) { //�p��1000 �h����ļ�I

			//find bounding box  around these objects / shapes	 
			float perimeter = arcLength(contours[i], true);
			/*
				The function arcLength computes a curve length or a closed contour perimeter.
				@param curve Input vector of 2D points, stored in std::vector or Mat.
				@param closed Flag indicating whether the curve is closed or not.
			*/

			//find # of curves/ corner points
			/*e.g.
				3:�T����
				�ܦh:���
			*/
			approxPolyDP(contours[i], contoursPolygun[i], 0.02 * perimeter, true);

			drawContours(img_proecessed, contoursPolygun, i, Scalar(255, 0, 255), 2); //���ⳡ����>1000����������

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
			drawContours(img_proecessed, contours, i, Scalar(255, 255, 255), 5); //�ѤU�¦⪺���� �h�O<1000(�L�p)
		}
	}
}


void shape_detction() {

	string path = "Resources/shapes.png";
	Mat img = imread(path);
	Mat imgGray; //Def ��¥ժ�img
	Mat imgBlur;//Def �ҽk�L��img
	Mat imgCanny; //Def ��t�˴��᪺img �|��Ϥ��W����yø�X��
	Mat imgDialation, imgErode; //Dialation:��canny���䵹�[��, Erode:��canny����β�
	//�ηN:canny���䦳�������䪺�ĪG,�z�Ldilation��i�a���䵹�[��,�[�j��t�˴����ĪG

	cvtColor(img, imgGray, COLOR_BGR2GRAY);  //convert color from the origin color(RGB) -> �Ǧ�

	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0); //�Nimg���ҽk��

	Canny(imgBlur, imgCanny, 25, 75); //��t�˴� threshold:�{���I

	Mat kernal = getStructuringElement(MORPH_RECT, Size(3, 3));

	dilate(imgCanny, imgDialation, kernal);  //��canny�X�Ӫ���t�˴��u����j

	erode(imgDialation, imgErode, kernal);

	//---------------�H�W��Preprocessing----------------

	getContours(imgDialation, img);

	imshow("Hi", img);
	/*imshow("Hi gray", imgGray);
	imshow("Hi blur", imgBlur);
	imshow("Hi canny", imgCanny);
	imshow("Hi Dilation", imgDialation);
	imshow("Hi Erode", imgErode);*/


	waitKey(0);
}

