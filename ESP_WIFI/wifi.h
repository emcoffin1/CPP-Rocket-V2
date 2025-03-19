#ifndef WIFI_H
#define WIFI_H
#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QScopedPointer>

class WIFI;
class ConstantUses;

class DataProcessor : public QObject {
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr, WIFI *wifiInstance = nullptr); // constructor
    void processJSON(const QJsonObject &jsonData); // process recieved json
    void emitData(const QJsonObject &jsonObj);
    WIFI* wifi;

signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QJsonObject jsonData);
    void sensorUpdated(QJsonObject jsonData);
    void positionUpdated(QJsonObject jsonData);
    void warningUpdated(QJsonObject jsonData);
    void rssiUpdated(QJsonObject jsonData);
    void testUpdated(QJsonObject jsonData);
    void padArmed(bool state);
    void dataUpdated(QJsonObject jsonData);


private:
    bool padArmState = false;

};

class WIFI : public QObject {
    Q_OBJECT

public:
    // get single instance
    static WIFI* getInstance();

    // connect to esp
    void connectToESP32(const QString &host, quint16 port);

    // disconnect
    void disconnectFromESP32() const;
    bool isConnected() const;


    // Change data type
    void setDataRandom(bool enabled);
    // send random
    void sendRandomValues();


    // send + receive
    void sendMessage(const QString &message);
    QString receiveMessage() const;


    // check connection
    int rssinum = -90;






signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QJsonObject jsonData);
    void sensorUpdated(QJsonObject jsonData);
    void positionUpdated(QJsonObject jsonData);
    void warningUpdated(QJsonObject jsonData);
    void rssiUpdated(QJsonObject jsonData);
    void testUpdated(QJsonObject jsonData);
    void padArmed(bool enabled);
    void dataUpdated(QJsonObject jsonData);
    //void connectionTypeChanged(QJsonObject jsonData);

public slots:
    void onDataReceived();

private:
    explicit WIFI(QObject *parent = nullptr);
    static QScopedPointer<WIFI> instance;
    QTcpSocket *socket;
    QTimer *dataTimer;
    bool dataRandom = false;
    DataProcessor *dataProcessor;
};


class ConstantUses : public QObject {
    Q_OBJECT

public:
    static ConstantUses* instance();

    static QPushButton* buttonMaker(const QString &text, int fontSize = 14, const QString &color = "black");
    QString currentTime() const;
    void startCountdown();
    void errorCode(const QString &error);
    void logEvent(const QString &message);

    static QStringList getConfig(const QString &key);

signals:
    void timeUpdated(QString newTime);
    void countdownUpdated(QString countdownTime);
    void errorEmit(QString error);
    void logEmit(QString event);

private:
    explicit ConstantUses(QObject *parent = nullptr);
    static QScopedPointer<ConstantUses> _instance;


    QTimer *timer;
    QTimer *countdownTimer;
    QString lastTime;
    QString formattedTime;
    int countdownValue;
    bool countDirection;

private slots:
    void updateTime();
    void updateCountdown();
    friend class QScopedPointer<ConstantUses>;
};


#endif //WIFI_H
