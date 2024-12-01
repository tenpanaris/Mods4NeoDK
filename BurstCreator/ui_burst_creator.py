# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'Burst_creator.ui'
##
## Created by: Qt User Interface Compiler version 6.8.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QButtonGroup, QHBoxLayout, QLabel,
    QMainWindow, QMenu, QMenuBar, QPushButton,
    QSizePolicy, QSlider, QSpacerItem, QSpinBox,
    QStatusBar, QTextEdit, QVBoxLayout, QWidget)

from qsliderWithTooltip import QSliderWithTooltip

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1303, 354)
        self.actionCOM_setting = QAction(MainWindow)
        self.actionCOM_setting.setObjectName(u"actionCOM_setting")
        self.actionExit = QAction(MainWindow)
        self.actionExit.setObjectName(u"actionExit")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.horizontalLayout_7 = QHBoxLayout(self.centralwidget)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.labelDuration = QLabel(self.centralwidget)
        self.labelDuration.setObjectName(u"labelDuration")

        self.horizontalLayout_2.addWidget(self.labelDuration)

        self.sliderDuration = QSliderWithTooltip(self.centralwidget)
        self.sliderDuration.setObjectName(u"sliderDuration")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy.setHorizontalStretch(2)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sliderDuration.sizePolicy().hasHeightForWidth())
        self.sliderDuration.setSizePolicy(sizePolicy)
        self.sliderDuration.setMinimum(5)
        self.sliderDuration.setMaximum(5000)
        self.sliderDuration.setSingleStep(10)
        self.sliderDuration.setValue(10)
        self.sliderDuration.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_2.addWidget(self.sliderDuration)

        self.horizontalSpacer_4 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_4)

        self.label_7 = QLabel(self.centralwidget)
        self.label_7.setObjectName(u"label_7")

        self.horizontalLayout_2.addWidget(self.label_7)

        self.sliderPolarity = QSlider(self.centralwidget)
        self.sliderPolarity.setObjectName(u"sliderPolarity")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.sliderPolarity.sizePolicy().hasHeightForWidth())
        self.sliderPolarity.setSizePolicy(sizePolicy1)
        self.sliderPolarity.setMinimumSize(QSize(40, 0))
        self.sliderPolarity.setMinimum(0)
        self.sliderPolarity.setMaximum(2)
        self.sliderPolarity.setPageStep(1)
        self.sliderPolarity.setOrientation(Qt.Orientation.Horizontal)
        self.sliderPolarity.setTickPosition(QSlider.TickPosition.TicksAbove)
        self.sliderPolarity.setTickInterval(1)

        self.horizontalLayout_2.addWidget(self.sliderPolarity)

        self.horizontalSpacer_5 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_5)

        self.label_8 = QLabel(self.centralwidget)
        self.label_8.setObjectName(u"label_8")

        self.horizontalLayout_2.addWidget(self.label_8)

        self.sliderPause = QSliderWithTooltip(self.centralwidget)
        self.sliderPause.setObjectName(u"sliderPause")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy2.setHorizontalStretch(1)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.sliderPause.sizePolicy().hasHeightForWidth())
        self.sliderPause.setSizePolicy(sizePolicy2)
        self.sliderPause.setMinimum(5)
        self.sliderPause.setMaximum(50)
        self.sliderPause.setSingleStep(1)
        self.sliderPause.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_2.addWidget(self.sliderPause)

        self.label_9 = QLabel(self.centralwidget)
        self.label_9.setObjectName(u"label_9")

        self.horizontalLayout_2.addWidget(self.label_9)

        self.spinRepeats = QSpinBox(self.centralwidget)
        self.spinRepeats.setObjectName(u"spinRepeats")

        self.horizontalLayout_2.addWidget(self.spinRepeats)


        self.verticalLayout.addLayout(self.horizontalLayout_2)

        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.horizontalSpacer_6 = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_6)

        self.label_12 = QLabel(self.centralwidget)
        self.label_12.setObjectName(u"label_12")

        self.horizontalLayout_6.addWidget(self.label_12, 0, Qt.AlignmentFlag.AlignBottom)


        self.verticalLayout.addLayout(self.horizontalLayout_6)

        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Preferred)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy3)
        self.label.setMinimumSize(QSize(100, 0))

        self.horizontalLayout_3.addWidget(self.label)

        self.sliderVoltage = QSliderWithTooltip(self.centralwidget)
        self.sliderVoltage.setObjectName(u"sliderVoltage")
        sizePolicy4 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy4.setHorizontalStretch(3)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.sliderVoltage.sizePolicy().hasHeightForWidth())
        self.sliderVoltage.setSizePolicy(sizePolicy4)
        self.sliderVoltage.setMinimumSize(QSize(0, 0))
        self.sliderVoltage.setMinimum(10)
        self.sliderVoltage.setMaximum(100)
        self.sliderVoltage.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_3.addWidget(self.sliderVoltage)

        self.horizontalSpacer_7 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer_7)

        self.label_10 = QLabel(self.centralwidget)
        self.label_10.setObjectName(u"label_10")

        self.horizontalLayout_3.addWidget(self.label_10)

        self.sliderVoltageModFreq = QSliderWithTooltip(self.centralwidget)
        self.sliderVoltageModFreq.setObjectName(u"sliderVoltageModFreq")
        sizePolicy.setHeightForWidth(self.sliderVoltageModFreq.sizePolicy().hasHeightForWidth())
        self.sliderVoltageModFreq.setSizePolicy(sizePolicy)
        self.sliderVoltageModFreq.setMinimumSize(QSize(0, 0))
        self.sliderVoltageModFreq.setMinimum(1)
        self.sliderVoltageModFreq.setMaximum(200)
        self.sliderVoltageModFreq.setSingleStep(1)
        self.sliderVoltageModFreq.setValue(10)
        self.sliderVoltageModFreq.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_3.addWidget(self.sliderVoltageModFreq)

        self.horizontalSpacer = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer)

        self.label_2 = QLabel(self.centralwidget)
        self.label_2.setObjectName(u"label_2")

        self.horizontalLayout_3.addWidget(self.label_2)

        self.sliderVoltageModAmt = QSliderWithTooltip(self.centralwidget)
        self.sliderVoltageModAmt.setObjectName(u"sliderVoltageModAmt")
        sizePolicy.setHeightForWidth(self.sliderVoltageModAmt.sizePolicy().hasHeightForWidth())
        self.sliderVoltageModAmt.setSizePolicy(sizePolicy)
        self.sliderVoltageModAmt.setMinimumSize(QSize(0, 0))
        self.sliderVoltageModAmt.setMinimum(100)
        self.sliderVoltageModAmt.setMaximum(800)
        self.sliderVoltageModAmt.setSingleStep(10)
        self.sliderVoltageModAmt.setValue(300)
        self.sliderVoltageModAmt.setOrientation(Qt.Orientation.Horizontal)
        self.sliderVoltageModAmt.setTickPosition(QSlider.TickPosition.NoTicks)
        self.sliderVoltageModAmt.setTickInterval(200)

        self.horizontalLayout_3.addWidget(self.sliderVoltageModAmt)

        self.horizontalSpacer_8 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer_8)

        self.sliderVoltageWaveform = QSlider(self.centralwidget)
        self.sliderVoltageWaveform.setObjectName(u"sliderVoltageWaveform")
        self.sliderVoltageWaveform.setMaximum(3)
        self.sliderVoltageWaveform.setPageStep(1)
        self.sliderVoltageWaveform.setOrientation(Qt.Orientation.Horizontal)
        self.sliderVoltageWaveform.setTickPosition(QSlider.TickPosition.TicksAbove)
        self.sliderVoltageWaveform.setTickInterval(1)

        self.horizontalLayout_3.addWidget(self.sliderVoltageWaveform)


        self.verticalLayout.addLayout(self.horizontalLayout_3)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.label_5 = QLabel(self.centralwidget)
        self.label_5.setObjectName(u"label_5")
        sizePolicy3.setHeightForWidth(self.label_5.sizePolicy().hasHeightForWidth())
        self.label_5.setSizePolicy(sizePolicy3)
        self.label_5.setMinimumSize(QSize(100, 0))

        self.horizontalLayout_5.addWidget(self.label_5)

        self.sliderPW = QSliderWithTooltip(self.centralwidget)
        self.sliderPW.setObjectName(u"sliderPW")
        sizePolicy4.setHeightForWidth(self.sliderPW.sizePolicy().hasHeightForWidth())
        self.sliderPW.setSizePolicy(sizePolicy4)
        self.sliderPW.setMinimum(600)
        self.sliderPW.setMaximum(2500)
        self.sliderPW.setSingleStep(100)
        self.sliderPW.setPageStep(100)
        self.sliderPW.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_5.addWidget(self.sliderPW)

        self.horizontalSpacer_9 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_9)

        self.label_11 = QLabel(self.centralwidget)
        self.label_11.setObjectName(u"label_11")

        self.horizontalLayout_5.addWidget(self.label_11)

        self.sliderPWModFreq = QSliderWithTooltip(self.centralwidget)
        self.sliderPWModFreq.setObjectName(u"sliderPWModFreq")
        sizePolicy.setHeightForWidth(self.sliderPWModFreq.sizePolicy().hasHeightForWidth())
        self.sliderPWModFreq.setSizePolicy(sizePolicy)
        self.sliderPWModFreq.setMinimum(1)
        self.sliderPWModFreq.setMaximum(200)
        self.sliderPWModFreq.setSingleStep(1)
        self.sliderPWModFreq.setValue(10)
        self.sliderPWModFreq.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_5.addWidget(self.sliderPWModFreq)

        self.horizontalSpacer_3 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_3)

        self.label_6 = QLabel(self.centralwidget)
        self.label_6.setObjectName(u"label_6")

        self.horizontalLayout_5.addWidget(self.label_6)

        self.sliderPWModAmt = QSliderWithTooltip(self.centralwidget)
        self.sliderPWModAmt.setObjectName(u"sliderPWModAmt")
        sizePolicy.setHeightForWidth(self.sliderPWModAmt.sizePolicy().hasHeightForWidth())
        self.sliderPWModAmt.setSizePolicy(sizePolicy)
        self.sliderPWModAmt.setMinimum(100)
        self.sliderPWModAmt.setMaximum(800)
        self.sliderPWModAmt.setSingleStep(10)
        self.sliderPWModAmt.setValue(300)
        self.sliderPWModAmt.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_5.addWidget(self.sliderPWModAmt)

        self.horizontalSpacer_11 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_11)

        self.sliderPWModWaveform = QSlider(self.centralwidget)
        self.sliderPWModWaveform.setObjectName(u"sliderPWModWaveform")
        self.sliderPWModWaveform.setMaximum(3)
        self.sliderPWModWaveform.setPageStep(1)
        self.sliderPWModWaveform.setOrientation(Qt.Orientation.Horizontal)
        self.sliderPWModWaveform.setTickPosition(QSlider.TickPosition.TicksAbove)
        self.sliderPWModWaveform.setTickInterval(1)

        self.horizontalLayout_5.addWidget(self.sliderPWModWaveform)


        self.verticalLayout.addLayout(self.horizontalLayout_5)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.label_3 = QLabel(self.centralwidget)
        self.label_3.setObjectName(u"label_3")
        sizePolicy3.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy3)
        self.label_3.setMinimumSize(QSize(100, 0))

        self.horizontalLayout_4.addWidget(self.label_3)

        self.sliderFrequency = QSliderWithTooltip(self.centralwidget)
        self.sliderFrequency.setObjectName(u"sliderFrequency")
        sizePolicy4.setHeightForWidth(self.sliderFrequency.sizePolicy().hasHeightForWidth())
        self.sliderFrequency.setSizePolicy(sizePolicy4)
        self.sliderFrequency.setMinimum(170)
        self.sliderFrequency.setMaximum(5000)
        self.sliderFrequency.setSingleStep(100)
        self.sliderFrequency.setPageStep(100)
        self.sliderFrequency.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_4.addWidget(self.sliderFrequency)

        self.horizontalSpacer_10 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer_10)

        self.label_13 = QLabel(self.centralwidget)
        self.label_13.setObjectName(u"label_13")

        self.horizontalLayout_4.addWidget(self.label_13)

        self.sliderFrequencyModFreq = QSliderWithTooltip(self.centralwidget)
        self.sliderFrequencyModFreq.setObjectName(u"sliderFrequencyModFreq")
        sizePolicy.setHeightForWidth(self.sliderFrequencyModFreq.sizePolicy().hasHeightForWidth())
        self.sliderFrequencyModFreq.setSizePolicy(sizePolicy)
        self.sliderFrequencyModFreq.setMinimum(1)
        self.sliderFrequencyModFreq.setMaximum(200)
        self.sliderFrequencyModFreq.setSingleStep(1)
        self.sliderFrequencyModFreq.setValue(10)
        self.sliderFrequencyModFreq.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_4.addWidget(self.sliderFrequencyModFreq)

        self.horizontalSpacer_2 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer_2)

        self.label_4 = QLabel(self.centralwidget)
        self.label_4.setObjectName(u"label_4")

        self.horizontalLayout_4.addWidget(self.label_4)

        self.sliderFrequencyModAmt = QSliderWithTooltip(self.centralwidget)
        self.sliderFrequencyModAmt.setObjectName(u"sliderFrequencyModAmt")
        sizePolicy.setHeightForWidth(self.sliderFrequencyModAmt.sizePolicy().hasHeightForWidth())
        self.sliderFrequencyModAmt.setSizePolicy(sizePolicy)
        self.sliderFrequencyModAmt.setMinimum(100)
        self.sliderFrequencyModAmt.setMaximum(800)
        self.sliderFrequencyModAmt.setSingleStep(10)
        self.sliderFrequencyModAmt.setValue(300)
        self.sliderFrequencyModAmt.setOrientation(Qt.Orientation.Horizontal)

        self.horizontalLayout_4.addWidget(self.sliderFrequencyModAmt)

        self.horizontalSpacer_12 = QSpacerItem(20, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer_12)

        self.sliderFrequencyModWaveform = QSlider(self.centralwidget)
        self.sliderFrequencyModWaveform.setObjectName(u"sliderFrequencyModWaveform")
        self.sliderFrequencyModWaveform.setMaximum(3)
        self.sliderFrequencyModWaveform.setPageStep(1)
        self.sliderFrequencyModWaveform.setOrientation(Qt.Orientation.Horizontal)
        self.sliderFrequencyModWaveform.setTickPosition(QSlider.TickPosition.TicksAbove)
        self.sliderFrequencyModWaveform.setTickInterval(1)

        self.horizontalLayout_4.addWidget(self.sliderFrequencyModWaveform)


        self.verticalLayout.addLayout(self.horizontalLayout_4)

        self.textEdit = QTextEdit(self.centralwidget)
        self.textEdit.setObjectName(u"textEdit")
        self.textEdit.setMinimumSize(QSize(0, 50))

        self.verticalLayout.addWidget(self.textEdit)


        self.horizontalLayout.addLayout(self.verticalLayout)

        self.buttonSend = QPushButton(self.centralwidget)
        self.buttonSend.setObjectName(u"buttonSend")
        self.buttonSend.setMinimumSize(QSize(0, 80))

        self.horizontalLayout.addWidget(self.buttonSend)


        self.horizontalLayout_7.addLayout(self.horizontalLayout)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1303, 22))
        self.menuFile = QMenu(self.menubar)
        self.menuFile.setObjectName(u"menuFile")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuFile.menuAction())
        self.menuFile.addAction(self.actionCOM_setting)
        self.menuFile.addAction(self.actionExit)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"Burst Creator", None))
        self.actionCOM_setting.setText(QCoreApplication.translate("MainWindow", u"COM setting", None))
        self.actionExit.setText(QCoreApplication.translate("MainWindow", u"Exit", None))
        self.labelDuration.setText(QCoreApplication.translate("MainWindow", u"Duration (s)", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"Polarity", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Pause (s) ", None))
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"Repeats", None))
        self.label_12.setText(QCoreApplication.translate("MainWindow", u"Off | Sin | Saw | Tri", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"Voltage  (V)", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Mod Freq", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"Mod Amt (%)", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"Pulse Width (us)", None))
        self.label_11.setText(QCoreApplication.translate("MainWindow", u"Mod Freq", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Mod Amt (%)", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"Frequency (Hz)", None))
        self.label_13.setText(QCoreApplication.translate("MainWindow", u"Mod Freq", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Mod Amt (%)", None))
        self.buttonSend.setText(QCoreApplication.translate("MainWindow", u"Send", None))
        self.menuFile.setTitle(QCoreApplication.translate("MainWindow", u"File", None))
    # retranslateUi

