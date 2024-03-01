#pragma once

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>

class Camera
{
public:
	Camera();
	~Camera();
	void startWebcamThread();
	cv::Mat getFrame();

private:
	void getWebcamImage(cv::Mat* frame);
	bool webcamThreadRunning = false;
	cv::Mat* frame;
	std::thread webcamThread;
	std::mutex frameMutex;
};
