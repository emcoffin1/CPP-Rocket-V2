#include "../headers/clickTest.h"

ClickTest::ClickTest(QWidget *parent) :QWidget(parent) {
    g_layout = new QGridLayout(this);
    setStyleSheet("background-color: white;");
    setContentsMargins(0,0,0,0);
    test = new QLabel();
    g_layout->addWidget(test);



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
