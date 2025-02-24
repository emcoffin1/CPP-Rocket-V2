#include "wifi.h"

#include <QJsonDocument>




WIFI* WIFI::instance = nullptr; // Init instance as null

// Singleton instance getter
WIFI* WIFI::getInstance() {
    if (instance == nullptr) {
        instance = new WIFI();
    }
    return instance;
}

// Private constructor
WIFI::WIFI(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    dataProcessor = new DataProcessor(this);

    connect(socket, &QTcpSocket::readyRead, this, &WIFI::onDataReceived);

    // Forward update signals
    connect(dataProcessor, &DataProcessor::sensorUpdated, this, &WIFI::sensorUpdated);
    connect(dataProcessor, &DataProcessor::positionUpdated, this, &WIFI::positionUpdated);
    connect(dataProcessor, &DataProcessor::warningUpdated, this, &WIFI::warningUpdated);
    connect(dataProcessor, &DataProcessor::valveUpdated, this, &WIFI::valveUpdated);

}

// Destructor
WIFI::~WIFI() {
    socket->close();
    delete socket;
    delete dataProcessor;
}


void WIFI::connectToESP32(const QString &host, quint16 port) const {
    qDebug() << "Checking if ESP32 is reachable at " << host << ":" << port;

    // Create a temporary socket for checking the connection
    QTcpSocket testSocket;
    testSocket.connectToHost(host, port);

    if (!testSocket.waitForConnected(2000)) {  // Wait up to 2 sec
        qDebug() << "ESP32 is NOT reachable! Error:" << testSocket.errorString();
        return;  // Stop connection attempt
    }

    qDebug() << "ESP32 is reachable, proceeding with connection...";

    // ✅ If ESP32 is reachable, proceed with actual connection
    socket->connectToHost(host, port);
    if (socket->waitForConnected(3000)) {
        qDebug() << "Connected to ESP32 at" << host << ":" << port;
    } else {
        qDebug() << "Failed to connect to ESP32:" << socket->errorString();
    }
}

void WIFI::disconnectFromESP32() const {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
        if (socket->state() != QTcpSocket::UnconnectedState) {
            socket->waitForDisconnected(3000);
        }
    }
}

void WIFI::sendMessage(const QString &message) const {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(1000);
    } else {
        qDebug() << "Not connected, cannot send message.";
    }
}



QString WIFI::receiveMessage() const {
    if (socket->waitForReadyRead(3000)) {  // Wait up to 3 seconds
        return socket->readAll();
    }
    return "";
}

void WIFI::onDataReceived() {
    QByteArray data = socket->readAll();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        dataProcessor->processJSON(jsonObj);
    } else {
        qDebug() << "JSON object is null";
    }
}

bool WIFI::isConnected() const {
    return socket->state() == QTcpSocket::ConnectedState;
}


DataProcessor::DataProcessor(QObject *parent) : QObject(parent) {}

void DataProcessor::processJSON(const QJsonObject &jsonData) {
    QJsonDocument doc(jsonData); // converts to readable string
    emitData(jsonData);
    //emit dataUpdated(displayText); // emit signal with processed text
}

void DataProcessor::emitData(const QJsonObject &jsonObj) {
    if (jsonObj.contains("Valves")) {
        QString valve_status = jsonObj.value("Valves").toString();
        emit valveUpdated(valve_status);
    }

    if (jsonObj.contains("Sensors")) {
        QString sensor_status = jsonObj.value("Sensors").toString();
        emit sensorUpdated(sensor_status);
    }

    if (jsonObj.contains("Positions")) {
        QString position_status = jsonObj.value("Positions").toString();
        emit positionUpdated(position_status);
    }

    if (jsonObj.contains("Warnings")) {
        QString warnings = jsonObj.value("Warnings").toString();
        emit warningUpdated(warnings);
    }
}
