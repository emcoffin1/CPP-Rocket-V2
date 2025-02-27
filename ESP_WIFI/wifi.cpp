#include "wifi.h"

#include <iostream>
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

    // Connect Wi-Fi
    connect(socket, &QTcpSocket::readyRead, this, &WIFI::onDataReceived);

    // Init timer
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &WIFI::sendRandomValues);

    // Forward update signals
    connect(dataProcessor, &DataProcessor::sensorUpdated, this, &WIFI::sensorUpdated);
    connect(dataProcessor, &DataProcessor::positionUpdated, this, &WIFI::positionUpdated);
    connect(dataProcessor, &DataProcessor::warningUpdated, this, &WIFI::warningUpdated);
    connect(dataProcessor, &DataProcessor::valveUpdated, this, &WIFI::valveUpdated);
    connect(dataProcessor, &DataProcessor::rssiUpdated, this, &WIFI::rssiUpdated);

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
        sendMessage("CPP: Connected");
    } else {
        QString error = QString("Can't connect to wifi: %1").arg(socket->errorString());
        QMessageBox::warning(nullptr, "Connection Error", error);

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
        qDebug() << "Sending message: " << message;
        socket->waitForBytesWritten(1000);
        qDebug() << "Message sent: " << message;
    } else {
        QMessageBox::warning(nullptr, "Error", "Cannot send message");
    }
}



QString WIFI::receiveMessage() const {
    if (socket->waitForReadyRead(3000)) {  // Wait up to 3 seconds
        return socket->readAll();
    }
    return "";
}

void WIFI::onDataReceived() {
    if (dataRandom == false) {
        // Stop timer if active
        setDataRandom(false);


        QByteArray data = socket->readAll();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            dataProcessor->processJSON(jsonObj);
        }
    }

    if (dataRandom == true) {
        setDataRandom(true);
    }
}

void WIFI::sendRandomValues() {
    QJsonObject jsonObj, jsonValve, jsonPos, jsonSens;
    int randomNum = QRandomGenerator::global() -> bounded(40);

    jsonValve["HighPress1"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["HighPress2"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["Chamber1"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["Chamber2"] = QRandomGenerator::global() -> bounded(2);

    jsonPos["ROLL"] = QRandomGenerator::global() -> bounded(31) - 15;
    jsonPos["PITCH"] = QRandomGenerator::global() -> bounded(31) - 15;

    jsonSens["FuelTank"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["LOXTank"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["Pneumatics"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["HighPress"] = QRandomGenerator::global() -> bounded(31);


    jsonObj["VALVES"] = jsonValve;
    jsonObj["SENSORS"] = jsonSens;
    jsonObj["POSITION"] = jsonPos;
    jsonObj["CONNECTION"] = rssinum;
    dataProcessor->processJSON(jsonObj);

}

bool WIFI::isConnected() const {
    return socket->state() == QTcpSocket::ConnectedState;
}

void WIFI::setDataRandom(bool enabled) {
    if (dataRandom == enabled) {
        return;
    }

    dataRandom = enabled;

    if (dataRandom) {
        if (!dataTimer->isActive()) {
            dataTimer->start(1000);
        }
    } else {
        dataTimer->stop();
    }
}

DataProcessor::DataProcessor(QObject *parent) : QObject(parent) {}

void DataProcessor::processJSON(const QJsonObject &jsonData) {
    QJsonDocument doc(jsonData); // converts to readable string
    emitData(jsonData);
    //emit dataUpdated(displayText); // emit signal with processed text
}

void DataProcessor::emitData(const QJsonObject &jsonObj) {
    QJsonObject valveData, sensorData, positionData, warningData, rssiData;
    if (jsonObj.contains("CONNECTION")) {
        rssiData["CONNECTION"] = jsonObj.value("CONNECTION");
        emit rssiUpdated(rssiData);
    }

    if (jsonObj.contains("VALVES")) {
        valveData["VALVES"] = jsonObj.value("VALVES");
        //qDebug() << "VALVES: " << valveData;
        emit valveUpdated(valveData);
    }

    if (jsonObj.contains("SENSORS")) {
        sensorData["SENSORS"] = jsonObj.value("SENSORS");
        emit sensorUpdated(sensorData);
    }

    if (jsonObj.contains("POSITION")) {
        positionData["POSITION"] = jsonObj.value("POSITION");
        //qDebug() << "POSITION: " << positionData;
        emit positionUpdated(positionData);
    }

    if (jsonObj.contains("WARNINGS")) {
        warningData["WARNING"] = jsonObj.value("WARNING").toString();
        emit warningUpdated(warningData);
    }
}
