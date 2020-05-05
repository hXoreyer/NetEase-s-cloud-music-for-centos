#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPalette>
#include <QPoint>
#include <QLabel>
#include <QRgb>
#include <QVector>
#include <QLineEdit>
#include <QAction>
#include <QStackedWidget>
#include <QSlider>
#include <QCloseEvent>
#include <QMediaPlaylist>
#include "playlist.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
public:
    QMediaPlayer*       player;
    QPushButton*        minBtn;
    QPushButton*        closeBtn;
    QLineEdit*          searchEdt;
    QAction*            searchAct;
    QPoint              offset;
    bool                ismove;
    QRect               screenRect;

    //stackwidgets
    QStackedWidget*     midStack;
    QPushButton*        myMusic;
    QPushButton*        localMusic;
    QPushButton*        downloadMusic;

    QVBoxLayout*        myMusicLayout;
    QVBoxLayout*        localMusicLayout;
    QVBoxLayout*        downloadMusicLayout;

    //bottom
    QPushButton*        lastSong;
    QPushButton*        nextSong;
    QPushButton*        playSong;
    QPushButton*        pauseSong;
    QLabel*             showTime;
    QSlider*            timeSlider;
    QPushButton*        soundBtn;
    QSlider*            soundSlider;
    QPushButton*        playList;
    QPushButton*        lyrics;
    PlayList*           list;
    QMediaPlaylist*     mediaPlayList;
    bool                timeSliderPre;
    bool                playorpause;
    bool                soundpause;
public:
    QLayout*            getTittleLayout();
    QLayout*            getMidLayout();
    QLayout*            getMidLeftLayout();
    QLayout*            getMidRightLayout();
    QLayout*            getbottomLayout();

    void                getListToPlaylist();

public slots:
    void                doNotMove();

protected:
    //重写mousePressEvent和mouseMoveEvent来允许鼠标移动程序
    void                mousePressEvent(QMouseEvent *event);
    void                mouseMoveEvent(QMouseEvent *event);
    void                closeEvent(QCloseEvent* event);
    void                hideEvent(QHideEvent*   event);
};

#endif // WIDGET_H
