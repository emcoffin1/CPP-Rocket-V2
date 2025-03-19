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





    // Battery state
    battery_layout = new QHBoxLayout();
    battery_label = new QLabel("Battery: ", this);
    battery_percentage = new QLabel("0%", this);
    battery_label->setStyleSheet("QLabel{"
                         "font-size: 12px;"
                         "color: white}");
    battery_percentage->setStyleSheet("QLabel{"
                             "font-size: 12px;"
                             "color: white}");
    battery_layout->addWidget(battery_label);
    battery_layout->addWidget(battery_percentage);
    battery_layout->addStretch(1);

    con_Layout->addLayout(battery_layout);
    con_Layout->addStretch(1);
    con_Layout->addWidget(timeLabel);
    con_Layout->addStretch(1);
    con_Layout->addWidget(statusCheck);



    // Create the QStackedWidget.
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setContentsMargins(0, 0, 0, 0);


    // Sample widgets.
    mainTab = new MainTab(this, wifi);
    testTab = new TestTab(this, wifi);
    setTab = new SetTab(this, wifi);
    debugTab = new DebugTab(this, wifi);
    dataTab = new DataDisplay(this, wifi);

    stackedWidget->addWidget(mainTab);
    stackedWidget->addWidget(testTab);
    stackedWidget->addWidget(setTab);
    stackedWidget->addWidget(debugTab);
    stackedWidget->addWidget(dataTab);

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
    QJsonObject connection = value["CONNECTION"].toObject();

    if (connection.contains("wifi")) {
        int val = connection["wifi"].toInt();
        QString color;

        if (val <= -100) {color = "#808080";}               // grey

        if (val <= -90 && val >= -99) {color = "#8B0000";}  // dark red

        if (val <= -80 && val >= -89) {color = "#FF4500";}  // orange red

        if (val <= -70 && val >= -79) {color = "#FFA500";}  // orange

        if (val <= -60 && val >= -69) { color = "#FFFF00";} // yellow

        if (val <= -50 && val >= -59) { color = "#7FFF00";} // chartreuse

        if (val <= 0 && val >= -49) {color = "#00FF00";}    // green

        // Update color code
        statusCheck->setStyleSheet(QString("QCheckBox {"
                                   "color:white; "
                                   "padding: 0px;}"
                                   "QCheckBox::indicator {"
                                   "background-color: %1;"
                                   "border-radius: 5px;}").arg(color));
    }

    if (connection.contains("Battery")) {
        // If battery in json, change battery status
        int percent = connection["Battery"].toInt();
        battery_percentage->setText(QString("%1%").arg(percent));
    }
}