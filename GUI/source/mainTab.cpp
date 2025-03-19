#include "../headers/mainTab.h"

#include <QPushButton>


MainTab::MainTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init
    wifi = wifiInstance;
    testPlot1 = new GraphController(this);
    testPlot2 = new GraphController(this);
    testPlot3 = new GraphController(this);
    testPlot4 = new GraphController(this);


    // Create layout
    g_layout = new QGridLayout(this);

    // Arm layout
    armLayout = new QVBoxLayout();
    armLayout->setContentsMargins(0,0,0,0);

    // Arm Indicator
    arm1 = ConstantUses::instance()->buttonMaker("Arm 1", 15, "#bc2626");
    arm1->setFixedSize(50,37.5);
    armLayout->addWidget(arm1);


    arm2 = ConstantUses::instance()->buttonMaker("Arm 2", 15, "#bc2626");
    arm2->setFixedSize(50,37.5);
    arm2->setEnabled(false);
    armLayout->addWidget(arm2);

    // Button
    btn = ConstantUses::instance()->buttonMaker("Fire", 25, "#bc2626");
    btn->setFixedHeight(75);

    // Plot
    plotLayout1 = new QVBoxLayout();
    plotWidget1 = new QWidget();
    QStringList plot1List = ConstantUses::instance()->getConfig("PLOT1");
    testPlot1->createPlot("Test", "time", "", plot1List, QColor("black"));
    plotLayout1->addWidget(testPlot1);
    plotWidget1->setLayout(plotLayout1);
    plotWidget1->setStyleSheet("border: 1px solid gray");

    plotWidget2 = new QWidget();
    plotLayout2 = new QVBoxLayout();
    QStringList plot2List = ConstantUses::instance()->getConfig("PLOT2");
    testPlot2->createPlot("Test", "time", "", plot2List, QColor("black"));
    plotLayout2->addWidget(testPlot2);
    plotWidget2->setLayout(plotLayout2);
    plotWidget2->setStyleSheet("border: 1px solid gray");

    plotWidget3 = new QWidget();
    plotLayout3 = new QVBoxLayout();
    QStringList plot3List = ConstantUses::instance()->getConfig("PLOT3");
    testPlot3->createPlot("Test", "time", "", plot3List, QColor("black"));
    plotLayout3->addWidget(testPlot3);
    plotWidget3->setLayout(plotLayout3);
    plotWidget3->setStyleSheet("border: 1px solid gray");

    plotWidget4 = new QWidget();
    plotLayout4 = new QVBoxLayout();
    QStringList plot4List = ConstantUses::instance()->getConfig("PLOT4");
    testPlot4->createPlot("Test", "time", "", plot4List, QColor("black"));
    plotLayout4->addWidget(testPlot4);
    plotWidget4->setLayout(plotLayout4);
    plotWidget4->setStyleSheet("border: 1px solid gray");

    // Valve Display
    valveTree = new ValveTree(this, wifiInstance);

    // Vehicle Displays
    QString topdown = "rocket_top_profile.png";
    rollDisplay = new RollDisplay(this, wifiInstance, topdown, "Roll",
                                    0, 150, 150);

    QString side = "rocket_side_profile_pointed.png";
    pitchDisplay = new RollDisplay(this, wifiInstance, side, "Pitch",
                                    90, 150, 400);


    // Velocity display
    vel_layout = new QHBoxLayout();
    velocity_label = new QLabel("Velocity: ", this);
    velocity_label->setStyleSheet("color: white;font-size: 15px;");
    velocity = new QLabel("0", this);
    velocity->setStyleSheet("color: white;font-size: 15px;");
    vel_layout->addWidget(velocity_label);
    vel_layout->addWidget(velocity);
    vel_layout->addStretch(1);
    vel_layout->setContentsMargins(0,0,0,0);

    // Altitude Display
    alt_layout = new QHBoxLayout();
    altitude_label = new QLabel("ALT:", this);
    altitude_label->setStyleSheet("color: white;font-size: 15px;");
    altitude = new QLabel("00.00", this);
    altitude->setStyleSheet("color: white;font-size: 15px;");
    alt_layout->addWidget(altitude_label);
    alt_layout->addWidget(altitude);
    alt_layout->addStretch(1);
    alt_layout->setContentsMargins(0,0,0,0);

    // Vel Alt layout
    data_layout = new QVBoxLayout();
    data_layout->setContentsMargins(0,0,0,0);
    data_layout->addStretch(1);
    data_layout->addLayout(alt_layout);
    data_layout->addLayout(vel_layout);
    data_layout->addStretch(1);



    // Clock label
    clockLabel = new QLabel("T-00:05",this);
    clockLabel->setStyleSheet("font-size: 30px; color: white;");

    // Add widgets to grid
    g_layout->addWidget(valveTree, 0,4,1,1);
    g_layout->addWidget(pitchDisplay, 1 , 4,2,1);
    g_layout->addWidget(rollDisplay, 3 , 4,1,1);
    g_layout->addWidget(plotWidget1, 0, 0, 2, 2);
    g_layout->addWidget(plotWidget2, 0, 2, 2, 2);
    g_layout->addWidget(plotWidget3, 2, 0, 2, 2);
    g_layout->addWidget(plotWidget4, 2, 2, 2, 2);
    g_layout->addLayout(armLayout, 4, 0, 1, 1, Qt::AlignRight);
    g_layout->addWidget(btn, 4, 1, 1,1);
    g_layout->addWidget(clockLabel, 4, 2, 1, 1, Qt::AlignCenter);
    g_layout->addLayout(data_layout, 4, 3, 1, 1);

    g_layout->setRowStretch(0,1);
    g_layout->setRowStretch(1,1);
    g_layout->setRowStretch(2,1);
    g_layout->setRowStretch(3,1);
    g_layout->setRowStretch(4,1);
    g_layout->setColumnStretch(0,1);
    g_layout->setColumnStretch(1,1);
    g_layout->setColumnStretch(2,1);
    g_layout->setColumnStretch(3,1);
    g_layout->setColumnStretch(4,1);
    //g_layout->setColumnStretch(5,1);


    setLayout(g_layout);

    if (wifi) {
        connect(wifi, &WIFI::sensorUpdated, this, &MainTab::updateTables);
        connect(wifi, &WIFI::padArmed, this, &MainTab::padArm);
    }
    connect(ConstantUses::instance(), &ConstantUses::countdownUpdated, this, &MainTab::updateTime);
    connect(btn, &QPushButton::clicked, this, &MainTab::startFire);
    connect(arm1, &QPushButton::clicked, this, &MainTab::armButton);
}

