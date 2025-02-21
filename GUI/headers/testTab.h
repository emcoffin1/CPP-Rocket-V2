#ifndef TESTTAB_H
#define TESTTAB_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include "../tests/headers/clickTest.h"

class TestTab : public QWidget {
    Q_OBJECT

private:
    QLabel *test;
    QGridLayout *g_layout;
    QGridLayout *button_grid;
public:

    explicit TestTab(QWidget *parent = nullptr);
    void switchTests(int index) const;


    QPushButton *click_btn;
    QPushButton *leak_btn;
    QPushButton *decay_btn;
    QPushButton *igniter_btn;

    QStackedWidget *stacked_widget;

    ClickTest *clickTest;
    //LeakTest *leakTest;
    //DecayTest *decayTest;
    //IgniterTest *igniterTest;

    QPushButton* createButton(const QString &text, int fontSize = 14, const QString &color = "black");
};


#endif
