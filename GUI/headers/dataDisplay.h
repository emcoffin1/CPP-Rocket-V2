#ifndef DATADISPLAY_H
#define DATADISPLAY_H
#include <QWidget>

#include "mainTab.h"
#include "../../ESP_WIFI/wifi.h"

class DataDisplay : public QWidget {
    Q_OBJECT

public:
    explicit DataDisplay(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

public slots:
    void updateEvery(QJsonObject message);
    void updateValve(QJsonObject message);
    void updatePosition(QJsonObject message);
    void updateSensor(QJsonObject message);


private:
    WIFI *wifi;
    QVBoxLayout *v_layout;
    QTabWidget *tab_widget;

    QVBoxLayout *sensorLayout;
    QWidget *sensorWidget;
    QTextEdit *sensorBox;

    QVBoxLayout *valveLayout;
    QWidget *valveWidget;
    QTextEdit *valveBox;

    QVBoxLayout *everyLayout;
    QWidget *everyWidget;
    QTextEdit *everyBox;

    QVBoxLayout *positionLayout;
    QWidget *positionWidget;
    QTextEdit *positionBox;


};

#endif //DATADISPLAY_H