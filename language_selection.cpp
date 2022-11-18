#include "language_selection.h"
#include "ui_language_selection.h"
#include <QScreen>
#include <QDebug>
#include <stdlib.h>

static int g_screenWidth;
static int g_screenHeight;
static QGraphicsView *view;

language_selection::language_selection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::language_selection)
{
    ui->setupUi(this);
    ui->rtn_English->setChecked(true);
    setLanguageFont();
}

language_selection::~language_selection()
{
    delete ui;
}

void language_selection::setLanguageFont()
{
    QScreen *screen = qApp->primaryScreen();
    g_screenWidth = screen->size().width();
    g_screenHeight = screen->size().height();
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = g_screenWidth / realX * 2.54;
    qreal realHeight = g_screenHeight / realY *2.54;
    QFont font;
    if(g_screenWidth<g_screenHeight)
    {
        if(realHeight < 15)
        {
            font.setPointSize(12);
            ui->lbl_tips->setFont(font);
            font.setPointSize(13);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(15);
            ui->lbl_tips->setFont(font);
            font.setPointSize(16);
        }
        else
        {
            font.setPointSize(18);
            ui->lbl_tips->setFont(font);
            font.setPointSize(19);
        }
    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(12);
            ui->lbl_tips->setFont(font);
            font.setPointSize(13);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(15);
            ui->lbl_tips->setFont(font);
            font.setPointSize(16);
        }
        else
        {
            font.setPointSize(18);
            ui->lbl_tips->setFont(font);
            font.setPointSize(19);
        }
    }
    ui->rtn_Chinese->setFont(font);
    ui->rtn_English->setFont(font);
}

void language_selection::on_btn_ok_clicked()
{
    QStringList list = g_database.tableShow("language");
    if(list.isEmpty())
    {
       g_database.insertTableOne("language","English");
    }
    if(ui->rtn_Chinese->isChecked())
    {
        g_database.updateTableOne("language","Chinese");
    }

    this->hide();
    if(g_screenWidth < g_screenHeight)
    {
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *w = scene->addWidget(&g_desktopWg);
        w->setRotation(90);                                           //Rotate 90 degrees,keep the landscape
        view = new QGraphicsView(scene);
        view->setWindowFlags(Qt::FramelessWindowHint);                //Set frameless
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->resize(g_screenWidth,g_screenHeight);
        g_desktopWg.resize(g_screenHeight,g_screenWidth);
        g_desktopWg.show();
        view->show();
    }
    else
    {
        g_desktopWg.resize(g_screenWidth,g_screenHeight);
        g_desktopWg.show();
    }
}

void language_selection::desktopShow()
{
    QStringList list = g_database.tableShow("language");
    if(list.at(0) == "Chinese")
    {
        ui->rtn_Chinese->setCheckable(true);
    }
    this->hide();
    if(g_screenWidth < g_screenHeight)
    {
        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *w = scene->addWidget(&g_desktopWg);
        w->setRotation(90);                                           //Rotate 90 degrees,keep the landscape
        view = new QGraphicsView(scene);
        view->setWindowFlags(Qt::FramelessWindowHint);                //Set frameless
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->resize(g_screenWidth,g_screenHeight);
        view->setAttribute(Qt::WA_ShowWithoutActivating,true);
        view->setFocusPolicy(Qt::NoFocus);view->setFrameStyle(QFrame::NoFrame);
        g_desktopWg.resize(g_screenHeight,g_screenWidth);
        g_desktopWg.show();
        view->show();
    }
    else
    {

        g_desktopWg.resize(g_screenWidth,g_screenHeight);
        g_desktopWg.show();
    }
}
