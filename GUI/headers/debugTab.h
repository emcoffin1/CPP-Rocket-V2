#ifndef DEBUGTAB_H
#define DEBUGTAB_H
#include <qsplitter.h>
#include <qtextedit.h>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include "../../ESP_WIFI/wifi.h"

class DebugTab : public QWidget {
    Q_OBJECT
public:
    explicit DebugTab(QWidget *parent, WIFI *wifiInstance);

    WIFI *wifi;




public slots:
    void updateEvent(QString message);
    void updateError(QString message);
    void saveEvent();
    void saveError();


private:
    void deleteEvent();
    void deleteError();




    QVBoxLayout *main_layout;

    QSplitter *splitter;

    // Events
    QVBoxLayout *event_layout;
    QWidget *event_widget;
    QTextEdit *eventLog;
    QLabel *eventLabel;
    QHBoxLayout *butboxEvent;
    QPushButton *saveEventButton;
    QPushButton *eraseEventButton;

    // Errors
    QVBoxLayout *error_layout;
    QWidget *error_widget;
    QTextEdit *errorLog;
    QLabel *errorLabel;
    QHBoxLayout *butboxError;
    QPushButton *saveErrorButton;
    QPushButton *eraseErrorButton;
};

#endif //DEBUGTAB_H
