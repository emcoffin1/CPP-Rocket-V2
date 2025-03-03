#ifndef IGNITERTEST_H
#define IGNITERTEST_H
#include <QWidget>
#include "../../../ESP_WIFI/wifi.h"
class IgniterTest : public QWidget {
    Q_OBJECT

public:
    explicit IgniterTest(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);
    WIFI *wifi;
};

#endif //IGNITERTEST_H
