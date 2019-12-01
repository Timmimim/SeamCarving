#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    static EnergyFunctions energy;
    static Seams seams;
    /* Initialisiere die UI Komponenten */
    setupUi();
}

MainWindow::~MainWindow()
{
    /* loesche die UI Komponenten */
    delete centralWidget;    
    
    /* schliesse alle offenen Fenster */
    cv::destroyAllWindows();
}

/* Methode oeffnet ein Bild und zeigt es in einem separaten Fenster an */
void MainWindow::on_pbOpenImage_clicked()
{
    /* oeffne Bild mit Hilfe eines Dateidialogs */
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image...", QString(), QString("Images *.png *.jpg *.tiff *.tif"));
    
    /* wenn ein gueltiger Dateipfad angegeben worden ist... */
    if(!imagePath.isNull() && !imagePath.isEmpty())
    {
        /* ...lese das Bild ein */
        cv::Mat img = ImageReader::readImage(QtOpencvCore::qstr2str(imagePath));
        
        /* wenn das Bild erfolgreich eingelesen worden ist... */
        if(!img.empty())
        {
            /* ...merke das Originalbild... */
            originalImage = img;
            
            /* ...aktiviere das UI... */
            enableGUI();
            
            /* ...zeige das Originalbild in einem separaten Fenster an */
            cv::namedWindow("Original Image", cv::WINDOW_KEEPRATIO);
            cv::imshow("Original Image", originalImage); 
        }
        else
        {
            /* ...sonst deaktiviere das UI */
            disableGUI();
        }
    }
}

void MainWindow::on_pbComputeSeams_clicked()
{
    /* Anzahl der Spalten, die entfernt werden sollen */
    int colsToRemove = sbCols->value();
    
    /* Anzahl der Zeilen, die entfernt werden sollen */
    int rowsToRemove = sbRows->value();
    
    cv::Mat grayscaleImage = EnergyFunctions::convertToGrayscale(originalImage);
    cv::Mat energyImage = EnergyFunctions::computeEnergy(grayscaleImage, comboChooseOperator->currentIndex());
    cv::namedWindow("Energy Image", cv::WINDOW_KEEPRATIO);
    cv::imshow("Energy Image", energyImage);

    int* seamuelLJackson;
    seamuelLJackson = Seams::horizontalSeam(energyImage);
    std::cout << "First x rows of found seam:" << std::endl;
    std::cout << seamuelLJackson[0] << std::endl;
    std::cout << seamuelLJackson[1] << std::endl;
    std::cout << seamuelLJackson[2] << std::endl;
    std::cout << seamuelLJackson[3] << std::endl;
    std::cout << seamuelLJackson[4] << std::endl;
    std::cout << seamuelLJackson[5] << std::endl;
    std::cout << seamuelLJackson[6] << std::endl;
    std::cout << seamuelLJackson[7] << std::endl;
    std::cout << seamuelLJackson[8] << std::endl;
    std::cout << seamuelLJackson[9] << std::endl;
    std::cout << seamuelLJackson[10] << std::endl;

    if(cbShowSeams->isChecked())
    {
        cv::Mat paintedSeams = originalImage.clone();
        for(int seamColCoordinate = 0; seamColCoordinate < originalImage.cols; ++seamColCoordinate)
        {
            paintedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[0] = 255;
            paintedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[1] = 0;
            paintedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[2] = 0;

        }
        cv::namedWindow("Original WITH Seams", cv::WINDOW_KEEPRATIO);
        cv::imshow("Original WITH Seams", paintedSeams);
    }



}

void MainWindow::on_pbRemoveSeams_clicked()
{
    /* .............. */
}

void MainWindow::on_pbDoubleSeams_clicked()
{
    /* .............. */
}

