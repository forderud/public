project(fontsampler)

set(fontsampler_SOURCES
    main.cpp
    mainwindow.cpp
    )

set(fontsampler_HEADERS
    mainwindow.h
    )

set(fontsampler_UI
    mainwindowbase.ui
    )

# Run moc & uic automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt libraries
find_package(Qt5 COMPONENTS Core Gui Widgets)

add_executable(fontsampler ${fontsampler_SOURCES} ${fontsampler_HEADERS} ${fontsampler_UI})
target_link_libraries(fontsampler Qt5::Core Qt5::Gui Qt5::Widgets)
