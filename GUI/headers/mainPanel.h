#ifndef MAINPANEL_H
#define MAINPANEL_H
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../headers/mainTab.h"
#include "../headers/testTab.h"
#include "../headers/setTab.h"
#include "../headers/debugTab.h"
#include "../headers/dataDisplay.h"
#include "../../ESP_WIFI/wifi.h"
#include "../../logic/headers/graphController.h"


class MainPanel : public QWidget {
    Q_OBJECT

public:
    // Constructor
    explicit MainPanel(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);
    ~MainPanel() override;
    void switchPanel(int index) const;
    // Generate tabs
    MainTab *mainTab;
    TestTab *testTab;
    SetTab *setTab;
    DebugTab *debugTab;
    DataDisplay *dataTab;
    WIFI *wifi;
    GraphController *graphController;
    ConstantUses *constantUses;

    //QVBoxLayout *v_layout;
    QStackedWidget *stackedWidget;
    QHBoxLayout *con_Layout;
    QHBoxLayout *battery_layout;
    QLabel *status;
    QPushButton *statusInd;
    QCheckBox *statusCheck;
    QLabel *timeLabel;
    QLabel *battery_percentage;
    QLabel *battery_label;

private slots:
    void changeConnectionStatus(QJsonObject value);
    void updateTime(const QString timeVal) const;

};

#endif
