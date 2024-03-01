#include "Camera.hpp"


Camera::Camera()
{
	frame = new cv::Mat();
	startWebcamThread();
}

Camera::~Camera()
{
	webcamThreadRunning = false;
	webcamThread.join();
	delete frame;
}

void Camera::startWebcamThread()
{
	webcamThreadRunning = true;
	webcamThread = std::thread(&Camera::getWebcamImage, this, frame);
}

cv::Mat Camera::getFrame()
{
	frameMutex.lock();
	cv::Mat tempFrame = frame->clone();
	frameMutex.unlock();
	return tempFrame;
}

void Camera::getWebcamImage(cv::Mat* frame)
{
	if (frame == nullptr)
		return;

	cv::VideoCapture cap(0);
	if (!cap.isOpened())
		return;

	while (webcamThreadRunning)
	{
		cv::Mat tempFrame;
		cap >> tempFrame;
		frameMutex.lock();
		*frame = tempFrame;
		frameMutex.unlock();
	}
}
