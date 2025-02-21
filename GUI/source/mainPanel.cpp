#include "../headers/mainPanel.h"

MainPanel::MainPanel(QWidget *parent) : QWidget(parent) {
    // Create a single QVBoxLayout for MainPanel.
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create the time stuff.
    test = new QLabel("yahoo", this);
    test->setStyleSheet("color:white;");
    test->setAlignment(Qt::AlignCenter);
    test->setContentsMargins(0, 0, 0, 0);
    test->setMargin(0);

    // Create the QStackedWidget.
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setContentsMargins(0, 0, 0, 0);
    //stackedWidget->setStyleSheet("QStackedWidget { border: 2px solid green; }");

    // Sample widgets.
    mainTab = new MainTab();
    testTab = new TestTab();
    setTab = new QWidget();

    stackedWidget->addWidget(mainTab);
    stackedWidget->addWidget(testTab);
    stackedWidget->addWidget(setTab);

    // Add the label and stacked widget to the main layout.
    mainLayout->addWidget(test);
    mainLayout->addWidget(stackedWidget);

    // This layout will now control the entire MainPanel with no extra margins.
    setLayout(mainLayout);
}


void MainPanel::switchPanel(int index) const {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    }
}
