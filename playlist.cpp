#include "playlist.h"
#include <QDebug>
PlayList::PlayList(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(getTittle());
    layout->addLayout(getBottomLayout());
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
}

void PlayList::setParentWidth(int width)
{
    parentWidth = width;
}

QHBoxLayout* PlayList::getTittle()
{
    QLabel *tittle = new QLabel("  播放列表  ");
    tittle->setFont(QFont("Microsoft YaHei",12,85));
    tittle->setStyleSheet("border: 1px solid #ffffff;border-radius:15px;background-color:white; color:#5f5f5f");
    CloseBtn = new QPushButton;
    QPixmap closePix(":/images/closeList.svg");
    CloseBtn->setIcon(closePix);
    CloseBtn->setStyleSheet("QPushButton{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:30px;height:30px}"
                            "QPushButton:hover{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:30px;height:30px;"
                            "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))}"
                            "QPushButton:pressed{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:30px;height:30px;"
                            "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(100,100,100,75))}");
    connect(CloseBtn,&QPushButton::clicked,this,[&](){
        hide();
    });
    CloseBtn->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(tittle);
    layout->addStretch();
    layout->addWidget(CloseBtn);

    QWidget* w = new QWidget;
    w->setLayout(layout);
    w->setStyleSheet("background-color:#E5E5E5");
    QHBoxLayout* ret = new QHBoxLayout;
    ret->setMargin(0);
    ret->setSpacing(0);
    ret->addWidget(w);
    return ret;
}

QVBoxLayout* PlayList::getBottomLayout()
{
    listWidget = new QListWidget;
    QVector<QJsonObject> value = readSongList();

    for(int i = 0; i < value.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setSizeHint(QSize(parentWidth / 2,40));
        listItem *listitem = new listItem(value[i]["songName"].toString(),value[i]["songSinger"].toString(),value[i]["times"].toString());
        listWidget->setItemWidget(item,listitem);
        listWidget->addItem(item);
    }
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(listWidget);
    return layout;
}
