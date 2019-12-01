#include "Seams.h"
#include <iostream>

Seams::Seams()
{

}

int* Seams::horizontalSeam(const cv::Mat& energyImage)
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
    uint minEnergyCoordinate = 0;
    uint currentMinimumEnegry = M[minEnergyCoordinate][cols-1];
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
    int* seam = new int[cols];
    seam[cols-1] = minEnergyCoordinate;

    //backtrack seam pixels in M
    for(int j = cols-2; j >= 0; --j)
    {
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
        /**
        //vorige Spalte, gleiche Reihe (Basiswahl)
        std::cout << minEnergyCoordinate << std::endl;
        seam[j] = minEnergyCoordinate;
        currentMinimumEnegry = M[minEnergyCoordinate][j];

        //vorige Spalte, eine Reihe hoch
        //Indexrange beachten und Vergleich
        if((minEnergyCoordinate-1 > -1) && (M[minEnergyCoordinate-1][j] < currentMinimumEnegry))
        {
            std::cout << minEnergyCoordinate+1 << std::endl;
            seam[j] = minEnergyCoordinate-1;
            currentMinimumEnegry = M[minEnergyCoordinate-1][j];
        }

        //vorige Spalte, eine Reihe runter
        //Indexrange beachten und Vergleich
        if((minEnergyCoordinate+1 < rows) && (M[minEnergyCoordinate+1][j] < currentMinimumEnegry))
        {
            std::cout << minEnergyCoordinate+1 << std::endl;
            seam[j] = minEnergyCoordinate+1;
            currentMinimumEnegry = M[minEnergyCoordinate+1][j];
        }
    }

    return seam;
}*/

int* Seams::verticalSeam(const cv::Mat& energyImage)
{

    const int n = energyImage.rows;
    const int m = energyImage.cols;
    //init M[n x m] for dynamic programming
    int M[n][m];

    for(int j=0; j < m; j++)
    {
        M[0][j] = energyImage.at<int>(0,j);
    }

    //Aufbau von M im Zuge dynmaischer Programmierung
    //Sonderfall am Rand von M, da j-1 < 0 bzw j+1 = m sein kann (index out of bounce stuff)
    for(int i=1; i<n; i++)
    {
        //Sonderfall j-1 zu klein
        M[i][0] = energyImage.at<int>(i,0) +
                std::min(M[i-1][0],M[i-1][1]);
        //Standard (alle drei benachbarten Pixel in Reihe i-1 kommen in betracht)
        for(int j=1; j<m-1; j++)
        {
            M[i][j] = energyImage.at<int>(i,j) +
                    std::min(M[i-1][j-1], std::min(M[i-1][j],M[i-1][j+1]));
        }
        //Sonderfall j+1 zu groß
        M[i][m-1] = energyImage.at<int>(i,m-1) +
                std::min(M[i-1][m-2],M[i-1][m-1]);
    }

    //finde seam mit minimaler energy Summe (minimaler Wert in letzter Reihe i=n)
    int sj = 0;
    int s = M[n-1][sj];
    for(int j=1; j<m; j++)
    {
        if(M[n-1][j] < s)
        {
            s = M[n-1][j];
            sj = j;
        }
    }

    //init seam array(Index für Zugriff auf seam entspricht Reihe des Pixels, gespeicherter Wert dessen Spalte)
    int seam[n];
    seam[n-1] = sj;

    //backtrack seam pixels in M
    for(int i = n-2; i > -1; i--)
    {
        //vorige Reihe, gleiche Spalte (Basiswahl)
        seam[i] = sj;
        s = M[i][sj];

        //vorige Reihe, eine Spalte nach links
        //Indexrange beachten und Vergleich
        if(sj-1 > -1 && M[i][sj-1] < s)
        {
            seam[i] = sj-1;
            s = M[i][sj-1];
        }

        //vorige Reihe, eine Spalte nach rechts
        //Indexrange beachten und Vergleich
        if(sj+1 < m && M[i][sj+1] < s)
        {
            seam[i] = sj+1;
            s = M[i][sj+1];
        }
    }

    return seam;
}
