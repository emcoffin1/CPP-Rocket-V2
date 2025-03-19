#include "../headers/dataDisplay.h"

DataDisplay::DataDisplay(QWidget *parent, WIFI *wifiInstance) {
    // Init
    wifi = wifiInstance;

    // Main layout
    v_layout = new QVBoxLayout(this);
    v_layout->setContentsMargins(0,0,0,0);
    v_layout->setSpacing(0);
    setStyleSheet("background-color: black; color: white;"
                  "QTabWidget {color:green;}");



    // Every tab
    everyWidget = new QWidget();
    everyLayout = new QVBoxLayout();
    everyBox = new QTextEdit(everyWidget);
    everyBox->setReadOnly(true);
    everyBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    everyBox->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    everyBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    everyBox->setWordWrapMode(QTextOption::NoWrap);
    everyBox->setStyleSheet("border: 1px solid gray");
    everyLayout->addWidget(everyBox);
    everyWidget->setLayout(everyLayout);

    // Sensor Tab
    sensorWidget = new QWidget();
    sensorLayout = new QVBoxLayout();
    sensorBox = new QTextEdit(sensorWidget);
    sensorBox->setReadOnly(true);
    sensorBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sensorBox->setStyleSheet("border: 1px solid gray");
    sensorLayout->addWidget(sensorBox);
    sensorWidget->setLayout(sensorLayout);

    // Valve Tab
    valveWidget = new QWidget();
    valveLayout = new QVBoxLayout();
    valveBox = new QTextEdit(valveWidget);
    valveBox->setReadOnly(true);
    valveBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    valveBox->setStyleSheet("border: 1px solid gray");
    valveLayout->addWidget(valveBox);
    valveWidget->setLayout(valveLayout);

    // Position Tab
    positionWidget = new QWidget();
    positionLayout = new QVBoxLayout();
    positionBox = new QTextEdit(positionWidget);
    positionBox->setReadOnly(true);
    positionBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    positionBox->setStyleSheet("border: 1px solid gray");
    positionLayout->addWidget(positionBox);
    positionWidget->setLayout(positionLayout);




    // Tabs
    tab_widget = new QTabWidget();
    tab_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tab_widget->addTab(everyWidget, "All");
    tab_widget->addTab(sensorWidget, "Sensor");
    tab_widget->addTab(valveWidget, "Valve");
    tab_widget->addTab(positionWidget, "Position");

    v_layout->addWidget(tab_widget);




    setLayout(v_layout);

    // Connects
    connect(wifi, &WIFI::positionUpdated, this, &DataDisplay::updatePosition);
    connect(wifi, &WIFI::sensorUpdated, this, &DataDisplay::updateSensor);
    connect(wifi, &WIFI::valveUpdated, this, &DataDisplay::updateValve);
    connect(wifi, &WIFI::dataUpdated, this, &DataDisplay::updateEvery);
}


void DataDisplay::updateEvery(QJsonObject message) {
    QString messageString = QJsonDocument(message).toJson(QJsonDocument::Compact);
    everyBox->append(messageString);
}

void DataDisplay::updateSensor(QJsonObject message) {
    QString messageString = QJsonDocument(message).toJson(QJsonDocument::Compact);
    sensorBox->append(messageString);
}

void DataDisplay::updateValve(QJsonObject message) {
    QString messageString = QJsonDocument(message).toJson(QJsonDocument::Compact);
    valveBox->append(messageString);}

void DataDisplay::updatePosition(QJsonObject message) {
    QString messageString = QJsonDocument(message).toJson(QJsonDocument::Compact);
    positionBox->append(messageString);}