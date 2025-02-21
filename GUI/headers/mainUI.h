#ifndef MAINUI_H
#define MAINUI_H

#include <QGraphicsDropShadowEffect>

#include "mainPanel.h"
#include "optTab.h"
#include <QGridLayout>
#include <QWidget>
#include "../../ESP_WIFI/wifi.h"

class MainUI : public QWidget {
    Q_OBJECT

public:
    explicit MainUI(WIFI *wifiInstance, QWidget *parent = nullptr);
    ~MainUI() override;

    MainPanel* getMainPanel();

    public slots:

    private:
    OptTab *optTab;
    MainPanel *mainPanel;
    QGridLayout *g_layout;
    QVBoxLayout *optionLayout;
    QGraphicsDropShadowEffect *shadowEffect;
    QWidget *optionPanel;

    WIFI *wifi;



};


#endif