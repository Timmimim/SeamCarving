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

    static cv::Mat foldWithHorizontalSobel(cv::Mat *image, cv::Mat *energyField);
    static cv::Mat foldWithVerticalSobel(cv::Mat *image, cv::Mat *energyField);

    static cv::Mat foldWithHorizontalFancyShit(cv::Mat *image, cv::Mat *energyField);
    static cv::Mat foldWithVerticalFancyShit(cv::Mat *image, cv::Mat *energyField);

private:
    char horizontalSobelOperator[3][3];
    char verticalSobelOperator[3][3];


};

#endif // ENERGYFUNCTIONS_HPP
