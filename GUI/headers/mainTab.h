#ifndef MAINTAB_H
#define MAINTAB_H
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include "../../ESP_WIFI/wifi.h"
#include "../../logic/headers/graphController.h"
#include "../../logic/headers/vehicleDisplay.h"

class MainTab : public QWidget {
    Q_OBJECT

public:

    explicit MainTab(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

public slots:

private:
    WIFI* wifi;
    ConstantUses* constants;


    QLabel *test;
    QGridLayout *g_layout;
    QPushButton *btn;
    GraphController *testPlot;
    GraphController *testPlot1;
    GraphController *testPlot2;
    GraphController *testPlot3;
    RollDisplay *rollDisplay, *pitchDisplay;
    ValveTree *valveTree;

    QLabel *clockLabel;

    QVBoxLayout *armLayout;

    QPushButton *arm1;
    QPushButton *arm2;

    QVBoxLayout *data_layout;

    // Velocity
    QLabel *velocity_label;
    QLabel *velocity;
    QHBoxLayout *vel_layout;

    // Altitude
    QLabel *altitude_label;
    QLabel *altitude;
    QHBoxLayout *alt_layout;


    bool armed = false;
    bool pad_armed = false;

private slots:
    void updateTables(QJsonObject jsonDoc) const;
    void updateTime(QString countdownTime) const;
    void updateValues(QJsonObject jsonDoc) const;

    void startFire();
    void armButton();
    void padArm();

};


#endif //MAINTAB_H