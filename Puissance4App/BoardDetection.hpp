#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "Board.hpp"

namespace BoardDetection {
	/// <summary>
	/// Search for circles in a given image using the HoughCircles() function from openCV
	/// </summary>
	/// <param name="image">Source image containing the board</param>
	/// <returns>A vector containing all the circles detected in the image</returns>
	std::vector<cv::Vec3f> detectCircle(cv::Mat image);

	/// <summary>
	/// Because the player start, the first played piece is searched
	/// </summary>
	/// <param name="image">Source image containing the board</param>
	/// <param name="circles">Vector containing all the circles detected in the image</param>
	/// <returns>A vector containing the coordinate of the circle and its radius</returns>
	cv::Vec3f searchFirstCircle(cv::Mat image, std::vector<cv::Vec3f> circles);

	/// <summary>
	/// Filter the circles to only keep the one from the real board.
	/// </summary>
	/// <param name="image">Source image containing the board</param>
	/// <param name="circles">Vector containing all the circles detected in the image</param>
	/// <returns>A vector containing the 42 coordinate of the circle from the board</returns>
	std::vector<cv::Vec3f> filterCircles(cv::Mat image, std::vector<cv::Vec3f> circles);

	/// <summary>
	/// Sort the circles from the board in the correct order :
	/// 
	/// 36 37 38 39 40 41 42
	/// 29 30 31 32 33 34 35 
	/// 22 23 24 25 26 27 28 
	/// 15 16 17 18 19 20 21
	/// 8  9  10 11 12 13 14
	/// 1  2  3  4  5  6  7
	/// </summary>
	/// <param name="boardCircles">Vector containing the circles from the board</param>
	/// <returns>A vector containing the circles from the boars in the correct order</returns>
	std::vector<cv::Vec3f> sortCircles(std::vector<cv::Vec3f> boardCircles);

	/// <summary>
	/// Detect the color inside the circle acording to the image
	/// </summary>
	/// <param name="image">Source image containing the board</param>
	/// <param name="boardCircles">Vector containing the circles from the board and in a correct order</param>
	/// <returns>A board object describing the status of the game</returns>
	Board detectColor(cv::Mat image, std::vector<cv::Vec3f> boardCircles);

	cv::Vec3b getColor(cv::Mat image, cv::Vec3f circle);
}

