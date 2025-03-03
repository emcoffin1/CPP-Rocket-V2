//
// Created by emcof on 3/2/2025.
//

#ifndef SURFACETEST_H
#define SURFACETEST_H
#include <QWidget>
#include "../../../ESP_WIFI/wifi.h"

class SurfaceTest : public QWidget {
    Q_OBJECT;

public:
    explicit SurfaceTest(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);
    WIFI *wifi;


};

#endif //SURFACETEST_H
