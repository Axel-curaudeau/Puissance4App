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

		
		uint deltaC = compareColors(detectedColor, playerColor);

		if (deltaC < closestCircleValue)
		{
			closestCircleValue = deltaC;
			closestCircleIndex = i;
		}
	}
	cv::Vec3f firstCircle = circles[closestCircleIndex];
	circles.erase(circles.begin() + closestCircleIndex);
	return firstCircle;
}

std::vector<cv::Vec3f> BoardDetection::filterCircles(cv::Mat image, std::vector<cv::Vec3f> circles, cv::Vec3f firstCircle)
{
	std::vector<cv::Vec3f> boardCircles;
	
	//Search in the same line
	uint i = 0;
	while(i < circles.size())
	{
		if (circles[i][1] >= firstCircle[1] - 10 && circles[i][1] <= firstCircle[1] + 10 && circles[i][2] >= firstCircle[2] - 5 && circles[i][2] <= firstCircle[2] + 5)
		{
			boardCircles.push_back(circles[i]);
			circles.erase(circles.begin() + i);
		}
		i++;
	}

	if (boardCircles.size() != 7)
	{
		return std::vector<cv::Vec3f>();
	}

	//Search in the same column
	for (int i = 0; i < boardCircles.size(); i++)
	{
		uint j = 0;
		while (j < circles.size())
		{
			if (circles[j][0] >= boardCircles[i][0] - 10 && circles[j][0] <= boardCircles[i][0] + 10 && circles[j][2] >= boardCircles[i][2] - 5 && circles[j][2] <= boardCircles[i][2] + 5)
			{
				boardCircles.push_back(circles[j]);
				circles.erase(circles.begin() + j);
			}
			j++;
		}
	}

	if (boardCircles.size() != 42)
	{
		return std::vector<cv::Vec3f>();
	}

	return boardCircles;
}

std::vector<cv::Vec3f> BoardDetection::sortCircles(std::vector<cv::Vec3f> boardCircles)
{
	if (boardCircles.size() != 42)
	{
		return std::vector<cv::Vec3f>();
	}

	std::vector<cv::Vec3f> sortedCircles;

	//For all the columns
	for (uint i = 0; i < 6; i++)
	{
		uint minX = 1000000;
		uint minY = 1000000;
		uint minIndex = 0;

		//Search the first circle at minimum x and y
		for (int j = 0; j < boardCircles.size(); j++)
		{
			if (boardCircles[j][0] < minX && boardCircles[j][1] < minY)
			{
				minX = boardCircles[j][0];
				minY = boardCircles[j][1];
				minIndex = j;
			}
		}

		//Add the first circle to the sortedCircles vector
		sortedCircles.push_back(boardCircles[minIndex]);
		//Remove the first circle from the boardCircles vector
		boardCircles.erase(boardCircles.begin() + minIndex);

		//Search the next 6 circles in the same line
		for (int j = 0; j < 6; j++)
		{
			minX = 1000000;
			minIndex = 0;
			for (int j = 0; j < boardCircles.size(); j++)
			{
				if (boardCircles[j][0] < minX)
				{
					minX = boardCircles[j][0];
					minIndex = j;
				}
			}
			sortedCircles.push_back(boardCircles[minIndex]);
			boardCircles.erase(boardCircles.begin() + minIndex);
		}
	}

	return sortedCircles;
}

Board BoardDetection::detectColors(cv::Mat image, std::vector<cv::Vec3f> boardCircles, cv::Vec3b playerColor, cv::Vec3b robotColor)
{
	if (boardCircles.size() != 42)
	{
		return Board();
	}

	Board board;
	uint playerThreshold = 400;
	uint robotThreshold = 300;

	for (int i = 0; i < boardCircles.size(); i++)
	{
		cv::Vec3b color = getColor(image, boardCircles[i]);
		uint deltaCPlayer = compareColors(color, playerColor);
		uint deltaCRobot = compareColors(color, robotColor);
		std::cout << i <<  " DeltaCPlayer: " << deltaCPlayer << " DeltaCRobot: " << deltaCRobot << std::endl;
		if (deltaCPlayer < playerThreshold)
		{
			board.setPlayerPiece(i % 7, i / 7, true);
		}
		else if (deltaCRobot < robotThreshold)
		{
			board.setRobotPiece(i % 7, i / 7, true);
		}
	}

	return board;
}

cv::Vec3b BoardDetection::getColor(cv::Mat image, cv::Vec3f circle)
{
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
	cv::circle(mask, cv::Point(circle[0], circle[1]), circle[2], cv::Scalar(255), -1);
	cv::Scalar mean = cv::mean(image, mask);
	return cv::Vec3b(mean[0], mean[1], mean[2]);
}

uint BoardDetection::compareColors(cv::Vec3b color1, cv::Vec3b color2)
{
	uint r = 0.5 * (color1[0] + color2[0]);
	uint deltaR_square = pow(color1[0] - color2[0], 2);
	uint deltaG_square = pow(color1[1] - color2[1], 2);
	uint deltaB_square = pow(color1[2] - color2[2], 2);
	uint deltaC = sqrt((2 + (r / 256)) * deltaR_square + 4 * deltaG_square + (2 + ((255 - r) / 256)) * deltaB_square);
	return deltaC;
}
