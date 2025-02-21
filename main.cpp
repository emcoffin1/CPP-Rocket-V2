#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "GUI/headers/mainUI.h"
#include "ESP_WIFI/wifi.h"

QJsonObject loadConfig(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open file %s", filePath.toStdString().c_str());
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}


int main(int argc, char *argv[]) {


    QApplication app(argc, argv);
    MainUI window;
    window.show();

    return app.exec();
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.