#include "../headers/clickTest.h"

ClickTest::ClickTest(QWidget *parent, WIFI *wifiInstance) :QWidget(parent) {
    g_layout = new QGridLayout(this);
    setContentsMargins(0,0,0,0);
    this->setStyleSheet(R"(
            QCheckBox {
                color: white;
                font-size: 16px;
                spacing 10px;
                padding: 10px;}
            QCheckBox::indicator {
                width: 20px;
                height: 20px;
                border-radius: 5px;
                background: lightgray;
                }
            QCheckBox::indicator:checked {
                background: green;}
            QCheckBox::indicator:unchecked {
                background: red;}
            QCheckBox::indicator:hover {
                width: 25px;
                height: 25px;
                }

    )");

    // WIFI Init
    wifi = wifiInstance;

    // Start Button
    start_button = createButton("START", 25, this, "#bc2626");
    connect(start_button, &QPushButton::clicked, this, &ClickTest::startTest);

    // Timer
    QFont font("Verdana", 25);
    timer = new QLabel("0:00", this);
    timer->setStyleSheet("color: white;");
    timer->setFont(font);

    // Check layout
    check_container = new QHBoxLayout();
    check_grid1 = new QVBoxLayout();
    check_grid2 = new QVBoxLayout();
    // Check boxes
    std::vector<QCheckBox*> valveCheckBoxes = {
    valve1 = new QCheckBox("Valve 1", this),
    valve2 = new QCheckBox("Valve 2", this),
    valve3 = new QCheckBox("Valve 3", this),
    valve4 = new QCheckBox("Valve 4", this),
    valve5 = new QCheckBox("Valve 5", this),
    valve6 = new QCheckBox("Valve 6", this),
    valve7 = new QCheckBox("Valve 7", this),
    valve8 = new QCheckBox("Valve 8", this),
    valve9 = new QCheckBox("Valve 9", this),
    valve10 = new QCheckBox("Valve 10", this)
    };
    for (int i = 0; i < 5; ++i) {
        valveCheckBoxes[i]->setLayoutDirection(Qt::RightToLeft);
    }

    for (int i = 5; i< 10; ++i) {
        valveCheckBoxes[i]->setLayoutDirection(Qt::LeftToRight);
    }

    for (int i = 0; i < 5; ++i) {
        check_grid1->addWidget(valveCheckBoxes[i]);
        check_grid2->addWidget(valveCheckBoxes[i + 5]);
    }

    check_container->setContentsMargins(10,10,10,10);
    check_container->addLayout(check_grid1);
    check_container->addLayout(check_grid2);

    // Layout
    g_layout->setRowStretch(0,1);
    g_layout->addLayout(check_container,1, 1, 2, 2, Qt::AlignCenter);
    g_layout->setRowStretch(4,1);
    g_layout->setColumnStretch(0,1);
    g_layout->addWidget(start_button, 5,1, 1, 1);
    g_layout->addWidget(timer, 5, 2, 1, 1, Qt::AlignCenter);
    g_layout->setRowStretch(6,1);
    g_layout->setColumnStretch(4,1);






}

QPushButton* ClickTest::createButton(const QString &text, int fontSize, QWidget *parent, const QString &color) {
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
            border-radius: 10%;
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

void ClickTest::startTest() {
    wifi->sendMessage("TEST: CLICK");
}
