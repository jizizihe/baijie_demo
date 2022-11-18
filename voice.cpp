#include "voice.h"
#include "ui_voice.h"
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <iostream>
#include <fstream>

static int g_screenWidth;
static int g_screenHeight;
static bool g_recordFlag = false;
static int g_playCurrentTime;
static int g_vioceTimeLength;            //the time length of the file
static int g_recordTimeLength;           //Current recording time length
static int g_playFlag = 0;
static int g_volumeOpenFlag;
static int g_showFirstFlag;
static QScreen *g_screen;
static QString g_saveFilePath;
static QString g_saveFileNmae;
static QString g_recordStartTime;
static QTimer *g_playTimer;
static QTimer *g_recordTimer;

voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice)
{
    ui->setupUi(this);
    g_screen = qApp->primaryScreen();
    g_screenWidth = g_screen->size().width();
    g_screenHeight = g_screen->size().height();
    setVoiceFont();
    g_volumeOpenFlag = 1;

    ui->stackedWidget->setCurrentIndex(0);
    ui->Slider_volume->setRange(0,30);               //Range of volume
    ui->Slider_volume->setValue(6);
    ui->combox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    ui->combox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);

    g_recordTimer = new QTimer(this);
    g_playTimer = new QTimer(this);

    connect(g_recordTimer,SIGNAL(timeout()),this,SLOT(show_recording_time()));
    connect(g_playTimer,SIGNAL(timeout()),this,SLOT(get_playing_time_length()));
    connect(&g_renameWg,SIGNAL(rename_finish_msg(QString)),this,SLOT(update_file(QString)));
    connect(&g_renameWg,SIGNAL(rename_back_msg()),this,SLOT(rename_file_widget_hide()));
    connect(&g_saveFileWg,SIGNAL(save_back_msg()),this,SLOT(save_file_widget_hide()));
    connect(&g_fileOprationWg,SIGNAL(file_rev_path_msg(QString)),this,SLOT(get_file_widget_path(QString)));
    connect(&g_fileOprationWg,SIGNAL(file_hide_msg()),this,SLOT(file_choose_widget_hide()));
    connect(this,SIGNAL(volume_slider_value_change_msg(int)),this,SLOT(volume_slider_change(int)));
}

voice::~voice()
{
    delete ui;
}

void voice::refreshFile(QString dirPath)
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
    if(!g_saveFileNmae.isEmpty())
    {
        for(int i = 0; i < ui->combox->count(); i++)
        {
            ui->combox->setCurrentIndex(i);
            if(ui->combox->currentText() == g_saveFileNmae)
            {
                break;
            }
        }
        g_saveFileNmae = "";
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
    if(g_recordFlag)
    {
        g_recordTimeLength++;
        QString length = timeToString(g_recordTimeLength);
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
    g_fileOprationWg.hide();g_renameWg.hide();g_saveFileWg.hide();
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
    QStringList list = g_database.tableShow("voice");
    if(!list.isEmpty())
    {
        g_saveFilePath = list.at(0);
        g_saveFileNmae = list.at(1);
    }
    if(g_saveFilePath.isEmpty())
    {
        ui->lbl_pathValue->setText("/data/");
    }
    else
    {
        ui->lbl_pathValue->setText(g_saveFilePath);
    }
    QString path = ui->lbl_pathValue->text();
    refreshFile(path);
    g_saveFilePath = "";
}

void voice::languageReload()
{
    ui->retranslateUi(this);
    g_renameWg.languageReload();
    g_saveFileWg.languageReload();
    g_fileOprationWg.languageReload();
}

void voice::get_playing_time_length()
{
    if(g_playFlag == 1)
    {
        g_playCurrentTime+=1;
        ui->Slider_voicelength->setValue(g_playCurrentTime);
        QString currentTime = timeToString(g_playCurrentTime);
        ui->lbl_timeStart->setText(currentTime);

        if(g_playCurrentTime >= g_vioceTimeLength)
        {
            g_playTimer->stop();
            ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/play.svg);");
            g_playFlag = 0;
            ui->lbl_timeEnd->setText("00:00:00");
            ui->lbl_timeStart->setText("00:00:00");
        }
    }
}

