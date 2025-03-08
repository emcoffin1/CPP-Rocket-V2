#include "../headers/setTab.h"

SetTab::SetTab(QWidget *parent, WIFI *wifiInstance) : QWidget (parent) {
    // Init items
    wifi = wifiInstance;


    // Set layout
    g_layout = new QGridLayout();
    setContentsMargins(0,0,0,0);

    // Add label
    title = new QLabel("OPTIONS", this);
    QFont font("Verdana", 40, QFont::Bold);
    title->setFont(font);
    title->setStyleSheet("color: white; padding-bottom: 50px;");
    title->setAlignment(Qt::AlignCenter);


    // Connection button
    connect_btn = ConstantUses::instance()->buttonMaker("WIFI\nConnect", 20, "red");
    connect_btn->setContentsMargins(50,50,50,50);
    connect_btn->setFixedSize(250,250);

    // Data picker
    dataPicker = new QComboBox(this);
    dataPicker->addItem("Real Values");
    dataPicker->addItem("ESP Random Values");
    dataPicker->addItem("Controller Random Values");
    dataPicker->setStyleSheet("QComboBox {color: white; "
                              "background-color: gray;"
                              "font-size: 18px; }"
                              "QComboBox QAbstractItemView { color: white;"
                              "background-color: gray;"
                              "font-size: 18px; }");
    connect_btn->setFixedSize(250,250);


    // Recorder button
    record_btn = ConstantUses::instance()->buttonMaker("Record", 20, "red");
    record_btn->setContentsMargins(50,50,50,50);
    record_btn->setFixedSize(250,250);

    // Open data file button
    open_data = ConstantUses::instance()->buttonMaker("Open", 18, "gray");
    open_data->setFixedSize(250,30);


    // Add to layout
    g_layout->addWidget(title, 0, 0, 1, 2);
    g_layout->addWidget(connect_btn, 1, 0, Qt::AlignCenter);
    g_layout->addWidget(record_btn, 1, 1, Qt::AlignCenter);
    g_layout->addWidget(dataPicker, 2, 0, Qt::AlignCenter);
    g_layout->addWidget(open_data, 2, 1, Qt::AlignCenter);
    //g_layout->setRowMinimumHeight(1, 200);
    //g_layout->setRowStretch(2,10);

    this->setLayout(g_layout);

    // Connect button
    connect(connect_btn, &QPushButton::clicked, this, &SetTab::connectWIFI);
    connect(dataPicker, &QComboBox::currentIndexChanged, this, &SetTab::changeDataType);


}


void SetTab::connectWIFI() const {
    if (!wifi) return;

    if (wifi->isConnected()) {
        wifi->disconnectFromESP32();
    } else {
        QString host = "192.168.4.1";
        quint16 port = 80;

        wifi->connectToESP32(host, port);
    }

    if (wifi->isConnected()) {
        connect_btn->setStyleSheet("background-color: green");
        connect_btn->setText("WIFI\nDisconnect");
    } else {
        connect_btn->setStyleSheet("background-color: red");
        connect_btn->setText("WIFI\nConnect");
    }
}

void SetTab::changeDataType() const {
    QJsonObject testMessage;
    // Changes type of data that system will receive
    if (!wifi) return;

    int index = dataPicker->currentIndex();

    if (index == 2) {
        // Tell cpp to use fake data
        wifi->setDataRandom(true) ;
        if (wifi->isConnected()) {
            wifi->disconnectFromESP32();
        }
    } else {
        wifi->setDataRandom(false) ;

        if (wifi->isConnected()) {
            testMessage["TEST"] = QJsonObject();
            QJsonObject testSubObject = testMessage["TEST"].toObject();

            if (index == 0) {
                // Request real data from esp
                testSubObject["RANDOM"] = 0;
            } else if (index == 1) {
                testSubObject["RANDOM"] = 1;
            }

            testMessage["TEST"] = testSubObject;
            QJsonDocument jsonDoc(testMessage);
            QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
            wifi->sendMessage(jsonString);
        }
    }
}