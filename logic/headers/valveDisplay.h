#ifndef VALVEDISPLAY_H
#define VALVEDISPLAY_H

#include <QCheckBox>
#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include "../../ESP_WIFI/wifi.h"

class ValveTree : public QWidget {
    Q_OBJECT
public:
    explicit ValveTree(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

    QGridLayout *g_layout;
    QMap<QString, QCheckBox*> valveCheckBoxMap;
    QList<QString> valveNames;
    WIFI *wifi;
public slots:
    void changeValveColor(QJsonObject jsonObj);



};


#endif //VALVEDISPLAY_H
