#include "listitem.h"

listItem::listItem(QString sName, QString sSinger, QString sTime)
{
    songName = new QLabel(this);
    songName->setStyleSheet("padding-left:40px");

    songSinger = new QLabel(this);

    songTime = new QLabel(this);

    songName->setText(sName);
    songSinger->setText(sSinger);
    songTime->setText(sTime);

    QHBoxLayout* l1 = new QHBoxLayout;
    QHBoxLayout* l2 = new QHBoxLayout;

    l1->addWidget(songName);
    l2->addWidget(songSinger);
    l2->addWidget(songTime);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addLayout(l1);
    layout->addLayout(l2);
    layout->setSpacing(10);

    setLayout(layout);


}
