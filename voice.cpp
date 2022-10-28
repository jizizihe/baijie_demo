#include "voice.h"
#include "ui_voice.h"
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <iostream>
#include <fstream>
using namespace std;

static QGraphicsView *renameView;
static QGraphicsView *saveFileView;
static QGraphicsView *chooseFileView;
static int screenFlag;
static int screenWidth;
static int screenHeight;
static int renameFirstFlag = 0;
static int saveFileFirstFlag = 0;
static int fileFirstFlag;              //the File_oprationw to display for the first time
static bool recordFlag = false;
static int viewShow;                   // 1:chooseFileView show  2: saveFileView show  3: renameView show
static int playCurrentTime;
static int vioceTimeLength;            //the time length of the file
static int recordTimeLength;           //Current recording time length
static int playFlag = 0;
static int volumeOpenFlag;
static int showFirstFlag;
static QScreen *screen;
static QString saveFilePath;
static QString saveFileNmae;
static QString recordStartTime;
static QTimer *playTimer;
static QTimer *recordTimer;

voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
    screen = qApp->primaryScreen();
    screenWidth = screen->size().width();
    screenHeight = screen->size().height();

    if(screenWidth < screenHeight)
    {
        screenFlag = 1;ui->line->setStyleSheet("background-color: rgb(186, 189, 182);");
    }
    voiceFont();
    volumeOpenFlag = 1;

    ui->stackedWidget->setCurrentIndex(0);
    ui->Slider_volume->setRange(0,30);               //Range of volume
    ui->Slider_volume->setValue(6);
    ui->lbl_timeStart->setText("00:00:00");
    ui->lbl_timeEnd->setText("00:00:00");

    recordTimer = new QTimer(this);
    playTimer = new QTimer(this);
    connect(recordTimer,SIGNAL(timeout()),this,SLOT(show_recording_time()));
    connect(playTimer,SIGNAL(timeout()),this,SLOT(get_playing_time_length()));
    connect(&renameWg,SIGNAL(rename_finish_msg(QString)),this,SLOT(update_file(QString)));
    connect(&renameWg,SIGNAL(rename_back_msg()),this,SLOT(rename_file_widget_hide()));
    connect(&saveFileWg,SIGNAL(save_back_msg()),this,SLOT(save_file_widget_hide()));
    connect(&File_oprationWg,SIGNAL(file_rev_path_msg(QString)),this,SLOT(file_path(QString)));
    connect(&File_oprationWg,SIGNAL(file_hide_msg()),this,SLOT(file_choose_widget_hide()));
    connect(this,SIGNAL(volume_slider_value_change_msg(int)),this,SLOT(volume_slider_change(int)));

    ui->btn_startRecord->setText(tr("start"));
    ui->btn_record->setText("record");
    ui->btn_play->setText("paly");
    ui->combox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    ui->combox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
}

voice::~voice()
{
    delete ui;
}

void voice::refresh(QString dirPath)
{ 
    QDir dir(dirPath);
    QStringList filters;

    filters << "*.wav" << "*.mp3" << "*.mp4";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    QStringList files;
    for(int i = 0; i != list.size(); i++)
    {
        files << list.at(i).fileName();
    }
    files.removeAll("\n");
    ui->combox->clear();
    ui->combox->addItems(files);
    if(!saveFileNmae.isEmpty())
    {
        for(int i = 0; i < ui->combox->count(); i++)
        {
            ui->combox->setCurrentIndex(i);
            if(ui->combox->currentText() == saveFileNmae)
            {
                break;
            }
        }
        saveFileNmae = "";
    }
}

void voice::setVolume(int value)
{
    QProcess *proc = new QProcess();
    int sound = 31 - value;
    proc->start(QString("amixer cset numid=6 %1").arg(sound));
    proc->waitForStarted(-1);
    proc->waitForFinished(-1);
}

