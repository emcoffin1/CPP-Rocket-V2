#include "../headers/mainTab.h"

#include <QPushButton>


MainTab::MainTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init
    wifi = wifiInstance;
    testPlot = new GraphController(this);
    testPlot1 = new GraphController(this);
    testPlot2 = new GraphController(this);
    testPlot3 = new GraphController(this);


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
    QStringList plot1List = ConstantUses::instance()->getConfig("PLOT1");
    testPlot->createPlot("Test", "time", "", plot1List, QColor("black"));
    //testPlot->setFixedSize(200,100);

    QStringList plot2List = ConstantUses::instance()->getConfig("PLOT2");
    testPlot1->createPlot("Test", "time", "", plot2List, QColor("black"));

    QStringList plot3List = ConstantUses::instance()->getConfig("PLOT3");
    testPlot2->createPlot("Test", "time", "", plot3List, QColor("black"));

    QStringList plot4List = ConstantUses::instance()->getConfig("PLOT4");
    testPlot3->createPlot("Test", "time", "", plot4List, QColor("black"));


    // Valve Display
    valveTree = new ValveTree(this, wifiInstance);

    // Vehicle Displays
    QString topdown = "rocket_top_profile.png";
    rollDisplay = new RollDisplay(this, wifiInstance, topdown, "ROLL",
                                    0, 150, 150);

    QString side = "rocket_side_profile_pointed.png";
    pitchDisplay = new RollDisplay(this, wifiInstance, side, "PITCH",
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
    g_layout->addWidget(testPlot, 0, 0, 2, 2);
    g_layout->addWidget(testPlot1, 0, 2, 2, 2);
    g_layout->addWidget(testPlot2, 2, 0, 2, 2);
    g_layout->addWidget(testPlot3, 2, 2, 2, 2);
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
    testPlot->addDataPoint(jsonDoc);
    testPlot1->addDataPoint(jsonDoc);
    testPlot2->addDataPoint(jsonDoc);
    testPlot3->addDataPoint(jsonDoc);
}

void MainTab::updateValues(QJsonObject jsonDoc) const {
    if (jsonDoc.contains("VELOCITY")) {
        velocity_label->setText(jsonDoc["VELOCITY"].toString());
    }
    if (jsonDoc.contains("ALTITUDE")) {
        altitude_label->setText(jsonDoc["ALTITUDE"].toString());
    }
}

void MainTab::updateTime(QString countdownTime) const {
    clockLabel->setText(countdownTime);
}

// Start a bunch of stuff to signal fire
void MainTab::startFire() {
    // Only trigger if both armed
    if (armed && pad_armed) {
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

    } else {
        pad_armed = true;
        arm2->setStyleSheet("background-color: green; font-size: 15px; color: white");
        arm2->setFixedSize(50,37.5);
    }
}

// Arm controller
void MainTab::armButton() {
    if (armed) {
        arm1->setStyleSheet("background-color: #bc2626; font-size: 15px; color: white;");
        arm1->setFixedSize(50,37.5);
        armed = false;
        // Send message to switch to test mode

    } else {
        arm1->setStyleSheet("background-color: green; font-size: 15px; color: white");
        arm1->setFixedSize(50,37.5);
        armed = true;
        // Send message to switch to fire mode

    }
}

