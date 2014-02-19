# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = Travelplanner

CONFIG += sailfishapp

SOURCES += src/Travelplanner.cpp

OTHER_FILES += qml/Travelplanner.qml \
    qml/cover/CoverPage.qml \
    rpm/Travelplanner.spec \
    rpm/Travelplanner.yaml \
    Travelplanner.desktop \
    qml/pages/SettingsPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/DialogTemp.qml \
    search.js \
    qml/search.js \
    qml/pages/SearchTemp.qml \
    qml/pages/database.js \
    qml/database.js

