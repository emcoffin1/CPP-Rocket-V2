#include "wifi.h"

#include <iostream>
#include <qcoreapplication.h>
#include <QDir>
#include <qjsonarray.h>
#include <QJsonDocument>




QScopedPointer<WIFI> WIFI::instance; // Init instance as null

// Singleton instance getter
WIFI* WIFI::getInstance() {
    if (instance.isNull()) {
        instance.reset(new WIFI());
    }
    return instance.data();
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
    connect(dataProcessor, &DataProcessor::testUpdated, this, &WIFI::testUpdated);
    connect(dataProcessor, &DataProcessor::padArmed, this, &WIFI::padArmed);
    connect(dataProcessor, &DataProcessor::dataUpdated, this, &WIFI::dataUpdated);

}




void WIFI::connectToESP32(const QString &host, quint16 port) {
    qDebug() << "Checking if ESP32 is reachable at " << host << ":" << port;
    // Create a temporary socket for checking the connection
    socket->abort();
    socket->connectToHost(host, port);
    if (socket->waitForConnected(2000)) {  // Wait up to 2 sec
        sendMessage("CPP: Connected");
    } else {
        QString error = QString("Can't connect to wifi: %1").arg(socket->errorString());
        ConstantUses::instance()->errorCode(error);
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




void WIFI::sendMessage(const QString &message) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        QElapsedTimer timer;
        timer.start();  // Start timing the send operation

        QByteArray dataToSend;
        if (message.trimmed() == "ResponseTest") {
            qint64 sendTime = QDateTime::currentMSecsSinceEpoch();
            QJsonObject jsonRequest;
            jsonRequest["TEST"] = QJsonObject({{"ResponseTest", sendTime}});
            QJsonDocument jsonDoc(jsonRequest);
            dataToSend = jsonDoc.toJson(QJsonDocument::Compact);
        } else {
            dataToSend = message.toUtf8();
        }

        qint64 bytesWritten = socket->write(dataToSend);
        socket->flush();

        if (bytesWritten == -1 || !socket->waitForBytesWritten(10)) {  // Reduce wait time

        }

    } else {
        ConstantUses::instance()->errorCode("Unable to send: " + message);
        QMessageBox::warning(nullptr, "Error", "Cannot send message - Socket Disconnected");
    }
}

