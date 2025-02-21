#include "../headers/leakTest.h"

#include <QLabel>


LeakTest::LeakTest(QWidget *parent) : QWidget(parent) {
    g_layout = new QGridLayout(this);
    setStyleSheet("background-color: white;");

    QLabel *label = new QLabel("filler", this);
    qDebug() << "Label parent:" << label->parent();


    label->setAlignment(Qt::AlignCenter);
    g_layout->addWidget(label, 0, 0, 1, 4);

}

LeakTest::~LeakTest() {
    // Destroys window
    qDebug() << "LeakTest destoryed";
    std::default_delete<LeakTest>(leakTest);
}