#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H
#include "../../libs/qcustomplot/qcustomplot/qcustomplot.h"
#include <QJsonObject>
#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <algorithm>
class GraphController : public QWidget {
    Q_OBJECT

public:
    explicit GraphController(QWidget *parent = nullptr);

    QCustomPlot* createPlot(const QString &title, const QString &x_label, const QString &y_label,
                            const QStringList &keys, const QColor &color);
    void addDataPoint(QJsonObject jsonDoc);
    void setGraphingStatus(bool enabled);

public slots:
    void setLiveStatus(bool enabled);

private:
    QCustomPlot* customPlot;
    QVector<double> xData;
    QVector<QVector<double>> yData;
    QStringList yKeys;
    bool isUpdating = true;
    bool isLive = false;
};


#endif //GRAPHCONTROLLER_H
