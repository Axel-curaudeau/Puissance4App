#include "BoardDetection.hpp"

std::vector<cv::Vec3f> BoardDetection::detectCircle(cv::Mat frame)
{
	if (frame.empty())
	{
		return std::vector<cv::Vec3f>();
	}

	cv::Mat grayFrame;
	cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(grayFrame, circles, cv::HOUGH_GRADIENT, 0.5, grayFrame.rows / 12, 200, 30, grayFrame.rows / 24, grayFrame.rows / 6);

	for (size_t i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		cv::circle(frame, center, radius, cv::Scalar(0, 255, 0), 2);
	}

	return circles;
}

cv::Vec3f BoardDetection::searchFirstCircle(cv::Mat image, std::vector<cv::Vec3f> circles)
{
	if (circles.size() == 0)
	{
		return cv::Vec3f();
	}

	for (int i = 0; i < circles.size(); i++)
	{
		cv::Vec3b color = getColor(image, circles[i]);
		std::cout << "Color : " << color << std::endl;
	}
	return circles[0];
}

cv::Vec3b BoardDetection::getColor(cv::Mat image, cv::Vec3f circle)
{
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
	cv::circle(mask, cv::Point(circle[0], circle[1]), circle[2], cv::Scalar(255), -1);
	cv::Scalar mean = cv::mean(image, mask);
	return cv::Vec3b(mean[0], mean[1], mean[2]);
}