void voice::show_recording_time()
{
    if(recordFlag)
    {
        recordTimeLength++;
        QString length = timeToString(recordTimeLength);
        ui->lbl_RecordTimeLength->setText(length);
    }
}

void voice::on_btn_ret_clicked()
{
    QProcess proc;
    proc.start("killall aplay");
    proc.waitForFinished(-1);
    emit voice_back_msg();
    proc.close();
    if(screenFlag == 1)
    {
        if(viewShow == 1)
        {
            File_oprationWg.hide();
            chooseFileView->hide();
        }
        if(viewShow == 2)
        {
            saveFileWg.hide();
            saveFileView->hide();
        }
        if(viewShow == 3)
        {
            renameView->hide();
            renameWg.hide();
        }
    }
    else
    {
        File_oprationWg.hide();renameWg.hide();saveFileWg.hide();
    }
}

void voice::on_btn_record_clicked()
{   
    ui->stackedWidget->setCurrentIndex(1);
    ui->lbl_RecordTimeLength->setText("00:00:00");
    ui->btn_record->setText(tr("record"));
}

QString voice::timeToString(int length)              //time length -> 00:00:00
{
    int s,m,h;
    QString strS,strM,strH;
    if(length < 60)
    {
        strH = QString("00");
        strM = QString("00");
        if(length < 10)
            strS = QString("0%1").arg(length);
        else
            strS = QString("%1").arg(length);
    }
    else if(length < 360)
    {
        strH = QString("00");
        m = length/60;
        s = (length%60);
        if(m < 10)
            strM = QString("0%1").arg(m);
        else
            strM = QString("%1").arg(m);
        if(s < 10)
            strS = QString("0%1").arg(s);
        else
            strS = QString("%1").arg(s);
    }
    else
    {
        h = length/360;
        m = (length%360)/60;
        s = (length%360)%60;
        if(h < 10)
            strH = QString("0%1").arg(h);
        else
            strH = QString("%1").arg(h);
        if(m < 10)
            strM = QString("0%1").arg(m);
        else
            strM = QString("%1").arg(m);
        if(s < 10)
            strS = QString("0%1").arg(s);
        else
            strS = QString("%1").arg(s);
    }

    QString strLength = QString("%1:%2:%3").arg(strH).arg(strM).arg(strS);
    return strLength;
}

void voice::on_btn_play_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->btn_play->setText(tr("play"));
    QStringList list = databaseWg.tableShow("voice");
    if(!list.isEmpty())
    {
        saveFilePath = list.at(0);
        saveFileNmae = list.at(1);
    }
    if(saveFilePath.isEmpty())
    {
        ui->lbl_pathValue->setText("/data/");
    }
    else
    {
        ui->lbl_pathValue->setText(saveFilePath);
    }
    QString path = ui->lbl_pathValue->text();
    refresh(path);
    saveFilePath = "";
}

void voice::languageReload()
{
    ui->retranslateUi(this);
    renameWg.languageReload();
    saveFileWg.languageReload();
    File_oprationWg.languageReload();
}

void voice::get_playing_time_length()
{
    if(playFlag == 1)
    {
        playCurrentTime+=1;
        ui->Slider_voicelength->setValue(playCurrentTime);
        QString currentTime = timeToString(playCurrentTime);
        ui->lbl_timeStart->setText(currentTime);

        if(playCurrentTime >= vioceTimeLength)
        {
            playTimer->stop();
            ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/play.svg);");
            playFlag = 0;
            ui->lbl_timeEnd->setText("00:00:00");
            ui->lbl_timeStart->setText("00:00:00");
        }
    }
}

