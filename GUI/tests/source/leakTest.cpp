#include "../headers/leakTest.h"

LeakTest::LeakTest(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    g_layout = new QGridLayout(this);
    //setStyleSheet("border: 2px solid green;");
    setContentsMargins(10, 25, 10, 25);
    // Initialize Table
    sensorTable = new QTableWidget(7, 5, this);



    sensorTable->setHorizontalHeaderLabels({"Time", "FuelTank1", "LOXTank1", "Pneumatics", "HighPress1"});
    //sensorTable->setVerticalHeaderLabels({"1:00", "2:00", "3:00", "4:00", "5:00", "Average"});
    sensorTable->horizontalHeader()->setVisible(true);
    sensorTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sensorTable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    sensorTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    sensorTable->setStyleSheet("QTableWidget {color: white;"
                               "font-size: 20px;"
                               "background-color: black;"
                               "border: 1px solid gray;}"
                               "QHeaderView::section::horizontal {"
                               "background-color: black;"
                               "color: gray;"
                               "font-size: 20px;}");
    sensorTable->setFixedSize(700,300);

    // Add Time Labels
    QStringList timeLabels = {"0:00", "1:00", "2:00", "3:00", "4:00", "5:00", "Average"};
    for (int i = 0; i < timeLabels.size(); ++i) {
        sensorTable->setItem(i, 0, new QTableWidgetItem(timeLabels[i]));
    }

    // Initialize Sensor Storage
    sensorValues.resize(5, std::vector<double>(4, 0.0));

    // Start Button
    start_b = createButton("START", 25, this, "#bc2626");

    // Timer label
    QFont font("Verdana", 25);
    timerLabel = new QLabel("0:00", this);
    timerLabel->setStyleSheet("font-size:30px; color:white;");
    timerLabel->setFont(font);


    // Grid layout
    g_layout->setRowStretch(0,1);
    g_layout->addWidget(sensorTable,1, 1, 2, 2, Qt::AlignCenter);
    g_layout->setRowStretch(4,1);
    g_layout->setColumnStretch(0,1);
    g_layout->addWidget(start_b, 5,1, 1, 1);
    g_layout->addWidget(timerLabel, 5, 2, 1, 1, Qt::AlignCenter);
    g_layout->setRowStretch(6,1);
    g_layout->setColumnStretch(4,1);



    // Timer for Data Collection
    dataTimer = new QTimer(this);
    dataTimer->setInterval(60000);  // 1 minute interval
    countUpTimer = new QTimer(this);
    countUpTimer->setInterval(1000); // Every 1 second

    // WIFI Init
    wifi = wifiInstance;

    // Connect signals
    connect(start_b, &QPushButton::clicked, this, &LeakTest::startOrCancelTest);
    connect(dataTimer, &QTimer::timeout, this, &LeakTest::recordLatestValues); // Enable recording on each timer tick
    connect(wifi, &WIFI::sensorUpdated, this, &LeakTest::captureSensorData);
    connect(countUpTimer, &QTimer::timeout, this, &LeakTest::updateTimerDisplay);
}

void LeakTest::startOrCancelTest() {
    if (recordingActive) {
        resetTest();
    } else {
        startTest();
    }
}

// **Start Test: Reset Values and Start Timer**
void LeakTest::startTest() {
    // Reset state
    recordingActive = false;
    currentTimeIndex = 0;
    elapsedTime = 0;

    // Clear table values (excluding Time column)
    for (int row = 0; row < 6; ++row) {
        for (int col = 1; col < 5; ++col) {
            sensorTable->setItem(row, col, new QTableWidgetItem("0.0"));
        }
    }

    // Reset storage
    for (auto &row : sensorValues) {
        std::fill(row.begin(), row.end(), 0.0);
    }
    sensorDict.clear();
    latestSensorValues.clear();

    // Change button
    start_b->setText("Stop Test");

    // Start data collection
    recordingActive = true;

    // Start timers
    dataTimer->start();
    countUpTimer->start();
}

void LeakTest::resetTest() {
    recordingActive = false;
    dataTimer->stop();
    countUpTimer->stop();
    elapsedTime = 0;

    // Reset GUI
    timerLabel->setText("0:00");
    start_b->setText("Start Test");
}

// **Capture Sensor Data if Recording is Active**
void LeakTest::captureSensorData(QJsonObject data) {
    if (!recordingActive) return;  // Only capture if active

    // Extract the sensors
    data = data["SENSORS"].toObject();

    for (const QString &key : data.keys()) {
        latestSensorValues[key] = data.value(key).toDouble();
    }

    if (currentTimeIndex == 0) {
        recordLatestValues();
    }
}

// **Every Minute: Record Latest Values**
void LeakTest::recordLatestValues() {
    if (!recordingActive) return;

    qDebug() << "Recording data at time index:" << currentTimeIndex;

    // Write latest buffered values to the table
    for (auto it = latestSensorValues.begin(); it != latestSensorValues.end(); ++it) {
        updateTable(currentTimeIndex, it.key(), it.value());
    }

    // Move to the next time index
    currentTimeIndex++;

    if (currentTimeIndex >= 5) {
        updateAverages();
        resetTest();
    }
}

void LeakTest::updateTimerDisplay() {
    elapsedTime++;

    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    timerLabel->setText(QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0')));

    if (minutes >= 5) {
        countUpTimer->stop();
    }
}

// **Update Table with Sensor Data**
void LeakTest::updateTable(int timeIndex, const QString &sensor, double value) {
    if (timeIndex < 0 || timeIndex >= 5) return;

    int sensorCol = -1;
    if (sensor == "FuelTank1") sensorCol = 1;
    else if (sensor == "LOXTank1") sensorCol = 2;
    else if (sensor == "Pneumatics") sensorCol = 3;
    else if (sensor == "HighPress1") sensorCol = 4;

    if (sensorCol != -1) {
        // Store value in sensorValues
        sensorValues[timeIndex][sensorCol - 1] = value;

        // Update table
        sensorTable->setItem(timeIndex, sensorCol, new QTableWidgetItem(QString::number(value, 'f', 2)));
    }
}

// **Compute and Display Averages**
void LeakTest::updateAverages() {
    for (int sensorCol = 1; sensorCol <= 4; ++sensorCol) {
        double sum = 0.0;
        int count = 0;

        for (int timeIndex = 0; timeIndex < 5; ++timeIndex) {
            if (sensorValues[timeIndex][sensorCol - 1] > 0.0) {
                sum += sensorValues[timeIndex][sensorCol - 1];
                count++;
            }
        }

        double avg = (count > 0) ? (sum / count) : 0.0;
        sensorTable->setItem(5, sensorCol, new QTableWidgetItem(QString::number(avg, 'f', 2)));
    }

    qDebug() << "Averages calculated and displayed.";
}


QPushButton* LeakTest::createButton(const QString &text, int fontSize, QWidget *parent, const QString &color) {
    if (!parent) {
        parent = this;
    }

    auto *btn = new QPushButton(text, parent);  // Ensure it has `this` as parent

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
            border-radius: 10%;
            padding: 0px;
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
      .arg("#1E1E1E", "#2A2A2A")); // Pressed color

    return btn;
}