#ifndef SETTAB_H
#define SETTAB_H
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "../../ESP_WIFI/wifi.h"
#include <QComboBox>
#include "../../logic/headers/graphController.h"

class SetTab : public QWidget {
    Q_OBJECT


public slots:
    void connectWIFI() const;
    void changeDataType() const;

public:
    explicit SetTab(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

    GraphController *graph_controller;
    QGridLayout *g_layout;
    QLabel *title;
    QComboBox *dataPicker;
    QPushButton *connect_btn;
    QPushButton *record_btn;
    QPushButton *open_data;
    QSlider *live_slider;

private:
    WIFI* wifi;
    ConstantUses* constants;

};


#endif