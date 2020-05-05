#include "function.h"
#include <QDebug>
QRect getScreenSize()
{
    QDesktopWidget *pDesktopWidget = QApplication::desktop();
    //QRect deskRect = QApplication::desktop()->availableGeometry();
    QRect screenRect = QApplication::desktop()->screenGeometry();
    /*
    int nScreen = QApplication::desktop()->screenCount();
    qDebug()<<"deskRect: "<<deskRect;
    qDebug()<<"screenRect: "<<screenRect;
    */
    return screenRect;
}

QString formatTime(int ms)
{
    int ss = 1000;
    int mi = ss * 60;
    int hh = mi * 60;
    int dd = hh * 24;

    long day = ms / dd;
    long hour = (ms - day * dd) / hh;
    long minute = (ms - day * dd - hour * hh) / mi;
    long second = (ms - day * dd - hour * hh - minute * mi) / ss;
    long milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

    QString hou = QString::number(hour,10);
    QString min = QString::number(minute,10);
    QString sec = QString::number(second,10);
    QString msec = QString::number(milliSecond,10);

    //qDebug() << "minute:" << min << "second" << sec << "ms" << msec <<endl;

    return QString("%1").arg(min.toInt(),2,10,QLatin1Char('0')) + ":" + QString("%1").arg(sec.toInt(),2,10,QLatin1Char('0')) ;
}

QVector<QJsonObject> readSongList()
{
    QVector<QJsonObject> ret;
    QFile file("./sets/listsongs.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString value = file.readAll();
    file.close();

    QJsonParseError paraseError;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(),&paraseError);
    if(!(paraseError.error == QJsonParseError::NoError))
    {
        qDebug()<<"json error";
        return ret;
    }

    QJsonObject jsonObject = document.object();
    QJsonValue arryValue = jsonObject.value(QStringLiteral("songs"));

    if(arryValue.isArray())
    {
        QJsonArray arry = arryValue.toArray();

        for(int i = 0; i < arry.size(); ++i)
        {
            QJsonValue info = arry.at(i);
            QJsonObject infoObj = info.toObject();

            ret.push_back(infoObj);

        }
    }

    return ret;

}

QString getUserName()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userName = userPath.section("/", -1, -1);
    return userName;
}
