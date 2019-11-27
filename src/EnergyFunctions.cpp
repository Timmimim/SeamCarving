#include "EnergyFunctions.hpp"

EnergyFunctions::EnergyFunctions()
{
    int horizontalSobelOperator[3][3] =
        {
           {-1, 0, 1},
           {-2, 0, 2},
           {-1, 0, 1}
       };
    int verticalSobelOperator[3][3] =
       {
           {-1, -2, -1},
           { 0,  0,  0},
           { 1,  2,  1}
       };


}

cv::Mat EnergyFunctions::foldWithHorizontalSobel(cv::Mat *image, cv::Mat *energyField)
{

}

cv::Mat EnergyFunctions::foldWithVerticalSobel(cv::Mat *image, cv::Mat *energyField)
{

}

cv::Mat EnergyFunctions::foldWithHorizontalFancyShit(cv::Mat *image, cv::Mat *energyField)
{

}

cv::Mat EnergyFunctions::foldWithVerticalFancyShit(cv::Mat *image, cv::Mat *energyField)
{

}
