#include "wifi.h"

WIFI::WIFI(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
}

WIFI::~WIFI() {
    socket->close();
    delete socket;
}

void WIFI::connectToESP32(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
    if (socket->waitForConnected(3000)) {  // Wait up to 3 seconds
        qDebug() << "Connected to ESP32 at" << host << ":" << port;
    } else {
        qDebug() << "Failed to connect to ESP32:" << socket->errorString();
    }
}

void WIFI::sendMessage(const QString &message) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(1000);
    } else {
        qDebug() << "Not connected, cannot send message.";
    }
}

QString WIFI::receiveMessage() {
    if (socket->waitForReadyRead(3000)) {  // Wait up to 3 seconds
        return socket->readAll();
    }
    return "";
}

bool WIFI::isConnected() const {
    return socket->state() == QTcpSocket::ConnectedState;
}


