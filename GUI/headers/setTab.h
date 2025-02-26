#ifndef SETTAB_H
#define SETTAB_H
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include "../../ESP_WIFI/wifi.h"
#include <QComboBox>

class SetTab : public QWidget {
    Q_OBJECT


public slots:
    void connectWIFI() const;
    void changeDataType() const;

public:
    explicit SetTab(WIFI *wifiInstance, QWidget *parent = nullptr);

    WIFI *wifiInstance;
    QGridLayout *g_layout;
    QLabel *title;
    QComboBox *dataPicker;
    QPushButton *connect_btn;
    QPushButton* createButton(const QString &text, int fontSize = 14, QWidget *parent = nullptr, const QString &color = "black");
};


#endif