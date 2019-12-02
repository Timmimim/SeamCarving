#ifndef SEAMS_H
#define SEAMS_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

class Seams
{
public:
    Seams();
    static std::vector<int> horizontalSeam(const cv::Mat& energyImage);
    static std::vector<int> verticalSeam(const cv::Mat& energyImage);

    static void resizeImage(cv::Mat& image, const int numRowsRemoved, const int numColsRemoved);

    template<typename T>
    static void removeHorizontalSeam(cv::Mat& image, const std::vector<int>& seam)
    {
        if(image.cols != static_cast<int>(seam.size()))
        {
            std::cout << "ERROR: Seam length and image dimensions (width) don't match. Seam removal failed." << std::endl;
            throw std::invalid_argument("Horizontal seam removal failed due to badly matching input dimensions.");
        }

        // for all columns, and starting from the row specified by the current seam, shift all pixels to the left (row -> row - 1)
        for(int col = 0; col < image.cols; ++col)
            for(int row = seam[static_cast<size_t>(col)]+1; row < image.rows; ++row)
                image.at<T>(row-1, col) = image.at<T>(row,col);

        // create a copy of the image, with the last column removed, so effectively cropping the image by one seam-width
        //image = image(cv::Range(0, image.rows-1), cv::Range(0,image.cols));
    }

    template<typename T>
    static void removeVerticalSeam(cv::Mat& image, const std::vector<int>& seam)
    {
        if(image.rows != static_cast<int>(seam.size()))
        {
            std::cout << "ERROR: Seam length and image dimensions (height) don't match. Seam removal failed." << std::endl;
            throw std::invalid_argument("Vertical seam removal failed due to badly matching input dimensions.");
        }

        // for all rows, and starting from the column specified by the current seam, shift all pixels to the left (column -> column - 1)
        for(int row = 0; row < image.rows; ++row)
            for(int col = seam[static_cast<size_t>(row)]+1; col < image.cols; ++col)
                image.at<T>(row, col-1) = image.at<T>(row,col);

        // create a copy of the image, with the last column removed, so effectively cropping the image by one seam-width
        //image = image(cv::Range(0, image.rows), cv::Range(0,image.cols-1));
    }

    template<typename T>
    static void duplicateHorizontalSeam(cv::Mat& extendedImage, const std::vector<int>& seam)
    {
        // for all rows, and starting from the column specified by the current seam, shift all pixels to the left (column -> column - 1)
        for(int col = 0; col < extendedImage.cols; ++col)
            for(int row = extendedImage.rows-2; row >= seam[static_cast<size_t>(col)]; --row)
                extendedImage.at<T>(row+1, col) = extendedImage.at<T>(row,col);
    }

    template<typename T>
    static void duplicateVerticalSeam(cv::Mat& extendedImage, const std::vector<int>& seam)
    {
        // for all rows, and starting from the column specified by the current seam, shift all pixels to the left (column -> column - 1)
        for(int row = 0; row < extendedImage.rows; ++row)
            for(int col = extendedImage.cols-2; col >= seam[static_cast<size_t>(row)]; --col)
                extendedImage.at<T>(row, col+1) = extendedImage.at<T>(row,col);
    }

    template<typename T>
    static void excludeHorizontalSeam(cv::Mat& image, const std::vector<int>& seam)
    {
        if(image.cols != static_cast<int>(seam.size()))
        {
            std::cout << "ERROR: Seam length and image dimensions (width) don't match. Seam removal failed." << std::endl;
            throw std::invalid_argument("Horizontal seam removal failed due to badly matching input dimensions.");
        }

        // for all columns, and starting from the row specified by the current seam, shift all pixels to the left (row -> row - 1)
        for(int col = 0; col < image.cols; ++col)
            for(int row = seam[static_cast<size_t>(col)]+1; row < image.rows; ++row)
                image.at<T>(row-1, col) = image.at<T>(row,col);

        // iterate over the last column, and add maximum energy to make it highly unlikely to chose a seam from this row
        bool black = true;
        for(int col = 0; col < image.cols; ++col)
        {
            if((col % 2) == 0)
                black = !black;
            if(black)
                image.at<T>(image.rows-1, col) = 1000;
            else
                image.at<T>(image.rows-1, col) = 700;
        }
    }

    template<typename T>
    static void excludeVerticalSeam(cv::Mat& image, const std::vector<int>& seam)
    {
        if(image.rows != static_cast<int>(seam.size()))
        {
            std::cout << "ERROR: Seam length and image dimensions (height) don't match. Seam removal failed." << std::endl;
            throw std::invalid_argument("Vertical seam removal failed due to badly matching input dimensions.");
        }

        // for all rows, and starting from the column specified by the current seam, shift all pixels to the left (column -> column - 1)
        for(int row = 0; row < image.rows; ++row)
            for(int col = seam[static_cast<size_t>(row)]+1; col < image.cols; ++col)
                image.at<T>(row, col-1) = image.at<T>(row,col);

        // iterate over the last column, and add maximum energy to make it highly unlikely to chose a seam from this row
        bool black = true;
        for(int row = 0; row < image.rows; ++row)
        {
            if((row % 2) == 0)
                black = !black;
            if(black)
                image.at<T>(row, image.cols-1) = 1000;
            else
                image.at<T>(row, image.cols-1) = 700;
        }
    }
};

#endif // SEAMS_H
