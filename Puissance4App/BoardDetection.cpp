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

cv::Vec3f BoardDetection::searchFirstCircle(cv::Mat image, std::vector<cv::Vec3f> circles, Color playerColor)
{
	if (circles.size() == 0)
	{
		return cv::Vec3f();
	}

	uint closestCircleIndex = 0;
	uint closestCircleValue = 1000000;
	for (int i = 0; i < circles.size(); i++)
	{
		cv::Vec3b detectedColor = getCircleMeanColor(image, circles[i]);

		
		/*uint deltaC = compareRGBColors(detectedColor, playerColor);

		if (deltaC < closestCircleValue)
		{
			closestCircleValue = deltaC;
			closestCircleIndex = i;
		}*/

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

Board BoardDetection::detectColors(cv::Mat image, std::vector<cv::Vec3f> boardCircles)
{
	if (boardCircles.size() != 42)
	{
		return Board();
	}

	Board board;
	uint playerThreshold = 200;
	uint robotThreshold = 150;

	for (int i = 0; i < boardCircles.size(); i++)
	{
		cv::Vec3b pieceColor = getCircleMeanColor(image, boardCircles[i]);
		cv::circle(image, cv::Point(boardCircles[i][0], boardCircles[i][1]), boardCircles[i][2] - 5, cv::Scalar(pieceColor[0], pieceColor[1], pieceColor[2]), -1);

		//à tester en profondeur
		if (abs(pieceColor[0] - pieceColor[1]) < 60 && abs(pieceColor[1] - pieceColor[2]) < 60 && abs(pieceColor[0] - pieceColor[2]) < 60)
		{
			continue;
		}
		if (abs(pieceColor[0] - pieceColor[1]) > 50 && abs(pieceColor[0] - pieceColor[2]) > 50)
		{
			board.setPlayerPiece(i / 6, 5 - i % 6, true);
		}
		else
		{
			board.setRobotPiece(i / 6, 5 - i % 6, true);
		}
	}
	cv::imshow("Debug", image);

	return board;
}

cv::Vec3b BoardDetection::getCircleMeanColor(cv::Mat image, cv::Vec3f circle)
{
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
	cv::circle(mask, cv::Point(circle[0], circle[1]), circle[2] - 5, cv::Scalar(255), -1);	
	cv::Scalar mean = cv::mean(image, mask);
	return cv::Vec3b(mean[0], mean[1], mean[2]);
}

BoardDetection::Color BoardDetection::getColor(cv::Vec3b color)
{
	if (abs(color[0] - color[1]) < 60 && abs(color[1] - color[2]) < 60 && abs(color[0] - color[2]) < 60)
	{
		return Color::EMPTY;
	}
	if (abs(color[0] - color[1]) > 50 && abs(color[0] - color[2]) > 50)
	{
		return Color::RED;
	}
	return Color::YELLOW;
}
