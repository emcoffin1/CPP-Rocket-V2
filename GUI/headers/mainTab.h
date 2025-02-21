#ifndef MAINTAB_H
#define MAINTAB_H
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPushButton>

class MainTab : public QWidget {
    Q_OBJECT

public:

    explicit MainTab(QWidget *parent = nullptr);

    QLabel *test;
    QGridLayout *g_layout;

    QPushButton* createButton(const QString &text, int fontSize = 14, const QString &color = "black");
};

#endif //MAINTAB_H