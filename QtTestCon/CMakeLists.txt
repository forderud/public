cmake_minimum_required(VERSION 3.24)
# Generated from testcon.pro.

#####################################################################
## testcon Binary:
#####################################################################
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS Gui Widgets AxContainer AxServer PrintSupport)

qt_add_executable(testcon # special case
#    OUTPUT_DIRECTORY "$$[QT_INSTALL_BINS]" # special case
#    INSTALL_DIRECTORY "$$[QT_INSTALL_BINS]" # special case
        ambientproperties.cpp ambientproperties.h ambientproperties.ui
        changeproperties.cpp changeproperties.h changeproperties.ui
        controlinfo.cpp controlinfo.h controlinfo.ui
        docuwindow.cpp docuwindow.h
        invokemethod.cpp invokemethod.h invokemethod.ui
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
 )
 
target_link_libraries(testcon PUBLIC
        Qt::AxContainer
        #Qt::AxContainerPrivate
        Qt::AxServer
        Qt::Gui
        Qt::PrintSupport
        Qt::Widgets
)

# Resources:
set(testcon_resource_files
    "images/controlmethods.png"
    "images/controlproperties.png"
    "images/filenew.png"
)

qt6_add_resources(testcon "testcon" # special case
    PREFIX
        "/"
    FILES
        ${testcon_resource_files}
)


#### Keys ignored in scope 1:.:.:testcon.pro:<TRUE>:
# RC_FILE = "testcon.rc"
# TEMPLATE = "app"

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:testcon.pro:NOT MINGW:
# QMAKE_POST_LINK = "midl" "$$shell_quote($$shell_path $$PWD/testcon.idl)" "&&" "move" "testcon.tlb" "$(TARGETDIR)"
