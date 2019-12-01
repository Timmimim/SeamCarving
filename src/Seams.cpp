#include "Seams.h"
#include <iostream>

Seams::Seams()
{

}

std::vector<int> Seams::horizontalSeam(const cv::Mat& energyImage)
{
    const int rows = energyImage.rows; // n
    const int cols = energyImage.cols; // m
    //init M[n x m] for dynamic programming
    int M[rows][cols];

    for(int i=0; i < rows; i++)
    {
        M[i][0] = energyImage.at<uchar>(i,0);
    }

    //Aufbau von M im Zuge dynamischer Programmierung
    //Sonderfall am Rand von M, da i-1 < 0 bzw i+1 = n sein kann (index out of bounce stuff)
    for(int j=1; j<cols; j++)
    {
        //Sonderfall i-1 zu klein
        M[0][j] = energyImage.at<uchar>(0,j) +
                std::min(M[0][j-1],M[1][j-1]);
        //Standard (alle drei benachbarten Pixel in Reihe i-1 kommen in betracht)
        for(int i=1; i<rows-1; i++)
        {
            M[i][j] = energyImage.at<uchar>(i,j) +
                    std::min(M[i-1][j-1], std::min(M[i][j-1],M[i+1][j-1]));
        }
        //Sonderfall j+1 zu groß
        M[rows-1][j] = energyImage.at<uchar>(rows-1,j) +
                std::min(M[rows-2][j-1],M[rows-1][j-1]);
    }

    //finde Seam mit minimaler Energy Summe (minimaler Wert in letzter Spalte j=m)
    int minEnergyCoordinate = 0;
    int currentMinimumEnegry = M[minEnergyCoordinate][cols-1];
    for(int i=1; i<rows; i++)
    {
        if(M[i][cols-1] < currentMinimumEnegry)
        {
            currentMinimumEnegry = M[i][cols-1];
            minEnergyCoordinate = i;
        }
    }

    //init seam array(Index für Zugriff auf seam entspricht Spalte des Pixels, gespeicherter Wert dessen Reihe)
    //Information:
    auto seam = std::vector<int>(static_cast<size_t>(energyImage.cols), 0);
    seam[cols-1] = minEnergyCoordinate;

    //backtrack seam pixels in M, comparing the three pixels bordering on the latest pixel in the track
    for(int j = cols-2; j >= 0; --j)
    {
        // if seam already runs along left-side image border, only compare two values
        if (minEnergyCoordinate == 0)
        {
            if(M[minEnergyCoordinate][j+1] <= M[minEnergyCoordinate+1][j+1])
                seam[j] = minEnergyCoordinate;
            else
            {
                ++minEnergyCoordinate;
                seam[j] = minEnergyCoordinate;
            }
        }
        // if seam already runs along right-side image border, only compare two values
        else if(minEnergyCoordinate == rows-1)
        {
            if(M[minEnergyCoordinate][j+1] <= M[minEnergyCoordinate-1][j+1])
                seam[j] = minEnergyCoordinate;
            else
            {
                --minEnergyCoordinate;
                seam[j] = minEnergyCoordinate;
            }
        }
        // if seam runs through the image without special cases to be regarded
        else
        {
            if(M[minEnergyCoordinate][j+1] <= M[minEnergyCoordinate-1][j+1] && M[minEnergyCoordinate][j+1] <= M[minEnergyCoordinate+1][j+1])
            {
                seam[j] = minEnergyCoordinate;
            }
            else if (M[minEnergyCoordinate][j+1] < M[minEnergyCoordinate-1][j+1])
            {
                ++minEnergyCoordinate;
                seam[j] = minEnergyCoordinate;
            }
            else
                --minEnergyCoordinate;
                seam[j] = minEnergyCoordinate;
        }
    }
    return seam;
 }

std::vector<int> Seams::verticalSeam(const cv::Mat& energyImage)
{
    const int rows = energyImage.rows; // n
    const int cols = energyImage.cols; // m
    //init M[n x m] for dynamic programming
    int M[rows][cols];

    for(int j=0; j < cols; ++j)
    {
        M[0][j] = energyImage.at<uchar>(0,j);
    }

    //Aufbau von M im Zuge dynamischer Programmierung
    //Sonderfall am Rand von M, da j-1 < 0 bzw j+1 = m sein kann (index out of bounce stuff)
    for(int i=1; i < rows; ++i)
    {
        //Sonderfall j-1 zu klein
        M[i][0] = energyImage.at<uchar>(i,0) +
                std::min(M[i-1][0],M[i-1][1]);
        //Standard (alle drei benachbarten Pixel in Reihe i-1 kommen in betracht)
        for(int j=1; j<cols-1; j++)
        {
            M[i][j] = energyImage.at<uchar>(i,j) +
                    std::min(M[i-1][j-1], std::min(M[i-1][j],M[i-1][j+1]));
        }
        //Sonderfall j+1 zu groß
        M[i][cols-1] = energyImage.at<uchar>(i,cols-1) +
                std::min(M[i-1][cols-2],M[i-1][cols-1]);
    }

    //finde Seam mit minimaler Energy Summe (minimaler Wert in letzter Reihe i=n)
    int minEnergyCoordinate = 0;
    int currentMinimumEnegry = M[rows-1][minEnergyCoordinate];
    for(int j=1; j<cols; j++)
    {
        if(M[rows-1][j] < currentMinimumEnegry)
        {
            currentMinimumEnegry = M[rows-1][j];
            minEnergyCoordinate = j;
        }
    }

    //init seam array(Index für Zugriff auf seam entspricht Reihe des Pixels, gespeicherter Wert dessen Spalte)
    auto seam = std::vector<int>(static_cast<size_t>(energyImage.rows), 0);
    seam[rows-1] = minEnergyCoordinate;

    //backtrack seam pixels in M, comparing the three pixels bordering on the latest pixel in the track
    for(int i = rows-2; i > -1; --i)
    {        
        // if seam already runs along left-side image border, only compare two values
        if (minEnergyCoordinate == 0)
        {
            if(M[i+1][minEnergyCoordinate] <= M[i+1][minEnergyCoordinate+1])
                seam[i] = minEnergyCoordinate;
            else
            {
                ++minEnergyCoordinate;
                seam[i] = minEnergyCoordinate;
            }
        }
        // if seam already runs along right-side image border, only compare two values
        else if(minEnergyCoordinate == cols-1)
        {
            if(M[i+1][minEnergyCoordinate] <= M[i+1][minEnergyCoordinate-1])
                seam[i] = minEnergyCoordinate;
            else
            {
                --minEnergyCoordinate;
                seam[i] = minEnergyCoordinate;
            }
        }
        // if seam runs through the image without special cases to be regarded
        else
        {
            if(M[i+1][minEnergyCoordinate] <= M[i+1][minEnergyCoordinate-1] && M[i+1][minEnergyCoordinate] <= M[i+1][minEnergyCoordinate+1])
            {
                seam[i] = minEnergyCoordinate;
            }
            else if (M[i+1][minEnergyCoordinate] < M[i+1][minEnergyCoordinate-1])
            {
                ++minEnergyCoordinate;
                seam[i] = minEnergyCoordinate;
            }
            else
                --minEnergyCoordinate;
                seam[i] = minEnergyCoordinate;
        }
    }

    return seam;
}