void voice::on_btn_playVoice_clicked()
{
    play();
}

void voice::play()
{
    if(ui->combox->currentText() == NULL)
    {
        return;
    }

    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if((g_vioceTimeLength > g_playCurrentTime)&&(g_playCurrentTime != 0))
    {
        ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/play.svg);");
        g_playCurrentTime=0;
        g_playFlag = 0;
        ui->Slider_voicelength->setValue(g_playCurrentTime);
        ui->lbl_timeEnd->setText("00:00:00");
        ui->lbl_timeStart->setText("00:00:00");
    }
    else
    {
        g_playFlag = 1;
        ui->btn_playVoice->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/stop.svg);");

        QString path = QString("%1").arg(ui->lbl_pathValue->text());
        QString name = ui->combox->currentText();
        QString str = QString("aplay %1%2").arg(path).arg(name);
        proc->start(str);
        g_playTimer->start(1000);
        g_playCurrentTime = 0;
        QString pathName = QString("%1%2").arg(path).arg(name);
        float len = getWavTimeLength(pathName);                //Calculate the wav file time length, returned value: minute.seconds
        QString timeLen = QString("%1").arg(len);
        g_vioceTimeLength = calculateTimeLength(timeLen);       //Calculate the recording time length
        ui->Slider_voicelength->setRange(0, g_vioceTimeLength);
    }
}

float voice::getWavTimeLength(QString file)
{
    double len;
    char * g_fileName = file.toLatin1().data();

    if (g_fileName != NULL)
    {
        FILE* fp;
        fp = fopen(g_fileName, "rb");
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
    std::cout << "g_fileName error!!" << std::endl;
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
    g_saveFileWg.getFileName(filen);
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
        mesg.move(g_screenWidth/3,g_screenHeight/3);
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
            mesg.move(g_screenWidth/3,g_screenHeight/3);
            mesg.exec();
            refreshFile(path);
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
        mesg.move(g_screenWidth/3,g_screenHeight/3);
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
        g_renameWg.getBeforeFile(name,path);
    }
    else
    {
        QMessageBox mesg(QMessageBox::Information,
                         tr("QMessageBox::information()"),
                         tr("No file to rename!"),
                         0,this);
        mesg.addButton(tr("OK"),QMessageBox::YesRole);
        mesg.move(g_screenWidth/3,g_screenHeight/3);
        mesg.exec();
    }
}
void voice::update_file(QString file)     //updata combox file
{
    QString path = QString("%1").arg(ui->lbl_pathValue->text());
    refreshFile(path);
    ui->combox->setCurrentText(file);
}

void voice::on_btn_volume_clicked()
{
    if(g_volumeOpenFlag == 1)
    {
        setVolume(0);
        g_volumeOpenFlag = 0;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_d.svg);");
    }
    else
    {
        int value = ui->Slider_volume->value();
        setVolume(value);
        g_volumeOpenFlag = 1;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_u.svg);");
    }
}

void voice::on_btn_startRecord_clicked()
{
    record();
}

void voice::record()
{
    QProcess *proc = new QProcess;
    proc->start("killall aplay");
    proc->waitForFinished(-1);

    if(!g_recordFlag)
    {
        g_recordStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
        QString path = "/data";
        QString strRecordName = QString("%1/%2_record.wav").arg(path).arg(g_recordStartTime);
        QString str = QString("arecord -r 16000 -c 1 -t wav %1").arg(strRecordName);
        proc->start(str);
        ui->btn_startRecord->setText(tr("stop"));
        ui->btn_startRecord->setIcon(QIcon(":/button_image/voice/stop.png"));
        g_recordFlag = true;
    }
    else
    {
        ui->btn_startRecord->setText(tr("start"));
        ui->lbl_RecordTimeLength->setText("00:00:00");
        g_recordTimeLength = 0;
        ui->btn_startRecord->setIcon(QIcon(":/button_image/voice/start.png"));
        g_fileName = QString("%1_record.wav").arg(g_recordStartTime);
        g_recordFlag = false;
        proc->start("killall arecord");
        record_save(g_fileName);
    }
}

