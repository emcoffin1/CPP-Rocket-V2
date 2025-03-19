#include "../headers/vehicleDisplay.h"



ValveTree::ValveTree(QWidget* parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init wifi
    wifi = wifiInstance;

    // Init layout
    g_layout = new QGridLayout(this);
    setStyleSheet("QCheckBox::indicator {"
                  "background-color: red;"
                  "border-radius: 5px; margin: 0px; padding: 10px;}");

    // Store checkboxes in a QMap for easy lookup
    valveCheckBoxMap = QMap<QString, QCheckBox*>();
    //QList<QString> valveNames {"HighPress", "HighVent", "LOXDomeReg", "LOXDomeVent", "FuelDomeReg",
     //                                       "FuelDomeVent", "LOXVent", "FuelVent", "LOXMV", "FuelMV"};
    QString configFilePath = QDir(QCoreApplication::applicationDirPath()).filePath("../data/config.json");

    valveNames = loadValveNamesFromConfig(configFilePath);

    // Create checkboxes and add them to the map
    for (int i = 1; i <= valveNames.size(); ++i) {
        QString valveName = valveNames[i-1];
        QCheckBox *checkBox = new QCheckBox(valveName, this);
        checkBox->setCheckable(false);
        checkBox->setFixedSize(30, 30);

        // Align text differently for left & right columns
        if (i <= valveNames.size() / 2) {
            checkBox->setLayoutDirection(Qt::RightToLeft);
        } else {
            checkBox->setLayoutDirection(Qt::LeftToRight);
        }

        // Store in QMap
        valveCheckBoxMap[valveName] = checkBox;
    }

    // Add checkboxes to the grid layout
    int row, col;
    for (int i = 1; i <= valveNames.size(); ++i) {
        row = (i - 1) % 5;
        col = ((i - 1) / 5) * 2;  // Assign to columns 0 and 2
        g_layout->addWidget(valveCheckBoxMap[valveNames[i-1]], row, col);
    }

    g_layout->setColumnMinimumWidth(1, 25);
    setLayout(g_layout);

    if (wifi) {
        connect(wifiInstance, &WIFI::valveUpdated, this, &ValveTree::changeValveColor);
    }
}

QList<QString> ValveTree::loadValveNamesFromConfig(const QString &fileName) {
    QList<QString> valveList;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return valveList; // Empty list if can't be found
    }

    // Read content
    QByteArray jsonData = file.readAll();
    file.close();

    // Parse data
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qDebug() << "JSON document is not an object";
        return valveList;
    }

    // Extract valves from data
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("VALVES") && jsonObj["VALVES"].isArray()) {
        QJsonArray valveArray = jsonObj["VALVES"].toArray();
        for (const QJsonValue &value : valveArray) {
            valveList.append(value.toString());
        }
    } else {
        qDebug() << "VALVES not found";
    }
    return valveList;


}

void ValveTree::changeValveColor(QJsonObject jsonObj) {
    // Changes the color of a specific valve

    // Check if sensors in list
    if (!jsonObj.contains("VALVES") || !jsonObj["VALVES"].isObject()) {
        return;
    }

    // Extract valves
    QJsonObject valves = jsonObj["VALVES"].toObject();

    for (QString valveName : valves.keys()) {
        int valveValue = valves[valveName].toInt();

        QString color;
        if (valveValue == 0) {
            color = "red";
        }
        else if (valveValue == 1) {
            color = "green";
        }

        if (valveCheckBoxMap.contains(valveName)) {
            QCheckBox *checkBox = valveCheckBoxMap[valveName];

            // Apply color
            checkBox->setStyleSheet(QString(
                "QCheckBox::indicator {"
                  "background-color: %1;"
                  "border-radius: "
                  "5px; margin: 0px; "
                  "padding: 10px;}").arg(color));
        }
    }
}


RollDisplay::RollDisplay(QWidget* parent, WIFI *wifiInstance, const QString &imageName,
                        const QString &posType, qreal initRoll, qreal startX, qreal startY) : QWidget(parent),
                        angle(initRoll), startX(startX), startY(startY) {
    wifi = wifiInstance;
    typePos = posType;
    initAngle = initRoll;
    setFixedSize(startX,startY);
    if (!imageName.isEmpty()) {
        loadImage(imageName);
    }

    connect(wifiInstance, &WIFI::positionUpdated, this, &RollDisplay::roll);

}

void RollDisplay::loadImage(const QString &imageName) {
    QString imagePath = QDir(QCoreApplication::applicationDirPath()).filePath("../data/images/" + imageName);
    //qDebug() << "Loading image: " << imagePath;

    if (!QFile::exists(imagePath)) {
        return;
    }

    if (!image.load(imagePath)) {
        return;
    }
    image = image.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setMinimumSize(image.size());
    update();
}

void RollDisplay::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    if (image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int x = width() / 2;
    int y = height() / 2;

    painter.translate(x,y);     // Move to center
    painter.rotate(angle);            // Rotate
    painter.translate(-x,-y);   // Move Back

    int imgX = (width() - image.width()) / 2;
    int imgY = (height() - image.height()) / 2;

    painter.drawPixmap(imgX, imgY, image);

}

void RollDisplay::roll(QJsonObject jsonObj) {
    //qDebug() << jsonObj["POSITION"].toString();
    // Check if it exists
    if (!jsonObj.contains("POSITION") || !jsonObj["POSITION"].isObject()) {
        return;
    }
    //qDebug() << jsonObj;
    // Extract positions
    QJsonObject positions = jsonObj["POSITION"].toObject();

    for (QString posit : positions.keys()) {
        if (typePos == "Roll" && posit == "Roll") {
            angle = positions["Roll"].toInt() + initAngle;
            update();
        }

        if (typePos == "Pitch" && posit == "Pitch") {
            angle = positions["Pitch"].toInt() + initAngle;
            update();
        }

        if (typePos == "Yaw" && posit == "Yaw") {
            angle = positions["Yaw"].toInt() + initAngle;
            update();
        }
    }
}