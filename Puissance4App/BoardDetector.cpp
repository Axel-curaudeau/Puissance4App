#include "BoardDetector.hpp"

std::vector<cv::Vec3f> BoardDetector::workingCircles;

Board BoardDetector::detectBoard(cv::Mat image, Color playerColor)
{
	//Detect the circles in the image
	std::vector<cv::Vec3f> circles = detectCircle(image);
	if (circles.size() == 0)
	{
		std::cout << "No circles detected" << std::endl;
		return Board();
	}
	for (int i = 0; i < circles.size(); i++)
	{
		cv::circle(image, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(255, 255, 255), 2);
	}

	//Search the first circle played by the player
	cv::Vec3f firstCircle = searchFirstCircle(image, circles, playerColor);
	if (firstCircle == cv::Vec3f())
	{
		std::cout << "No first circle detected" << std::endl;
		return Board();
	}
	cv::circle(image, cv::Point(firstCircle[0], firstCircle[1]), 5, cv::Scalar(255, 255, 255), -1);

	//Filter the circles to only keep the one from the board
	std::vector<cv::Vec3f> boardCircles = filterCircles(image, circles, firstCircle);
	if (boardCircles.size() == 0)
	{
		std::cout << "No board circles detected" << std::endl;
	}

	//Sort the circles in the correct order
	std::vector<cv::Vec3f> sortedCircles = sortCircles(boardCircles);
	if (sortedCircles.size() == 0)
	{
		std::cout << "No sorted circles detected" << std::endl;
	}

	//Detect the colors of the circles
	Board board = detectColors(image, sortedCircles);

	return board;
}

std::vector<cv::Vec3f> BoardDetector::detectCircle(cv::Mat frame)
{
	if (frame.empty())
	{
		return std::vector<cv::Vec3f>();
	}

	//Convert the frame to gray
	cv::Mat grayFrame;
	cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

	//Search for circles in the frame
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(grayFrame, circles, cv::HOUGH_GRADIENT, 0.5, grayFrame.rows / 12, 200, 30, grayFrame.rows / 24, grayFrame.rows / 6);

	return circles;
}

cv::Vec3f BoardDetector::searchFirstCircle(cv::Mat image, std::vector<cv::Vec3f> circles, Color playerColor)
{
	if (circles.size() == 0)
	{
		return cv::Vec3f();
	}

	//Search the first circle with the player color inside
	uint closestCircleIndex = 0;
	uint closestCircleValue = 1000000;
	for (int i = 0; i < circles.size(); i++)
	{
		cv::Vec3b detectedColor = getCircleMeanColor(image, circles[i]);

		Color detectedColorType = getColor(detectedColor);
		if (detectedColorType == Color::EMPTY)
		{
			continue;
		}
		if (detectedColorType == playerColor)
		{
			closestCircleIndex = i;
			break;
		}
	}
	cv::Vec3f firstCircle = circles[closestCircleIndex];

	//Remove the first circle from the circles vector
	circles.erase(circles.begin() + closestCircleIndex);
	return firstCircle;
}

