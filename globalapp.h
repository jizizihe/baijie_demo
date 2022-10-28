#ifndef GLOBALAPP_H
#define GLOBALAPP_H
#include <QApplication>

extern bool touchFlag;

class globalApp : public QApplication
{
public:

    globalApp(int &argc,char **argv);
//    ~globalApp();
    bool notify(QObject*, QEvent *);
    void setWindowInstance(QWidget*wnd);
private:
    QWidget *widget;
};
#endif // GLOBALAPP_H
