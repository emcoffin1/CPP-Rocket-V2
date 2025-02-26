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

class DataProcessor : public QObject {
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr); // constructor
    void processJSON(const QJsonObject &jsonData); // process recieved json
    void emitData(const QJsonObject &jsonObj);

signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QJsonObject jsonData);
    void sensorUpdated(QJsonObject jsonData);
    void positionUpdated(QJsonObject jsonData);
    void warningUpdated(QJsonObject jsonData);
    void rssiUpdated(QJsonObject jsonData);

private:

};

class WIFI : public QObject {
    Q_OBJECT

public:
    // get single instance
    static WIFI* getInstance();
    QTimer *dataTimer;
    bool dataRandom = false;

    // delete copies
    WIFI(const WIFI&) = delete;
    WIFI& operator=(const WIFI&) = delete;

    // connect to esp
    void connectToESP32(const QString &host, quint16 port) const;

    // send random
    void sendRandomValues();

    // disconnect
    void disconnectFromESP32() const;

    // send + receive
    void sendMessage(const QString &message) const;
    QString receiveMessage() const;

    // check connection
    bool isConnected() const;
    int rssinum = -100;


signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QJsonObject jsonData);
    void sensorUpdated(QJsonObject jsonData);
    void positionUpdated(QJsonObject jsonData);
    void warningUpdated(QJsonObject jsonData);
    void rssiUpdated(QJsonObject jsonData);
    //void connectionTypeChanged(QJsonObject jsonData);

public slots:
    void onDataReceived();

private:
    explicit WIFI(QObject *parent = nullptr);
    ~WIFI();

    static WIFI* instance;
    QTcpSocket *socket;

    DataProcessor *dataProcessor;
};



#endif //WIFI_H
