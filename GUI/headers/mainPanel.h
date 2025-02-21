#ifndef MAINPANEL_H
#define MAINPANEL_H
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>
#include "../headers/mainTab.h"
#include "../headers/testTab.h"

class MainPanel : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *v_layout;
    QStackedWidget *stackedWidget;
    QLabel *test;

    public slots:
        //void applyPalette(const QPalette &palette);

    public:
    // Constructor
    explicit MainPanel(QWidget *parent = nullptr);
    void switchPanel(int index) const;
    // Generate tabs
    MainTab *mainTab;
    TestTab *testTab;
    QWidget *setTab;


};

#endif
