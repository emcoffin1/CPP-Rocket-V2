#ifndef CLICKTEST_H
#define CLICKTEST_H
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

class ClickTest : public QWidget {
    Q_OBJECT

private:
    QLabel *test;
    QGridLayout *button_grid;

public:
    //void StartTest();
    explicit ClickTest(QWidget *parent = nullptr);
    QGridLayout *g_layout;

    QPushButton* createButton(const QString &text, int fontSize = 14, const QString &color = "black");
};

#endif //CLICKTEST_H
