cmake_minimum_required(VERSION 3.16)

project(project-chat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt5 REQUIRED COMPONENTS Core Network)

set(TS_FILES project-chat_za_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(project-chat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res/images.qrc
        res/images/logo.rc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET project-chat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(project-chat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(project-chat
            ${PROJECT_SOURCES}
            res/images.qrc
            login.h login.cpp login.ui
            register.h register.cpp register.ui
            style/stylesheet.qss
            global.h
            global.cpp
            singleton.h
            httpmgr.h httpmgr.cpp
            config.ini
            timerbtn.h timerbtn.cpp
            clickedlabel.h clickedlabel.cpp
            res/rc.qrc
            resetdialog.cpp resetdialog.h resetdialog.ui
            tcpmgr.h tcpmgr.cpp
            usermgr.cpp usermgr.h


        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(project-chat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(project-chat PRIVATE Qt5::Core Qt5::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.project-chat)
endif()
set_target_properties(project-chat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS project-chat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(project-chat)
endif()

# 设置目标配置文件路径
set(TargetConfig "${CMAKE_SOURCE_DIR}/config.ini")
# 设置输出目录
set(OutputDir "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
# 替换路径分隔符为 Windows 风格
if (WIN32)
    string(REPLACE "/" "\\" TargetConfig "${TargetConfig}")
    string(REPLACE "/" "\\" OutputDir "${OutputDir}")
endif()
# 添加自定义命令，在构建后拷贝文件
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${TargetConfig}"
    "${OutputDir}"
    COMMENT "Copying config.ini to output directory"
)
