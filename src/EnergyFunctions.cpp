#include "EnergyFunctions.hpp"
#include <iostream>
#include <thread>
#include <algorithm>

EnergyFunctions::EnergyFunctions()
{
    enum operatorChoice {sobel, previtt};
/*
    const int horizontalSobelOperator[3][3] =
        {
           {-1, 0, 1},
           {-2, 0, 2},
           {-1, 0, 1}
       };
    const int verticalSobelOperator[3][3] =
       {
           {-1, -2, -1},
           { 0,  0,  0},
           { 1,  2,  1}
       };

    const int horizontalPrevittOperator[3][3] =
        {
           {-1, 0, 1},
           {-1, 0, 1},
           {-1, 0, 1}
       };
    const int verticalPrevittOperator[3][3] =
       {
           {-1, -1, -1},
           { 0,  0,  0},
           { 1,  1,  1}
       };*/

}


cv::Mat EnergyFunctions::convertToGrayscale(const cv::Mat &image)
{
    // Check validity of input image
    if(image.depth() != CV_8U)  // cv 8 Bit unsigned
    {
        std::cout << "ERROR: Input image does not have 8-Bit depth. The format is not supported." << std::endl;
        throw std::invalid_argument("Invalid image depth, grayscaling not possible.");
    }

    switch (image.channels())
    {
    case 1:     // one channel means image already grayscaled
    {
        std::cout << "WARNING: Image already grayscaled, nothing to do." << std::endl;
        return image.clone();
    }
    case 3:     // three channels => colour image => grayscale by averaging channels
    {
        // create image matrix for grayscale img; set all values to 0 using Scalar vector (special constructor)
        auto grayscaleImage = cv::Mat(image.size(), CV_8UC1, cv::Scalar::all(0));

        // Perform grayscaling using multi-threading (if available)
        // find number of available concurrent threads (returned as uint, convert to int);
        // clamp number of threads to maximum number of rows (more would not make sense)
        auto numThreads = std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, image.rows);
        // declare a vector to manage threads
        auto threads = std::vector<std::thread>{};

        // use threads to perform averaging
        for(int t = 0; t < numThreads; ++t)
        {
            // divide image into segments to be averaged separately in parallel
            // calculate start and end of matrix interval per thread
            int start = image.rows * t / numThreads;
            int end = image.rows * (t+1) / numThreads;

            // start the current thread, add to vector
            threads.emplace_back([start, end, &image, &grayscaleImage] () {
                for(int row = start; row < end; ++row)
                    for(int col = 0; col < image.cols; ++col)
                        for(int channel = 0; channel < image.channels(); ++channel)
                            grayscaleImage.at<uchar>(row,col) += image.at<cv::Vec<uchar,3>>(row,col)[channel] / image.channels();
            });
        }
        for(auto& thread : threads)
            thread.join();

        return grayscaleImage;
    }
    default:
        std::cout << "ERROR: input image has invalid number of channels. Only grayscale and 3-channel colour images supported." << std::endl;
        throw std::invalid_argument("Invalid number of channels for input image.");
    }
}

cv::Mat EnergyFunctions::computeEnergy(const cv::Mat& grayscaleImage, const int choiceOperator)
{
    auto energyField = grayscaleImage.clone();

    int verticalOperator[3][3];
    int horizontalOperator[3][3];

    // Choice of operator in accordance with the enum listing the available operator implementations
    switch (choiceOperator) {
    case 0: // Sobel
        std::copy(&verticalSobelOperator[0][0], &verticalSobelOperator[0][0]+3*3,&verticalOperator[0][0]);
        std::copy(&horizontalSobelOperator[0][0], &horizontalSobelOperator[0][0]+3*3,&horizontalOperator[0][0]);
        break;
    case 1: // Previtt
        std::copy(&verticalPrevittOperator[0][0], &verticalPrevittOperator[0][0]+3*3,&verticalOperator[0][0]);
        std::copy(&horizontalPrevittOperator[0][0], &horizontalPrevittOperator[0][0]+3*3,&horizontalOperator[0][0]);
        break;
    default:
        std::cout << "ERROR: Operator not defined, unable to compute energy. Good-Bye.";
        throw std::invalid_argument("Invalid choice of derivation operator.");
    }

    // Perform derivation using multi-threading (if available)
    // find number of available concurrent threads (returned as uint, convert to int);
    // clamp number of threads to maximum number of rows (more would not make sense)
    auto numThreads = std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, grayscaleImage.rows);
    // declare a vector to manage threads
    auto threads = std::vector<std::thread>{};

    // use threads to perform averaging
    for(int t = 0; t < numThreads; ++t)
    {
        // devide image into segments to be averaged separately in parallel
        // calculate start and end of matrix interval per thread
        int start = grayscaleImage.rows * t / numThreads;
        int end = grayscaleImage.rows * (t+1) / numThreads;

        // start the current thread, add to vector
        threads.emplace_back([start, end, &grayscaleImage, &energyField, &verticalOperator, &horizontalOperator] ()
        {
            int verticalDerivative = 0;
            int horizontalDerivative = 0;

            for(int row = start; row < end; ++row)
            {
                for(int col = 0; col < grayscaleImage.cols; ++col)
                {
                    verticalDerivative = 0;
                    horizontalDerivative = 0;

                    for(int offsetRow = 0; offsetRow < 3; ++offsetRow)
                    {
                        for(int offsetCol = 0; offsetCol < 3; ++offsetCol)
                        {
                            verticalDerivative += verticalOperator[offsetRow][offsetCol] * clampImageBorder<uchar>(grayscaleImage, row + offsetRow-1, col + offsetCol-1) ;
                            horizontalDerivative += horizontalOperator[offsetRow][offsetCol] * clampImageBorder<uchar>(grayscaleImage, row + offsetRow-1, col + offsetCol-1);
                        }
                    }
                    // Calculate the gradients via Euclidean norm
                    // Scale to [0, 255] (i.e. [0, max(uchar)]), by dividing gradients by 3, calculating the Euclidean length, and deviding by sqrt(2);
                    // Further clamp result to the interval covered by uchar to prevent over-/underflow due to floating point arithmetic.
                    energyField.at<uchar>(row,col) = static_cast<uchar>(std::clamp(
                                                                        std::sqrt(horizontalDerivative*horizontalDerivative / 9
                                                                                + verticalDerivative * verticalDerivative / 9 )   / std::sqrt(2.0),
                                                                        0.0,
                                                                        static_cast<double>(std::numeric_limits<uchar>::max())));

                    // Calculate mere sum of absolute gradient values;
                    // Scale down to [0, max(uchar)] by deviding the sum by 2 (combined division by 6)
                    energyField.at<uchar>(row,col) = static_cast<uchar>((std::abs(horizontalDerivative) + std::abs(verticalDerivative)) / 5);
              }
            }
        });
    }

    for (auto& thread : threads)
        thread.join();

    return energyField;
}

