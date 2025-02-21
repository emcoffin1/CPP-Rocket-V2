#include "../headers/mainTab.h"

#include <QPushButton>


MainTab::MainTab(QWidget *parent) : QWidget(parent) {
    // Create layout
    g_layout = new QGridLayout();

    test = new QLabel("Test");
    test->setStyleSheet("color:white;");
    test->setAlignment(Qt::AlignCenter);

    //


    // Add widgets to grid
    g_layout->addWidget(test, 0, 1, 1, 5);



    setLayout(g_layout);
}


QPushButton* MainTab::createButton(const QString &text, int fontSize, const QString &color) {
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
      .arg("#1E1E1E")  // Hover color
      .arg("#2A2A2A")); // Pressed color

    return btn;
}