void voice::on_btn_playVoice_clicked()
{
    if(ui->combox->currentText() == NULL)
    {
        return;
    }

    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if((vioceTimeLength > playCurrentTime)&&(playCurrentTime != 0))
    {
        ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/play.svg);");
        playCurrentTime=0;
        playFlag = 0;
        ui->Slider_voicelength->setValue(playCurrentTime);
        ui->lbl_timeEnd->setText("00:00:00");
        ui->lbl_timeStart->setText("00:00:00");
    }
    else
    {
        playFlag = 1;
        ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/stop.svg);");

        QString path = QString("%1").arg(ui->lbl_pathValue->text());
        QString name = ui->combox->currentText();
        QString str = QString("aplay %1%2").arg(path).arg(name);
        proc->start(str);
        playTimer->start(1000);
        playCurrentTime = 0;
        QString pathName = QString("%1%2").arg(path).arg(name);

        float len = getWavTimeLength(pathName);               //Calculate the wav file time length, returned value: minute.seconds
        QString timeLen = QString("%1").arg(len);
        vioceTimeLength = calculateTimeLength(timeLen);       //Calculate the recording time length
        ui->Slider_voicelength->setRange(0, vioceTimeLength);
    }
}


float voice::getWavTimeLength(QString file)
{
    double len;
    char * fileName = file.toLatin1().data();
    cout << fileName << endl;
    if (fileName != NULL)
    {
        FILE* fp;
        fp = fopen(fileName, "rb");
        if (fp != NULL)
        {
            int i;
            int j;
            fseek(fp, 28, SEEK_SET);
            fread(&i, sizeof(4), 1, fp);
            fseek(fp ,0 ,SEEK_END);
            j = ftell(fp) - 44;

            fclose(fp);
            fp = NULL;

            len = (double)j/(double)i;
            int aa = len * 10;
            if((aa % 10) > 4) aa = (aa / 10) + 1;
            else aa /= 10;
            int integer = aa / 60;
            int decimals = aa % 60;
            aa = (integer * 100) + decimals;
            return (float)aa / 100.0;
        }
        else
        {
            std::cout << "open error!!" << std::endl;
        }
    }
    std::cout << "filename error!!" << std::endl;
    return 0;
}

int voice::calculateTimeLength(QString length)   //Calculate the recording time length
{
    int firstMinute = length.section('.',0,0).toInt();
    int sec = length.section('.',1,1).toInt();
    int hour,minute,len;
    QString strHour,strMinute,strSec;

    if(firstMinute>60)
    {
        hour = firstMinute/60;
        minute = firstMinute%60;
    }
    else
    {
        hour = 0;
        minute = firstMinute;
    }
    len = hour*60*60+minute*60+sec;

    if(hour>=10)
        strHour = QString("%1").arg(hour);
    else
        strHour = QString("0%1").arg(hour);

    if(minute>=10)
        strHour = QString("%1").arg(minute);
    else
        strMinute = QString("0%1").arg(minute);

    if(sec >= 10)
        strSec = QString("%1").arg(sec);
    else
        strSec = QString("0%1").arg(sec);

    QString timeEnd = QString("%1:%2:%3").arg(strHour).arg(strMinute).arg(strSec);
    ui->lbl_timeEnd->setText(timeEnd);
    return len;
}

void voice::record_save(QString filen)   //record save file
{
    saveFileWg.getFileName(filen);
    saveFileWidgetShow();
}

