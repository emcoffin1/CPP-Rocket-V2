#include "../headers/valveDisplay.h"


ValveTree::ValveTree(QWidget* parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init wifi
    wifi = wifiInstance;

    // Init layout
    g_layout = new QGridLayout(this);
    setStyleSheet("QCheckBox {color:yellow;}"
                  "QCheckBox::indicator {"
                  "background-color: red;"
                  "border-radius: 5px; margin: 0px; padding: 10px;}");

    // Store checkboxes in a QMap for easy lookup
    valveCheckBoxMap = QMap<QString, QCheckBox*>();
    QList<QString> valveNames {"HP1", "HP2", "LOXT1", "LOXT2", "FUELT1",
        "FUELT2", "LDREG", "FDREG", "LOXIN", "FUELIN", "CHAMB1", "CHAMB2"};
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
        row = (i - 1) % 6;
        col = ((i - 1) / 6) * 2;  // Assign to columns 0 and 2
        g_layout->addWidget(valveCheckBoxMap[valveNames[i-1]], row, col);
    }

    g_layout->setColumnMinimumWidth(1, 25);
    setLayout(g_layout);

    if (wifi) {
        connect(wifiInstance, &WIFI::valveUpdated, this, &ValveTree::changeValveColor);
    }
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