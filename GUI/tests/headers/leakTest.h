#ifndef LEAKTEST_H
#define LEAKTEST_H
#include <QGridLayout>
#include <QWidget>
#include "../../../ESP_WIFI/wifi.h"

class LeakTest : public QWidget {
    Q_OBJECT
private:
    WIFI *wifi;

public:
    explicit LeakTest(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

    static void startTest();
    void updateListener(QString data);

    // Things
    QGridLayout *g_layout;




};

#endif //LEAKTEST_H
