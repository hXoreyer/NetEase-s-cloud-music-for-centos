#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include <QPushButton>
#include <QPixmap>
#include <QLayout>
#include <QListWidget>
#include "listitem.h"
#include "function.h"

class PlayList : public QWidget
{
    Q_OBJECT
public:
    explicit PlayList(QWidget *parent = nullptr);

public:
    QPushButton*        CloseBtn;
    QListWidget*        listWidget;
    int                 parentWidth;

public:
    void                setParentWidth(int width);
    QHBoxLayout*        getTittle();
    QVBoxLayout*        getBottomLayout();
};

#endif // PLAYLIST_H
