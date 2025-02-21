cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)

# Set Qt installation path (Adjust to match your actual Qt MinGW installation)
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.2/mingw_64")

# Find Qt components
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Network)

# Run Qt's MOC (Meta-Object Compiler) for headers containing Q_OBJECT
qt6_wrap_cpp(MOC_SOURCES
        GUI/headers/mainPanel.h
        GUI/headers/mainTab.h
        GUI/headers/mainUI.h
        GUI/headers/optTab.h
        GUI/headers/testTab.h
        GUI/tests/headers/clickTest.h
        ESP_WIFI/wifi.h
)

# Create the Qt application using `add_executable()`
add_executable(MyQtApp
        main.cpp
        GUI/source/mainPanel.cpp
        GUI/source/mainTab.cpp
        GUI/source/mainUI.cpp
        GUI/source/optTab.cpp
        GUI/source/testTab.cpp
        ESP_WIFI/wifi.cpp
        GUI/tests/source/clickTest.cpp
        ${MOC_SOURCES}  # Ensure MOC-generated files are included
)

# Link Qt libraries using the correct signature
target_link_libraries(MyQtApp
        PRIVATE Qt6::Widgets
        PRIVATE Qt6::Gui
        PRIVATE Qt6::Core
        PRIVATE Qt6::Network
)
