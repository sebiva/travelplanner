# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-travelplanner

CONFIG += sailfishapp

SOURCES += src/harbour-travelplanner.cpp \
    src/parser.cpp \
    src/trip.cpp \
    src/leg.cpp \
    src/search.cpp \
    src/timehelper.cpp \
    src/vasttrafik.cpp \
    src/sl.cpp \
    src/skane.cpp

OTHER_FILES += qml/harbour-travelplanner.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-travelplanner.spec \
    rpm/harbour-travelplanner.yaml \
    harbour-travelplanner.desktop \
    qml/pages/MainPage.qml \
    qml/pages/SearchPage.qml \
    search.js \
    qml/pages/database.js \
    qml/database.js \
    qml/pages/SettingsPage.qml

HEADERS += \
    src/parser.h \
    src/trip.h \
    src/leg.h \
    src/search.h \
    src/timehelper.h \
    src/vasttrafik.h \
    src/sl.h \
    src/skane.h

TRANSLATIONS += languages/harbour_travelplanner_en.ts \
                languages/harbour_travelplanner_sv.ts

RESOURCES += \
    languages.qrc

lupdate_only {
    SOURCES = qml/*.qml \
                qml/pages/*.qml
    TRANSLATIONS = languages/harbour_travelplanner_en.ts \
                languages/harbour_travelplanner_sv.ts
}
