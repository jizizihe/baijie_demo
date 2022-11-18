#ifndef GLOBALAPP_H
#define GLOBALAPP_H
#include <QApplication>

extern bool g_touchFlag;

class globalApp : public QApplication
{
public:
    globalApp(int &argc,char **argv);
    bool notify(QObject*, QEvent *);
    void setWindowInstance(QWidget *wnd);

private:
    QWidget *g_widget;
};
#endif // GLOBALAPP_H
