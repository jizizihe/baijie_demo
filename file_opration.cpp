#include "file_opration.h"
#include "ui_file_opration.h"
#include <QMessageBox>
#include <QDebug>
#include <QScreen>

static QString path_last;
static QString path_pri;
static QString file_path;
static int s_width;
static int s_height;
static int screen_flag;
static QScreen *screen;

File_opration::File_opration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::File_opration)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    s_width = screen->size().width();			//屏幕宽
    s_height = screen->size().height();
    if(s_width < s_height)
    {
        screen_flag = 1;
        this->setMinimumSize(s_height*2/3,s_width*3/4);
        this->setMaximumSize(s_height*2/3,s_width*3/4);
    }
    else
    {
        this->setMinimumSize(s_width*2/3,s_height*2/3);
        this->setMaximumSize(s_width*2/3,s_height*2/3);
    }
    connect(&pro_path, SIGNAL(readyReadStandardOutput()), this, SLOT(readBashStandardOutputInfo()));
    this->setWindowModality(Qt::ApplicationModal);

    pro_path.start("bash");
}

File_opration::~File_opration()
{
    delete ui;
    pro_path.close();
}

void File_opration::readBashStandardOutputInfo()
{
    QString out = pro_path.readAllStandardOutput();
    QStringList list = out.split("\n");
    list.removeAll("");

    ui->treeWidget->clear();
    if(filepath_flag == 1)
    {
        if(list.count() < 3)
        {

        }
        else
        {
            for(int i = 2;i<list.size();i++)
            {
                QTreeWidgetItem *g = new QTreeWidgetItem(ui->treeWidget);
                g->setText(0,list[i]);
            }
        }
    }
    else
    {
        for(int i = 2;i<list.size();i++)
        {
            QTreeWidgetItem *g = new QTreeWidgetItem(ui->treeWidget);
            g->setText(0,list[i]);
        }
    }
}

void File_opration::on_btn_back_clicked()
{
    file_path = ui->pathname_2->text();
    if(QString::compare(file_path,QString("/"),Qt::CaseSensitive))
    {
      file_path.chop(1);
      file_path = file_path.mid(0,file_path.lastIndexOf("/")+1);

       QString s = "cd "+ file_path + " \n";
      pro_path.write(s.toUtf8());
      if(filepath_flag == 0)
      {
       pro_path.write("ls -ap \n");
      }
      else
      {
          pro_path.write("ls -ap| grep / \n");
      }
      ui->pathname_2->setText(file_path);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("The current path is the most forward path!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screen_flag == 1)
        mesg.move(s_width*2/3,s_height/3);
        else
        mesg.move(s_width/3,s_height/3);
        mesg.exec();
    }
}

void File_opration::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    path_last = QString(item->text(column));
    path_pri = ui->pathname_2->text();
    QString path;
    if(filepath_flag == 0)
    {
        if(path_last.contains("/",Qt::CaseSensitive))
        {
            path = path_pri + path_last;
            QString s = "cd "+ path + " \n";
            pro_path.write(s.toUtf8());
            pro_path.write("ls -ap \n");
            ui->pathname_2->setText(path);
        }
        else
        {

        }
    }
    else
    {
          path = path_pri + path_last;
          QString s = "cd "+ path + " \n";
          pro_path.write(s.toUtf8());
          pro_path.write("ls -ap| grep / \n");
          ui->pathname_2->setText(path);
    }

}

void File_opration::showEvent(QShowEvent *event)
{
    this->move(s_width/2-this->width()/2,s_height/2-this->height()/2);
   if(filepath_flag == 0)
   {
       ui->pathname_2->setText("/");
       QString s = "cd / \n";
       pro_path.write(s.toUtf8());
       pro_path.write("ls -ap \n");
   }
   else
   {
       ui->pathname_2->setText("/");;
       QString s = "cd / \n";
       pro_path.write(s.toUtf8());
       pro_path.write("ls -ap| grep / \n");
   }
   QWidget::showEvent(event);

}
void File_opration::on_btn_cancel_clicked()
{
    emit file_hide();
}

void File_opration::on_pushButton_clicked()
{
    if(filepath_flag == 1)
    {
        QString str = ui->pathname_2->text();
        emit file_hide();
        emit file_rev2(str);

    }
   else
    {
        if(path_last.contains("/",Qt::CaseSensitive)||(path_last == "/"))
        {
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("Please choose file!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screen_flag == 1)
            mesg.move(s_width*2/3,s_height/3);
            else
            mesg.move(s_width/3,s_height/3);
            mesg.exec();
        }
        else
        {
         emit file_hide();
         emit file_rev1(path_pri,path_last);        
        }
    }
}

void File_opration::language_reload()
{
    ui->retranslateUi(this);
}

void File_opration::file_font()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = s_width / realX * 2.54;
    qreal realHeight = s_height / realY *2.54;
    QFont font;
    if(screen_flag)
    {
        if(realHeight < 15)
        {
            font.setPointSize(10);
        }
        else if (realHeight < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }

    }
    else
    {
        if(realWidth < 15)
        {
            font.setPointSize(10);
        }
        else if (realWidth < 17)
        {
            font.setPointSize(12);
        }
        else
        {
            font.setPointSize(14);
        }
    }
    ui->btn_back->setFont(font);
    ui->btn_cancel->setFont(font);
    ui->pushButton->setFont(font);
    ui->label->setFont(font);
    ui->pathname_2->setFont(font);
    ui->treeWidget->setFont(font);
}
