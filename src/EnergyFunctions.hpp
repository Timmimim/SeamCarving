#ifndef ENERGYFUNCTIONS_HPP
#define ENERGYFUNCTIONS_HPP

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class EnergyFunctions
{
public:
    EnergyFunctions();

    static cv::Mat foldWithHorizontalOperator(const cv::Mat& image, const int choiceOperator);
    static cv::Mat foldWithVerticalOperator(const cv::Mat& image, const int choiceOperator);

private:
    static int horizontalSobelOperator[3][3];
    static int verticalSobelOperator[3][3];
    static int horizontalPrevittOperator[3][3];
    static int verticalPrevittOperator[3][3];
};

#endif // ENERGYFUNCTIONS_HPP
