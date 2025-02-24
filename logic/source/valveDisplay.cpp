#include "../headers/valveDisplay.h"
#include <QBrush>
#include <QDebug>

// CircleItem constructor
CircleItem::CircleItem(qreal x, qreal y, qreal size, QGraphicsItem* parent)
    : QGraphicsEllipseItem(x, y, size, size, parent) {
    setBrush(QBrush(Qt::gray));  // Default color
}

// Change circle color method
void CircleItem::changeColor(const QColor& color) {
    setBrush(QBrush(color));
}

// LadderWidget constructor
LadderWidget::LadderWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    int diameter = 40;
    int x_start = 25;
    int x_end = x_start + 50;
    int y_seperation = 40;

    // Create Graphics View and Scene
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(0, 0, 200, 500, this);
    view->setScene(scene);
    layout->addWidget(view);



    // Create circles in a ladder pattern
    for (int i = 0; i < 5; ++i) {
        int y_offset = y_seperation * i;  // Adjust spacing

        // Left side
        CircleItem* leftCircle = new CircleItem(x_start, y_offset, diameter);
        scene->addItem(leftCircle);
        leftCircles.append(leftCircle);

        // Right side
        CircleItem* rightCircle = new CircleItem(x_end, y_offset, diameter);
        scene->addItem(rightCircle);
        rightCircles.append(rightCircle);
    }
}
/*
    // Example: Add a button to change the color of a specific circle
    QPushButton *changeColorBtn = new QPushButton("Change Left 1 Color", this);
    layout->addWidget(changeColorBtn);
    connect(changeColorBtn, &QPushButton::clicked, this, &LadderWidget::changeCircleColor);
}*/

// Slot function to change color
void LadderWidget::changeCircleColor(int valveNumber, QString status) {
    QString colorString = QString("Qt::%1").arg(status);
    if (!leftCircles.isEmpty() && valveNumber <= 5) {
        leftCircles[0]->changeColor(colorString);  // Change first left circle to red
    }
    if (!rightCircles.isEmpty() && valveNumber > 5) {
        rightCircles[valveNumber-5]->changeColor(colorString);
    }
}

// Destructor (optional)
LadderWidget::~LadderWidget() {
    qDebug() << "LadderWidget destroyed.";
}
