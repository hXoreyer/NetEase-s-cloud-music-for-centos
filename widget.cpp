#include "widget.h"
#include <QDebug>
#include <QMouseEvent>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    playorpause = true;
    timeSliderPre = false;
    //窗口大小
    setWindowFlags(Qt::FramelessWindowHint);
    screenRect = getScreenSize();
    setFixedSize(screenRect.width()*2 / 3,screenRect.height()*2 / 3);
    //白色背景
    QPalette pal;
    pal.setColor(QPalette::Background,Qt::white);
    setPalette(pal);
    //居中显示
    move(screenRect.width()* 1 / 6,screenRect.height()* 1 / 6);


    player = new QMediaPlayer;
    connect(player,&QMediaPlayer::stateChanged,this,[&](QMediaPlayer::State sta){
        if(sta == QMediaPlayer::PausedState)
        {
            playorpause = false;
            playSong->setStyleSheet("QPushButton{border-image:url(:/images/play.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                                    "QPushButton:hover{border-image:url(:/images/playP.svg);}");
        }else if(sta == QMediaPlayer::PlayingState)
        {
            playorpause = true;
            playSong->setStyleSheet("QPushButton{border-image:url(:/images/pause.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                                    "QPushButton:hover{border-image:url(:/images/pauseP.svg);}");
        }else if(sta == QMediaPlayer::StoppedState)
        {
            playorpause = false;
            playSong->setStyleSheet("QPushButton{border-image:url(:/images/play.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                                    "QPushButton:hover{border-image:url(:/images/playP.svg);}");
        }
    });
    //player->setMedia(QUrl::fromUserInput("https://music.163.com/song/media/outer/url?id=1316563427.mp3"));
    //player->setMedia(QUrl::fromLocalFile("/home/hxoreyer/QtProjects/build-Music-Desktop_Qt_5_9_5_GCC_64bit-Debug/bbibbi.uc"));
    player->setVolume(100);

    //布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(getTittleLayout());
    layout->addLayout(getMidLayout());
    QLabel* line = new QLabel;
    line->setStyleSheet("border:1px solid #DFDFDF");
    line->setFixedHeight(1);
    layout->addWidget(line);
    layout->addLayout(getbottomLayout());
    //QHBoxLayout *bt = new QHBoxLayout;
    //bt->addWidget(play);
    //bt->addWidget(pause);
    //layout->addLayout(bt);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    getListToPlaylist();
    player->setPlaylist(mediaPlayList);

    connect(player,&QMediaPlayer::durationChanged,this,[&](qint64 value){
        timeSlider->setMaximum(value);
    });

    connect(player,&QMediaPlayer::positionChanged,this,[&](qint64 value){
        timeSlider->setValue(value);
        showTime->setText(formatTime(player->duration()-player->position()));
    });


}

Widget::~Widget()
{
}

QLayout* Widget::getTittleLayout()
{

    //icon and tittle

    QLabel* icon= new QLabel;
    QImage img(":/images/format.ico");
    QPixmap iconpix = QPixmap::fromImage(img);
    icon->setPixmap(iconpix.scaled(30,30));

    QLabel* tittle = new QLabel("网易云音乐");
    QFont font("Arial",16,75);
    /*
    font.setPointSize(14);
    font.setFamily("Microsoft YaHei");
    font.setBold(true);
    */
    tittle->setFont(font);
    QPalette fontpal;
    fontpal.setColor(QPalette::WindowText, Qt::white);
    tittle->setPalette(fontpal);

    QHBoxLayout* ct = new QHBoxLayout;
    ct->addWidget(icon);
    ct->addWidget(tittle);
    ct->setSpacing(10);
    ct->setMargin(10);

    //search

    searchEdt = new QLineEdit;
    searchEdt->setStyleSheet("height:30px;border:0px solid;border-radius:15px;background-color: rgba(0, 0, 0, 15%);padding-left:10px;color:white;");
    searchEdt->setPlaceholderText("搜索音乐");
    searchAct = new QAction;
    searchAct->setIcon(QIcon(QPixmap(":/images/search.svg")));
    searchEdt->addAction(searchAct,QLineEdit::TrailingPosition);
    searchAct->setToolTip("搜索");

    //min btn and close btn

    minBtn = new QPushButton;
    QPixmap minPix(":/images/min.svg");
    minBtn->setIcon(minPix);
    minBtn->setStyleSheet("QPushButton{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;color:#D6D6D6}"
                          "QPushButton:hover{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;"
                          "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))}"
                          "QPushButton:pressed{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;"
                          "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(100,100,100,75))}");
    connect(minBtn,&QPushButton::clicked,this,[=](){
       this->showMinimized();
    });


    closeBtn = new QPushButton;
    QPixmap closePix(":/images/close.svg");
    closeBtn->setIcon(closePix);
    closeBtn->setStyleSheet("QPushButton{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;color:#D6D6D6}"
                          "QPushButton:hover{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;"
                          "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))}"
                          "QPushButton:pressed{border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;width:40px;height:40px;"
                          "background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(100,100,100,75))}");
    connect(closeBtn,&QPushButton::clicked,this,[=](){
       close();
    });

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(ct);
    layout->addStretch();
    layout->addWidget(searchEdt);
    layout->addStretch();
    layout->addWidget(minBtn);
    layout->addWidget(closeBtn);

    QWidget *w = new QWidget;
    w->setLayout(layout);
    w->setStyleSheet("background-color: #AF0000");
    QHBoxLayout *ret = new QHBoxLayout;
    ret->addWidget(w);
    return ret;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(event->pos().ry()< 50 && event->pos().rx() < screenRect.width() * 2 / 3 - 100)
        {
            offset.setX(event->globalPos().rx()-this->pos().rx());
            offset.setY(event->globalPos().ry()-this->pos().ry());
            ismove = true;
        }
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(event->pos().ry()< 50 && event->pos().rx() < screenRect.width() * 2 / 3 - 100)
        {
            if(ismove)
            {
                move(event->globalX()-this->offset.rx(),event->globalY()-this->offset.ry());
                list->move(this->pos().rx() + this->width() / 2,this->pos().ry() + this->height() * 1 / 3 - 80);
            }
        }
    }
}

void Widget::doNotMove()
{
    ismove = false;
}

QLayout* Widget::getMidLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    QLayout* left = getMidLeftLayout();
    QLayout* right = getMidRightLayout();
    layout->addLayout(left);
    layout->addLayout(right);
    layout->setStretchFactor(left,1);
    layout->setStretchFactor(right,6);
    layout->setMargin(0);
    layout->setSpacing(0);

    connect(myMusic,&QPushButton::clicked,this,[=](){
        midStack->setCurrentIndex(0);
    });
    connect(localMusic,&QPushButton::clicked,this,[=](){
        midStack->setCurrentIndex(1);
    });
    connect(downloadMusic,&QPushButton::clicked,this,[=](){
        midStack->setCurrentIndex(2);
    });
    return layout;
}

