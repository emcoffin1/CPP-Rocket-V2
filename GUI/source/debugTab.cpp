#include "../headers/debugTab.h"

#include <qcoreapplication.h>
#include <qfiledialog.h>

DebugTab::DebugTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Init stuff
    wifi = wifiInstance;

    main_layout = new QVBoxLayout(this);
    setStyleSheet("QTextEdit {"
                     "background-color: black;"
                     "color: white;"
                     "border: 1px solid grey"
                     "}"
                     "QVBoxLayout {"
                     "background-color: grey;"
                     "}"
                     "QPushButton {"
                     "background-color: grey;"
                     "border: 1px solid black;"
                     "color: white;"
                     "height: 50px;"
                     "font-size: 20px;"
                     "}");


    // Event log window (left)
    event_widget = new QWidget(this);
    event_layout = new QVBoxLayout(event_widget);

    // Event label
    eventLabel = new QLabel("Event Log", event_widget);
    eventLabel->setStyleSheet("color:white; font-size:25px;");
    eventLabel->setAlignment(Qt::AlignCenter);
    event_layout->addWidget(eventLabel);

    // Event text edit
    eventLog = new QTextEdit(event_widget);
    eventLog->setReadOnly(true);
    eventLog->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    eventLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    event_layout->addWidget(eventLog);

    // Event options
    butboxEvent = new QHBoxLayout();
    saveEventButton = new QPushButton("Save", event_widget);
    butboxEvent->addWidget(saveEventButton);
    eraseEventButton = new QPushButton("Erase", event_widget);
    butboxEvent->addWidget(eraseEventButton);
    event_layout->addLayout(butboxEvent);

    event_widget->setLayout(event_layout);




    // Error log window (right)
    error_widget = new QWidget(this);
    error_layout = new QVBoxLayout(error_widget);

    // Error Label
    errorLabel = new QLabel("Error Log", error_widget);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setStyleSheet("color:white; font-size:25px;");
    error_layout->addWidget(errorLabel);

    // Error text edit
    errorLog = new QTextEdit(error_widget);
    errorLog->setReadOnly(true);
    errorLog->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    errorLog->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    error_layout->addWidget(errorLog);

    // Error options
    butboxError = new QHBoxLayout();
    saveErrorButton = new QPushButton("Save", error_widget);
    butboxError->addWidget(saveErrorButton);
    eraseErrorButton = new QPushButton("Erase", error_widget);
    butboxError->addWidget(eraseErrorButton);
    error_layout->addLayout(butboxError);

    error_widget->setLayout(error_layout);



    // Splitter
    splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(event_widget);
    splitter->addWidget(error_widget);

    main_layout->addWidget(splitter);

    setLayout(main_layout);


    // Connect signals
    connect(ConstantUses::instance(), &ConstantUses::errorEmit, this, &DebugTab::updateError, Qt::QueuedConnection);
    connect(ConstantUses::instance(), &ConstantUses::logEmit, this, &DebugTab::updateEvent);

    connect(eraseErrorButton, &QPushButton::clicked, this, &DebugTab::deleteError);
    connect(eraseEventButton, &QPushButton::clicked, this, &DebugTab::deleteEvent);
    connect(saveErrorButton, &QPushButton::clicked, this, &DebugTab::saveError);
    connect(saveEventButton, &QPushButton::clicked, this, &DebugTab::saveEvent);

}

void DebugTab::updateEvent(QString message) {
    eventLog->append(message);
}

void DebugTab::updateError(QString message) {
    errorLog->append(message);
}


void DebugTab::deleteEvent() {
    eventLog->clear();
}

void DebugTab::deleteError() {
    errorLog->clear();
}


void DebugTab::saveEvent() {


    // Define the file path inside the project folder
    QString filePath = QDir(QCoreApplication::applicationDirPath()).filePath("../data/eventLogger_"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".txt");

    // Ensure directory exists
    QDir dir(QFileInfo(filePath).absolutePath());
    if (!dir.exists()) {
        // Create if doesn't exist
        dir.mkpath(".");
    }

    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "=== Event Log (" << QDateTime::currentDateTime().toString("yyyy-MM-dd") << ") ===\n";
        out << eventLog->toPlainText() << "\n\n";
        file.close();
        QMessageBox::warning(nullptr, "SUCCESS", "Event Log Saved");
    } else {
        QMessageBox::warning(nullptr, "ERROR","Could not save event log");
    }

}

void DebugTab::saveError() {

    // Define the file path inside the project folder
    QString filePath = QDir(QCoreApplication::applicationDirPath()).filePath("../data/errorLogger_"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".txt");

    // Ensure directory exists
    QDir dir(QFileInfo(filePath).absolutePath());
    if (!dir.exists()) {
        // Create if doesn't exist
        dir.mkpath(".");
    }

    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "=== Event Log (" << QDateTime::currentDateTime().toString("yyyy-MM-dd") << ") ===\n";
        out << eventLog->toPlainText() << "\n\n";
        file.close();
        QMessageBox::warning(nullptr, "SUCCESS", "Error Log Saved");
    } else {
        QMessageBox::warning(nullptr, "ERROR","Could not save error log");
    }
}
