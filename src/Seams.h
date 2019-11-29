#ifndef SEAMS_H
#define SEAMS_H

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>

class Seams
{
public:
    Seams();
    static int* horizontalSeam(const cv::Mat& energyImage);
    static int* verticalSeam(const cv::Mat& energyImage);
};

#endif // SEAMS_H
