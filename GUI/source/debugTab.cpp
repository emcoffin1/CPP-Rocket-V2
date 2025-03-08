#include "../headers/debugTab.h"

DebugTab::DebugTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init stuff
    wifi = wifiInstance;

    main_layout = new QVBoxLayout(this);
    setStyleSheet("background-color: grey;");


    setLayout(main_layout);
}