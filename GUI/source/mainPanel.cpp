#include "../headers/mainPanel.h"

#include <iostream>

//#include <bits/fs_fwd.h>

MainPanel::MainPanel(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    if (!wifiInstance) {
        qDebug() << "wifiInstance is null";
    }

    // Create a single QVBoxLayout for MainPanel.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // WIFI Init
    wifi = wifiInstance;


    // Top layout
    con_Layout = new QHBoxLayout();


    // Status Checker
    statusCheck = new QCheckBox("Connection:",this);

    statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: grey;"
                               "border-radius: 5px;}");
    statusCheck->setLayoutDirection(Qt::RightToLeft);
    statusCheck->setCheckable(false);


    // Time Display
    QString timeVal = ConstantUses::instance()->currentTime();
    timeLabel = new QLabel(timeVal, this);
    timeLabel->setStyleSheet("QLabel{"
                             "font-size: 12px;"
                             "color: white}");


    con_Layout->addWidget(timeLabel, Qt::AlignCenter);
    con_Layout->addWidget(statusCheck, Qt::AlignRight);




    // Create the QStackedWidget.
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setContentsMargins(0, 0, 0, 0);


    // Sample widgets.
    mainTab = new MainTab(this, wifi);
    testTab = new TestTab(this, wifi);
    setTab = new SetTab(this, wifi);

    stackedWidget->addWidget(mainTab);
    stackedWidget->addWidget(testTab);
    stackedWidget->addWidget(setTab);

    // Add the label and stacked widget to the main layout.

    mainLayout->addLayout(con_Layout);
    mainLayout->addWidget(stackedWidget);

    // This layout will now control the entire MainPanel with no extra margins.
    setLayout(mainLayout);

    connect(ConstantUses::instance(), &ConstantUses::timeUpdated, this, &MainPanel::updateTime);
    connect(wifiInstance, &WIFI::rssiUpdated, this, &MainPanel::changeConnectionStatus);
}

void MainPanel::updateTime(const QString timeVal) const {
    timeLabel->setText(timeVal);
}

void MainPanel::switchPanel(int index) const {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    }
}

MainPanel::~MainPanel() {
    delete mainTab;
    delete setTab;
    delete testTab;
}

void MainPanel::changeConnectionStatus(QJsonObject value) {
    int val = value["CONNECTION"].toInt();

    if (val <= -100) {
        // No connection, set to grey
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #808080;"
                               "border-radius: 5px;}");

    }
    if (val <= -90 && val >= -99) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                       "color:white; "
                       "padding: 0px;}"
                       "QCheckBox::indicator {"
                       "background-color: #8B0000;"
                       "border-radius: 5px;}");

    }
    if (val <= -80 && val >= -89) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #FF4500;"
                               "border-radius: 5px;}");

    }
    if (val <= -70 && val >= -79) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #FFA500;"
                               "border-radius: 5px;}");

    }
    if (val <= -60 && val >= -69) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #FFFF00;"
                               "border-radius: 5px;}");

    }
    if (val <= -50 && val >= -59) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #7FFF00;"
                               "border-radius: 5px;}");

    }
    if (val <= 0 && val >= -49) {
        // Terrible connection set to red
        statusCheck->setStyleSheet("QCheckBox {"
                               "color:white; "
                               "padding: 0px;}"
                               "QCheckBox::indicator {"
                               "background-color: #00FF00;"
                               "border-radius: 5px;}");

    }
}