void voice::on_btn_delete_clicked()  //delete
{
    if(!ui->combox->currentText().isEmpty())
    {
        QString str = ui->combox->currentText();
        str = QString(tr("Do you want to delete %1?")).arg(str);
        QMessageBox mesg(QMessageBox::Question,
                         tr("QMessageBox::question()"),
                         tr(str.toUtf8()),
                         0,this);

        QPushButton *yesButton = mesg.addButton(tr("Yes"), QMessageBox::ActionRole);
        QPushButton *noButton = mesg.addButton(tr("No"),QMessageBox::ActionRole);
        if(screenFlag == 1)
            mesg.move(screenWidth*2/3,screenHeight/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();

        if (mesg.clickedButton() == yesButton) {
            QProcess *pro = new QProcess();
            QString name = ui->combox->currentText();
            QString path = QString("%1").arg(ui->lbl_pathValue->text());
            str = QString("rm %1%2").arg(path).arg(name);
            pro->start(str);
            QMessageBox mesg(QMessageBox::Information,
                             tr("QMessageBox::information()"),
                             tr("delete successfully!"),
                             0,this);
            mesg.addButton(tr("OK"),QMessageBox::YesRole);
            if(screenFlag == 1)
                mesg.move(screenWidth*2/3,screenHeight/3);
            else
                mesg.move(screenWidth/3,screenHeight/3);
            mesg.exec();
            refresh(path);
            pro->close();
        }
        else if (mesg.clickedButton() == noButton)
        {
            return;
        }
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No file to delete!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}

void voice::on_btn_rename_clicked()  //rename
{   
    if(!ui->combox->currentText().isEmpty())
    {
        renameFileWidgetShow();
        QString name = ui->combox->currentText();
        QString path = QString("%1").arg(ui->lbl_pathValue->text());
        renameWg.renameChang(name,path);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No file to rename!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        if(screenFlag == 1)
            mesg.move(screenHeight/3,screenWidth*2/3);
        else
            mesg.move(screenWidth/3,screenHeight/3);
        mesg.exec();
    }
}
void voice::update_file(QString file)     //updata combox file
{
    QString path = QString("%1").arg(ui->lbl_pathValue->text());
    refresh(path);
    ui->combox->setCurrentText(file);
}

void voice::on_btn_volume_clicked()
{
    if(volumeOpenFlag == 1)
    {
        setVolume(0);
        volumeOpenFlag = 0;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_d.svg);");
    }
    else
    {
        int value = ui->Slider_volume->value();
        setVolume(value);
        volumeOpenFlag = 1;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_u.svg);");
    }
}

void voice::on_btn_startRecord_clicked()
{ 
    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if(!recordFlag)
    {
        recordStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        QString path = "/data";
        QString strRecordName = QString("%1/%2_record.wav").arg(path).arg(recordStartTime);
        QString str = QString("arecord -r 16000 -c 1 -t wav %1").arg(strRecordName);
        proc->start(str);
        ui->btn_startRecord->setText(tr("stop"));
        ui->btn_startRecord->setIcon(QIcon(":/button_image/voice/stop.png"));
        recordFlag = true;
    }
    else
    {
        ui->btn_startRecord->setText(tr("start"));
        ui->lbl_RecordTimeLength->setText("00:00:00");
        recordTimeLength = 0;
        ui->btn_startRecord->setIcon(QIcon(":/button_image/voice/start.png"));
        fileName = QString("%1_record.wav").arg(recordStartTime);
        recordFlag = false;
        proc->start("killall arecord");
        record_save(fileName);
    }
}


void voice::on_Slider_volume_valueChanged(int value)
{
    emit volume_slider_value_change_msg(value);
}

