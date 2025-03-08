#include "../headers/responseTest.h"

#include <qlineedit.h>

ResponseTest::ResponseTest(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init
    wifi = wifiInstance;

    // Layout
    g_layout = new QVBoxLayout(this);
    g_layout->setAlignment(Qt::AlignCenter);

    formwidget = new QWidget();
    formLayoutWrapper = new QVBoxLayout(formwidget);
    f_layout = new QFormLayout();
    f_layout->setAlignment(Qt::AlignHCenter);
    formLayoutWrapper->addLayout(f_layout);
    formwidget->setLayout(formLayoutWrapper);
    formwidget->setFixedSize(QSize(300, 100));


    // Start Button
    startTest = ConstantUses::buttonMaker("Start Test", 25, "#bc2626");
    startTest->setFixedSize(200,50);


    // Test Data
    recv_label = new QLabel("Recv time: ", this);
    recv_label->setAlignment(Qt::AlignLeft);
    recv_label->setStyleSheet("color: white; font-size: 20px;");
    recv_time = new QLabel("00:00", this);
    recv_time->setAlignment(Qt::AlignRight);
    recv_time->setStyleSheet("color: white; font-size: 20px;");
    f_layout->addRow(recv_label,recv_time);

    // CAN Data
    CANBus1 = new QLabel("Sensor CAN Bus: ", this);
    CANBus1->setAlignment(Qt::AlignLeft);
    CANBus1->setStyleSheet("color: white; font-size: 20px;");
    CAN1Val = new QLabel("Error", this);
    CAN1Val->setStyleSheet("color: white; font-size: 20px;");
    CAN1Val->setAlignment(Qt::AlignRight);
    CANBus2= new QLabel("Controller CAN Bus: ", this);
    CANBus2->setAlignment(Qt::AlignLeft);
    CANBus2->setStyleSheet("color: white; font-size: 20px;");
    CAN2Val = new QLabel("Error", this);
    CAN2Val->setStyleSheet("color: white; font-size: 20px;");
    CAN2Val->setAlignment(Qt::AlignRight);

    f_layout->addRow(CANBus1, CAN1Val);
    f_layout->addRow(CANBus2, CAN2Val);

    // Layout
    g_layout->addStretch(2);
    g_layout->addWidget(formwidget, Qt::AlignVCenter);
    g_layout->addStretch(2);
    g_layout->addWidget(startTest,0 , Qt::AlignCenter);
    g_layout->addStretch(1);
    setLayout(g_layout);

    connect(startTest, &QPushButton::clicked, this, &ResponseTest::runTest);
    connect(wifi, &WIFI::testUpdated, this, &ResponseTest::updateValues);
};

// Perform actions to run test
void ResponseTest::runTest() const {
    // Figure out logic to determine send time and receive time
    wifi->sendMessage("ResponseTest");
}

// Update the values
void ResponseTest::updateValues(QJsonObject jsonObj) {

    // Parse data
    if (jsonObj.contains("RoundTripTime")) {
        int roundTrip = jsonObj["RoundTripTime"].toInt();
        recv_time->setText(QString::number(roundTrip));

        if (jsonObj.contains("CANBus1")) {
            bool can1 = jsonObj["CANBus1"].toBool();
            CAN1Val->setText(QString::number(can1));
        }
        if (jsonObj.contains("CANBus2")) {
            bool can2 = jsonObj["CANBus2"].toBool();
            CAN2Val->setText(QString::number(can2));
        } else {
            CAN1Val->setText("Error");
            CAN2Val->setText("Error");
        }
    }

}
