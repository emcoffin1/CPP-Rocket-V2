#include "../headers/setTab.h"

SetTab::SetTab(WIFI *wifiInstance, QWidget *parent) : QWidget (parent), wifiInstance(wifiInstance) {
    // Set layout
    g_layout = new QGridLayout();
    setContentsMargins(0,0,0,0);

    // Add label
    title = new QLabel("TESTS");
    QFont font("Verdana", 40, QFont::Bold);
    title->setFont(font);
    title->setStyleSheet("color: white; padding-bottom: 50px;");
    title->setAlignment(Qt::AlignCenter);


    connect_btn = createButton("WIFI\nConnect", 20, this,"red");
    connect_btn->setContentsMargins(50,50,50,50);
    connect_btn->setFixedSize(250,250);


    // Add to layout
    g_layout->addWidget(title, 0, 0, 1, 2);
    g_layout->addWidget(connect_btn, 1, 0, Qt::AlignCenter);
    //g_layout->setRowMinimumHeight(1, 200);
    g_layout->setRowStretch(2,10);

    this->setLayout(g_layout);

    // Connect button
    connect(connect_btn, &QPushButton::clicked, this, &SetTab::connectWIFI);

}

QPushButton* SetTab::createButton(const QString &text, int fontSize, QWidget *parent, const QString &color) {
    if (!parent) {
        parent = this;
    }

    auto *btn = new QPushButton(text, parent);  // Ensure it has `this` as parent

    // Set font
    QFont font;
    font.setPointSize(fontSize);
    font.setBold(false);
    btn->setFont(font);

    // Apply button styling
    btn->setStyleSheet(QString(R"(
        QPushButton {
            background-color: %1;
            color: white;
            border-radius: 0px;
            padding: 0px;
            margin: 0px;
            border: none;
            font-size: %2px;
            font-weight: normal;
        }

        QPushButton:hover {
            background-color: %3;
        }

        QPushButton:pressed {
            background-color: %4;
        }

        QPushButton:focus {
            outline: none;
        }
    )").arg(color)
      .arg(fontSize)
      .arg("#1E1E1E", "#2A2A2A")); // Pressed color

    return btn;
}


void SetTab::connectWIFI() {
    if (!wifiInstance) return;

    if (wifiInstance->isConnected()) {
        wifiInstance->disconnectFromESP32();
    } else {
        QString host = "192.168.4.1";
        quint16 port = 80;

        wifiInstance->connectToESP32(host, port);
    }

    if (wifiInstance->isConnected()) {
        connect_btn->setStyleSheet("background-color: green");
        connect_btn->setText("WIFI\nDisconnect");
    } else {
        connect_btn->setStyleSheet("background-color: red");
        connect_btn->setText("WIFI\nConnect");
    }
}