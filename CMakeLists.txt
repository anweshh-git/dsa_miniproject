cmake_minimum_required(VERSION 3.16)
project(CampusMap VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

set(SOURCES
    main.cpp

    # Core DSA
    core/graph/Graph.cpp
    core/graph/Dijkstra.cpp
    core/queue/ShuttleQueue.cpp
    core/linkedlist/PathHistory.cpp
    core/linkedlist/VisitLog.cpp
    core/sorting/BuildingSorter.cpp
    core/searching/HashTable.cpp
    core/searching/BinarySearch.cpp

    # UI
    ui/MainWindow.cpp
    ui/MapView.cpp
    ui/panels/SearchPanel.cpp
    ui/panels/ShuttlePanel.cpp
    ui/panels/HistoryPanel.cpp
    ui/panels/SortPanel.cpp
    ui/dialogs/BuildingInfoDialog.cpp
)

set(HEADERS
    core/models/Building.h
    core/models/Path.h
    core/graph/Graph.h
    core/graph/Dijkstra.h
    core/queue/ShuttleQueue.h
    core/linkedlist/PathHistory.h
    core/linkedlist/VisitLog.h
    core/sorting/BuildingSorter.h
    core/searching/HashTable.h
    core/searching/BinarySearch.h
    ui/MainWindow.h
    ui/MapView.h
    ui/panels/SearchPanel.h
    ui/panels/ShuttlePanel.h
    ui/panels/HistoryPanel.h
    ui/panels/SortPanel.h
    ui/dialogs/BuildingInfoDialog.h
)

add_executable(CampusMap ${SOURCES} ${HEADERS})

target_include_directories(CampusMap PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/ui
)

target_link_libraries(CampusMap PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
