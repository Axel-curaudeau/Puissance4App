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

cv::Vec3f BoardDetection::searchFirstCircle(cv::Mat image, std::vector<cv::Vec3f> circles, cv::Vec3b playerColor)
{
	if (circles.size() == 0)
	{
		return cv::Vec3f();
	}

	uint closestCircleIndex = 0;
	uint closestCircleValue = 1000000;
	for (int i = 0; i < circles.size(); i++)
	{
		cv::Vec3b detectedColor = getColor(image, circles[i]);

		uint r = 0.5 * (playerColor[0] + detectedColor[0]);
		uint deltaR_square = pow(playerColor[0] - detectedColor[0], 2);
		uint deltaG_square = pow(playerColor[1] - detectedColor[1], 2);
		uint deltaB_square = pow(playerColor[2] - detectedColor[2], 2);
		uint deltaC = sqrt((2 + (r / 256)) * deltaR_square + 4 * deltaG_square + (2 + ((255 - r) / 256)) * deltaB_square);

		std::cout << "DeltaC : " << deltaC << std::endl;

		if (deltaC < closestCircleValue)
		{
			closestCircleValue = deltaC;
			closestCircleIndex = i;
		}
	}
	return circles[closestCircleIndex];
}

std::vector<cv::Vec3f> BoardDetection::filterCircles(cv::Mat image, std::vector<cv::Vec3f> circles, cv::Vec3f firstCircle)
{
	std::vector<cv::Vec3f> boardCircles;
	//TODO
	return boardCircles;
}

cv::Vec3b BoardDetection::getColor(cv::Mat image, cv::Vec3f circle)
{
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
	cv::circle(mask, cv::Point(circle[0], circle[1]), circle[2], cv::Scalar(255), -1);
	cv::Scalar mean = cv::mean(image, mask);
	return cv::Vec3b(mean[0], mean[1], mean[2]);
}
