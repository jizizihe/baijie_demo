#include "sys_setting.h"
#include "ui_sys_setting.h"
#include "mainwindow.h"

static int screen_flag = 0;
static int Width;  //屏幕宽
static int Height;

static QGraphicsView *backlight_view;
static QGraphicsView *time_view;
static QGraphicsView *user_view;
static QGraphicsView *board_view;
static QScreen *screen;
static int backlight_flag = 0;
static int time_flag = 0;
static int user_flag = 0;
static int board_flag = 0;
static int show_num;

sys_setting::sys_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sys_setting)
{
    ui->setupUi(this);

    screen = qApp->primaryScreen();
    Width = screen->size().width();
    Height = screen->size().height();

    if(Width < Height)
    {
        screen_flag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    sys_font();
    this->setAttribute(Qt::WA_StyledBackground,true);

    connect(&backlight_w,SIGNAL(Mysignal()),this,SLOT(backlight_back()));
    connect(&timeset_w,SIGNAL(Mysignal()),this,SLOT(time_back()));
    connect(&user_w,SIGNAL(Mysignal()),this,SLOT(user_back()));
    connect(&board_w,SIGNAL(Mysignal()),this,SLOT(board_back()));

//    slidebtn = new slideButton(this);
//    slidebtn->setGeometry(720,300,100,100);
//    slidebtn->initflag = 0;
//    connect(slidebtn,SIGNAL(buttonChange()),this,SLOT(out()));

}

sys_setting::~sys_setting()
{
    delete ui;
}

void sys_setting::on_return_2_clicked()
{
    emit sigmain();
}

void sys_setting::on_backlight_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
        backlight_w.resize(Width,Height);
        backlight_w.show();
    }
    else
    {
        if(backlight_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&backlight_w);
            w->setRotation(90);

            backlight_view = new QGraphicsView(scene);

            backlight_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            backlight_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            backlight_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            backlight_view->resize(Width,Height);
            backlight_w.resize(Height,Width);
            backlight_w.show();
            backlight_view->show();
            backlight_flag++;
        }
        else
        {
            backlight_w.resize(Height,Width);
            backlight_w.show();
            backlight_view->resize(Width,Height);
            backlight_view->show();
        }
    }
}

void sys_setting::on_rtc_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
       timeset_w.resize(Width,Height);
       timeset_w.show();
       timeset_w.activateWindow();timeset_w.setFocus();
    }
    else
    {
        if(time_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&timeset_w);
            w->setRotation(90);

            time_view = new QGraphicsView(scene);
            time_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            time_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            time_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            time_view->resize(Width,Height);
            timeset_w.resize(Height,Width);
            timeset_w.show();
            time_view->show();
            time_view->activateWindow();time_view->setFocus();
            timeset_w.activateWindow();timeset_w.setFocus();
            time_flag++;
        }
        else
        {
            timeset_w.resize(Height,Width);
            timeset_w.show();
            time_view->resize(Width,Height);
            time_view->show();
            time_view->activateWindow();time_view->setFocus();
            timeset_w.activateWindow();timeset_w.setFocus();
        }
    }
}

void sys_setting::on_cn_clicked()
{

    static bool flag = 0;
    if(flag)
    {
        delete transl;
        transl = NULL;
    }
    else
    {
        transl = new QTranslator();
        transl->load(":/chinese.qm");
    }
    flag = !flag;
    emit main_cn(); //发送一个信号,翻译主界面的内容
    emit voice_cn();

    ui->retranslateUi(this);

    timeset_w.language_reload();
    backlight_w.language_reload();

    board_w.language_reload();
    user_w.language_reload();

}

void sys_setting::on_user_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
        user_w.resize(Width,Height);
        user_w.show();
    }
    else
    {
        if(user_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&user_w);
            w->setRotation(90);

            user_view = new QGraphicsView(scene);

            user_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            user_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            user_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            user_view->resize(Width,Height);
            user_w.resize(Height,Width);
            user_w.show();
            user_view->show();
            user_flag++;
        }
        else
        {
            user_w.resize(Height,Width);
            user_w.show();
            user_view->resize(Width,Height);
            user_view->show();
        }
    }
}

void sys_setting::on_about_clicked()
{
    this->hide();
    if(screen_flag == 0)
    {
        board_w.resize(Width,Height);
        board_w.show();
    }
    else
    {
        if(board_flag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&board_w);
            w->setRotation(90);

            board_view = new QGraphicsView(scene);

            board_view->setWindowFlags(Qt::FramelessWindowHint);//无边框
            board_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            board_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            board_view->resize(Width,Height);
            board_w.resize(Height,Width);
            board_w.show();
            board_view->show();
            board_flag++;
        }
        else
        {
            board_w.resize(Height,Width);
            board_w.show();
            board_view->resize(Width,Height);
            board_view->show();
        }
    }
}

void sys_setting::backlight_back()
{
    if(screen_flag == 1)
    {
      backlight_view->hide();
    }
   backlight_w.hide();
   this->show();
}

void sys_setting::time_back()
{
    if(screen_flag == 1)
    {
      time_view->hide();
    }
    timeset_w.hide();    
    this->show();
    this->activateWindow();this->setFocus();
}

void sys_setting::board_back()
{
    if(screen_flag == 1)
    {
       board_view->hide();
    }
    board_w.hide();   
    this->show();
}

void sys_setting::user_back()
{
    if(screen_flag == 1)
    {
          user_view->hide();
    }
    user_w.hide();
    this->show();
}

void sys_setting::sys_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = Width / realX * 2.54;
    qreal realHeight = Height / realY *2.54;
    QFont font;
    if(screen_flag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(12);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(12);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(14);
        }
        else
        {
            font.setPointSize(17);
        }
    }
    ui->label_2->setFont(font);
    ui->label_29->setFont(font);
    ui->label_30->setFont(font);
    ui->label_key_6->setFont(font);
    ui->label_key_8->setFont(font);
    ui->label->setFont(font);
}

void sys_setting::closeEvent(QCloseEvent *event)
{
    if(show_num == 0)
    {
        on_rtc_clicked();
        time_back();
        show_num++;
    }
   QWidget::closeEvent(event);
}
