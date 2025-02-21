#ifndef WIFI_H
#define WIFI_H
#include <QObject>
#include <QTcpSocket>
#include <QDebug>

class WIFI : public QObject {
    Q_OBJECT

public:
    explicit WIFI(QObject *parent = nullptr);
    ~WIFI();

    void connectToESP32(const QString &host, quint16 port);
    void sendMessage(const QString &message);
    QString receiveMessage();
    bool isConnected() const;

private:
    QTcpSocket *socket;
};

#endif //WIFI_H
