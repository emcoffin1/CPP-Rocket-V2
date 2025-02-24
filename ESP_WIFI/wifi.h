#ifndef WIFI_H
#define WIFI_H
#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class DataProcessor : public QObject {
    Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr); // constructor
    void processJSON(const QJsonObject &jsonData); // process recieved json
    void emitData(const QJsonObject &jsonObj);

signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QString displayText);
    void sensorUpdated(QString displayText);
    void positionUpdated(QString displayText);
    void warningUpdated(QString displayText);

private:





};

class WIFI : public QObject {
    Q_OBJECT

public:
    // get single instance
    static WIFI* getInstance();

    // delete copies
    WIFI(const WIFI&) = delete;
    WIFI& operator=(const WIFI&) = delete;

    // connect to esp
    void connectToESP32(const QString &host, quint16 port) const;

    // disconnect
    void disconnectFromESP32() const;

    // send + receive
    void sendMessage(const QString &message) const;
    [[nodiscard]] QString receiveMessage() const;

    // check connection
    bool isConnected() const;



signals:
    void jsonDataReceived(QJsonDocument json);
    void valveUpdated(QString displayText);
    void sensorUpdated(QString displayText);
    void positionUpdated(QString displayText);
    void warningUpdated(QString displayText);

private slots:
    void onDataReceived();

private:
    explicit WIFI(QObject *parent = nullptr);
    ~WIFI();

    static WIFI* instance;
    QTcpSocket *socket;
    DataProcessor *dataProcessor;
};



#endif //WIFI_H
