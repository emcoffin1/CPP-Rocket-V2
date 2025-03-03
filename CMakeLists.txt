cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

# ✅ Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Correct Qt Path (No Spaces & Points to Qt)
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.2/mingw_64")

# ✅ Ensure MinGW is Used (Adjust if needed)
set(CMAKE_C_COMPILER "C:/Qt/Tools/mingw1310_64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Qt/Tools/mingw1310_64/bin/g++.exe")
set(CMAKE_MAKE_PROGRAM "C:/Qt/Tools/mingw1310_64/bin/mingw32-make.exe")

# ✅ Enable Qt Features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ✅ Find Required Qt Components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network SerialPort Bluetooth PrintSupport)

# Include QCustomPlot
include_directories(
        ${CMAKE_SOURCE_DIR}/libs/qcustomplot/qcustomplot
        ${Qt6Core_INCLUDE_DIRS}
        ${QtGui_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS})

# ✅ Add Executable
add_executable(MyQtApp
        main.cpp
        GUI/source/mainUI.cpp
        GUI/headers/mainUI.h
        GUI/source/optTab.cpp
        GUI/headers/optTab.h
        GUI/source/mainPanel.cpp
        GUI/headers/mainPanel.h
        GUI/source/mainTab.cpp
        GUI/headers/mainTab.h
        GUI/headers/testTab.h
        GUI/source/testTab.cpp
        GUI/tests/headers/clickTest.h
        GUI/tests/source/clickTest.cpp
        ESP_WIFI/wifi.cpp
        ESP_WIFI/wifi.h
        GUI/source/setTab.cpp
        GUI/headers/setTab.h
        GUI/tests/headers/leakTest.h
        GUI/tests/source/leakTest.cpp
        logic/headers/vehicleDisplay.h
        logic/source/vehicleDisplay.cpp
        GUI/tests/headers/responseTest.h
        GUI/tests/source/responseTest.cpp
        logic/headers/graphController.h
        logic/source/graphController.cpp
        GUI/tests/headers/surfaceTest.h
        GUI/tests/source/surfaceTest.cpp
        GUI/tests/headers/igniterTest.h
        GUI/tests/source/igniterTest.cpp
)

# Add QCustomPlot Library
add_library(qcustomplot STATIC libs/qcustomplot/qcustomplot/qcustomplot.cpp)
target_link_libraries(qcustomplot PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# ✅ Link Qt Libraries
target_link_libraries(MyQtApp
        PRIVATE qcustomplot
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::SerialPort
        Qt6::Bluetooth
        Qt6::PrintSupport
)



# ✅ Automatically Deploy Qt DLLs on Windows
if (WIN32)
    add_custom_command(TARGET MyQtApp POST_BUILD
            COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe" --release "$<TARGET_FILE:MyQtApp>"
            COMMENT "Deploying Qt dependencies with windeployqt..."
    )
endif()
