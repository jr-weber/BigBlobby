/********************************************************************************
** Form generated from reading UI file 'BlobFiltersWindow.ui'
**
** Created: Wed Sep 28 14:40:21 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOBFILTERSWINDOW_H
#define UI_BLOBFILTERSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlobFiltersWindowClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_7;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_dynamicSubtration;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioButton_dynamicSubtractionOn;
    QRadioButton *radioButton_dynamicSubtractionOff;
    QHBoxLayout *horizontalLayout_3;
    QSlider *slider_dynamicSubtraction;
    QSpinBox *spinBox_dynamicSubtraction;
    QLabel *label_learnSpeed;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_subtractBackground2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *label_learnSpeed_2;
    QFrame *frame_backgroundImage;
    QHBoxLayout *horizontalLayout_backgroundImage;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_learnSpeed_3;
    QFrame *frame_smoothingImage;
    QHBoxLayout *horizontalLayout_smoothingImage;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_learnSpeed_4;
    QFrame *frame_highpassImage;
    QHBoxLayout *horizontalLayout_highpassImage;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_learnSpeed_5;
    QFrame *frame_amplifyImage;
    QHBoxLayout *horizontalLayout_amplifyImage;

    void setupUi(QWidget *BlobFiltersWindowClass)
    {
        if (BlobFiltersWindowClass->objectName().isEmpty())
            BlobFiltersWindowClass->setObjectName(QString::fromUtf8("BlobFiltersWindowClass"));
        BlobFiltersWindowClass->resize(900, 167);
        horizontalLayout_2 = new QHBoxLayout(BlobFiltersWindowClass);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalSpacer_2 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);

        widget = new QWidget(BlobFiltersWindowClass);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(150, 160));
        verticalLayout_6 = new QVBoxLayout(widget);
        verticalLayout_6->setContentsMargins(2, 2, 2, 2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        groupBox_dynamicSubtration = new QGroupBox(widget);
        groupBox_dynamicSubtration->setObjectName(QString::fromUtf8("groupBox_dynamicSubtration"));
        sizePolicy.setHeightForWidth(groupBox_dynamicSubtration->sizePolicy().hasHeightForWidth());
        groupBox_dynamicSubtration->setSizePolicy(sizePolicy);
        groupBox_dynamicSubtration->setMinimumSize(QSize(140, 120));
        QFont font;
        font.setPointSize(8);
        groupBox_dynamicSubtration->setFont(font);
        groupBox_dynamicSubtration->setAlignment(Qt::AlignCenter);
        verticalLayout_2 = new QVBoxLayout(groupBox_dynamicSubtration);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        radioButton_dynamicSubtractionOn = new QRadioButton(groupBox_dynamicSubtration);
        radioButton_dynamicSubtractionOn->setObjectName(QString::fromUtf8("radioButton_dynamicSubtractionOn"));
        radioButton_dynamicSubtractionOn->setChecked(false);

        horizontalLayout_4->addWidget(radioButton_dynamicSubtractionOn);

        radioButton_dynamicSubtractionOff = new QRadioButton(groupBox_dynamicSubtration);
        radioButton_dynamicSubtractionOff->setObjectName(QString::fromUtf8("radioButton_dynamicSubtractionOff"));
        radioButton_dynamicSubtractionOff->setChecked(true);

        horizontalLayout_4->addWidget(radioButton_dynamicSubtractionOff);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        slider_dynamicSubtraction = new QSlider(groupBox_dynamicSubtration);
        slider_dynamicSubtraction->setObjectName(QString::fromUtf8("slider_dynamicSubtraction"));
        slider_dynamicSubtraction->setEnabled(false);
        slider_dynamicSubtraction->setMaximum(500);
        slider_dynamicSubtraction->setSingleStep(1);
        slider_dynamicSubtraction->setPageStep(50);
        slider_dynamicSubtraction->setOrientation(Qt::Horizontal);
        slider_dynamicSubtraction->setTickPosition(QSlider::TicksBelow);
        slider_dynamicSubtraction->setTickInterval(50);

        horizontalLayout_3->addWidget(slider_dynamicSubtraction);

        spinBox_dynamicSubtraction = new QSpinBox(groupBox_dynamicSubtration);
        spinBox_dynamicSubtraction->setObjectName(QString::fromUtf8("spinBox_dynamicSubtraction"));
        spinBox_dynamicSubtraction->setEnabled(false);
        spinBox_dynamicSubtraction->setMaximum(500);

        horizontalLayout_3->addWidget(spinBox_dynamicSubtraction);


        verticalLayout_2->addLayout(horizontalLayout_3);

        label_learnSpeed = new QLabel(groupBox_dynamicSubtration);
        label_learnSpeed->setObjectName(QString::fromUtf8("label_learnSpeed"));
        label_learnSpeed->setEnabled(false);
        label_learnSpeed->setMinimumSize(QSize(0, 10));
        label_learnSpeed->setFont(font);
        label_learnSpeed->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_learnSpeed);


        verticalLayout_6->addWidget(groupBox_dynamicSubtration);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_subtractBackground2 = new QPushButton(widget);
        pushButton_subtractBackground2->setObjectName(QString::fromUtf8("pushButton_subtractBackground2"));

        horizontalLayout->addWidget(pushButton_subtractBackground2);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_6->addLayout(horizontalLayout);


        verticalLayout_7->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_7);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_learnSpeed_2 = new QLabel(BlobFiltersWindowClass);
        label_learnSpeed_2->setObjectName(QString::fromUtf8("label_learnSpeed_2"));
        label_learnSpeed_2->setMinimumSize(QSize(0, 20));
        QFont font1;
        font1.setPointSize(10);
        label_learnSpeed_2->setFont(font1);
        label_learnSpeed_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_learnSpeed_2);

        frame_backgroundImage = new QFrame(BlobFiltersWindowClass);
        frame_backgroundImage->setObjectName(QString::fromUtf8("frame_backgroundImage"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_backgroundImage->sizePolicy().hasHeightForWidth());
        frame_backgroundImage->setSizePolicy(sizePolicy1);
        frame_backgroundImage->setMinimumSize(QSize(180, 135));
        frame_backgroundImage->setFrameShape(QFrame::Box);
        frame_backgroundImage->setLineWidth(2);
        horizontalLayout_backgroundImage = new QHBoxLayout(frame_backgroundImage);
        horizontalLayout_backgroundImage->setSpacing(0);
        horizontalLayout_backgroundImage->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_backgroundImage->setObjectName(QString::fromUtf8("horizontalLayout_backgroundImage"));

        verticalLayout->addWidget(frame_backgroundImage);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_learnSpeed_3 = new QLabel(BlobFiltersWindowClass);
        label_learnSpeed_3->setObjectName(QString::fromUtf8("label_learnSpeed_3"));
        label_learnSpeed_3->setMinimumSize(QSize(0, 20));
        label_learnSpeed_3->setFont(font1);
        label_learnSpeed_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_learnSpeed_3);

        frame_smoothingImage = new QFrame(BlobFiltersWindowClass);
        frame_smoothingImage->setObjectName(QString::fromUtf8("frame_smoothingImage"));
        sizePolicy1.setHeightForWidth(frame_smoothingImage->sizePolicy().hasHeightForWidth());
        frame_smoothingImage->setSizePolicy(sizePolicy1);
        frame_smoothingImage->setMinimumSize(QSize(180, 135));
        frame_smoothingImage->setFrameShape(QFrame::Box);
        frame_smoothingImage->setLineWidth(2);
        horizontalLayout_smoothingImage = new QHBoxLayout(frame_smoothingImage);
        horizontalLayout_smoothingImage->setSpacing(0);
        horizontalLayout_smoothingImage->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_smoothingImage->setObjectName(QString::fromUtf8("horizontalLayout_smoothingImage"));

        verticalLayout_3->addWidget(frame_smoothingImage);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_learnSpeed_4 = new QLabel(BlobFiltersWindowClass);
        label_learnSpeed_4->setObjectName(QString::fromUtf8("label_learnSpeed_4"));
        label_learnSpeed_4->setMinimumSize(QSize(0, 20));
        label_learnSpeed_4->setFont(font1);
        label_learnSpeed_4->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_learnSpeed_4);

        frame_highpassImage = new QFrame(BlobFiltersWindowClass);
        frame_highpassImage->setObjectName(QString::fromUtf8("frame_highpassImage"));
        sizePolicy1.setHeightForWidth(frame_highpassImage->sizePolicy().hasHeightForWidth());
        frame_highpassImage->setSizePolicy(sizePolicy1);
        frame_highpassImage->setMinimumSize(QSize(180, 135));
        frame_highpassImage->setFrameShape(QFrame::Box);
        frame_highpassImage->setLineWidth(2);
        horizontalLayout_highpassImage = new QHBoxLayout(frame_highpassImage);
        horizontalLayout_highpassImage->setSpacing(0);
        horizontalLayout_highpassImage->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_highpassImage->setObjectName(QString::fromUtf8("horizontalLayout_highpassImage"));

        verticalLayout_4->addWidget(frame_highpassImage);


        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_learnSpeed_5 = new QLabel(BlobFiltersWindowClass);
        label_learnSpeed_5->setObjectName(QString::fromUtf8("label_learnSpeed_5"));
        label_learnSpeed_5->setMinimumSize(QSize(0, 20));
        label_learnSpeed_5->setFont(font1);
        label_learnSpeed_5->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_learnSpeed_5);

        frame_amplifyImage = new QFrame(BlobFiltersWindowClass);
        frame_amplifyImage->setObjectName(QString::fromUtf8("frame_amplifyImage"));
        sizePolicy1.setHeightForWidth(frame_amplifyImage->sizePolicy().hasHeightForWidth());
        frame_amplifyImage->setSizePolicy(sizePolicy1);
        frame_amplifyImage->setMinimumSize(QSize(180, 135));
        frame_amplifyImage->setFrameShape(QFrame::Box);
        frame_amplifyImage->setLineWidth(2);
        horizontalLayout_amplifyImage = new QHBoxLayout(frame_amplifyImage);
        horizontalLayout_amplifyImage->setSpacing(0);
        horizontalLayout_amplifyImage->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_amplifyImage->setObjectName(QString::fromUtf8("horizontalLayout_amplifyImage"));

        verticalLayout_5->addWidget(frame_amplifyImage);


        horizontalLayout_2->addLayout(verticalLayout_5);


        retranslateUi(BlobFiltersWindowClass);

        QMetaObject::connectSlotsByName(BlobFiltersWindowClass);
    } // setupUi

    void retranslateUi(QWidget *BlobFiltersWindowClass)
    {
        BlobFiltersWindowClass->setWindowTitle(QApplication::translate("BlobFiltersWindowClass", "Big Blobby Background and Filters", 0, QApplication::UnicodeUTF8));
        groupBox_dynamicSubtration->setTitle(QApplication::translate("BlobFiltersWindowClass", "Dynamic Background", 0, QApplication::UnicodeUTF8));
        radioButton_dynamicSubtractionOn->setText(QApplication::translate("BlobFiltersWindowClass", "On", 0, QApplication::UnicodeUTF8));
        radioButton_dynamicSubtractionOff->setText(QApplication::translate("BlobFiltersWindowClass", "Off", 0, QApplication::UnicodeUTF8));
        label_learnSpeed->setText(QApplication::translate("BlobFiltersWindowClass", "Learn Speed", 0, QApplication::UnicodeUTF8));
        pushButton_subtractBackground2->setText(QApplication::translate("BlobFiltersWindowClass", "Subtract Background", 0, QApplication::UnicodeUTF8));
        label_learnSpeed_2->setText(QApplication::translate("BlobFiltersWindowClass", "Background", 0, QApplication::UnicodeUTF8));
        label_learnSpeed_3->setText(QApplication::translate("BlobFiltersWindowClass", "Smoothing", 0, QApplication::UnicodeUTF8));
        label_learnSpeed_4->setText(QApplication::translate("BlobFiltersWindowClass", "Highpass", 0, QApplication::UnicodeUTF8));
        label_learnSpeed_5->setText(QApplication::translate("BlobFiltersWindowClass", "Amplify", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BlobFiltersWindowClass: public Ui_BlobFiltersWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOBFILTERSWINDOW_H
