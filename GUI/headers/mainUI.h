#ifndef MAINUI_H
#define MAINUI_H

#include <QGraphicsDropShadowEffect>

#include "mainPanel.h"
#include "optTab.h"
#include <QGridLayout>
#include <QWidget>

class MainUI : public QWidget {
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = nullptr);
    ~MainUI();

    MainPanel* getMainPanel();

    public slots:

    private:
    OptTab *optTab;
    MainPanel *mainPanel;
    QGridLayout *g_layout;
    QVBoxLayout *optionLayout;
    QGraphicsDropShadowEffect *shadowEffect;
    QWidget *optionPanel;



};


#endif