std::vector<cv::Vec3f> BoardDetector::filterCircles(cv::Mat image, std::vector<cv::Vec3f> circles, cv::Vec3f firstCircle)
{
	std::vector<cv::Vec3f> boardCircles;
	
	//Search in the same line
	uint i = 0;
	while(i < circles.size())
	{
		if (circles[i][1] >= firstCircle[1] - 10 && circles[i][1] <= firstCircle[1] + 10) //&& circles[i][2] >= firstCircle[2] - 5 && circles[i][2] <= firstCircle[2] + 5)
		{
			boardCircles.push_back(circles[i]);
			circles.erase(circles.begin() + i);
		}
		i++;
	}

	if (boardCircles.size() != 7)
	{
		std::cout << "Not enough circles in the same line" << std::endl;
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

	//If the board is not complete
	if (boardCircles.size() != 42)
	{
		std::cout << "Not enough circles in the same column" << std::endl;
		return std::vector<cv::Vec3f>();
	}

	return boardCircles;
}

std::vector<cv::Vec3f> BoardDetector::sortCircles(std::vector<cv::Vec3f> boardCircles)
{
	if (boardCircles.size() != 42)
	{
		return std::vector<cv::Vec3f>();
	}

	std::vector<cv::Vec3f> sortedCircles;

	//For all the columns
	for (uint i = 0; i < 7; i++)
	{
		uint min = 1000000;
		uint minIndex = 0;

		//Search the first circle at minimum x and y
		for (int j = 0; j < boardCircles.size(); j++)
		{
			if (boardCircles[j][0] + boardCircles[j][1] < min)
			{
				min = boardCircles[j][0] + boardCircles[j][1];
				minIndex = j;
			}
		}

		//Add the first circle to the sortedCircles vector
		sortedCircles.push_back(boardCircles[minIndex]);
		//Remove the first circle from the boardCircles vector
		boardCircles.erase(boardCircles.begin() + minIndex);

		//Search the next 5 circles in the same line
		for (int j = 0; j < 5; j++)
		{
			min = 1000000;
			minIndex = 0;
			for (int j = 0; j < boardCircles.size(); j++)
			{
				if (abs(boardCircles[j][0] - sortedCircles[i * 6][0]) < sortedCircles[i*6][2] && boardCircles[j][1] < min)
				{
					min = boardCircles[j][1];
					minIndex = j;
				}
			}
			sortedCircles.push_back(boardCircles[minIndex]);
			boardCircles.erase(boardCircles.begin() + minIndex);
		}
	}

	return sortedCircles;
}

Board BoardDetector::detectColors(cv::Mat image, std::vector<cv::Vec3f> boardCircles)
{
	if (boardCircles.size() != 42)
	{
		if (workingCircles.size() == 42)
		{
			boardCircles = workingCircles;
		}
		else
		{
			return Board();
		}
	}
	else
	{
		workingCircles = boardCircles;
	}

	Board board;
	uint playerThreshold = 200;
	uint robotThreshold = 150;

	for (int i = 0; i < boardCircles.size(); i++)
	{
		cv::Vec3b pieceColor = getCircleMeanColor(image, boardCircles[i]);

		//Draw the circle with the detected color for debug
		cv::circle(image, cv::Point(boardCircles[i][0], boardCircles[i][1]), boardCircles[i][2] - 5, cv::Scalar(pieceColor[0], pieceColor[1], pieceColor[2]), -1);

		//Detect the color of the circle
		Color color = getColor(pieceColor);

		if (color == Color::EMPTY)
		{
			continue;
		}
		else if (color == Color::RED)
		{

			board.setPlayerPiece(i / 6, 5 - i % 6, true);
		}
		else
		{
			board.setRobotPiece(i / 6, 5 - i % 6, true);
		}
	}
	//Uncomment to show the debug image with the detected circles
	//cv::imshow("Debug", image);

	return board;
}

cv::Vec3b BoardDetector::getCircleMeanColor(cv::Mat image, cv::Vec3f circle)
{
	//Create a mask to get only the circle pixels
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
	cv::circle(mask, cv::Point(circle[0], circle[1]), circle[2] - 5, cv::Scalar(255), -1);	

	//Get the mean color of the circle
	cv::Scalar mean = cv::mean(image, mask);
	return cv::Vec3b(mean[0], mean[1], mean[2]);
}

BoardDetector::Color BoardDetector::getColor(cv::Vec3b color)
{
	if (abs(color[0] - color[1]) < 60 && abs(color[1] - color[2]) < 60 && abs(color[0] - color[2]) < 60)
	{
		return Color::EMPTY;
	}
	if (abs(color[2] - color[0]) > 50 && abs(color[2] - color[1]) > 50)
	{
		return Color::RED;
	}
	return Color::YELLOW;
}

std::vector<cv::Vec3f> BoardDetector::addAndRemoveDuplicates(std::vector<cv::Vec3f> newCircles, std::vector<cv::Vec3f> oldCircles)
{
	std::vector<cv::Vec3f> result = newCircles;

	for (int i = 0; i < oldCircles.size(); i++)
	{
		result.push_back(oldCircles[i]);
	}

	for (int i = 0; i < result.size(); i++)
	{
		for (int j = i + 1; j < result.size(); j++)
		{
			if (abs(result[i][0] - result[j][0]) < 10 && abs(result[i][1] - result[j][1]) < 10)
			{
				result.erase(result.begin() + j);
			}
		}
	}

	return result;
}
