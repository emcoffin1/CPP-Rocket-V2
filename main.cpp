#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "GUI/headers/mainUI.h"
#include "ESP_WIFI/wifi.h"
#include "GUI/headers/debugTab.h"

DebugTab *globalDebugTab = nullptr;

void crashHandler(int signal) {
    if (globalDebugTab) {
        QString error = "PROGRAM CRASHED";
        globalDebugTab->updateError(error);
        globalDebugTab->updateEvent(error);
        globalDebugTab->saveError();
        globalDebugTab->saveEvent();
    }

    exit(signal);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Regist Crash handler
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
    signal(SIGFPE, crashHandler);

    WIFI *wifiInstance = WIFI::getInstance();
    DebugTab *debug_tab = new DebugTab(nullptr, wifiInstance);

    globalDebugTab = debug_tab;

    MainUI window(wifiInstance);
    window.show();

    return app.exec();
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.