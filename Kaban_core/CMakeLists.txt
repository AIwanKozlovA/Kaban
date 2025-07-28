cmake_minimum_required(VERSION 3.16)

project(Kaban VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/icon.ico")
if (WIN32)
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -DICON_PATH=${APP_ICON_RESOURCE_WINDOWS}")
endif()



# 2. Поиск GMP (C и C++ интерфейсы)
#find_path(GMP_INCLUDE_DIR gmp.h PATHS /usr/include REQUIRED)
#find_path(GMPXX_INCLUDE_DIR gmpxx.h PATHS /usr/include REQUIRED)
#find_library(GMP_LIB NAMES gmp PATHS /usr/lib/x86_64-linux-gnu REQUIRED)
#find_library(GMPXX_LIB NAMES gmpxx PATHS /usr/lib/x86_64-linux-gnu REQUIRED)





set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# 4. Подключение GMP с C++ интерфейсом
#target_link_libraries(Kaban PRIVATE GMP::GMP)
#target_link_options(Kaban PRIVATE "-lgmpxx -lgmp -lpthread -lm")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Kaban
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Kaban APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Kaban SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Kaban
            ${PROJECT_SOURCES}
            AES_graf.cpp
            sly_funk.cpp
        )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Algoriyms_shifrovans/AES
    )
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core  # Или Qt6::Core
    Qt5::Gui
    Qt5::Widgets
)
# 5. Подключаем GMP (важно: сначала C++ интерфейс, потом C)
#target_link_libraries(Kaban PRIVATE
#    ${GMPXX_LIB}
#    ${GMP_LIB}
#    pthread
#    m
#)

# 6. Добавляем include-директории
#target_include_directories(Kaban PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${GMP_INCLUDE_DIR}
#    ${GMPXX_INCLUDE_DIR}
#)

# 7. Проверка наличия всех компонентов
#if(NOT Qt5_FOUND OR NOT GMP_LIB OR NOT GMPXX_LIB)
#    message(FATAL_ERROR "Не найдены необходимые зависимости!")
#endif()


    endif()
endif()


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Kaban)
endif()
set_target_properties(Kaban PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Kaban
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Kaban)
endif()