void voice::saveFileWidgetShow()
{
    if(screenFlag == 0)
    {
        int funcWidth = ui->function->width();
        int stackWidth = ui->stackedWidget->width();
        int moveWidth = funcWidth+(stackWidth/2-screenWidth*31/50/2);
        QPoint p = ui->btn_startRecord->mapToGlobal(QPoint(0, 0));
        int btnStartRecordY = p.y()+ui->btn_startRecord->height();
        int moveHeight = screenHeight-(screenHeight - btnStartRecordY)-screenHeight*2/3;
        saveFileWg.resize(screenWidth*31/50,screenHeight*2/3);
        moveWidth = screenWidth*2/9+((screenWidth-screenWidth*2/9)/2-saveFileWg.width()/2);
        moveHeight = screenHeight/6+((screenHeight-screenHeight/6)/2-saveFileWg.height()/2);
        saveFileWg.move(moveWidth,moveHeight);
        saveFileWg.show();
        saveFileWg.activateWindow();saveFileWg.setFocus();
    }
    else
    {
        viewShow = 2;
        if(saveFileFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&saveFileWg);
            w->setRotation(90);

            saveFileView = new QGraphicsView(scene);
            saveFileView->setWindowFlags(Qt::FramelessWindowHint);
            saveFileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            saveFileView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            saveFileWg.resize(screenHeight*2/3,screenWidth*2/3);
            saveFileView->resize(screenWidth*2/3,screenHeight*2/3);
            saveFileWg.show();
            saveFileView->show();
            saveFileView->activateWindow();saveFileView->setFocus();
            saveFileWg.activateWindow();saveFileWg.setFocus();
            int moveWidth=(screenWidth-screenWidth/6)/2-saveFileWg.height()/2;
            int moveHeight=screenHeight*2/9+(screenHeight*7/9/2-saveFileWg.width()/2);
            saveFileView->move(moveWidth,moveHeight);
            saveFileFirstFlag++;
        }
        else
        {
            saveFileWg.show();
            saveFileView->show();
            saveFileView->activateWindow();saveFileView->setFocus();
            saveFileWg.activateWindow();saveFileWg.setFocus();
            int moveWidth = (screenWidth-screenWidth/6)/2-saveFileWg.height()/2;
            int moveHeight = screenHeight*2/9+(screenHeight*7/9/2-saveFileWg.width()/2);
            saveFileView->move(moveWidth,moveHeight);
        }
    }
}
void voice::renameFileWidgetShow()
{
    if(screenFlag == 0)
    {
        int funcWidth = ui->function->width();
        int stackWidth = ui->stackedWidget->width();
        int moveWidth = funcWidth+(stackWidth/2-screenWidth*31/50/2);
        QPoint p = ui->stackedWidget->mapToGlobal(QPoint(0, 0));
        int moveHeight = p.y();
        renameWg.move(moveWidth,moveHeight);
        renameWg.show();
        renameWg.activateWindow();renameWg.setFocus();
        renameWg.resize(screenWidth*3/5,ui->stackedWidget->height()*3/4);
    }
    else
    {
        if(renameFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&renameWg);
            w->setRotation(90);

            renameView = new QGraphicsView(scene);
            renameView->setWindowFlags(Qt::FramelessWindowHint);
            renameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            renameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            renameWg.resize(screenHeight*2/3,screenWidth*2/3);
            renameView->resize(screenWidth*2/3,screenHeight*2/3);
            renameWg.show();
            renameView->show();
            int moveWidth =(screenWidth-screenWidth/6)/2-renameWg.height()/2;
            int moveHeight=screenHeight-screenHeight*7/9+(screenHeight*7/9/2-renameWg.width()/2);
            renameView->move(moveWidth,moveHeight);
            renameView->activateWindow();renameView->setFocus();
            renameWg.activateWindow();renameWg.setFocus();
            renameFirstFlag++;
        }
        else
        {
            viewShow = 3;
            renameWg.show();
            renameView->show();
            renameView->activateWindow();renameView->setFocus();
            renameWg.activateWindow();renameWg.setFocus();
            int moveWidth=(screenWidth-screenWidth/6)/2-renameWg.height()/2;
            int moveHeight=screenHeight*2/9+(screenHeight*7/9/2-renameWg.width()/2);
            renameView->move(moveWidth,moveHeight);
        }
    }
}

void voice::save_file_widget_hide()
{
    viewShow = 0;
    if(screenFlag == 1)
    {
        saveFileWg.hide();
        saveFileView->hide();
        this->hide();
        this->show();
    }
    else
        saveFileWg.hide();
    this->activateWindow();this->setFocus();
}

void voice::rename_file_widget_hide()
{
    viewShow = 0;
    if(screenFlag == 1)
    {
        renameWg.hide();
        renameView->hide();
        this->hide();
        this->show();
    }
    else
        renameWg.hide();
    this->activateWindow();this->setFocus();
}

