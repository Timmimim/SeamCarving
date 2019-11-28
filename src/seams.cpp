#include "seams.h"

seams::seams()
{

}

int* seams::horizontalSeam(const cv::Mat& energyImage)
{
    //traversiere Energybild ab Spalte 2 (l -> r)
    int seam[3];
    return seam;
}

int* seams::verticalSeam(const cv::Mat& energyImage)
{
    //init M[n x m] for dynamic programming
    int M[energyImage.rows][energyImage.cols];

    for(int j=0; j < M.cols; j++)
    {
        M[0][j] = energyImage[0][j];
    }

    //Aufbau von M im Zuge dynmaischer Programmierung
    //Sonderfall am Rand von M, da j-1 < 0 bzw j+1 = M.cols sein kann (index out of bounce stuff)
    for(int i=1; i<M.rows; i++)
    {
        //Sondefall j-1 zu klein
        M[i][0] = energyImage[i][0] +
                std::min(M[i-1][0],M[i-1][1]);
        //Standard (alle drei benachbarten Pixel in Reihe i-1 kommen in betracht)
        for(int j=1; j<M.cols-1; j++)
        {
            M[i][j] = energyImage[i][j] +
                    std::min(M[i-1][j-1],M[i-1][j],M[i-1][j+1]);
        }
        //Sonderfall j+1 zu groß
        M[i][M.cols-1] = energyImage[i][M.cols-1] +
                std::min(M[i-1][M.cols-2],M[i-1][M.cols-1]);
    }

    //finde seam mit minimaler energy Summe (minimaler Wert in letzter Reihe)
    int sj = 0;
    int s = M[M.rows-1][sj];
    for(int j=1; j<M.cols; j++)
    {
        if(M[M.rows-1][j] < s)
        {
            s = M[M.rows-1][j];
            sj = j;
        }
    }

    //init seam array(Index für Zugriff auf seam entspricht Reihe des Pixels, gespeicherter Wert dessen Spalte)
    int seam[M.rows];
    seam[M.rows-1] = sj;

    //backtrack seam pixels in M
    for(int i = M.rows-2; i > -1; i--)
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
        if(sj+1 < M.cols && M[i][sj+1] < s)
        {
            seam[i] = sj+1;
            s = M[i][sj+1];
        }
    }

    return seam;
}
