#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QStatusBar>

#include "ImageReader.hpp"
#include "QtOpencvCore.hpp"
#include "EnergyFunctions.hpp"
#include "Seams.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    
    /* Konstruktor */
    explicit MainWindow(QWidget *parent = 0);
    
    /* Destruktor */
    ~MainWindow();
    
private slots:  
    
    /* Funktionen werden ausgeloest, wenn auf den entsprechenden Button geklickt wird */
    void on_pbOpenImage_clicked();
    void on_pbComputeSeams_clicked();
    void on_pbRemoveSeams_clicked();
    void on_pbDoubleSeams_clicked();
    
private:

    
    /* UI Komponenten */
    QWidget     *centralWidget;
    
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_3;
    
    QPushButton *pbOpenImage;
    QPushButton *pbRemoveSeams;
    QPushButton *pbDoubleSeams;

    QPushButton *pbComputeSeams;
    QCheckBox   *cbShowSeams;
    QComboBox   *comboChooseOperator;

    QStringList operatorChoices;

    QLabel      *lCaption;
    QLabel      *comboCaption;
    QLabel      *lCols;
    QLabel      *lRows;
    
    QSpinBox    *sbCols;
    QSpinBox    *sbRows;
    
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    /*****************************************/
    
    /* Originalbild */
    cv::Mat         originalImage;

    /* Eventuell weitere Klassenattribute */
    cv::Mat         grayscaleImage;
    cv::Mat         energyImage;
    cv::Mat         origImageWithHighlightedSeams;
    cv::Mat         carvedImage;
    cv::Mat         extendedImage;

    std::vector<std::vector<int>> horizontalSeams;
    std::vector<std::vector<int>> verticalSeams;

    bool onlySeamsInOneDimensionCalculated = false;

    /* Methode initialisiert die UI */
    void setupUi();
    
    /* Methoden aktivieren bzw. deaktivieren die UI */
    void enableGUI();
    void disableGUI();
};

#endif // MAINWINDOW_HPP