void voice::on_Slider_volume_valueChanged(int value)
{
    emit volume_slider_value_change_msg(value);
}

void voice::saveFileWidgetShow()
{
    int funcWidth = ui->function->width();
    int stackWidth = ui->stackedWidget->width();
    int moveWidth = funcWidth+(stackWidth/2-g_screenWidth*31/50/2);
    QPoint p = ui->btn_startRecord->mapToGlobal(QPoint(0, 0));
    int btnStartRecordY = p.y()+ui->btn_startRecord->height();
    int moveHeight = g_screenHeight-(g_screenHeight - btnStartRecordY)-g_screenHeight*2/3;
    g_saveFileWg.resize(g_screenWidth*33/50,g_screenHeight*2/3);
    moveWidth = g_screenWidth*2/9+((g_screenWidth-g_screenWidth*2/9)/2-g_saveFileWg.width()/2);
    moveHeight = g_screenHeight/6+((g_screenHeight-g_screenHeight/6)/2-g_saveFileWg.height()/2);
    g_saveFileWg.move(moveWidth,moveHeight);
    g_saveFileWg.show();
    g_saveFileWg.activateWindow();g_saveFileWg.setFocus();
}
void voice::renameFileWidgetShow()
{
    int funcWidth = ui->function->width();
    int stackWidth = ui->stackedWidget->width();
    int moveWidth = funcWidth+(stackWidth/2-g_screenWidth*31/50/2);
    int moveHeight = g_screenHeight/6+(g_screenHeight*5/12-g_fileOprationWg.height()/2);
    g_renameWg.move(moveWidth,moveHeight);
    g_renameWg.show();
    g_renameWg.activateWindow();g_renameWg.setFocus();
    g_renameWg.resize(g_screenWidth*3/5,ui->stackedWidget->height()*3/4);
}

void voice::save_file_widget_hide()
{
   g_saveFileWg.hide();
   this->activateWindow();this->setFocus();
}

void voice::rename_file_widget_hide()
{
    g_renameWg.hide();
    this->activateWindow();this->setFocus();
}

void voice::setVoiceFont()
{
    qreal realX = g_screen->physicalDotsPerInchX();
    qreal realWidth = g_screenWidth / realX * 2.54;
    QFont font;

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
    if(g_showFirstFlag == 0)
    {
        saveFileWidgetShow();
        save_file_widget_hide();
        renameFileWidgetShow();
        rename_file_widget_hide();
        g_showFirstFlag++;
    }
    g_recordTimer->stop();
    QWidget::closeEvent(event);
}

void voice::showEvent(QShowEvent *event)
{
    g_recordTimer->start(1000);
}

void voice::on_btn_chooseFile_clicked()
{
    g_fileOprationWg.filePathFlag = 1;
    chooseFileWidgetShow();
}

void voice::get_file_widget_path(QString fpath)
{
    ui->lbl_pathValue->setText(fpath);
    refreshFile(fpath);
}

void voice::chooseFileWidgetShow()
{
    int moveWidth = g_screenWidth*2/9+((g_screenWidth-g_screenWidth*2/9)/2-g_fileOprationWg.width()/2);
    int moveHeight = g_screenHeight/6+(g_screenHeight*5/12-g_fileOprationWg.height()/2);
    g_fileOprationWg.show();
    g_fileOprationWg.move(moveWidth,moveHeight);
}

void voice::file_choose_widget_hide()
{
   g_fileOprationWg.hide();
}

void voice::volume_slider_change(int value)
{
    if(g_volumeOpenFlag == 0)
    {
        g_volumeOpenFlag = 1;
        ui->btn_volume->setStyleSheet("background-color:transparent;border-image: url(:/button_image/voice/sound_u.svg);");
    }
    setVolume(value);
}
