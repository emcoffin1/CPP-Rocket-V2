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

    // Connect wifi
    connect(socket, &QTcpSocket::readyRead, this, &WIFI::onDataReceived);

    // Connect bt

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
    socket->abort();
    socket->connectToHost(host, port);
    if (socket->waitForConnected(3000)) {  // Wait up to 2 sec
        qDebug() << "Connected to ESP";
        sendMessage("CPP: Connected");
    } else {
        qDebug() << "Failed to connect to ESP" << socket->errorString();
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
    QJsonObject valveData, sensorData, positionData, warningData, rssiData;
    if (jsonObj.contains("RSSI")) {
        rssiData["RSSI"] = jsonObj.value("RSSI");
        emit rssiUpdated(rssiData);
    }

    if (jsonObj.contains("VALVES")) {
        valveData["VALVES"] = jsonObj.value("VALVES");
        emit valveUpdated(valveData);
    }

    if (jsonObj.contains("SENSORS")) {
        sensorData["SENSORS"] = jsonObj.value("SENSORS");
        emit sensorUpdated(sensorData);
    }

    if (jsonObj.contains("POSITIONS")) {
        positionData["POSITION"] = jsonObj.value("POSITION").toString();
        emit positionUpdated(positionData);
    }

    if (jsonObj.contains("WARNINGS")) {
        warningData["WARNING"] = jsonObj.value("WARNING").toString();
        emit warningUpdated(warningData);
    }
}
