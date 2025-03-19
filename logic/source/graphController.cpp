#include "../../logic/headers/graphController.h"

GraphController::GraphController(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  customPlot = new QCustomPlot(this);

  layout->addWidget(customPlot);
  setLayout(layout);
}


// Creates plot
QCustomPlot* GraphController::createPlot(const QString& title, const QString& x_label, const QString& y_label, const QStringList &keys, const QColor &color) {

  // Determine number of graphs that need to be made
  yKeys = keys;
  int numGraphs = yKeys.size();

  for (int i = 0; i < numGraphs; i++) {
    // Add a line
    customPlot->addGraph();
    // Change the color
    customPlot->graph(i)->setPen(QPen(QColor::fromHsv(i * 60, 255, 255)));
    customPlot->graph(i)->setName(keys[i]);
  }

  // Config axes
  customPlot->xAxis->setLabel(x_label);
  customPlot->xAxis->setLabelColor(Qt::white);
  customPlot->xAxis->setRange(0,10);
  customPlot->xAxis->setTickLabelColor(Qt::white);


  customPlot->yAxis->setLabel(y_label);
  customPlot->yAxis->setLabelColor(Qt::white);
  customPlot->yAxis->setRange(-1,1);
  customPlot->yAxis->setTickLabelColor(Qt::white);


  customPlot->setBackground(color);
  customPlot->axisRect()->setBackground(color);
  customPlot->axisRect()->setMinimumMargins(QMargins(0, 0, 0, 0));
  customPlot->xAxis->grid()->setVisible(false);
  customPlot->yAxis->grid()->setVisible(false);
  /*
  // Add a custom border
  QCPItemRect *border = new QCPItemRect(customPlot);
  border->topLeft->setType(QCPItemPosition::ptPlotCoords);
  border->bottomRight->setType(QCPItemPosition::ptPlotCoords);

  // Set rectangle size to match axisRect
  border->topLeft->setCoords(customPlot->xAxis->range().lower, customPlot->yAxis->range().upper);
  border->bottomRight->setCoords(customPlot->xAxis->range().upper, customPlot->yAxis->range().lower);

  // Set border pen (color & thickness)
  border->setPen(QPen(Qt::gray, 3));  // Black border with 2px thickness
  */

  // **Adding Margins to Ensure Visibility**
  customPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0)); // Add a 50px margin around

  // **Ensure layout updates properly**
  customPlot->plotLayout()->setAutoMargins(QCP::msAll);


  // Size
  customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  customPlot->setMinimumSize(200,150);


  // Legend
  customPlot->legend->setFillOrder(QCPLegend::foColumnsFirst);
  customPlot->legend->setWrap(1);  // Force single row
  customPlot->legend->setVisible(true);
  customPlot->legend->setFont(QFont("Arial", 7));
  customPlot->legend->setTextColor(Qt::white);
  customPlot->legend->setBrush(QBrush(Qt::black)); // Background color
  //customPlot->legend->setBorderPen(QPen(Qt::blue));

  QCPLayoutInset *insetLayout = customPlot->axisRect()->insetLayout();
  insetLayout->addElement(customPlot->legend, Qt::AlignTop | Qt::AlignLeft); // Position it at the top of the graph

  // **Reduce Legend Margins**
  customPlot->legend->setMargins(QMargins(0, 0, 0, 0)); // Remove extra spacing around legend


  return customPlot;
};

// Adds data to plot
void GraphController::addDataPoint(QJsonObject jsonDoc) {
  if (!isUpdating) return;
  QJsonObject jsonSens = jsonDoc["SENSORS"].toObject();


  double x = 0;
  // Current time structure-- string , mm:ss
  if (isLive && jsonSens.contains("time")) {
    x = jsonSens["time"].toDouble();
  } else {
    //qDebug() << "improper date stucture";
    x = (xData.isEmpty()) ? 0 : xData.last() + 1;
  }

  QVector<double> yValues(yKeys.size(), 0.0); //Default y values
  //qDebug() << jsonSens["HighPress1"];

  // Get new y values
  for (int i = 0; i < yKeys.size(); i++) {
    if (jsonSens.contains(yKeys[i])) {
      yValues[i] = jsonSens[yKeys[i]].toDouble();
      //qDebug() << "yValues" << yValues[i];
    }
  }

  xData.append(x);


  if (yData.isEmpty()) {
    yData.resize(yKeys.size());
  }

  for (int i = 0; i < yKeys.size(); ++i) {
    yData[i].append(yValues[i]);
  }


  // Delete data every 200 values for live data and 11 for fake
  if (isLive) {
    if (xData.size() > 200) {
      xData.remove(0);
      for (int i = 0; i < yKeys.size(); ++i) {
        yData[i].remove(0);
      }
    }
  } else {
    if (xData.size() > 11) {
      xData.remove(0);
      for (int i = 0; i < yKeys.size(); ++i) {
        yData[i].remove(0);
      }
    }
  }


  // Update graph data
  for (int i = 0; i < yKeys.size(); ++i) {
    customPlot->graph(i)->setData(xData, yData[i]);
  }


  auto minY = *std::min_element(yData.front().begin(), yData.front().end());
  auto maxY = *std::max_element(yData.front().begin(), yData.front().end());

  // Iterate through all inner QVectors
  for (const auto& vec : yData) {
    if (!vec.isEmpty()) {
      auto minIt = std::min_element(vec.begin(), vec.end());
      auto maxIt = std::max_element(vec.begin(), vec.end());

      if (minIt != vec.end()) minY = std::min(minY, *minIt);
      if (maxIt != vec.end()) maxY = std::max(maxY, *maxIt);
    }
  }

  if (isLive) {
    customPlot->xAxis->setTickLabelColor(Qt::white);
  } else {
    customPlot->xAxis->setTickLabelColor(Qt::black);
  }


  customPlot->xAxis->setRange(x-9, x+1);
  customPlot->yAxis->setRange(minY, maxY);
  customPlot->replot();

};

void GraphController::setGraphingStatus(bool enabled) {
  isUpdating = enabled;
};

void GraphController::setLiveStatus(bool enabled) {
  isLive = enabled;
}




