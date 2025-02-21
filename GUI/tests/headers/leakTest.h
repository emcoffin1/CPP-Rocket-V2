#ifndef LEAKTEST_H
#define LEAKTEST_H
#include <QGridLayout>
#include <QWidget>

class LeakTest : public QWidget {
    Q_OBJECT

public:
    explicit LeakTest(QWidget *parent = nullptr);
    ~LeakTest() override;

    // Things
    QGridLayout *g_layout;




};

#endif //LEAKTEST_H
