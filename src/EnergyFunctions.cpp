#include "EnergyFunctions.hpp"

EnergyFunctions::EnergyFunctions()
{
    enum operatorChoice {sobel, previtt};

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

    int horizontalPrevittOperator[3][3] =
        {
           {-1, 0, 1},
           {-1, 0, 1},
           {-1, 0, 1}
       };
    int verticalPrevittOperator[3][3] =
       {
           {-1, -1, -1},
           { 0,  0,  0},
           { 1,  1,  1}
       };

}

cv::Mat EnergyFunctions::convertToGrayscale(const cv::Mat& image){
    // TODO
}

cv::Mat EnergyFunctions::foldWithHorizontalOperator(const cv::Mat& image, const int choiceOperator)
{
    cv::Mat energyField = image.clone();
    int *horizontalOperator;
    int *verticalOperator;

    // Choice of operator in accordance with the enum listing the available operator implementations
    switch (choiceOperator) {
    case 1: // Sobel
        horizontalOperator = *EnergyFunctions::horizontalSobelOperator;
        break;
    case 2: // Previtt
        horizontalOperator = *EnergyFunctions::horizontalPrevittOperator;
        break;
    default:
        std::cout << "ERROR: Operator not defined, unable to compute energy. Good-Bye.";
        break;
    }

    for(int row = 0; row < image.rows; row++)
    {
        for(int col = 0; col < image.cols; col++)
        {
            if (row == 0 || row == image.rows-1) {
                //energyField[row][col] = 0;
            }

        }
    }
}


cv::Mat EnergyFunctions::foldWithVerticalOperator(const cv::Mat& image, const int choiceOperator)
{
    cv::Mat energyField = image.clone();
    int *horizontalOperator;
    int *verticalOperator;

    // Choice of operator in accordance with the enum listing the available operator implementations
    switch (choiceOperator) {
    case 1: // Sobel
        verticalOperator = *EnergyFunctions::verticalSobelOperator;
        break;
    case 2: // Previtt
        verticalOperator = *EnergyFunctions::verticalPrevittOperator;
        break;
    default:
        std::cout << "ERROR: Operator not defined, unable to compute energy. Good-Bye.";
        break;
    }

    for(int row = 0; row < image.rows; row++)
    {
        for(int col = 0; col < image.cols; col++)
        {
            if (col == 0 || col == image.rows-1) {
                //energyField[row][col] = 0;
            }

        }
    }
}