void MainWindow::setupUi()
{
    /* Boilerplate code */
    /*********************************************************************************************/
    resize(260, 330);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(260, 330));
    setMaximumSize(QSize(260, 330));
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString("centralWidget"));
    
    horizontalLayout = new QHBoxLayout(centralWidget);
    verticalLayout = new QVBoxLayout();
    
    pbOpenImage = new QPushButton(QString("Open Image"), centralWidget);
    verticalLayout->addWidget(pbOpenImage);
    
    
    verticalLayout_3 = new QVBoxLayout();
    lCaption = new QLabel(QString("#Pixels to Remove/Add:"), centralWidget);
    lCaption->setEnabled(false);
    verticalLayout_3->addWidget(lCaption);
    
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString("horizontalLayout_3"));
    lCols = new QLabel(QString("Cols"), centralWidget);
    lCols->setEnabled(false);
    lRows = new QLabel(QString("Rows"), centralWidget);
    lRows->setEnabled(false);
    horizontalLayout_3->addWidget(lCols);
    horizontalLayout_3->addWidget(lRows);
    verticalLayout_3->addLayout(horizontalLayout_3);
    
    horizontalLayout_2 = new QHBoxLayout();
    sbCols = new QSpinBox(centralWidget);
    sbCols->setEnabled(false);
    horizontalLayout_2->addWidget(sbCols);
    sbRows = new QSpinBox(centralWidget);
    sbRows->setEnabled(false);
    horizontalLayout_2->addWidget(sbRows);
    verticalLayout_3->addLayout(horizontalLayout_2);
    verticalLayout->addLayout(verticalLayout_3);
    
    horizontalLayout_4 = new QHBoxLayout();
    comboChooseOperator = new QComboBox(centralWidget);
    comboCaption = new QLabel(QString("Choose Energy Operator"), centralWidget);
    comboCaption->setEnabled(false);
    comboChooseOperator->setObjectName(QString("operatorChoice"));
    comboChooseOperator->setEnabled(false);

    operatorChoices = QStringList() << tr("Sobel") << tr("Previtt");
    comboChooseOperator->addItems(operatorChoices);

    horizontalLayout_4->addWidget(comboCaption);
    horizontalLayout_4->addWidget(comboChooseOperator);
    verticalLayout->addLayout(horizontalLayout_4);

    pbComputeSeams = new QPushButton(QString("Compute Seams"), centralWidget);
    pbComputeSeams->setEnabled(false);
    verticalLayout->addWidget(pbComputeSeams);

    cbShowSeams = new QCheckBox(QString("Show Seams in Original"), centralWidget);
    cbShowSeams->setEnabled(false);
    verticalLayout->addWidget(cbShowSeams);
    
    pbRemoveSeams = new QPushButton(QString("Remove Seams"), centralWidget);
    pbRemoveSeams->setEnabled(false);
    verticalLayout->addWidget(pbRemoveSeams);

    pbDoubleSeams = new QPushButton(QString("Double Seams"), centralWidget);
    pbDoubleSeams->setEnabled(false);
    verticalLayout->addWidget(pbDoubleSeams);
    
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    verticalLayout->addItem(verticalSpacer);
    horizontalLayout->addLayout(verticalLayout);
    
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    setCentralWidget(centralWidget);
    /*********************************************************************************************/
    
    
    /* Verbindung zwischen den Buttonklicks und den Methoden, die beim jeweiligen Buttonklick ausgefuehrt werden sollen */
    connect(pbOpenImage,    &QPushButton::clicked, this, &MainWindow::on_pbOpenImage_clicked);  
    connect(pbComputeSeams, &QPushButton::clicked, this, &MainWindow::on_pbComputeSeams_clicked); 
    connect(pbRemoveSeams,  &QPushButton::clicked, this, &MainWindow::on_pbRemoveSeams_clicked);
}

void MainWindow::enableGUI()
{
    lCaption->setEnabled(true);
    
    lCols->setEnabled(true);
    lRows->setEnabled(true);
    
    sbCols->setEnabled(true);
    sbRows->setEnabled(true);

    comboCaption->setEnabled(true);
    comboChooseOperator->setEnabled(true);

    pbComputeSeams->setEnabled(true);
    cbShowSeams->setEnabled(true);
    pbRemoveSeams->setEnabled(true);
    pbDoubleSeams->setEnabled(true);
    
    sbRows->setMinimum(0);
    sbRows->setMaximum(originalImage.rows);
    sbRows->setValue(2);
    
    sbCols->setMinimum(0);
    sbCols->setMaximum(originalImage.cols);
    sbCols->setValue(2);
}

void MainWindow::disableGUI()
{
    lCaption->setEnabled(false);
    
    lCols->setEnabled(false);
    lRows->setEnabled(false);
    
    sbCols->setEnabled(false);
    sbRows->setEnabled(false);

    comboCaption->setEnabled(false);
    comboChooseOperator->setEnabled(false);
    
    pbComputeSeams->setEnabled(false);
    cbShowSeams->setEnabled(false);
    pbRemoveSeams->setEnabled(false);
    pbDoubleSeams->setEnabled(false);
}
