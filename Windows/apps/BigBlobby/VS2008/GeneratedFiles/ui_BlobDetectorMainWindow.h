/********************************************************************************
** Form generated from reading UI file 'BlobDetectorMainWindow.ui'
**
** Created: Wed Sep 28 14:40:21 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOBDETECTORMAINWINDOW_H
#define UI_BLOBDETECTORMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlobDetectorMainWindowClass
{
public:
    QAction *action_saveSettingsToFile;
    QAction *action_quit;
    QAction *action_showAllImages;
    QAction *action_showBackgroundImage;
    QAction *action_showFilterImages;
    QAction *action_showSourceImage;
    QAction *action_flipImageVertical;
    QAction *action_flipImageHorizontal;
    QAction *action_openCameraSettings;
    QAction *action_useCamera;
    QAction *action_useTestVideo;
    QAction *action_advancedOptions;
    QAction *action_findNextCamera;
    QAction *action_useTuioUdp;
    QAction *action_useFlashXml;
    QAction *action_useBinaryTop;
    QAction *action_openAboutBox;
    QAction *action_openHelpInWebBrowser;
    QAction *actionShow_Frames_Per_Second;
    QAction *actionShow_Calculation_Time;
    QAction *action_useGpuShaders;
    QAction *action_useCpu;
    QAction *action_showGraphicsCardInfo;
    QAction *action_;
    QAction *action_preferences;
    QAction *actionBig_Blobs;
    QAction *action_loadSettingsFromFile;
    QAction *action_noOutput;
    QAction *action_showBlobOutlines;
    QAction *action_showIdNumbers;
    QAction *action_trackDarkBlobs;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget_controlPanel;
    QWidget *tab;
    QVBoxLayout *verticalLayout_12;
    QSpacerItem *verticalSpacer_2;
    QTextEdit *textEdit_outputMessage;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_subtractBackground;
    QPushButton *pushButton_calibrateMonitorBoundaries;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_13;
    QWidget *tab_basic;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_6;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_imageThresholdBox;
    QLabel *label_imageThresholdScale;
    QLabel *label_2;
    QLabel *label_minBlobSizeBox;
    QLabel *label_minBlobSizeScale;
    QLabel *label_3;
    QLabel *label_maxBlobSizeBox;
    QLabel *label_maxBlobSizeScale;
    QLabel *label_4;
    QLabel *label_movementFilterScale;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_movementFilterBox;
    QSpacerItem *verticalSpacer_4;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_6;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_22;
    QLabel *label_smoothingValueBox;
    QLabel *label_smoothingValueScale;
    QLabel *label_33;
    QLabel *label_amplifyValueBox;
    QLabel *label_amplifyValueScale;
    QLabel *label_highpassNoiseValueBox;
    QLabel *label_highpassNoiseValueScale;
    QLabel *label_highpassBlurValueBox;
    QLabel *label_highpassBlurValueScale;
    QLabel *label_amplifyStatusBox;
    QLabel *label_highpassNoiseStatusBox;
    QLabel *label_highpassBlurStatusBox;
    QLabel *label_smoothingStatusBox;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_blobs;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_imageThreshold;
    QHBoxLayout *horizontalLayout_8;
    QSlider *slider_imageThreshold;
    QSpinBox *spinBox_imageThreshold;
    QSpacerItem *verticalSpacer_8;
    QGroupBox *groupBox_minBlobSize;
    QHBoxLayout *horizontalLayout_11;
    QSlider *slider_minBlobSize;
    QSpinBox *spinBox_minBlobSize;
    QSpacerItem *verticalSpacer_9;
    QGroupBox *groupBox_maxBlobSize;
    QHBoxLayout *horizontalLayout_7;
    QSlider *slider_maxBlobSize;
    QSpinBox *spinBox_maxBlobSize;
    QSpacerItem *verticalSpacer_10;
    QGroupBox *groupBox_minMovementThreshold;
    QHBoxLayout *horizontalLayout_9;
    QSlider *slider_minMovementThreshold;
    QSpinBox *spinBox_minMovementThreshold;
    QWidget *tab_filters;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *groupBox_smoothing;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_10;
    QRadioButton *radioButton_smoothingOn;
    QRadioButton *radioButton_smoothingOff;
    QHBoxLayout *horizontalLayout_12;
    QSlider *slider_smoothing;
    QSpinBox *spinBox_smoothing;
    QSpacerItem *verticalSpacer_14;
    QGroupBox *groupBox_highpass;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_18;
    QRadioButton *radioButton_highpassOn;
    QRadioButton *radioButton_highpassOff;
    QHBoxLayout *horizontalLayout_17;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_highpassBlur;
    QLabel *label_highpassNoise;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_13;
    QSlider *slider_highpassBlur;
    QSpinBox *spinBox_highpassBlur;
    QHBoxLayout *horizontalLayout_14;
    QSlider *slider_highpassNoise;
    QSpinBox *spinBox_highpassNoise;
    QSpacerItem *verticalSpacer_15;
    QGroupBox *groupBox_amplify;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_16;
    QRadioButton *radioButton_amplifyOn;
    QRadioButton *radioButton_amplifyOff;
    QHBoxLayout *horizontalLayout_15;
    QSlider *slider_amplify;
    QSpinBox *spinBox_amplify;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_source_5;
    QLabel *label_source;
    QFrame *frame_sourceImage;
    QHBoxLayout *horizontalLayout_sourceImage;
    QLabel *label_source_2;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_source_4;
    QLabel *label_tracked;
    QFrame *frame_background;
    QVBoxLayout *horizontalLayout_trackedImage;
    QLabel *label_source_3;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuCamera;
    QMenu *menuNetwork;
    QMenu *menuBlobs;

    void setupUi(QMainWindow *BlobDetectorMainWindowClass)
    {
        if (BlobDetectorMainWindowClass->objectName().isEmpty())
            BlobDetectorMainWindowClass->setObjectName(QString::fromUtf8("BlobDetectorMainWindowClass"));
        BlobDetectorMainWindowClass->resize(923, 433);
        BlobDetectorMainWindowClass->setMinimumSize(QSize(900, 420));
        action_saveSettingsToFile = new QAction(BlobDetectorMainWindowClass);
        action_saveSettingsToFile->setObjectName(QString::fromUtf8("action_saveSettingsToFile"));
        action_quit = new QAction(BlobDetectorMainWindowClass);
        action_quit->setObjectName(QString::fromUtf8("action_quit"));
        action_showAllImages = new QAction(BlobDetectorMainWindowClass);
        action_showAllImages->setObjectName(QString::fromUtf8("action_showAllImages"));
        action_showBackgroundImage = new QAction(BlobDetectorMainWindowClass);
        action_showBackgroundImage->setObjectName(QString::fromUtf8("action_showBackgroundImage"));
        action_showFilterImages = new QAction(BlobDetectorMainWindowClass);
        action_showFilterImages->setObjectName(QString::fromUtf8("action_showFilterImages"));
        action_showSourceImage = new QAction(BlobDetectorMainWindowClass);
        action_showSourceImage->setObjectName(QString::fromUtf8("action_showSourceImage"));
        action_flipImageVertical = new QAction(BlobDetectorMainWindowClass);
        action_flipImageVertical->setObjectName(QString::fromUtf8("action_flipImageVertical"));
        action_flipImageVertical->setCheckable(true);
        action_flipImageHorizontal = new QAction(BlobDetectorMainWindowClass);
        action_flipImageHorizontal->setObjectName(QString::fromUtf8("action_flipImageHorizontal"));
        action_flipImageHorizontal->setCheckable(true);
        action_openCameraSettings = new QAction(BlobDetectorMainWindowClass);
        action_openCameraSettings->setObjectName(QString::fromUtf8("action_openCameraSettings"));
        action_useCamera = new QAction(BlobDetectorMainWindowClass);
        action_useCamera->setObjectName(QString::fromUtf8("action_useCamera"));
        action_useCamera->setCheckable(true);
        action_useCamera->setChecked(false);
        action_useTestVideo = new QAction(BlobDetectorMainWindowClass);
        action_useTestVideo->setObjectName(QString::fromUtf8("action_useTestVideo"));
        action_useTestVideo->setCheckable(true);
        action_advancedOptions = new QAction(BlobDetectorMainWindowClass);
        action_advancedOptions->setObjectName(QString::fromUtf8("action_advancedOptions"));
        action_findNextCamera = new QAction(BlobDetectorMainWindowClass);
        action_findNextCamera->setObjectName(QString::fromUtf8("action_findNextCamera"));
        action_useTuioUdp = new QAction(BlobDetectorMainWindowClass);
        action_useTuioUdp->setObjectName(QString::fromUtf8("action_useTuioUdp"));
        action_useTuioUdp->setCheckable(true);
        action_useFlashXml = new QAction(BlobDetectorMainWindowClass);
        action_useFlashXml->setObjectName(QString::fromUtf8("action_useFlashXml"));
        action_useFlashXml->setCheckable(true);
        action_useBinaryTop = new QAction(BlobDetectorMainWindowClass);
        action_useBinaryTop->setObjectName(QString::fromUtf8("action_useBinaryTop"));
        action_useBinaryTop->setCheckable(true);
        action_openAboutBox = new QAction(BlobDetectorMainWindowClass);
        action_openAboutBox->setObjectName(QString::fromUtf8("action_openAboutBox"));
        action_openHelpInWebBrowser = new QAction(BlobDetectorMainWindowClass);
        action_openHelpInWebBrowser->setObjectName(QString::fromUtf8("action_openHelpInWebBrowser"));
        actionShow_Frames_Per_Second = new QAction(BlobDetectorMainWindowClass);
        actionShow_Frames_Per_Second->setObjectName(QString::fromUtf8("actionShow_Frames_Per_Second"));
        actionShow_Frames_Per_Second->setCheckable(true);
        actionShow_Frames_Per_Second->setChecked(true);
        actionShow_Calculation_Time = new QAction(BlobDetectorMainWindowClass);
        actionShow_Calculation_Time->setObjectName(QString::fromUtf8("actionShow_Calculation_Time"));
        actionShow_Calculation_Time->setCheckable(true);
        actionShow_Calculation_Time->setChecked(true);
        action_useGpuShaders = new QAction(BlobDetectorMainWindowClass);
        action_useGpuShaders->setObjectName(QString::fromUtf8("action_useGpuShaders"));
        action_useGpuShaders->setCheckable(true);
        action_useCpu = new QAction(BlobDetectorMainWindowClass);
        action_useCpu->setObjectName(QString::fromUtf8("action_useCpu"));
        action_useCpu->setCheckable(true);
        action_useCpu->setChecked(true);
        action_showGraphicsCardInfo = new QAction(BlobDetectorMainWindowClass);
        action_showGraphicsCardInfo->setObjectName(QString::fromUtf8("action_showGraphicsCardInfo"));
        action_ = new QAction(BlobDetectorMainWindowClass);
        action_->setObjectName(QString::fromUtf8("action_"));
        action_preferences = new QAction(BlobDetectorMainWindowClass);
        action_preferences->setObjectName(QString::fromUtf8("action_preferences"));
        actionBig_Blobs = new QAction(BlobDetectorMainWindowClass);
        actionBig_Blobs->setObjectName(QString::fromUtf8("actionBig_Blobs"));
        action_loadSettingsFromFile = new QAction(BlobDetectorMainWindowClass);
        action_loadSettingsFromFile->setObjectName(QString::fromUtf8("action_loadSettingsFromFile"));
        action_noOutput = new QAction(BlobDetectorMainWindowClass);
        action_noOutput->setObjectName(QString::fromUtf8("action_noOutput"));
        action_noOutput->setCheckable(true);
        action_noOutput->setChecked(true);
        action_showBlobOutlines = new QAction(BlobDetectorMainWindowClass);
        action_showBlobOutlines->setObjectName(QString::fromUtf8("action_showBlobOutlines"));
        action_showBlobOutlines->setCheckable(true);
        action_showIdNumbers = new QAction(BlobDetectorMainWindowClass);
        action_showIdNumbers->setObjectName(QString::fromUtf8("action_showIdNumbers"));
        action_showIdNumbers->setCheckable(true);
        action_trackDarkBlobs = new QAction(BlobDetectorMainWindowClass);
        action_trackDarkBlobs->setObjectName(QString::fromUtf8("action_trackDarkBlobs"));
        action_trackDarkBlobs->setCheckable(true);
        centralwidget = new QWidget(BlobDetectorMainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(900, 400));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tabWidget_controlPanel = new QTabWidget(centralwidget);
        tabWidget_controlPanel->setObjectName(QString::fromUtf8("tabWidget_controlPanel"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget_controlPanel->sizePolicy().hasHeightForWidth());
        tabWidget_controlPanel->setSizePolicy(sizePolicy);
        tabWidget_controlPanel->setMinimumSize(QSize(220, 0));
        tabWidget_controlPanel->setMaximumSize(QSize(230, 16777215));
        QFont font;
        font.setPointSize(8);
        tabWidget_controlPanel->setFont(font);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_12 = new QVBoxLayout(tab);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_2);

        textEdit_outputMessage = new QTextEdit(tab);
        textEdit_outputMessage->setObjectName(QString::fromUtf8("textEdit_outputMessage"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textEdit_outputMessage->sizePolicy().hasHeightForWidth());
        textEdit_outputMessage->setSizePolicy(sizePolicy1);
        textEdit_outputMessage->setMinimumSize(QSize(120, 120));
        textEdit_outputMessage->setMaximumSize(QSize(16777215, 160));
        textEdit_outputMessage->setFont(font);
        textEdit_outputMessage->setReadOnly(true);

        verticalLayout_12->addWidget(textEdit_outputMessage);

        verticalSpacer_5 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_5 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_subtractBackground = new QPushButton(tab);
        pushButton_subtractBackground->setObjectName(QString::fromUtf8("pushButton_subtractBackground"));
        pushButton_subtractBackground->setFont(font);

        verticalLayout_2->addWidget(pushButton_subtractBackground);

        pushButton_calibrateMonitorBoundaries = new QPushButton(tab);
        pushButton_calibrateMonitorBoundaries->setObjectName(QString::fromUtf8("pushButton_calibrateMonitorBoundaries"));
        pushButton_calibrateMonitorBoundaries->setFont(font);

        verticalLayout_2->addWidget(pushButton_calibrateMonitorBoundaries);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout_12->addLayout(horizontalLayout);

        verticalSpacer_13 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_13);

        tabWidget_controlPanel->addTab(tab, QString());
        tab_basic = new QWidget();
        tab_basic->setObjectName(QString::fromUtf8("tab_basic"));
        verticalLayout_4 = new QVBoxLayout(tab_basic);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalSpacer_6 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_6);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(tab_basic);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(8);
        label->setFont(font1);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_imageThresholdBox = new QLabel(tab_basic);
        label_imageThresholdBox->setObjectName(QString::fromUtf8("label_imageThresholdBox"));
        label_imageThresholdBox->setFont(font1);
        label_imageThresholdBox->setFrameShape(QFrame::Box);
        label_imageThresholdBox->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_imageThresholdBox, 1, 1, 1, 1);

        label_imageThresholdScale = new QLabel(tab_basic);
        label_imageThresholdScale->setObjectName(QString::fromUtf8("label_imageThresholdScale"));
        label_imageThresholdScale->setFont(font1);
        label_imageThresholdScale->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_imageThresholdScale, 1, 2, 1, 1);

        label_2 = new QLabel(tab_basic);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_minBlobSizeBox = new QLabel(tab_basic);
        label_minBlobSizeBox->setObjectName(QString::fromUtf8("label_minBlobSizeBox"));
        label_minBlobSizeBox->setFont(font1);
        label_minBlobSizeBox->setFrameShape(QFrame::Box);
        label_minBlobSizeBox->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_minBlobSizeBox, 2, 1, 1, 1);

        label_minBlobSizeScale = new QLabel(tab_basic);
        label_minBlobSizeScale->setObjectName(QString::fromUtf8("label_minBlobSizeScale"));
        label_minBlobSizeScale->setFont(font1);
        label_minBlobSizeScale->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_minBlobSizeScale, 2, 2, 1, 1);

        label_3 = new QLabel(tab_basic);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_maxBlobSizeBox = new QLabel(tab_basic);
        label_maxBlobSizeBox->setObjectName(QString::fromUtf8("label_maxBlobSizeBox"));
        label_maxBlobSizeBox->setFont(font1);
        label_maxBlobSizeBox->setFrameShape(QFrame::Box);
        label_maxBlobSizeBox->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_maxBlobSizeBox, 3, 1, 1, 1);

        label_maxBlobSizeScale = new QLabel(tab_basic);
        label_maxBlobSizeScale->setObjectName(QString::fromUtf8("label_maxBlobSizeScale"));
        label_maxBlobSizeScale->setFont(font1);
        label_maxBlobSizeScale->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_maxBlobSizeScale, 3, 2, 1, 1);

        label_4 = new QLabel(tab_basic);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font1);

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_movementFilterScale = new QLabel(tab_basic);
        label_movementFilterScale->setObjectName(QString::fromUtf8("label_movementFilterScale"));
        label_movementFilterScale->setFont(font1);
        label_movementFilterScale->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_movementFilterScale, 4, 2, 1, 1);

        label_25 = new QLabel(tab_basic);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font2.setPointSize(8);
        font2.setUnderline(true);
        label_25->setFont(font2);
        label_25->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_25, 0, 1, 1, 1);

        label_26 = new QLabel(tab_basic);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setFont(font2);
        label_26->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_26, 0, 2, 1, 1);

        label_27 = new QLabel(tab_basic);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setFont(font2);
        label_27->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_27, 0, 0, 1, 1);

        label_movementFilterBox = new QLabel(tab_basic);
        label_movementFilterBox->setObjectName(QString::fromUtf8("label_movementFilterBox"));
        label_movementFilterBox->setFont(font1);
        label_movementFilterBox->setFrameShape(QFrame::Box);
        label_movementFilterBox->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_movementFilterBox, 4, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(tab_basic);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        label_8 = new QLabel(tab_basic);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        label_7 = new QLabel(tab_basic);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font1);

        gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

        label_6 = new QLabel(tab_basic);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        label_34 = new QLabel(tab_basic);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        QFont font3;
        font3.setPointSize(8);
        font3.setUnderline(true);
        label_34->setFont(font3);
        label_34->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_34, 0, 0, 1, 1);

        label_35 = new QLabel(tab_basic);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setFont(font3);
        label_35->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_35, 0, 1, 1, 1);

        label_22 = new QLabel(tab_basic);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font3);
        label_22->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_22, 0, 2, 1, 1);

        label_smoothingValueBox = new QLabel(tab_basic);
        label_smoothingValueBox->setObjectName(QString::fromUtf8("label_smoothingValueBox"));
        label_smoothingValueBox->setFont(font);
        label_smoothingValueBox->setFrameShape(QFrame::Box);
        label_smoothingValueBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_smoothingValueBox, 1, 2, 1, 1);

        label_smoothingValueScale = new QLabel(tab_basic);
        label_smoothingValueScale->setObjectName(QString::fromUtf8("label_smoothingValueScale"));
        label_smoothingValueScale->setFont(font);
        label_smoothingValueScale->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_smoothingValueScale, 1, 3, 1, 1);

        label_33 = new QLabel(tab_basic);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setFont(font3);
        label_33->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_33, 0, 3, 1, 1);

        label_amplifyValueBox = new QLabel(tab_basic);
        label_amplifyValueBox->setObjectName(QString::fromUtf8("label_amplifyValueBox"));
        label_amplifyValueBox->setFont(font);
        label_amplifyValueBox->setFrameShape(QFrame::Box);
        label_amplifyValueBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_amplifyValueBox, 4, 2, 1, 1);

        label_amplifyValueScale = new QLabel(tab_basic);
        label_amplifyValueScale->setObjectName(QString::fromUtf8("label_amplifyValueScale"));
        label_amplifyValueScale->setFont(font);
        label_amplifyValueScale->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_amplifyValueScale, 4, 3, 1, 1);

        label_highpassNoiseValueBox = new QLabel(tab_basic);
        label_highpassNoiseValueBox->setObjectName(QString::fromUtf8("label_highpassNoiseValueBox"));
        label_highpassNoiseValueBox->setFont(font);
        label_highpassNoiseValueBox->setFrameShape(QFrame::Box);
        label_highpassNoiseValueBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassNoiseValueBox, 3, 2, 1, 1);

        label_highpassNoiseValueScale = new QLabel(tab_basic);
        label_highpassNoiseValueScale->setObjectName(QString::fromUtf8("label_highpassNoiseValueScale"));
        label_highpassNoiseValueScale->setFont(font);
        label_highpassNoiseValueScale->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassNoiseValueScale, 3, 3, 1, 1);

        label_highpassBlurValueBox = new QLabel(tab_basic);
        label_highpassBlurValueBox->setObjectName(QString::fromUtf8("label_highpassBlurValueBox"));
        label_highpassBlurValueBox->setFont(font);
        label_highpassBlurValueBox->setFrameShape(QFrame::Box);
        label_highpassBlurValueBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassBlurValueBox, 2, 2, 1, 1);

        label_highpassBlurValueScale = new QLabel(tab_basic);
        label_highpassBlurValueScale->setObjectName(QString::fromUtf8("label_highpassBlurValueScale"));
        label_highpassBlurValueScale->setFont(font);
        label_highpassBlurValueScale->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassBlurValueScale, 2, 3, 1, 1);

        label_amplifyStatusBox = new QLabel(tab_basic);
        label_amplifyStatusBox->setObjectName(QString::fromUtf8("label_amplifyStatusBox"));
        label_amplifyStatusBox->setFont(font);
        label_amplifyStatusBox->setFrameShape(QFrame::Box);
        label_amplifyStatusBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_amplifyStatusBox, 4, 1, 1, 1);

        label_highpassNoiseStatusBox = new QLabel(tab_basic);
        label_highpassNoiseStatusBox->setObjectName(QString::fromUtf8("label_highpassNoiseStatusBox"));
        label_highpassNoiseStatusBox->setFont(font);
        label_highpassNoiseStatusBox->setFrameShape(QFrame::Box);
        label_highpassNoiseStatusBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassNoiseStatusBox, 3, 1, 1, 1);

        label_highpassBlurStatusBox = new QLabel(tab_basic);
        label_highpassBlurStatusBox->setObjectName(QString::fromUtf8("label_highpassBlurStatusBox"));
        label_highpassBlurStatusBox->setFont(font);
        label_highpassBlurStatusBox->setFrameShape(QFrame::Box);
        label_highpassBlurStatusBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_highpassBlurStatusBox, 2, 1, 1, 1);

        label_smoothingStatusBox = new QLabel(tab_basic);
        label_smoothingStatusBox->setObjectName(QString::fromUtf8("label_smoothingStatusBox"));
        label_smoothingStatusBox->setFont(font);
        label_smoothingStatusBox->setFrameShape(QFrame::Box);
        label_smoothingStatusBox->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_smoothingStatusBox, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        tabWidget_controlPanel->addTab(tab_basic, QString());
        tab_blobs = new QWidget();
        tab_blobs->setObjectName(QString::fromUtf8("tab_blobs"));
        verticalLayout_9 = new QVBoxLayout(tab_blobs);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox_imageThreshold = new QGroupBox(tab_blobs);
        groupBox_imageThreshold->setObjectName(QString::fromUtf8("groupBox_imageThreshold"));
        groupBox_imageThreshold->setFont(font);
        groupBox_imageThreshold->setAlignment(Qt::AlignCenter);
        horizontalLayout_8 = new QHBoxLayout(groupBox_imageThreshold);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        slider_imageThreshold = new QSlider(groupBox_imageThreshold);
        slider_imageThreshold->setObjectName(QString::fromUtf8("slider_imageThreshold"));
        slider_imageThreshold->setMaximum(255);
        slider_imageThreshold->setOrientation(Qt::Horizontal);
        slider_imageThreshold->setTickPosition(QSlider::TicksBelow);
        slider_imageThreshold->setTickInterval(25);

        horizontalLayout_8->addWidget(slider_imageThreshold);

        spinBox_imageThreshold = new QSpinBox(groupBox_imageThreshold);
        spinBox_imageThreshold->setObjectName(QString::fromUtf8("spinBox_imageThreshold"));
        spinBox_imageThreshold->setMaximum(255);

        horizontalLayout_8->addWidget(spinBox_imageThreshold);


        verticalLayout_9->addWidget(groupBox_imageThreshold);

        verticalSpacer_8 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_8);

        groupBox_minBlobSize = new QGroupBox(tab_blobs);
        groupBox_minBlobSize->setObjectName(QString::fromUtf8("groupBox_minBlobSize"));
        groupBox_minBlobSize->setFont(font);
        groupBox_minBlobSize->setAlignment(Qt::AlignCenter);
        horizontalLayout_11 = new QHBoxLayout(groupBox_minBlobSize);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        slider_minBlobSize = new QSlider(groupBox_minBlobSize);
        slider_minBlobSize->setObjectName(QString::fromUtf8("slider_minBlobSize"));
        slider_minBlobSize->setMaximum(500);
        slider_minBlobSize->setOrientation(Qt::Horizontal);
        slider_minBlobSize->setTickPosition(QSlider::TicksBelow);
        slider_minBlobSize->setTickInterval(50);

        horizontalLayout_11->addWidget(slider_minBlobSize);

        spinBox_minBlobSize = new QSpinBox(groupBox_minBlobSize);
        spinBox_minBlobSize->setObjectName(QString::fromUtf8("spinBox_minBlobSize"));
        spinBox_minBlobSize->setMaximum(500);

        horizontalLayout_11->addWidget(spinBox_minBlobSize);


        verticalLayout_9->addWidget(groupBox_minBlobSize);

        verticalSpacer_9 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_9);

        groupBox_maxBlobSize = new QGroupBox(tab_blobs);
        groupBox_maxBlobSize->setObjectName(QString::fromUtf8("groupBox_maxBlobSize"));
        groupBox_maxBlobSize->setFont(font);
        groupBox_maxBlobSize->setAlignment(Qt::AlignCenter);
        horizontalLayout_7 = new QHBoxLayout(groupBox_maxBlobSize);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        slider_maxBlobSize = new QSlider(groupBox_maxBlobSize);
        slider_maxBlobSize->setObjectName(QString::fromUtf8("slider_maxBlobSize"));
        slider_maxBlobSize->setMaximum(1000);
        slider_maxBlobSize->setOrientation(Qt::Horizontal);
        slider_maxBlobSize->setTickPosition(QSlider::TicksBelow);
        slider_maxBlobSize->setTickInterval(100);

        horizontalLayout_7->addWidget(slider_maxBlobSize);

        spinBox_maxBlobSize = new QSpinBox(groupBox_maxBlobSize);
        spinBox_maxBlobSize->setObjectName(QString::fromUtf8("spinBox_maxBlobSize"));
        spinBox_maxBlobSize->setMaximum(1000);

        horizontalLayout_7->addWidget(spinBox_maxBlobSize);


        verticalLayout_9->addWidget(groupBox_maxBlobSize);

        verticalSpacer_10 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_10);

        groupBox_minMovementThreshold = new QGroupBox(tab_blobs);
        groupBox_minMovementThreshold->setObjectName(QString::fromUtf8("groupBox_minMovementThreshold"));
        groupBox_minMovementThreshold->setFont(font);
        groupBox_minMovementThreshold->setAlignment(Qt::AlignCenter);
        horizontalLayout_9 = new QHBoxLayout(groupBox_minMovementThreshold);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        slider_minMovementThreshold = new QSlider(groupBox_minMovementThreshold);
        slider_minMovementThreshold->setObjectName(QString::fromUtf8("slider_minMovementThreshold"));
        slider_minMovementThreshold->setMaximum(15);
        slider_minMovementThreshold->setPageStep(3);
        slider_minMovementThreshold->setOrientation(Qt::Horizontal);
        slider_minMovementThreshold->setTickPosition(QSlider::TicksBelow);
        slider_minMovementThreshold->setTickInterval(1);

        horizontalLayout_9->addWidget(slider_minMovementThreshold);

        spinBox_minMovementThreshold = new QSpinBox(groupBox_minMovementThreshold);
        spinBox_minMovementThreshold->setObjectName(QString::fromUtf8("spinBox_minMovementThreshold"));
        spinBox_minMovementThreshold->setMaximum(15);

        horizontalLayout_9->addWidget(spinBox_minMovementThreshold);


        verticalLayout_9->addWidget(groupBox_minMovementThreshold);

        tabWidget_controlPanel->addTab(tab_blobs, QString());
        tab_filters = new QWidget();
        tab_filters->setObjectName(QString::fromUtf8("tab_filters"));
        verticalLayout_10 = new QVBoxLayout(tab_filters);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        groupBox_smoothing = new QGroupBox(tab_filters);
        groupBox_smoothing->setObjectName(QString::fromUtf8("groupBox_smoothing"));
        groupBox_smoothing->setFont(font);
        groupBox_smoothing->setAlignment(Qt::AlignCenter);
        verticalLayout_3 = new QVBoxLayout(groupBox_smoothing);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        radioButton_smoothingOn = new QRadioButton(groupBox_smoothing);
        radioButton_smoothingOn->setObjectName(QString::fromUtf8("radioButton_smoothingOn"));
        radioButton_smoothingOn->setFont(font);
        radioButton_smoothingOn->setChecked(false);

        horizontalLayout_10->addWidget(radioButton_smoothingOn);

        radioButton_smoothingOff = new QRadioButton(groupBox_smoothing);
        radioButton_smoothingOff->setObjectName(QString::fromUtf8("radioButton_smoothingOff"));
        radioButton_smoothingOff->setFont(font);
        radioButton_smoothingOff->setChecked(true);

        horizontalLayout_10->addWidget(radioButton_smoothingOff);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        slider_smoothing = new QSlider(groupBox_smoothing);
        slider_smoothing->setObjectName(QString::fromUtf8("slider_smoothing"));
        slider_smoothing->setMaximum(15);
        slider_smoothing->setPageStep(3);
        slider_smoothing->setOrientation(Qt::Horizontal);
        slider_smoothing->setTickPosition(QSlider::TicksBelow);
        slider_smoothing->setTickInterval(3);

        horizontalLayout_12->addWidget(slider_smoothing);

        spinBox_smoothing = new QSpinBox(groupBox_smoothing);
        spinBox_smoothing->setObjectName(QString::fromUtf8("spinBox_smoothing"));
        spinBox_smoothing->setMaximum(15);

        horizontalLayout_12->addWidget(spinBox_smoothing);


        verticalLayout_3->addLayout(horizontalLayout_12);


        verticalLayout_10->addWidget(groupBox_smoothing);

        verticalSpacer_14 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_14);

        groupBox_highpass = new QGroupBox(tab_filters);
        groupBox_highpass->setObjectName(QString::fromUtf8("groupBox_highpass"));
        groupBox_highpass->setFont(font);
        groupBox_highpass->setAlignment(Qt::AlignCenter);
        verticalLayout_8 = new QVBoxLayout(groupBox_highpass);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        radioButton_highpassOn = new QRadioButton(groupBox_highpass);
        radioButton_highpassOn->setObjectName(QString::fromUtf8("radioButton_highpassOn"));
        radioButton_highpassOn->setFont(font);
        radioButton_highpassOn->setChecked(false);

        horizontalLayout_18->addWidget(radioButton_highpassOn);

        radioButton_highpassOff = new QRadioButton(groupBox_highpass);
        radioButton_highpassOff->setObjectName(QString::fromUtf8("radioButton_highpassOff"));
        radioButton_highpassOff->setFont(font);
        radioButton_highpassOff->setChecked(true);

        horizontalLayout_18->addWidget(radioButton_highpassOff);


        verticalLayout_8->addLayout(horizontalLayout_18);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_highpassBlur = new QLabel(groupBox_highpass);
        label_highpassBlur->setObjectName(QString::fromUtf8("label_highpassBlur"));
        label_highpassBlur->setFont(font);

        verticalLayout_6->addWidget(label_highpassBlur);

        label_highpassNoise = new QLabel(groupBox_highpass);
        label_highpassNoise->setObjectName(QString::fromUtf8("label_highpassNoise"));
        label_highpassNoise->setFont(font);

        verticalLayout_6->addWidget(label_highpassNoise);


        horizontalLayout_17->addLayout(verticalLayout_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        slider_highpassBlur = new QSlider(groupBox_highpass);
        slider_highpassBlur->setObjectName(QString::fromUtf8("slider_highpassBlur"));
        slider_highpassBlur->setMaximum(200);
        slider_highpassBlur->setPageStep(20);
        slider_highpassBlur->setOrientation(Qt::Horizontal);
        slider_highpassBlur->setTickPosition(QSlider::TicksBelow);
        slider_highpassBlur->setTickInterval(20);

        horizontalLayout_13->addWidget(slider_highpassBlur);

        spinBox_highpassBlur = new QSpinBox(groupBox_highpass);
        spinBox_highpassBlur->setObjectName(QString::fromUtf8("spinBox_highpassBlur"));
        spinBox_highpassBlur->setMaximum(200);

        horizontalLayout_13->addWidget(spinBox_highpassBlur);


        verticalLayout_5->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        slider_highpassNoise = new QSlider(groupBox_highpass);
        slider_highpassNoise->setObjectName(QString::fromUtf8("slider_highpassNoise"));
        slider_highpassNoise->setMaximum(30);
        slider_highpassNoise->setPageStep(3);
        slider_highpassNoise->setOrientation(Qt::Horizontal);
        slider_highpassNoise->setTickPosition(QSlider::TicksBelow);
        slider_highpassNoise->setTickInterval(3);

        horizontalLayout_14->addWidget(slider_highpassNoise);

        spinBox_highpassNoise = new QSpinBox(groupBox_highpass);
        spinBox_highpassNoise->setObjectName(QString::fromUtf8("spinBox_highpassNoise"));
        spinBox_highpassNoise->setMaximum(30);

        horizontalLayout_14->addWidget(spinBox_highpassNoise);


        verticalLayout_5->addLayout(horizontalLayout_14);


        horizontalLayout_17->addLayout(verticalLayout_5);


        verticalLayout_8->addLayout(horizontalLayout_17);


        verticalLayout_10->addWidget(groupBox_highpass);

        verticalSpacer_15 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_15);

        groupBox_amplify = new QGroupBox(tab_filters);
        groupBox_amplify->setObjectName(QString::fromUtf8("groupBox_amplify"));
        groupBox_amplify->setFont(font);
        groupBox_amplify->setAlignment(Qt::AlignCenter);
        verticalLayout_7 = new QVBoxLayout(groupBox_amplify);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        radioButton_amplifyOn = new QRadioButton(groupBox_amplify);
        radioButton_amplifyOn->setObjectName(QString::fromUtf8("radioButton_amplifyOn"));
        radioButton_amplifyOn->setFont(font);
        radioButton_amplifyOn->setChecked(false);

        horizontalLayout_16->addWidget(radioButton_amplifyOn);

        radioButton_amplifyOff = new QRadioButton(groupBox_amplify);
        radioButton_amplifyOff->setObjectName(QString::fromUtf8("radioButton_amplifyOff"));
        radioButton_amplifyOff->setFont(font);
        radioButton_amplifyOff->setChecked(true);

        horizontalLayout_16->addWidget(radioButton_amplifyOff);


        verticalLayout_7->addLayout(horizontalLayout_16);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        slider_amplify = new QSlider(groupBox_amplify);
        slider_amplify->setObjectName(QString::fromUtf8("slider_amplify"));
        slider_amplify->setMaximum(300);
        slider_amplify->setPageStep(30);
        slider_amplify->setOrientation(Qt::Horizontal);
        slider_amplify->setTickPosition(QSlider::TicksBelow);
        slider_amplify->setTickInterval(30);

        horizontalLayout_15->addWidget(slider_amplify);

        spinBox_amplify = new QSpinBox(groupBox_amplify);
        spinBox_amplify->setObjectName(QString::fromUtf8("spinBox_amplify"));
        spinBox_amplify->setMaximum(300);

        horizontalLayout_15->addWidget(spinBox_amplify);


        verticalLayout_7->addLayout(horizontalLayout_15);


        verticalLayout_10->addWidget(groupBox_amplify);

        tabWidget_controlPanel->addTab(tab_filters, QString());

        horizontalLayout_2->addWidget(tabWidget_controlPanel);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        label_source_5 = new QLabel(centralwidget);
        label_source_5->setObjectName(QString::fromUtf8("label_source_5"));
        label_source_5->setMinimumSize(QSize(0, 48));
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        label_source_5->setFont(font4);
        label_source_5->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_source_5);

        label_source = new QLabel(centralwidget);
        label_source->setObjectName(QString::fromUtf8("label_source"));
        label_source->setMinimumSize(QSize(0, 20));
        label_source->setFont(font4);
        label_source->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_source);

        frame_sourceImage = new QFrame(centralwidget);
        frame_sourceImage->setObjectName(QString::fromUtf8("frame_sourceImage"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_sourceImage->sizePolicy().hasHeightForWidth());
        frame_sourceImage->setSizePolicy(sizePolicy2);
        frame_sourceImage->setMinimumSize(QSize(324, 244));
        frame_sourceImage->setFrameShape(QFrame::Box);
        frame_sourceImage->setLineWidth(2);
        horizontalLayout_sourceImage = new QHBoxLayout(frame_sourceImage);
        horizontalLayout_sourceImage->setSpacing(0);
        horizontalLayout_sourceImage->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_sourceImage->setObjectName(QString::fromUtf8("horizontalLayout_sourceImage"));

        verticalLayout_13->addWidget(frame_sourceImage);

        label_source_2 = new QLabel(centralwidget);
        label_source_2->setObjectName(QString::fromUtf8("label_source_2"));
        sizePolicy1.setHeightForWidth(label_source_2->sizePolicy().hasHeightForWidth());
        label_source_2->setSizePolicy(sizePolicy1);
        label_source_2->setMinimumSize(QSize(0, 48));
        label_source_2->setFont(font4);
        label_source_2->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_source_2);


        horizontalLayout_2->addLayout(verticalLayout_13);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_source_4 = new QLabel(centralwidget);
        label_source_4->setObjectName(QString::fromUtf8("label_source_4"));
        label_source_4->setMinimumSize(QSize(0, 48));
        label_source_4->setFont(font4);
        label_source_4->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_source_4);

        label_tracked = new QLabel(centralwidget);
        label_tracked->setObjectName(QString::fromUtf8("label_tracked"));
        label_tracked->setMinimumSize(QSize(0, 21));
        label_tracked->setFont(font4);
        label_tracked->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_tracked);

        frame_background = new QFrame(centralwidget);
        frame_background->setObjectName(QString::fromUtf8("frame_background"));
        sizePolicy2.setHeightForWidth(frame_background->sizePolicy().hasHeightForWidth());
        frame_background->setSizePolicy(sizePolicy2);
        frame_background->setMinimumSize(QSize(324, 244));
        frame_background->setFrameShape(QFrame::Box);
        frame_background->setLineWidth(2);
        horizontalLayout_trackedImage = new QVBoxLayout(frame_background);
        horizontalLayout_trackedImage->setSpacing(0);
        horizontalLayout_trackedImage->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_trackedImage->setObjectName(QString::fromUtf8("horizontalLayout_trackedImage"));

        verticalLayout_11->addWidget(frame_background);

        label_source_3 = new QLabel(centralwidget);
        label_source_3->setObjectName(QString::fromUtf8("label_source_3"));
        sizePolicy1.setHeightForWidth(label_source_3->sizePolicy().hasHeightForWidth());
        label_source_3->setSizePolicy(sizePolicy1);
        label_source_3->setMinimumSize(QSize(0, 48));
        QFont font5;
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        label_source_3->setFont(font5);
        label_source_3->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_source_3);


        horizontalLayout_2->addLayout(verticalLayout_11);

        BlobDetectorMainWindowClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BlobDetectorMainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 923, 26));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuCamera = new QMenu(menubar);
        menuCamera->setObjectName(QString::fromUtf8("menuCamera"));
        menuNetwork = new QMenu(menubar);
        menuNetwork->setObjectName(QString::fromUtf8("menuNetwork"));
        menuBlobs = new QMenu(menubar);
        menuBlobs->setObjectName(QString::fromUtf8("menuBlobs"));
        BlobDetectorMainWindowClass->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuCamera->menuAction());
        menubar->addAction(menuBlobs->menuAction());
        menubar->addAction(menuNetwork->menuAction());
        menuFile->addAction(action_loadSettingsFromFile);
        menuFile->addAction(action_saveSettingsToFile);
        menuFile->addAction(action_quit);
        menuView->addAction(action_showFilterImages);
        menuCamera->addAction(action_useCamera);
        menuCamera->addAction(action_useTestVideo);
        menuCamera->addSeparator();
        menuCamera->addAction(action_openCameraSettings);
        menuCamera->addSeparator();
        menuCamera->addAction(action_flipImageVertical);
        menuCamera->addAction(action_flipImageHorizontal);
        menuCamera->addSeparator();
        menuNetwork->addAction(action_useTuioUdp);
        menuNetwork->addAction(action_useFlashXml);
        menuNetwork->addAction(action_useBinaryTop);
        menuNetwork->addAction(action_noOutput);
        menuBlobs->addAction(action_showBlobOutlines);
        menuBlobs->addAction(action_showIdNumbers);
        menuBlobs->addAction(action_trackDarkBlobs);

        retranslateUi(BlobDetectorMainWindowClass);

        tabWidget_controlPanel->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BlobDetectorMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *BlobDetectorMainWindowClass)
    {
        BlobDetectorMainWindowClass->setWindowTitle(QApplication::translate("BlobDetectorMainWindowClass", "Big Blobby", 0, QApplication::UnicodeUTF8));
        action_saveSettingsToFile->setText(QApplication::translate("BlobDetectorMainWindowClass", "Save Settings to File...", 0, QApplication::UnicodeUTF8));
        action_quit->setText(QApplication::translate("BlobDetectorMainWindowClass", "Quit", 0, QApplication::UnicodeUTF8));
        action_showAllImages->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show All Images", 0, QApplication::UnicodeUTF8));
        action_showBackgroundImage->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Background Image", 0, QApplication::UnicodeUTF8));
        action_showFilterImages->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Background and Filters...", 0, QApplication::UnicodeUTF8));
        action_showSourceImage->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Source Image", 0, QApplication::UnicodeUTF8));
        action_flipImageVertical->setText(QApplication::translate("BlobDetectorMainWindowClass", "Flip Image Vertically", 0, QApplication::UnicodeUTF8));
        action_flipImageHorizontal->setText(QApplication::translate("BlobDetectorMainWindowClass", "Flip Image Horizontally", 0, QApplication::UnicodeUTF8));
        action_openCameraSettings->setText(QApplication::translate("BlobDetectorMainWindowClass", "Open Camera Settings...", 0, QApplication::UnicodeUTF8));
        action_useCamera->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use Camera", 0, QApplication::UnicodeUTF8));
        action_useTestVideo->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use Test Video", 0, QApplication::UnicodeUTF8));
        action_advancedOptions->setText(QApplication::translate("BlobDetectorMainWindowClass", "Advanced Options...", 0, QApplication::UnicodeUTF8));
        action_findNextCamera->setText(QApplication::translate("BlobDetectorMainWindowClass", "Find Next Camera", 0, QApplication::UnicodeUTF8));
        action_useTuioUdp->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use TUIO UDP", 0, QApplication::UnicodeUTF8));
        action_useFlashXml->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use Flash XML", 0, QApplication::UnicodeUTF8));
        action_useBinaryTop->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use Binary TOP", 0, QApplication::UnicodeUTF8));
        action_openAboutBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "Open About Box...", 0, QApplication::UnicodeUTF8));
        action_openHelpInWebBrowser->setText(QApplication::translate("BlobDetectorMainWindowClass", "Open Help in Web Browser...", 0, QApplication::UnicodeUTF8));
        actionShow_Frames_Per_Second->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Frames Per Second", 0, QApplication::UnicodeUTF8));
        actionShow_Calculation_Time->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Calculation Time", 0, QApplication::UnicodeUTF8));
        action_useGpuShaders->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use GPU Shaders", 0, QApplication::UnicodeUTF8));
        action_useCpu->setText(QApplication::translate("BlobDetectorMainWindowClass", "Use CPU", 0, QApplication::UnicodeUTF8));
        action_showGraphicsCardInfo->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Graphics Card Info...", 0, QApplication::UnicodeUTF8));
        action_->setText(QApplication::translate("BlobDetectorMainWindowClass", "Factory Default Settings", 0, QApplication::UnicodeUTF8));
        action_preferences->setText(QApplication::translate("BlobDetectorMainWindowClass", "Preferences...", 0, QApplication::UnicodeUTF8));
        actionBig_Blobs->setText(QApplication::translate("BlobDetectorMainWindowClass", "Big Blobs", 0, QApplication::UnicodeUTF8));
        action_loadSettingsFromFile->setText(QApplication::translate("BlobDetectorMainWindowClass", "Load Settings from File...", 0, QApplication::UnicodeUTF8));
        action_noOutput->setText(QApplication::translate("BlobDetectorMainWindowClass", "No Output", 0, QApplication::UnicodeUTF8));
        action_showBlobOutlines->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show Blob Outlines", 0, QApplication::UnicodeUTF8));
        action_showIdNumbers->setText(QApplication::translate("BlobDetectorMainWindowClass", "Show ID Numbers", 0, QApplication::UnicodeUTF8));
        action_trackDarkBlobs->setText(QApplication::translate("BlobDetectorMainWindowClass", "Track Dark Blobs", 0, QApplication::UnicodeUTF8));
        textEdit_outputMessage->setHtml(QApplication::translate("BlobDetectorMainWindowClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Video Resolution:  320 x 240</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Video Framerate:  30 fps</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Calculation Time:   5 mS</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
                        " margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Sending TUIO messages to:</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Host: 127.0.0.1</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Port: 3000</p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_subtractBackground->setText(QApplication::translate("BlobDetectorMainWindowClass", "Subtract Background", 0, QApplication::UnicodeUTF8));
        pushButton_calibrateMonitorBoundaries->setText(QApplication::translate("BlobDetectorMainWindowClass", "Calibrate Monitor Boundaries", 0, QApplication::UnicodeUTF8));
        tabWidget_controlPanel->setTabText(tabWidget_controlPanel->indexOf(tab), QApplication::translate("BlobDetectorMainWindowClass", "Out", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("BlobDetectorMainWindowClass", "Image Threshold", 0, QApplication::UnicodeUTF8));
        label_imageThresholdBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_imageThresholdScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "255", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("BlobDetectorMainWindowClass", "Min Blob Size", 0, QApplication::UnicodeUTF8));
        label_minBlobSizeBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_minBlobSizeScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "500", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("BlobDetectorMainWindowClass", "Max Blob Size", 0, QApplication::UnicodeUTF8));
        label_maxBlobSizeBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_maxBlobSizeScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "1000", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("BlobDetectorMainWindowClass", "Movement Filter", 0, QApplication::UnicodeUTF8));
        label_movementFilterScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "15", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("BlobDetectorMainWindowClass", "Value", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("BlobDetectorMainWindowClass", "Scale", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("BlobDetectorMainWindowClass", "Blobs", 0, QApplication::UnicodeUTF8));
        label_movementFilterBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("BlobDetectorMainWindowClass", "Smoothing", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("BlobDetectorMainWindowClass", "Highpass Blur", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("BlobDetectorMainWindowClass", "Highpass Noise", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("BlobDetectorMainWindowClass", "Amplify", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("BlobDetectorMainWindowClass", "Filter", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("BlobDetectorMainWindowClass", "Status", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("BlobDetectorMainWindowClass", "Value", 0, QApplication::UnicodeUTF8));
        label_smoothingValueBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_smoothingValueScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "15", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("BlobDetectorMainWindowClass", "Scale", 0, QApplication::UnicodeUTF8));
        label_amplifyValueBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_amplifyValueScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "300", 0, QApplication::UnicodeUTF8));
        label_highpassNoiseValueBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_highpassNoiseValueScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "30", 0, QApplication::UnicodeUTF8));
        label_highpassBlurValueBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "0", 0, QApplication::UnicodeUTF8));
        label_highpassBlurValueScale->setText(QApplication::translate("BlobDetectorMainWindowClass", "200", 0, QApplication::UnicodeUTF8));
        label_amplifyStatusBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        label_highpassNoiseStatusBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        label_highpassBlurStatusBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        label_smoothingStatusBox->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        tabWidget_controlPanel->setTabText(tabWidget_controlPanel->indexOf(tab_basic), QApplication::translate("BlobDetectorMainWindowClass", "Values", 0, QApplication::UnicodeUTF8));
        groupBox_imageThreshold->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Image Threshold", 0, QApplication::UnicodeUTF8));
        groupBox_minBlobSize->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Min Blob Size", 0, QApplication::UnicodeUTF8));
        groupBox_maxBlobSize->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Max Blob Size", 0, QApplication::UnicodeUTF8));
        groupBox_minMovementThreshold->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Min Movement Threshold", 0, QApplication::UnicodeUTF8));
        tabWidget_controlPanel->setTabText(tabWidget_controlPanel->indexOf(tab_blobs), QApplication::translate("BlobDetectorMainWindowClass", "Blobs", 0, QApplication::UnicodeUTF8));
        groupBox_smoothing->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Smoothing", 0, QApplication::UnicodeUTF8));
        radioButton_smoothingOn->setText(QApplication::translate("BlobDetectorMainWindowClass", "On", 0, QApplication::UnicodeUTF8));
        radioButton_smoothingOff->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        groupBox_highpass->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Highpass", 0, QApplication::UnicodeUTF8));
        radioButton_highpassOn->setText(QApplication::translate("BlobDetectorMainWindowClass", "On", 0, QApplication::UnicodeUTF8));
        radioButton_highpassOff->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        label_highpassBlur->setText(QApplication::translate("BlobDetectorMainWindowClass", "Blur", 0, QApplication::UnicodeUTF8));
        label_highpassNoise->setText(QApplication::translate("BlobDetectorMainWindowClass", "Noise", 0, QApplication::UnicodeUTF8));
        groupBox_amplify->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Amplify", 0, QApplication::UnicodeUTF8));
        radioButton_amplifyOn->setText(QApplication::translate("BlobDetectorMainWindowClass", "On", 0, QApplication::UnicodeUTF8));
        radioButton_amplifyOff->setText(QApplication::translate("BlobDetectorMainWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        tabWidget_controlPanel->setTabText(tabWidget_controlPanel->indexOf(tab_filters), QApplication::translate("BlobDetectorMainWindowClass", "Filters", 0, QApplication::UnicodeUTF8));
        label_source_5->setText(QString());
        label_source->setText(QApplication::translate("BlobDetectorMainWindowClass", "Source", 0, QApplication::UnicodeUTF8));
        label_source_2->setText(QString());
        label_source_4->setText(QString());
        label_tracked->setText(QApplication::translate("BlobDetectorMainWindowClass", "Tracked", 0, QApplication::UnicodeUTF8));
        label_source_3->setText(QString());
        menuFile->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "View", 0, QApplication::UnicodeUTF8));
        menuCamera->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Camera", 0, QApplication::UnicodeUTF8));
        menuNetwork->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Network", 0, QApplication::UnicodeUTF8));
        menuBlobs->setTitle(QApplication::translate("BlobDetectorMainWindowClass", "Blobs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BlobDetectorMainWindowClass: public Ui_BlobDetectorMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOBDETECTORMAINWINDOW_H
