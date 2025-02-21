#ifndef OPTTAB_H
#define OPTTAB_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "mainPanel.h"

class OptTab : public QWidget {
    Q_OBJECT

public:
    explicit OptTab(QWidget *parent = nullptr, MainPanel *mainPanel = nullptr);
    ~OptTab() override;

private slots:
    void showMainPanel() const;
    void showTestPanel() const;
    void showSetPanel() const;

private:
    // Layout
    QVBoxLayout *mainLayout;

    // Buttons
    QPushButton *mainBtn;
    QPushButton *testBtn;
    QPushButton *setBtn;
    QPushButton *exitBtn;

    MainPanel *mainPanel;

    QPushButton* createButton(const QString &text, int fontSize = 14, const QString &color = "#1E1E1E");
};

#endif  // OPTTAB_H

