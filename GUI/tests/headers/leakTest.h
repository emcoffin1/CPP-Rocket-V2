#ifndef LEAKTEST_H
#define LEAKTEST_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QJsonObject>
#include <QTimer>
#include <unordered_map>
#include <vector>

#include "../../../ESP_WIFI//wifi.h"

class LeakTest : public QWidget {
    Q_OBJECT
public:
    explicit LeakTest(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

private slots:
    void startOrCancelTest();
    void startTest();
    void captureSensorData(QJsonObject data);
    void updateAverages();
    void recordLatestValues();
    void updateTimerDisplay();


private:
    QGridLayout *g_layout;
    QTableWidget *sensorTable;
    QPushButton *start_b;
    QTimer *dataTimer, *countUpTimer;
    QLabel *timerLabel;

    QMap<QString, double> latestSensorValues;

    WIFI *wifi;
    bool recordingActive = false;
    int currentTimeIndex = 0;  // Tracks which row to update (0-4)
    int elapsedTime = 0;

    std::unordered_map<std::string, std::vector<double>> sensorDict;
    std::vector<std::vector<double>> sensorValues;  // Stores values per time index

    void updateTable(int timeIndex, const QString &sensor, double value);
    void resetTest();

};

#endif // LEAKTEST_H
