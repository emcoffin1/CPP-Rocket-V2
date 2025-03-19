#include "../headers/testTab.h"


TestTab::TestTab(QWidget *parent, WIFI *wifiInstance) : QWidget(parent) {
    // Create main grid layout
    g_layout = new QGridLayout(this);
    g_layout->setContentsMargins(0, 0, 0, 0);
    g_layout->setSpacing(0);

    // WIFI Init
    wifi = wifiInstance;

    // Create secondary layout for buttons
    button_grid = new QGridLayout();
    button_grid->setContentsMargins(0, 0, 0, 0);
    button_grid->setSpacing(0);

    // Create and style label
    QFont font("Verdana", 40, QFont::Bold);
    test = new QLabel("TESTS");
    test->setFont(font);
    test->setStyleSheet("color: white; padding-bottom: 15px;");
    test->setAlignment(Qt::AlignCenter);

    // Create buttons
    click_btn = ConstantUses::buttonMaker("Click", 20);
    leak_btn = ConstantUses::buttonMaker("Leak", 20);
    response_btn = ConstantUses::buttonMaker("Response", 20);
    igniter_btn = ConstantUses::buttonMaker("Igniter", 20);
    surface_btn = ConstantUses::buttonMaker("Control Surfaces", 20);

    // Button connects
    connect(click_btn, &QPushButton::clicked, [this]() { switchTests(0); });
    connect(leak_btn, &QPushButton::clicked, [this]() { switchTests(1); });
    connect(response_btn, &QPushButton::clicked, [this]() { switchTests(2); });
    connect(igniter_btn, &QPushButton::clicked, [this]() { switchTests(3); });
    connect(surface_btn, &QPushButton::clicked, [this]() { switchTests(4); });


    // Add buttons to the button grid
    button_grid->addWidget(click_btn, 0, 0);
    button_grid->addWidget(leak_btn, 0, 1);
    button_grid->addWidget(response_btn, 0, 2);
    button_grid->addWidget(igniter_btn, 0, 3);
    button_grid->addWidget(surface_btn, 0, 4);

    // Create stacked widgets
    stacked_widget = new QStackedWidget(this);
    clickTest = new ClickTest(this, wifiInstance);
    leakTest = new LeakTest(this, wifiInstance);
    responseTest = new ResponseTest(this, wifiInstance);
    surfaceTest = new SurfaceTest(this, wifiInstance);
    igniterTest = new IgniterTest(this, wifiInstance);
    stacked_widget->addWidget(clickTest);
    stacked_widget->addWidget(leakTest);
    stacked_widget->addWidget(responseTest);
    stacked_widget->addWidget(igniterTest);
    stacked_widget->addWidget(surfaceTest);
    stacked_widget->setContentsMargins(0,0,0,0);

    // Create v layout for random data
    v_layout = new QVBoxLayout();
    //v_layout->setContentsMargins(0, 0, 0, 0);


    // Create vehicle display
    // Valve tree
    valveDisplay = new ValveTree(this, wifiInstance);
    v_layout->addWidget(valveDisplay);
    QString topdown = "rocket_top_profile.png";

    // roll display
    rollDisplay = new RollDisplay(this, wifiInstance, topdown, "Roll",
                                    0, 150, 150);
    v_layout->addWidget(rollDisplay, Qt::AlignVCenter);
    v_layout->addStretch(1);

    // pitch display
    QString side = "rocket_side_profile_pointed.png";
    pitchDisplay = new RollDisplay(this, wifiInstance, side, "Pitch",
                                    90, 150, 400);

    v_layout->addWidget(pitchDisplay, Qt::AlignVCenter);
    v_layout->addStretch(5);


    // Add button layout to the main layout
    g_layout->addWidget(test, 0, 0, 1, 4);
    g_layout->addLayout(button_grid, 1, 0, 1, 4);
    g_layout->addWidget(stacked_widget,2,0, 1, 3);
    g_layout->addLayout(v_layout,2,3, 6, 1, Qt::AlignCenter);


    // Connect listener for ladder updates
    connect(wifi, &WIFI::valveUpdated, this, updateValves);
}


void TestTab::switchTests(int index) const {
    if (index >= 0 && index < stacked_widget->count()) {
        stacked_widget->setCurrentIndex(index);
    }
}

void TestTab::updateValves(QJsonObject valveData) {
    //valveDisplay->changeCircleColor(1, "red");
}