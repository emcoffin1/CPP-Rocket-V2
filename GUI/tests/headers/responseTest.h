
#ifndef RESPONSETEST_H
#define RESPONSETEST_H
#include <qformlayout.h>
#include <QVBoxLayout>
#include <QWidget>
#include "../../../ESP_WIFI/wifi.h"
#include <QLabel>


class ResponseTest : public QWidget {
    Q_OBJECT
public:
    explicit ResponseTest(QWidget *parent = 0, WIFI *wifiInstance = nullptr);
    WIFI *wifi;



private:
    QPushButton *startTest;
    QVBoxLayout *g_layout;
    QFormLayout *f_layout;
    QWidget *formwidget;
    QVBoxLayout *formLayoutWrapper;

    QLabel *send_label;
    QLabel *send_time;
    QLabel *recv_label;
    QLabel *recv_time;

private slots:
    void runTest();
    void updateValues(QJsonObject jsonObj);

};

#endif //RESPONSETEST_H