QLayout* Widget::getMidLeftLayout()
{
    QLabel* my = new QLabel("      我的音乐");
    my->setStyleSheet("color:#494949");
    my->setFont(QFont("Microsoft YaHei",11));
    myMusic = new QPushButton("   我的音乐");
    myMusic->setStyleSheet("QPushButton{border:0px solid;background-color:#F5F5F5;color:#494949;padding-right:10px}"
                           "QPushButton:hover{color:#202020;}"
                           "QPushButton:focus{background-color:#dfdfdf}");
    myMusic->setFocus();
    myMusic->setFixedHeight(36);
    myMusic->setFont(QFont("Microsoft YaHei",13));
    myMusic->setCursor(QCursor(Qt::PointingHandCursor));
    myMusic->setIcon(QIcon(QPixmap(":/images/myMusic.svg")));
    myMusic->setIconSize(QSize(20,20));

    localMusic = new QPushButton("   本地音乐");
    localMusic->setStyleSheet("QPushButton{border:0px solid;background-color:#F5F5F5;color:#494949;padding-right:10px}"
                              "QPushButton:hover{color:#202020}"
                              "QPushButton:focus{background-color:#dfdfdf}");
    localMusic->setFont(QFont("Microsoft YaHei",13));
    localMusic->setCursor(QCursor(Qt::PointingHandCursor));
    localMusic->setIcon(QIcon(QPixmap(":/images/localMusic.svg")));
    localMusic->setIconSize(QSize(20,20));
    localMusic->setFixedHeight(36);

    downloadMusic = new QPushButton("   下载管理");
    downloadMusic->setStyleSheet("QPushButton{border:0px solid;background-color:#F5F5F5;color:#494949;padding-right:10px}"
                                 "QPushButton:hover{color:#202020}"
                                 "QPushButton:focus{background-color:#dfdfdf}");
    downloadMusic->setFont(QFont("Microsoft YaHei",13));
    downloadMusic->setCursor(QCursor(Qt::PointingHandCursor));
    downloadMusic->setIcon(QIcon(QPixmap(":/images/downloadMusic.svg")));
    downloadMusic->setIconSize(QSize(20,20));
    downloadMusic->setFixedHeight(36);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addSpacing(10);
    layout->addWidget(my);
    layout->addSpacing(20);
    layout->addWidget(myMusic);
    layout->addWidget(localMusic);
    layout->addWidget(downloadMusic);
    layout->addStretch();
    layout->setSpacing(0);
    layout->setMargin(0);

    QWidget* ww = new QWidget;
    ww->setLayout(layout);
    ww->setStyleSheet("background-color: #F5F5F5");
    QVBoxLayout* ret = new QVBoxLayout;
    ret->addWidget(ww);
    ret->setMargin(0);
    ret->setSpacing(0);
    return ret;
}