void voice::voiceFont()
{
    qreal realX = screen->physicalDotsPerInchX();
    qreal realY = screen->physicalDotsPerInchY();
    qreal realWidth = screenWidth / realX * 2.54;
    qreal realHeight = screenHeight / realY *2.54;
    QFont font;
    if(screenFlag)
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
        ui->btn_chooseFile->setIconSize(QSize(40,40));
        ui->btn_volume->setIconSize(QSize(40,40));
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
        ui->btn_chooseFile->setIconSize(QSize(32,32));
        ui->btn_volume->setIconSize(QSize(32,32));
    }
    ui->btn_play->setFont(font);
    ui->btn_record->setFont(font);
    ui->btn_volume->setFont(font);
    ui->btn_playVoice->setFont(font);
    ui->btn_delete->setFont(font);
    ui->btn_rename->setFont(font);
    ui->lbl_path->setFont(font);
    ui->lbl_timeEnd->setFont(font);
    ui->lbl_timeStart->setFont(font);
    ui->combox->setFont(font);
    ui->btn_startRecord->setFont(font);
    ui->lbl_RecordTimeLength->setFont(font);
    ui->lbl_audio->setFont(font);
    ui->lbl_pathValue->setFont(font);
}

void voice::closeEvent(QCloseEvent *event)
{
    if(showFirstFlag == 0)
    {
        saveFileWidgetShow();
        save_file_widget_hide();
        renameFileWidgetShow();
        rename_file_widget_hide();
        showFirstFlag++;
    }
    recordTimer->stop();
    QWidget::closeEvent(event);
}

void voice::showEvent(QShowEvent *event)
{
    recordTimer->start(1000);
}

void voice::on_btn_chooseFile_clicked()
{
    File_oprationWg.filePathFlag = 1;
    fileChooseWidgetshow();
}

void voice::file_path(QString fpath)
{
    ui->lbl_pathValue->setText(fpath);
    refresh(fpath);
}

void voice::fileChooseWidgetshow()
{
    if(screenFlag == 1)
    {
        viewShow=1;
        if(fileFirstFlag == 0)
        {
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *w = scene->addWidget(&File_oprationWg);
            w->setRotation(90);

            chooseFileView = new QGraphicsView(scene);
            chooseFileView->setWindowFlags(Qt::FramelessWindowHint);
            chooseFileView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            chooseFileView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            chooseFileView->resize(File_oprationWg.height(),File_oprationWg.width());
            File_oprationWg.show();
            chooseFileView->show();
            int moveWidth = screenWidth*5/6-File_oprationWg.height();
            int moveHeight = screenHeight*2/9+(screenHeight*7/9/2-File_oprationWg.width()/2);
            chooseFileView->move(moveWidth,moveHeight);
            fileFirstFlag++;
        }
        else
        {
            File_oprationWg.show();
            chooseFileView->show();
            int moveWidth = screenWidth*5/6-File_oprationWg.height();
            int moveHeight = screenHeight*2/9+(screenHeight*7/9/2-File_oprationWg.width()/2);
            chooseFileView->move(moveWidth,moveHeight);
        }
    }
    else
    {
        int moveWidth = screenWidth*2/9+((screenWidth-screenWidth*2/9)/2-File_oprationWg.width()/2);
        QPoint p = ui->stackedWidget->mapToGlobal(QPoint(0, 0));
        int movHeight = p.y();
        File_oprationWg.show();File_oprationWg.move(moveWidth,movHeight);
    }
}

void voice::file_choose_widget_hide()
{
    viewShow = 0;
    if(screenFlag == 1)
    {
        File_oprationWg.hide();
        chooseFileView->hide();
        this->hide();this->show();
        this->activateWindow();this->setFocus();
    }
    else
    {
        File_oprationWg.hide();
    }
}

void voice::volume_slider_change(int value)
{
    if(volumeOpenFlag == 0)
    {
        volumeOpenFlag = 1;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_u.svg);");
    }
    setVolume(value);
}
