#ifndef DEBUGTAB_H
#define DEBUGTAB_H
#include <QVBoxLayout>
#include <QWidget>
#include "../../ESP_WIFI/wifi.h"

class DebugTab : public QWidget {
    Q_OBJECT
public:
    explicit DebugTab(QWidget *parent, WIFI *wifiInstance);

    WIFI *wifi;

private:
    QVBoxLayout *main_layout;
};

#endif //DEBUGTAB_H
