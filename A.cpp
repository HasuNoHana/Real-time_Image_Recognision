#include<iostream>
#include <opencv2/opencv.hpp>


int main() {
    	cv::VideoCapture camera(0);
	if(!camera.isOpened()){
		std::cout<<"Nie otworzono kamere\n";
		return 1;
	}
	cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
	cv::Mat frame;
	int i = 0;
	while(1){
		camera >> frame;
		if(cv::waitKey(10) >= 0){
			cv::imshow("Webcam", frame);
			++i;
			if(i==6){
				break;
			}
		}
	}
	return 0;
}
