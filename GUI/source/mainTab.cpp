#include "../headers/mainTab.h"

#include <QPushButton>


MainTab::MainTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init
    wifi = wifiInstance;
    testPlot = new GraphController(this);
    testPlot1 = new GraphController(this);
    testPlot2 = new GraphController(this);


    // Create layout
    g_layout = new QGridLayout(this);

    // Arm layout
    armLayout = new QVBoxLayout();
    armLayout->setContentsMargins(0,0,0,0);

    // Arm Indicator
    arm1 = ConstantUses::instance()->buttonMaker("Arm 1", 15, "red");
    arm1->setFixedSize(50,37.5);
    armLayout->addWidget(arm1);


    arm2 = ConstantUses::instance()->buttonMaker("Arm 2", 15, "red");
    arm2->setFixedSize(50,37.5);
    arm2->setEnabled(false);
    armLayout->addWidget(arm2);

    // Button
    btn = ConstantUses::instance()->buttonMaker("Fire", 25, "red");
    btn->setFixedHeight(75);

    // Plot
    QStringList plot1List = ConstantUses::instance()->getConfig("PLOT1");
    testPlot->createPlot("Test", "time", "", plot1List, QColor("black"));
    //testPlot->setFixedSize(200,100);

    QStringList plot2List = ConstantUses::instance()->getConfig("PLOT2");
    testPlot1->createPlot("Test", "time", "", plot2List, QColor("black"));

    QStringList plot3List = ConstantUses::instance()->getConfig("PLOT3");
    testPlot2->createPlot("Test", "time", "", plot3List, QColor("black"));


    // Valve Display
    valveTree = new ValveTree(this, wifiInstance);

    // Vehicle Displays
    QString topdown = "rocket_top_profile.png";
    rollDisplay = new RollDisplay(this, wifiInstance, topdown, "ROLL",
                                    0, 150, 150);

    QString side = "rocket_side_profile_pointed.png";
    pitchDisplay = new RollDisplay(this, wifiInstance, side, "PITCH",
                                    90, 150, 400);


    // Clock label
    clockLabel = new QLabel("T-00:05",this);
    clockLabel->setStyleSheet("font-size: 30px; color: white;");

    // Add widgets to grid
    g_layout->addWidget(valveTree, 0,5,1,1);
    g_layout->addWidget(pitchDisplay, 1 , 5,2,1);
    g_layout->addWidget(rollDisplay, 3 , 5,1,1);
    g_layout->addWidget(testPlot, 1, 0, 1, 1);
    g_layout->addWidget(testPlot1, 1, 2, 1, 1);
    g_layout->addWidget(testPlot2, 1, 4, 1, 1);
    g_layout->addLayout(armLayout, 3, 0, 1, 1, Qt::AlignRight);
    g_layout->addWidget(btn, 3, 1, 1,2);
    g_layout->addWidget(clockLabel, 3, 3, 1, 1, Qt::AlignCenter);

    g_layout->setRowStretch(0,1);
    g_layout->setRowStretch(1,1);
    g_layout->setRowStretch(2,1);
    g_layout->setRowStretch(3,1);
    g_layout->setColumnStretch(0,1);
    g_layout->setColumnStretch(1,1);
    g_layout->setColumnStretch(2,1);
    g_layout->setColumnStretch(3,1);
    g_layout->setColumnStretch(4,1);
    g_layout->setColumnStretch(5,1);


    setLayout(g_layout);

    if (wifi) {
        connect(wifi, &WIFI::sensorUpdated, this, &MainTab::updateTables);
    }
    connect(ConstantUses::instance(), &ConstantUses::countdownUpdated, this, &MainTab::updateTime);
    connect(btn, &QPushButton::clicked, this, &MainTab::startFire);
    connect(arm1, &QPushButton::clicked, this, &MainTab::armButton);
}

void MainTab::updateTables(QJsonObject jsonDoc) const {
    testPlot->addDataPoint(jsonDoc);
    testPlot1->addDataPoint(jsonDoc);
    testPlot2->addDataPoint(jsonDoc);
}

void MainTab::updateTime(QString countdownTime) const {
    clockLabel->setText(countdownTime);
}

// Start a bunch of stuff to signal fire
void MainTab::startFire() {
    ConstantUses::instance()->startCountdown();
}

// Arm controller
void MainTab::armButton() {
    if (armed) {
        arm1->setStyleSheet("background-color: red; font-size: 15px; color: white;");
        arm1->setFixedSize(50,37.5);
        armed = false;
    } else {
        arm1->setStyleSheet("background-color: green; font-size: 15px; color: white");
        arm1->setFixedSize(50,37.5);
        armed = true;
    }
}

