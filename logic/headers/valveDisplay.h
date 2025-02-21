#ifndef LADDER_H
#define LADDER_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QPushButton>

class CircleItem : public QGraphicsEllipseItem {
public:
    CircleItem(qreal x, qreal y, qreal size, QGraphicsItem* parent = nullptr);

    void changeColor(const QColor& color);
};

class LadderWidget : public QWidget {
    Q_OBJECT

public:
    explicit LadderWidget(QWidget *parent = nullptr);
    ~LadderWidget();

    private slots:
        void changeCircleColor(); // Example slot to change a circle's color

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QList<CircleItem*> leftCircles;
    QList<CircleItem*> rightCircles;
};



#endif //VALVEDISPLAY_H
