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
    f_layout->setAlignment(Qt::AlignCenter);
    formLayoutWrapper->addLayout(f_layout);
    formwidget->setLayout(formLayoutWrapper);
    formwidget->setFixedSize(QSize(200, 100));


    // Start Button
    startTest = ConstantUses::buttonMaker("Start Test", 25, "#bc2626");
    startTest->setFixedSize(200,50);


    // Test Data
    send_label = new QLabel("Send time: ");
    send_label->setAlignment(Qt::AlignLeft);
    send_label->setStyleSheet("color: white; font-size: 20px;");
    send_time = new QLabel("00:00");
    send_time->setAlignment(Qt::AlignRight);
    send_time->setStyleSheet("color: white; font-size: 20px;");
    f_layout->addRow(send_label,send_time);

    recv_label = new QLabel("Recv time: ");
    recv_label->setAlignment(Qt::AlignLeft);
    recv_label->setStyleSheet("color: white; font-size: 20px;");
    recv_time = new QLabel("00:00");
    recv_time->setAlignment(Qt::AlignRight);
    recv_time->setStyleSheet("color: white; font-size: 20px;");
    f_layout->addRow(recv_label,recv_time);


    // Layout
    g_layout->addStretch(2);
    g_layout->addWidget(formwidget, Qt::AlignVCenter);
    g_layout->addStretch(2);
    g_layout->addWidget(startTest,0 , Qt::AlignCenter);
    g_layout->addStretch(1);
    setLayout(g_layout);

    connect(startTest, &QPushButton::clicked, this, &ResponseTest::runTest);

};

// Perform actions to run test
void ResponseTest::runTest() const {
    // Figure out logic to determine send time and receive time
    wifi->sendMessage("TEST: ResponseTest");
}

// Update the values
void ResponseTest::updateValues(QJsonObject jsonObj) {
    jsonObj = jsonObj["TEST"].toObject();

    if (jsonObj.contains("RoundTripTime")) {
        int roundTrip = jsonObj["RoundTripTime"].toInt();
        int oneWay = jsonObj["OneWayTime"].toInt();
        send_time->setText(QString::number(roundTrip));
        recv_time->setText(QString::number(oneWay));
    }
}
