#include "../headers/testTab.h"

#include <stack>

#include "../tests/headers/clickTest.h"


TestTab::TestTab(QWidget *parent) : QWidget(parent) {
    // Create main grid layout
    g_layout = new QGridLayout();
    g_layout->setContentsMargins(0, 0, 0, 0);
    g_layout->setSpacing(0);


    // Create secondary layout for buttons
    button_grid = new QGridLayout();
    button_grid->setContentsMargins(0, 0, 0, 0);
    button_grid->setSpacing(0);

    // Create and style label
    QFont font("Verdana", 40, QFont::Bold);
    test = new QLabel("TESTS");
    test->setFont(font);
    test->setStyleSheet("color: white; padding-bottom: 15px;");
    test->setAlignment(Qt::AlignCenter);

    // Create buttons
    click_btn = createButton("Click", 20);
    leak_btn = createButton("Leak", 20);
    decay_btn = createButton("Decay", 20);
    igniter_btn = createButton("Igniter", 20);

    // Button connects
    connect(click_btn, &QPushButton::clicked, [this]() { switchTests(0); });
    connect(leak_btn, &QPushButton::clicked, [this]() { switchTests(1); });
    connect(decay_btn, &QPushButton::clicked, [this]() { switchTests(2); });
    connect(igniter_btn, &QPushButton::clicked, [this]() { switchTests(3); });


    // Add buttons to the button grid
    button_grid->addWidget(click_btn, 0, 0);
    button_grid->addWidget(leak_btn, 0, 1);
    button_grid->addWidget(decay_btn, 0, 2);
    button_grid->addWidget(igniter_btn, 0, 3);

    // Create stackedwidgets
    stacked_widget = new QStackedWidget();
    clickTest = new ClickTest();
    stacked_widget->setStyleSheet("border: 2px solid green;");
    stacked_widget->addWidget(clickTest);
    stacked_widget->setContentsMargins(0,0,0,0);


    // Add button layout to the main layout
    g_layout->addWidget(test, 0, 0, 1, 4);
    g_layout->addLayout(button_grid, 1, 0, 1, 4);
    g_layout->addWidget(stacked_widget,2,0, 6, 4);

    // Set main layout
    setLayout(g_layout);
}



QPushButton* TestTab::createButton(const QString &text, int fontSize, const QString &color) {
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
      .arg("#1E1E1E")  // Hover color
      .arg("#2A2A2A")); // Pressed color

    return btn;
}

void TestTab::switchTests(int index) const {
    if (index >= 0 && index < stacked_widget->count()) {
        stacked_widget->setCurrentIndex(index);
    }
}