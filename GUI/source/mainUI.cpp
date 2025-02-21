#include "../headers/mainUI.h"

#include "../../ESP_WIFI/wifi.h"

MainUI::MainUI (WIFI *wifiInstance, QWidget *parent) : QWidget (parent) {

    // Window settings
    setWindowTitle("FARTS");
    resize(1980,800);
    setStyleSheet("background-color: black");

    // Main window layout
    g_layout = new QGridLayout();
    g_layout->setContentsMargins(0,0,0,0);

    // Init options and main tabs
    mainPanel = new MainPanel(this, wifiInstance);
    optTab = new OptTab(this, mainPanel);

    // Actual panel for the options
    QWidget *optionPanel = new QWidget(this);
    optionPanel->setStyleSheet("background-color: #1E1E1E;");

    // Apply a drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(optionPanel);
    shadowEffect->setBlurRadius(100); // Adjust blur intensity
    shadowEffect->setOffset(-10, 0);  // Shift shadow to appear on top, left, and bottom
    shadowEffect->setColor(QColor(18, 222, 224, 50)); // Semi-transparent black shadow

    optionPanel->setGraphicsEffect(shadowEffect);

    // Layout for option panel
    QVBoxLayout *optionLayout = new QVBoxLayout(optionPanel);
    optionLayout->addWidget(optTab);
    optionLayout->setContentsMargins(0,0,0,0);
    optionLayout->setSpacing(0);


    // Add items
    g_layout->addWidget(optionPanel, 0, 0);
    g_layout->addWidget(mainPanel, 0, 1);

    // Format columns
    g_layout->setColumnStretch(0,1);
    g_layout->setColumnStretch(1,15);


    // Set layout
    setLayout(g_layout);
}

MainUI::~MainUI() {
    delete g_layout;
}

MainPanel* MainUI::getMainPanel() {
    return mainPanel;
}
