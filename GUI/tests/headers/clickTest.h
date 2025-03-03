#ifndef CLICKTEST_H
#define CLICKTEST_H
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "../../../ESP_WIFI/wifi.h"
#include <QFont>
#include <QCheckBox>


class ClickTest : public QWidget {
    Q_OBJECT

public:
    explicit ClickTest(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

    QLabel *timer;
    QGridLayout *g_layout;
    QVBoxLayout *check_grid1, *check_grid2;
    QHBoxLayout *check_container;

    QPushButton *start_button;

    QCheckBox *valve1, *valve2, *valve3, *valve4, *valve5, *valve6, *valve7, *valve8, *valve9, *valve10;


    WIFI *wifi;

private slots:
    void startTest();



};

#endif //CLICKTEST_H
