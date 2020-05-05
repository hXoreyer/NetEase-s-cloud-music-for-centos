#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class listItem : public QWidget
{
    Q_OBJECT
public:
    //explicit listItem(QWidget *parent = nullptr);
    listItem(QString sName,QString sSinger,QString sTime);

public:
    QLabel*         songName;
    QLabel*         songSinger;
    QLabel*         songTime;

signals:

public slots:
};

#endif // LISTITEM_H
