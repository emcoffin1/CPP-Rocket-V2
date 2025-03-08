#include "../headers/optTab.h"
#include <QDebug>
#include "../headers/mainUI.h"
#include "../headers/mainPanel.h"

OptTab::OptTab(QWidget *parent, MainPanel *mainPanelREF) : QWidget(parent), mainPanel(mainPanelREF) {
    // Ensure the layout is created with this as its parent
    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create buttons, ensuring they have `this` as their parent
    mainBtn = createButton("Main", 25);
    testBtn = createButton("Test", 25);
    setBtn = createButton("Settings", 25);
    debugBtn = createButton("Debug", 25);
    exitBtn = createButton("Exit", 25);

    // Connect buttons to panel switcher
    connect(mainBtn, &QPushButton::clicked, this, &OptTab::showMainPanel);
    connect(testBtn, &QPushButton::clicked, this, &OptTab::showTestPanel);
    connect(setBtn, &QPushButton::clicked, this, &OptTab::showSetPanel);
    connect(debugBtn, &QPushButton::clicked, this, &OptTab::showDebugPanel);

    // Add widgets to layout
    mainLayout->addWidget(mainBtn);
    mainLayout->addWidget(testBtn);
    mainLayout->addWidget(setBtn);
    mainLayout->addWidget(debugBtn);
    mainLayout->addStretch(1);
    mainLayout->addWidget(exitBtn);
    mainLayout->setContentsMargins(0,0,0,0);

    // Set the layout for the widget
    setLayout(mainLayout);
}

QPushButton* OptTab::createButton(const QString &text, int fontSize, const QString &color) {
    QPushButton *btn = new QPushButton(text, this);  // Ensure it has `this` as parent

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
            padding: 5px;
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
      .arg("#333333")  // Hover color
      .arg("#2A2A2A")); // Pressed color

    return btn;
}

OptTab::~OptTab() {
    delete mainLayout;
}

void OptTab::showMainPanel() const {
    if (mainPanel) mainPanel->switchPanel(0);
}

void OptTab::showTestPanel() const {
    if (mainPanel) mainPanel->switchPanel(1);
}

void OptTab::showSetPanel() const {
    if (mainPanel) mainPanel->switchPanel(2);
}

void OptTab::showDebugPanel() const {
    if (mainPanel) mainPanel->switchPanel(3);
}