QLayout* Widget::getMidRightLayout()
{
    QLabel* l1 = new QLabel("                               my musics aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiiijjjjjkkkkklllllmmmmmnnnnnooooopppppqqqqqrrrrrsssssttttt");
    QLabel* l2 = new QLabel("                               local music");
    QLabel* l3 = new QLabel("                               download music");

    myMusicLayout = new QVBoxLayout;
    myMusicLayout->addWidget(l1);
    QWidget* w1 = new QWidget;
    w1->setLayout(myMusicLayout);

    localMusicLayout = new QVBoxLayout;
    localMusicLayout->addWidget(l2);
    QWidget* w2 = new QWidget;
    w2->setLayout(localMusicLayout);

    downloadMusicLayout = new QVBoxLayout;
    downloadMusicLayout->addWidget(l3);
    QWidget* w3 = new QWidget;
    w3->setLayout(downloadMusicLayout);

    midStack = new QStackedWidget;
    midStack->addWidget(w1);
    midStack->addWidget(w2);
    midStack->addWidget(w3);

    midStack->setCurrentIndex(0);



    list = new PlayList();
    list->setParentWidth(this->width());
    list->setFixedWidth(this->width() / 2);
    list->setFixedHeight(this->height() * 2 / 3);
    list->move(this->pos().rx() + this->width() / 2,this->pos().ry() + this->height() * 1 / 3 - 80);
    list->setStyleSheet("QListWidget::item:hover{background:#E8E8E8;border:0px}"
                        "QListWidget::item:selected{background:#E3E3E3;border:0px;outline:none}"
                        "QListWidget{border:0}");
    list->setCursor(QCursor(Qt::PointingHandCursor));
    list->setFocusPolicy(Qt::NoFocus);

    connect(list->listWidget,&QListWidget::itemDoubleClicked,this,[&](QListWidgetItem *item){
        int index = list->listWidget->row(item);

        /*
        QWidget *w = list->listWidget->itemWidget(item);
        QList<QLabel*> labelList = w->findChildren<QLabel*>();

        QString name = "/home/" + getUserName();
        name = name + "/musicCache/";
        name = name + labelList[0]->text() + ".mp3";

        player->setMedia(QUrl::fromLocalFile(name));
        */
        mediaPlayList->setCurrentIndex(index);
        player->play();
        playorpause = false;


    });


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(midStack);

    layout->setMargin(0);
    layout->setSpacing(0);
    return layout;
}

