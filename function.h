#ifndef FUNCTION_H
#define FUNCTION_H

#include <QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QFile>
#include <QStandardPaths>


QRect getScreenSize();
QString formatTime(int ms);

QVector<QJsonObject> readSongList();
QString getUserName();

#endif // FUNCTION_H

