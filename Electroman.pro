#-------------------------------------------------
#
# Project created by QtCreator 2018-11-13T17:40:22
#
#-------------------------------------------------

QT += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Electroman
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    bullets.cpp \
    enemies.cpp \
    player.cpp \
    game.cpp \
    map.cpp \
    xml.cpp \
    textures.cpp \
    base_classes.cpp \
    items.cpp

HEADERS += \
    bullets.h \
    enemies.h \
    player.h \
    game.h \
    map.h \
    xml.h \
    textures.h \
    base_classes.h \
    items.h

FORMS += \
    game.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    source_files/levels/level1_1.tmx \
    source_files/sounds/boom.ogg \
    source_files/sounds/chip.ogg \
    source_files/sounds/death.ogg \
    source_files/sounds/exit.ogg \
    source_files/sounds/intro.ogg \
    source_files/sounds/nogi1.ogg \
    source_files/sounds/nogi2.ogg \
    source_files/sounds/prizemlenie.ogg \
    source_files/sounds/pushka1.ogg \
    source_files/sounds/shag.ogg \
    source_files/sounds/slowa1.ogg \
    source_files/sounds/slowa2.ogg \
    source_files/sounds/sound_weapon1.ogg \
    source_files/sounds/sound_weapon2.ogg \
    source_files/sounds/sound_weapon3.ogg \
    source_files/sounds/sound_weapon4.ogg \
    source_files/sounds/sound_weapon5.ogg \
    source_files/sounds/teleport.ogg \
    source_files/sounds/zapis_obrezan.ogg \
    source_files/sprites/level/eb_back1.png \
    source_files/sprites/level/eb_chip.png \
    source_files/sprites/level/eb_chip2.png \
    source_files/sprites/level/eb_level_kolonna.png \
    source_files/sprites/level/eb_level_kolonna2.png \
    source_files/sprites/level/eb_level_kub-ten.png \
    source_files/sprites/level/eb_level_kub.png \
    source_files/sprites/level/eb_level_kub2.png \
    source_files/sprites/level/eb_level_kub3.png \
    source_files/sprites/level/eb_level_kub4.png \
    source_files/sprites/level/eb_level_kub_b.png \
    source_files/sprites/level/eb_level_kub_b2.png \
    source_files/sprites/level/eb_level_kub_b3.png \
    source_files/sprites/level/eb_level_kub_c.png \
    source_files/sprites/level/eb_level_kub_c2.png \
    source_files/sprites/level/eb_level_niz_1.png \
    source_files/sprites/level/eb_level_ten2.png \
    source_files/sprites/level/eb_level_ten4.png \
    source_files/sprites/level/eb_level_werh-ten.png \
    source_files/sprites/level/eb_level_werh.png \
    source_files/sprites/level/eb_migalka1.png \
    source_files/sprites/level/eb_most1.png \
    source_files/sprites/level/eb_most1_ten.png \
    source_files/sprites/level/eb_most2.png \
    source_files/sprites/level/eb_most3.png \
    source_files/sprites/level/eb_most3_ten.png \
    source_files/sprites/level/eb_most4.png \
    source_files/sprites/level/eb_most4_ten.png \
    source_files/sprites/level/eb_most5.png \
    source_files/sprites/level/eb_most5_ten.png \
    source_files/sprites/level/eb_yashik.png \
    source_files/sprites/level/ebback2.png \
    source_files/sprites/level/ebback2b.png \
    source_files/sprites/level/ebback3.png \
    source_files/sprites/level/ebback4a.png \
    source_files/sprites/level/ebback4b.png \
    source_files/sprites/level/ebsave.png \
    source_files/sprites/level/ebsave2.png \
    source_files/sprites/level/energy.png \
    source_files/sprites/level/lev00.png \
    source_files/sprites/level/lev01.png \
    source_files/sprites/level/lev02.png \
    source_files/sprites/level/lev03.png \
    source_files/sprites/level/lev10.png \
    source_files/sprites/level/lev11.png \
    source_files/sprites/level/lev12.png \
    source_files/sprites/level/lev13.png \
    source_files/sprites/level/lev20.png \
    source_files/sprites/level/lev21.png \
    source_files/sprites/level/lev22.png \
    source_files/sprites/level/lev23.png \
    source_files/sprites/level/lev30.png \
    source_files/sprites/level/lev31.png \
    source_files/sprites/level/lev32.png \
    source_files/sprites/level/lev33.png \
    source_files/sprites/level/level1.png \
    source_files/sprites/level/monster0.png \
    source_files/sprites/level/nadpis1.png \
    source_files/sprites/level/nogi_pulya1.png \
    source_files/sprites/level/nogi_pulya2.png \
    source_files/sprites/level/portal1.png \
    source_files/sprites/level/portal2.png \
    source_files/sprites/level/portal3.png \
    source_files/sprites/level/portal4.png \
    source_files/sprites/level/portal4b.png \
    source_files/sprites/level/rail.png \
    source_files/sprites/level/reshotka1.png \
    source_files/sprites/level/reshotka2.png \
    source_files/sprites/level/reshotka3.png \
    source_files/sprites/level/sneg1.png \
    source_files/sprites/level/strelka1.png \
    source_files/sprites/level/strelka2.png \
    source_files/sprites/level/strelka3.png \
    source_files/sprites/level/strelka4.png \
    source_files/sprites/level/ten_reshotka.png \
    source_files/sprites/level/tile1.png \
    source_files/sprites/level/tile2.png \
    source_files/sprites/level/tile2_bok1.png \
    source_files/sprites/level/tile2_bok2.png \
    source_files/sprites/level/tile2_niz.png \
    source_files/sprites/level/tile2_ugol1.png \
    source_files/sprites/level/tile2_ugol2.png \
    source_files/sprites/level/tile2_ugol3.png \
    source_files/sprites/level/tile2_ugol4.png \
    source_files/sprites/level/tile2_ugol5.png \
    source_files/sprites/level/tile2_ugol6.png \
    source_files/sprites/level/tile2_ugol7.png \
    source_files/sprites/level/tile2_ugol8.png \
    source_files/sprites/level/tile2_werh.png \
    source_files/sprites/level/truba0a.png \
    source_files/sprites/level/truba0b.png \
    source_files/sprites/level/truba0c.png \
    source_files/sprites/level/truba0d.png \
    source_files/sprites/level/truba0e.png \
    source_files/sprites/level/truba1a.png \
    source_files/sprites/level/truba1b.png \
    source_files/sprites/level/truba1c.png \
    source_files/sprites/level/truba1d.png \
    source_files/sprites/level/truba2a.png \
    source_files/sprites/level/truba2b.png \
    source_files/sprites/level/truba2c.png \
    source_files/sprites/level/uzor1.png \
    source_files/sprites/level/uzor2.png \
    source_files/sprites/level/uzor3.png \
    source_files/sprites/monsters/eb_migalka1a.png \
    source_files/sprites/monsters/eb_migalka1b.png \
    source_files/sprites/monsters/eb_migalka2a.png \
    source_files/sprites/monsters/eb_migalka2b.png \
    source_files/sprites/monsters/eb_pucha1_puli.png \
    source_files/sprites/monsters/eb_pushka1.png \
    source_files/sprites/monsters/eb_pushka2.png \
    source_files/sprites/monsters/eb_pushka3.png \
    source_files/sprites/monsters/eb_pushka4.png \
    source_files/sprites/monsters/monster0.png \
    source_files/sprites/monsters/monster1.png \
    source_files/sprites/monsters/nogi_pulya1.png \
    source_files/sprites/monsters/nogi_pulya2.png \
    source_files/sprites/eb0.png \
    source_files/sprites/eb1.png \
    source_files/sprites/eb10.png \
    source_files/sprites/eb11.png \
    source_files/sprites/eb12.png \
    source_files/sprites/eb13.png \
    source_files/sprites/eb14.png \
    source_files/sprites/eb15.png \
    source_files/sprites/eb16.png \
    source_files/sprites/eb17.png \
    source_files/sprites/eb18.png \
    source_files/sprites/eb19.png \
    source_files/sprites/eb2.png \
    source_files/sprites/eb20.png \
    source_files/sprites/eb21.png \
    source_files/sprites/eb22.png \
    source_files/sprites/eb23.png \
    source_files/sprites/eb24.png \
    source_files/sprites/eb25.png \
    source_files/sprites/eb26.png \
    source_files/sprites/eb27.png \
    source_files/sprites/eb28.png \
    source_files/sprites/eb29.png \
    source_files/sprites/eb3.png \
    source_files/sprites/eb30.png \
    source_files/sprites/eb31.png \
    source_files/sprites/eb32.png \
    source_files/sprites/eb33.png \
    source_files/sprites/eb34.png \
    source_files/sprites/eb4.png \
    source_files/sprites/eb5.png \
    source_files/sprites/eb6.png \
    source_files/sprites/eb7.png \
    source_files/sprites/eb8.png \
    source_files/sprites/eb9.png \
    source_files/sprites/eb_boom.png \
    source_files/sprites/eb_indicator.png \
    source_files/sprites/eb_indicator_green.png \
    source_files/sprites/eb_indicator_red.png \
    source_files/sprites/eb_indicator_yellow.png \
    source_files/sprites/electroman.png \
    source_files/sprites/electroman2.png \
    source_files/sprites/exit.png \
    source_files/sprites/exit2.png \
    source_files/sprites/load1.png \
    source_files/sprites/load2.png \
    source_files/sprites/load3.png \
    source_files/sprites/PLAN1.png \
    source_files/sprites/PLAN2.png \
    source_files/sprites/PLAN3.png \
    source_files/sprites/PLAN4.png \
    source_files/sprites/tile1.png \
    source_files/sprites/tile_black.png \
    source_files/sprites/tile_setka.png \
    source_files/sprites/weapon1.png \
    source_files/sprites/weapon1b.png \
    source_files/sprites/weapon2.png \
    source_files/sprites/weapon3.png \
    source_files/sprites/weapon3r.png \
    source_files/sprites/weapon4.png \
    source_files/sprites/weapon4r.png \
    source_files/sprites/weapon5.png \
    source_files/sprites/weapon5r.png \
    source_files/levels/x1y1.txt

RESOURCES +=