QLayout* Widget::getbottomLayout()
{
    QWidget *w = new QWidget;
    w->setStyleSheet("background-color:#F5F5F5");


    lastSong = new QPushButton;
    lastSong->setStyleSheet("QPushButton{border-image:url(:/images/lastSong.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                            "QPushButton:hover{border-image:url(:/images/lastSongP.svg);}");
    lastSong->setFixedWidth(60);
    lastSong->setCursor(QCursor(Qt::PointingHandCursor));

    playSong = new QPushButton;
    playSong->setStyleSheet("QPushButton{border-image:url(:/images/play.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                            "QPushButton:hover{border-image:url(:/images/playP.svg);}");
    playSong->setFixedWidth(60);
    playSong->setCursor(QCursor(Qt::PointingHandCursor));

    nextSong = new QPushButton;
    nextSong->setStyleSheet("QPushButton{border-image:url(:/images/nextSong.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                            "QPushButton:hover{border-image:url(:/images/nextSongP.svg);}");
    nextSong->setFixedWidth(60);
    nextSong->setCursor(QCursor(Qt::PointingHandCursor));


    QHBoxLayout* musicControl = new QHBoxLayout;
    musicControl->addWidget(lastSong);
    musicControl->addWidget(playSong);
    musicControl->addWidget(nextSong);

    connect(lastSong,&QPushButton::clicked,this,[&](){
        mediaPlayList->previous();
    });

    connect(nextSong,&QPushButton::clicked,this,[&](){
        mediaPlayList->next();
    });

    connect(playSong,&QPushButton::clicked,this,[&](){
        if(playorpause)
        {
            playSong->setStyleSheet("QPushButton{border-image:url(:/images/pause.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                                    "QPushButton:hover{border-image:url(:/images/pauseP.svg);}");
            player->play();
        }else
        {
            playSong->setStyleSheet("QPushButton{border-image:url(:/images/play.svg);border-left:0px;border-right:0px;border-top:0px;border-bottom:0px;height:60px;}"
                                    "QPushButton:hover{border-image:url(:/images/playP.svg);}");
            player->pause();
        }
        playorpause = !playorpause;
    });


    showTime = new QLabel("00:00");
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setStyleSheet("QSlider:handle{background-color:#af0000;border: 4px solid #ffffff; border-radius:7px}"
                              "QSlider:sub-page{background-color:#af0000;border-radius:3px}"
                              "QSlider:add-page{background-color:#e5e5e5;border-radius:3px}");
    timeSlider->setFixedWidth(screenRect.width() / 3 );

    connect(timeSlider,&QSlider::sliderPressed,this,[&](){
        timeSliderPre = true;
    });
    connect(timeSlider,&QSlider::sliderReleased,this,[&](){
        timeSliderPre = false;
    });
    connect(timeSlider,&QSlider::valueChanged,this,[&](qint64 value){
        if(timeSliderPre)
        {
            player->setPosition(value);
        }
    });
    QHBoxLayout* timeControl = new QHBoxLayout;
    timeControl->addWidget(showTime);
    timeControl->addWidget(timeSlider);



    soundpause = true;
    soundBtn = new QPushButton;
    soundBtn->setStyleSheet("border-image:url(:/images/sound.svg)");
    soundBtn->setFixedSize(20,20);
    soundBtn->setCursor(QCursor(Qt::PointingHandCursor));

    connect(soundBtn,&QPushButton::clicked,this,[&](){
        if(soundpause)
        {
            soundBtn->setStyleSheet("border-image:url(:/images/stopsound.svg)");
            player->setVolume(0);
            soundSlider->setValue(0);
        }else
        {
            soundBtn->setStyleSheet("border-image:url(:/images/sound.svg)");
            player->setVolume(100);
            soundSlider->setValue(100);
        }
        soundpause = !soundpause;
    });

    soundSlider = new QSlider(Qt::Horizontal);
    soundSlider->setStyleSheet("QSlider:handle{background-color:#af0000;border: 4px solid #ffffff; border-radius:7px}"
                              "QSlider:sub-page{background-color:#af0000;border-radius:3px}"
                              "QSlider:add-page{background-color:#e5e5e5;border-radius:3px}");
    soundSlider->setFixedWidth(screenRect.width()* 1 / 20);
    soundSlider->setValue(100);
    connect(soundSlider,&QSlider::valueChanged,this,[&](int value){
        player->setVolume(value);
        if(value == 0)
        {
            soundBtn->setStyleSheet("border-image:url(:/images/stopsound.svg)");
        }else
        {
            soundBtn->setStyleSheet("border-image:url(:/images/sound.svg)");
        }
    });

    QHBoxLayout* soundControl = new QHBoxLayout;
    soundControl->addWidget(soundBtn);
    soundControl->addWidget(soundSlider);


    playList = new QPushButton;
    playList->setStyleSheet("QPushButton{border-image:url(:/images/playList.svg)}"
                            "QPushButton:hover{border-image:url(:/images/playListP.svg)}");
    playList->setFixedSize(25,25);
    playList->setCursor(QCursor(Qt::PointingHandCursor));
    connect(playList,&QPushButton::clicked,this,[&](){
        if(list->isHidden())
        {
            list->show();
        }else
        {
            list->hide();
        }
    });

    lyrics = new QPushButton;
    lyrics->setStyleSheet("QPushButton{border-image:url(:/images/lyric.svg)}"
                          "QPushButton:hover{border-image:url(:/images/lyricP.svg)}");
    lyrics->setFixedSize(25,25);
    lyrics->setCursor(QCursor(Qt::PointingHandCursor));


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(musicControl);
    layout->addLayout(timeControl);
    layout->addLayout(soundControl);
    layout->addStretch();
    layout->addWidget(playList);
    layout->addWidget(lyrics);
    layout->addStretch();
    layout->setSpacing(20);

    w->setLayout(layout);
    QHBoxLayout* ret = new QHBoxLayout;
    ret->addWidget(w);

    return ret;
}

void Widget::closeEvent(QCloseEvent *event)
{
    list->close();
}

void Widget::getListToPlaylist()
{
    mediaPlayList = new QMediaPlaylist;
    mediaPlayList->setCurrentIndex(1);
    mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    int size = list->listWidget->count();


    for(int i = 0; i < size; ++i)
    {
        QWidget* w = list->listWidget->itemWidget(list->listWidget->item(i));
        QList<QLabel*> labelList = w->findChildren<QLabel*>();

        QString name = "/home/" + getUserName();
        name = name + "/musicCache/";
        name = name + labelList[0]->text() + ".mp3";
        mediaPlayList->addMedia(QMediaContent(QUrl::fromLocalFile(name)));
    }


    connect(mediaPlayList,&QMediaPlaylist::currentMediaChanged,this,[&](QMediaContent content){
        QString name = content.canonicalUrl().fileName();
        name.remove(name.size() - 4, 4);

        for(int i = 0; i < list->listWidget->count(); ++i)
        {
            QWidget* w = list->listWidget->itemWidget(list->listWidget->item(i));
            QList<QLabel*> findList = w->findChildren<QLabel*>();
            if(!findList[0]->text().compare(name))
            {
                list->listWidget->setCurrentRow(i);
                break;
            }

        }

    });


}

void Widget::hideEvent(QHideEvent *event)
{
    list->hide();
}