QString WIFI::receiveMessage() const {
    if (socket->waitForReadyRead(20)) {  // Wait up to 20 milliseconds
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
    QJsonObject jsonObj, jsonValve, jsonPos, jsonSens, jsonCon;

    QString timeStamp = QDateTime::currentDateTimeUtc().toString("mm:ss");

    jsonValve["time"] = timeStamp;
    jsonValve["HighPress"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["HighVent"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["LOXDomeReg"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["LOXDomeVent"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["FuelDomeReg"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["FuelDomeVent"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["LOXVent"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["FuelVent"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["LOXMV"] = QRandomGenerator::global() -> bounded(2);
    jsonValve["FuelMV"] = QRandomGenerator::global() -> bounded(2);

    jsonPos["time"] = timeStamp;
    jsonPos["Roll"] = QRandomGenerator::global() -> bounded(31) - 15;
    jsonPos["Pitch"] = QRandomGenerator::global() -> bounded(31) - 15;

    jsonSens["time"] = timeStamp;
    jsonSens["HighPress1"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["HighPress2"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["LOXTank1"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["LOXTank2"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["FuelTank1"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["FuelTank2"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["LOXDomeReg"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["FuelDomeReg"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["LOXInlet"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["FuelInlet"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["Chamber1"] = QRandomGenerator::global() -> bounded(31);
    jsonSens["Chamber2"] = QRandomGenerator::global() -> bounded(31);

    jsonCon["wifi"] = rssinum;
    jsonCon["Battery"] = QRandomGenerator::global() -> bounded(101);

    jsonObj["VALVES"] = jsonValve;
    jsonObj["SENSORS"] = jsonSens;
    jsonObj["POSITION"] = jsonPos;
    jsonObj["CONNECTION"] = jsonCon;
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


DataProcessor::DataProcessor(QObject *parent, WIFI *wifiInstance) : QObject(parent) {
    wifi = wifiInstance;
}

void DataProcessor::processJSON(const QJsonObject &jsonData) {
    //QJsonDocument doc(jsonData); // converts to readable string
    emitData(jsonData);
    //emit dataUpdated(displayText); // emit signal with processed text
}

void DataProcessor::emitData(const QJsonObject &jsonObj) {
    QJsonObject valveData, sensorData, positionData, warningData, rssiData, testData, padData;
    emit dataUpdated(jsonObj);
    if (jsonObj.contains("CONNECTION")) {
        rssiData["CONNECTION"] = jsonObj.value("CONNECTION");
        //qDebug()<< "RSSI" << rssiData;
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

        QString jsonString = QJsonDocument(warningData).toJson(QJsonDocument::Compact);
        ConstantUses::instance()->errorCode(jsonString);
        emit warningUpdated(warningData);

    }

    if (jsonObj.contains("TEST")) {
        QJsonObject testObj = jsonObj["TEST"].toObject();

        // Filter by test type
        if (testObj.contains("ResponseTest")) {
            QJsonObject responseTestObj = testObj["ResponseTest"].toObject();
            // Trip time
            qint64 recvTime = QDateTime::currentMSecsSinceEpoch();
            qint64 sendTime = responseTestObj["Time"].toVariant().toLongLong();
            qint64 roundTripTime = recvTime - sendTime;


            // CAN Status
            bool can1 = responseTestObj["Can1"].toBool();
            bool can2 = responseTestObj["Can2"].toBool();



            testData["RoundTripTime"] = roundTripTime;
            testData["CANBus1"] = can1;
            testData["CANBus2"] = can2;

        }

        emit testUpdated(testData);
    }

    if (jsonObj.contains("PADSTATUS")) {
        // Save pad state
        bool value = jsonObj["PADSTATUS"].toBool();

        // Update and emit value if it's been changed
        if (value != padArmState) {
            padArmState = value;

            emit padArmed(padArmState);
        }
    }
}


QScopedPointer<ConstantUses> ConstantUses::_instance;

// Singleton implementation
ConstantUses* ConstantUses::instance() {
    if (_instance.isNull()) {
        _instance.reset(new ConstantUses());
    }
    return _instance.data();
}

// Constructor - Starts the Timer for continuous updates
ConstantUses::ConstantUses(QObject *parent) :
        QObject(parent), timer(new QTimer(this)), countdownTimer(new QTimer(this)), countdownValue(5) {

    connect(timer, &QTimer::timeout, this, &ConstantUses::updateTime);
    timer->start(1000); // Update every second

    connect(countdownTimer, &QTimer::timeout, this, &ConstantUses::updateCountdown);
    lastTime = QDateTime::currentDateTime().toString("hh:mm:ss"); // Set initial time
}

// Function to update time every second for the world clock
void ConstantUses::updateTime() {
    lastTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    emit timeUpdated(lastTime); // Emit updated time to all connected slots
}

void ConstantUses::updateCountdown() {
    if (countdownValue < 0) { // Count down mode
        formattedTime = QString("T-%1:%2")
                        .arg(abs(countdownValue) / 60, 2, 10, QChar('0'))
                        .arg(abs(countdownValue) % 60, 2, 10, QChar('0'));

    } else { // Count up mode
        formattedTime = QString("T+%1:%2")
                        .arg(countdownValue / 60, 2, 10, QChar('0'))
                        .arg(countdownValue % 60, 2, 10, QChar('0'));
    }
    countdownValue++;

    emit countdownUpdated(formattedTime);
}


void ConstantUses::startCountdown() {
    countdownValue = -5;
    countDirection = false;
    countdownTimer->start(1000);
}

void ConstantUses::errorCode(const QString &error) {
    // Print the error message
    if (!error.isEmpty()) {
        QString timeStamp = QDateTime::currentDateTimeUtc().toString("hh:mm:ss");
        QString message = timeStamp + ": " + error;
        emit errorEmit(message);
    }
}

void ConstantUses::logEvent(const QString &message) {
    // Print Event
    if (!message.isEmpty()) {
        QString timeStamp = QDateTime::currentDateTimeUtc().toString("hh:mm:ss");
        QString sendMessage = timeStamp + ": " + message;
        emit logEmit(sendMessage);
    }
}

// Returns the latest updated time
QString ConstantUses::currentTime() const {
    return lastTime; // Uses the cached time instead of calling QDateTime multiple times
}

QStringList ConstantUses::getConfig(const QString &key) {
    QString configFilePath = QDir(QCoreApplication::applicationDirPath()).filePath("../data/config.json");
    QFile file(configFilePath);
    QStringList config;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open config file";
        return {};
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        ConstantUses::instance()->errorCode("Failed to read config file");
        return {};
    }

    QJsonObject obj = doc.object();

    // Check if key exists
    if (obj.contains(key) && obj[key].isArray()) {
        QJsonArray jsonArray = obj[key].toArray();
        for (const QJsonValue &value : jsonArray) {
            config.append(value.toString());
        }
    } else {
        qDebug() << "Key not found";
        return {};
    }
    return config;
}



// Your existing buttonMaker function remains unchanged
QPushButton* ConstantUses::buttonMaker(const QString &text, int fontSize, const QString &color) {
    auto *btn = new QPushButton(text);  // No explicit parent

    // Set font
    QFont font;
    font.setPointSize(fontSize);
    font.setBold(false);
    btn->setFont(font);

    // Apply button styling
    btn->setStyleSheet(QString(R"(
        QPushButton {
            background-color: %1;
            color: white;
            border-radius: 5px;
            padding: 5px;
            margin: 0px;
            border: none;
            font-size: %2px;
            font-weight: normal;
        }

        QPushButton:hover {
            background-color: %3;
        }

        QPushButton:pressed {
            background-color: %4;
        }

        QPushButton:focus {
            outline: none;
        }
    )").arg(color)
        .arg(fontSize)
        .arg("#1E1E1E")  // Hover color
        .arg("#2A2A2A")); // Pressed color

    return btn;
}