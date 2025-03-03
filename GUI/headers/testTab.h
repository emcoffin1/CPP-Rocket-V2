#ifndef TESTTAB_H
#define TESTTAB_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include "../tests/headers/clickTest.h"
#include "../tests/headers/leakTest.h"
#include "../tests/headers/responseTest.h"
#include "../tests/headers/surfaceTest.h"
#include "../tests/headers/igniterTest.h"
#include "../../logic/headers/vehicleDisplay.h"
#include "../../ESP_WIFI/wifi.h"


class TestTab : public QWidget {
    Q_OBJECT
private:
    WIFI *wifi;

    ClickTest *clickTest;
    LeakTest *leakTest;
    ResponseTest *responseTest;
    IgniterTest *igniterTest;
    SurfaceTest *surfaceTest;

public:

    explicit TestTab(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);
    void switchTests(int index) const;



    QPushButton *click_btn;
    QPushButton *leak_btn;
    QPushButton *response_btn;
    QPushButton *igniter_btn;
    QPushButton *surface_btn;

    QStackedWidget *stacked_widget;
    QVBoxLayout *v_layout;


    ValveTree *valveDisplay;
    RollDisplay *rollDisplay, *pitchDisplay;




    QLabel *test;
    QGridLayout *g_layout;
    QGridLayout *button_grid;

public slots:
    void updateValves(QJsonObject valveData);



private:
};


#endif
