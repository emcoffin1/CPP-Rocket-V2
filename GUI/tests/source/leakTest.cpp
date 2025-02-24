#include "../headers/leakTest.h"
#include <QLabel>

LeakTest::LeakTest(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    g_layout = new QGridLayout(this);
    setStyleSheet("background-color: white;");


    // WIFI Init
    wifi = wifiInstance;

    // Data connect
    connect(wifi, &WIFI::sensorUpdated, this, &LeakTest::updateListener);


}


void LeakTest::startTest() {

}

void LeakTest::updateListener(QString data) {

    qDebug() << "LeakTest data recieved:" << data;
}