void MainTab::updateTables(QJsonObject jsonDoc) const {
    testPlot1->addDataPoint(jsonDoc);
    testPlot2->addDataPoint(jsonDoc);
    testPlot3->addDataPoint(jsonDoc);
    testPlot4->addDataPoint(jsonDoc);
}

void MainTab::updateValues(QJsonObject jsonDoc) const {
    if (jsonDoc.contains("Velocity")) {
        velocity_label->setText(jsonDoc["Velocity"].toString());
    }
    if (jsonDoc.contains("Altitude")) {
        altitude_label->setText(jsonDoc["Altitude"].toString());
    }
}

void MainTab::updateTime(QString countdownTime) const {
    clockLabel->setText(countdownTime);
}

// Start a bunch of stuff to signal fire
void MainTab::startFire() {
    // Only trigger if both armed
    if (armed && pad_armed) {
        ConstantUses::instance()->logEvent("Fire Initiated");
        ConstantUses::instance()->startCountdown();
    }

}

// Pad arm
void MainTab::padArm() {
    // Change color and state
    if (pad_armed) {
        pad_armed = false;
        arm2->setStyleSheet("background-color: #bc2626; font-size: 15px; color: white;");
        arm2->setFixedSize(50,37.5);
        ConstantUses::instance()->logEvent("Pad Disarmed");

    } else {
        pad_armed = true;
        arm2->setStyleSheet("background-color: green; font-size: 15px; color: white");
        arm2->setFixedSize(50,37.5);
        ConstantUses::instance()->logEvent("Pad Armed");
    }
}

// Arm controller
void MainTab::armButton() {
    if (armed) {
        arm1->setStyleSheet("background-color: #bc2626; font-size: 15px; color: white;");
        arm1->setFixedSize(50,37.5);
        armed = false;
        ConstantUses::instance()->logEvent("Controller Disarmed");
        // Send message to switch to test mode

    } else {
        arm1->setStyleSheet("background-color: green; font-size: 15px; color: white");
        arm1->setFixedSize(50,37.5);
        armed = true;
        ConstantUses::instance()->logEvent("Controller Armed");
        // Send message to switch to fire mode

    }
}

