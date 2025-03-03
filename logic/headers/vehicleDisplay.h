#ifndef VEHICLEDISPLAY_H
#define VEHICLEDISPLAY_H
#include <QFile>
#include <QDir>
#include <QCheckBox>
#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <qcoreapplication.h>
#include <qjsonarray.h>
#include "../../ESP_WIFI/wifi.h"

class ValveTree : public QWidget {
    Q_OBJECT
public:
    explicit ValveTree(QWidget *parent = nullptr, WIFI *wifiInstance = nullptr);

    QGridLayout *g_layout;
    QMap<QString, QCheckBox*> valveCheckBoxMap;
    QList<QString> valveNames;
    WIFI *wifi;

    QList<QString> loadValveNamesFromConfig(const QString &fileName);

public slots:
    void changeValveColor(QJsonObject jsonObj);



};



class RollDisplay : public QWidget {
  Q_OBJECT
public:
    explicit RollDisplay(QWidget *parent, WIFI *wifiInstance, const QString &imageName,
                        const QString &posType, qreal initAngle = 0, qreal startX = 100, qreal startY = 100);

    QPixmap image;
    qreal angle;
    qreal startX, startY;
    QString typePos;
    qreal initAngle;

    WIFI *wifi = nullptr;


protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void roll(QJsonObject jsonObj);
private:
    void loadImage(const QString &imageName);
    const QString basePath = "C:/Users/emcof/CLionProjects/RocketGUI_cpp/data/images/";
    QString posType;


};



#endif
