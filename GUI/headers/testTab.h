#ifndef TESTTAB_H
#define TESTTAB_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include "../tests/headers/clickTest.h"
#include "../tests/headers/leakTest.h"
#include "../../logic/headers/valveDisplay.h"
#include "../../ESP_WIFI/wifi.h"


class TestTab : public QWidget {
    Q_OBJECT
private:
    WIFI *wifi;

    ClickTest *clickTest;
    LeakTest *leakTest;
    //DecayTest *decayTest;
    //IgniterTest *igniterTest;


public:

    explicit TestTab(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);
    void switchTests(int index) const;



    QPushButton *click_btn;
    QPushButton *leak_btn;
    QPushButton *decay_btn;
    QPushButton *igniter_btn;

    QStackedWidget *stacked_widget;


    LadderWidget *valveDisplay;


    QPushButton* createButton(const QString &text, int fontSize = 14, QWidget *parent = nullptr, const QString &color = "black");

    QLabel *test;
    QGridLayout *g_layout;
    QGridLayout *button_grid;

public slots:
    void updateValves(QJsonObject valveData);



private:
};


#endif
