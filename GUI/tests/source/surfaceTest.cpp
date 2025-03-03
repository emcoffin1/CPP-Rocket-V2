#include "../headers/surfaceTest.h"

#include <QWidget>

SurfaceTest::SurfaceTest(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Wifi init
    wifi = wifiInstance;

}
