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

    // clear out previously calculated seams and reserve memory for new batch
    verticalSeams.clear();
    verticalSeams.reserve(static_cast<size_t>(colsToRemove));
    horizontalSeams.clear();
    horizontalSeams.reserve(static_cast<size_t>(rowsToRemove));

    // build a grayscale-, and from it an energy-image, and display the latter
    grayscaleImage = EnergyFunctions::convertToGrayscale(originalImage);
    energyImage = EnergyFunctions::computeEnergy(grayscaleImage, comboChooseOperator->currentIndex());
    cv::namedWindow("Energy Image", cv::WINDOW_KEEPRATIO);
    cv::imshow("Energy Image", energyImage);

    // create a clone of the original image to display the calculated seams in
    origImageWithHighlightedSeams = originalImage.clone();

    // create as many vertical seams as columns are to be removed
    for(int verticalSeam = 0; verticalSeam < colsToRemove; ++verticalSeam)
    {
        // re-calculate energy (must be done on every iteration, as the grayscale image is changed)
        // this is done to prevent chosing the same seam multiple times (force new seam)
        energyImage = EnergyFunctions::computeEnergy(grayscaleImage, comboChooseOperator->currentIndex());

        // calculate seam
        auto seamuelLJackson = Seams::verticalSeam(energyImage);
        // exclude the seam from the grayscale image, to not find it twice
        Seams::excludeVerticalSeam<uchar>(grayscaleImage, seamuelLJackson);

        // shift seam, if previously found/removed seam had smaller coordinat
        // (i.e. the new coordinate is too low for the actual position in the yet-untouched original image)
        for(int seamRowCoordinate = 0; seamRowCoordinate < energyImage.rows; ++seamRowCoordinate)
            for (int seam = verticalSeams.size()-2; seam >= 0; --seam) {
                if(verticalSeams[static_cast<size_t>(seam)][static_cast<size_t>(seamRowCoordinate)] <= seamuelLJackson[static_cast<size_t>(seamRowCoordinate)])
                    ++seamuelLJackson[static_cast<size_t>(seamRowCoordinate)];
            }

        verticalSeams.push_back(seamuelLJackson);

        if(cbShowSeams->isChecked())
        {
            for(int seamRowCoordinate = 0; seamRowCoordinate < energyImage.rows; ++seamRowCoordinate)
            {
                if(origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamRowCoordinate, seamuelLJackson[seamRowCoordinate])[0] == 255)
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamRowCoordinate, seamuelLJackson[seamRowCoordinate])[2] += 255;
                else
                {
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamRowCoordinate, seamuelLJackson[seamRowCoordinate])[0] = 0;
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamRowCoordinate, seamuelLJackson[seamRowCoordinate])[1] = 0;
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamRowCoordinate, seamuelLJackson[seamRowCoordinate])[2] = 255;
                }
            }
        }
    }

    grayscaleImage = EnergyFunctions::convertToGrayscale(originalImage);
    for(int horizontalSeam = 0; horizontalSeam < rowsToRemove; ++horizontalSeam)
    {
        energyImage = EnergyFunctions::computeEnergy(grayscaleImage, comboChooseOperator->currentIndex());

        auto seamuelLJackson = Seams::horizontalSeam(energyImage);

        Seams::excludeHorizontalSeam<uchar>(grayscaleImage, seamuelLJackson);
        horizontalSeams.push_back(seamuelLJackson);

        for(int seamColCoordinate = 0; seamColCoordinate < energyImage.cols; ++seamColCoordinate)
            for (int seam = static_cast<int>(horizontalSeams.size()-2); seam >= 0; --seam) {
                if(horizontalSeams[static_cast<size_t>(seam)][static_cast<size_t>(seamColCoordinate)] <= seamuelLJackson[static_cast<size_t>(seamColCoordinate)])
                    ++seamuelLJackson[static_cast<size_t>(seamColCoordinate)];
            }

        if(cbShowSeams->isChecked())
        {
            for(int seamColCoordinate = 0; seamColCoordinate < energyImage.cols; ++seamColCoordinate)
            {
                if(origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[2] == 255)
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[0] += 255;
                else
                {
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[0] = 255;
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[1] = 0;
                    origImageWithHighlightedSeams.at<cv::Vec<uchar, 3>>(seamuelLJackson[seamColCoordinate], seamColCoordinate)[2] = 0;
                }
            }
        }
    }

    if(cbShowSeams->isChecked())
    {
        cv::namedWindow("Original WITH Seams", cv::WINDOW_KEEPRATIO);
        cv::imshow("Original WITH Seams", origImageWithHighlightedSeams);
    }
    pbRemoveSeams->setEnabled(true);
    if(onlySeamsInOneDimensionCalculated)
        pbDoubleSeams->setEnabled(true);
    else
        pbDoubleSeams->setEnabled(false);
}

void MainWindow::on_pbRemoveSeams_clicked()
{
    carvedImage = originalImage.clone();

    for(const auto& seam : verticalSeams)
        Seams::removeVerticalSeam<cv::Vec<uchar, 3>>(carvedImage, seam);    

    for(const auto& seam : horizontalSeams)
        Seams::removeHorizontalSeam<cv::Vec<uchar, 3>>(carvedImage, seam);

    Seams::resizeImage(carvedImage, horizontalSeams.size(), verticalSeams.size());

    //verticalSeams.clear();
    //horizontalSeams.clear();

    cv::namedWindow("Carved Image", cv::WINDOW_KEEPRATIO);
    cv::imshow("Carved Image", carvedImage);
}

void MainWindow::on_pbDoubleSeams_clicked()
{
    extendedImage = originalImage.clone();

    for (const auto& seam : verticalSeams)
    {
        auto extensionCol = cv::Mat::zeros(extendedImage.rows, 1, 16);
        //extendedImage = extendedImage(cv::Range(0, extendedImage.rows), cv::Range(0, extendedImage.cols+1));
        cv::hconcat(extendedImage, extensionCol, extendedImage);
        Seams::duplicateVerticalSeam<cv::Vec<uchar, 3>>(extendedImage, seam);
    }

    for(const auto& seam : horizontalSeams)
    {
        auto extensionRow = cv::Mat::zeros(1, extendedImage.cols, 16);
        //extendedImage = extendedImage(cv::Range(0, extendedImage.rows+1), cv::Range(0, extendedImage.cols));
        extendedImage.push_back(extensionRow);
        //cv::vconcat(extendedImage, extensionRow, extendedImage);  // not working, causes error after second addition...
        Seams::duplicateHorizontalSeam<cv::Vec<uchar, 3>>(extendedImage, seam);
    }

    cv::namedWindow("Extended Image", cv::WINDOW_KEEPRATIO);
    cv::imshow("Extended Image", extendedImage);

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
    connect(pbDoubleSeams,  &QPushButton::clicked, this, &MainWindow::on_pbDoubleSeams_clicked);

    connect(sbCols, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i)
    {
        if(i == 0)
            onlySeamsInOneDimensionCalculated = true;
        else if(sbRows->value() != 0)
            onlySeamsInOneDimensionCalculated = false;
    });
    connect(sbRows, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i)
    {
        if(i == 0)
            onlySeamsInOneDimensionCalculated = true;
        else if(sbCols->value() != 0)
            onlySeamsInOneDimensionCalculated = false;
    });
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
    //pbRemoveSeams->setEnabled(true);
    //pbDoubleSeams->setEnabled(true);
    
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
