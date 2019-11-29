#include "seams.h"

seams::seams()
{

}

int* seams::horizontalSeam(const cv::Mat& energyImage)
{
    const int n = energyImage.rows;
    const int m = energyImage.cols;
    //init M[n x m] for dynamic programming
    int M[n][m];

    for(int i=0; i < n; i++)
    {
        M[i][0] = energyImage.at<int>(i,0);
    }

    //Aufbau von M im Zuge dynmaischer Programmierung
    //Sonderfall am Rand von M, da i-1 < 0 bzw i+1 = n sein kann (index out of bounce stuff)
    for(int i=1; i<n; i++)
    {
        //Sondefall j-1 zu klein
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
        //Basiswahl
        seam[i] = sj;
        s = M[i][sj];

        //Indexrange nach unten beachten und Vergleich
        if(sj-1 > -1 && M[i][sj-1] < s)
        {
            seam[i] = sj-1;
            s = M[i][sj-1];
        }

        //Indexrange nach oben beachten und Vergleich
        if(sj+1 < m && M[i][sj+1] < s)
        {
            seam[i] = sj+1;
            s = M[i][sj+1];
        }

    }
}

int* seams::verticalSeam(const cv::Mat& energyImage)
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
        //Sondefall j-1 zu klein
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
        //Basiswahl
        seam[i] = sj;
        s = M[i][sj];

        //Indexrange nach unten beachten und Vergleich
        if(sj-1 > -1 && M[i][sj-1] < s)
        {
            seam[i] = sj-1;
            s = M[i][sj-1];
        }

        //Indexrange nach oben beachten und Vergleich
        if(sj+1 < m && M[i][sj+1] < s)
        {
            seam[i] = sj+1;
            s = M[i][sj+1];
        }
    }

    return seam;
}
