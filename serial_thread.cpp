#include "serial_thread.h"

serial_thread::serial_thread(int portId,QString port,long baud,int Databit,QString Stopbit,int *OpenFlag,QObject *parent) : QObject(parent)
{
    my_thread = new QThread();
    //show_fun_id();
    //show_slots_id();
    m_port = new QSerialPort();
    m_portId = portId;

    *OpenFlag = 0;
    init_port(port,baud,Databit,Stopbit,OpenFlag);

    this->moveToThread(my_thread);
    m_port->moveToThread(my_thread);
    my_thread->start();//
}

void serial_thread::show_fun_id()
{
    //qDebug()<<"fun_id is:"<<QThread::currentThreadId();
}
void serial_thread::show_slots_id()
{
    //qDebug()<<"slots_id is:"<<QThread::currentThreadId();
    show_fun_id();
}

void serial_thread::init_port(QString port,long baud,int Databit,QString Stopbit,int *OpenFlag)
{
    m_port->setPortName(port);
    m_port->setBaudRate(baud);//波特率
    init_Databit(Databit);//数据位
    m_port->setParity(QSerialPort::NoParity);//奇偶校验
    init_Stopbit(Stopbit);
    //m_port->setStopBits(QSerialPort::OneStop);//停止位
    m_port->setFlowControl(QSerialPort::NoFlowControl);//控制位


    if(m_port->open(QIODevice::ReadWrite) == false)//读写方式打开
    {
       //  qDebug()<<"open port " <<"failed!"<<endl;
         *OpenFlag = 0;
    }
    else
    {
       // qDebug()<<"Port "<<" have been opened."<<endl;
        *OpenFlag = 1;
    }

    //m_readTimer = new QTimer(this);
    //m_readTimer->start(100); /*开启定时器，并且每100ms后询问一次串口。定时的时间一到，马上产生timeout（）信号，继续执行自定义槽函数readMyCom() */
    //connect(m_readTimer,SIGNAL(timeout()),this,SLOT(read_data()));
    connect(m_port,SIGNAL(readyRead()),this,SLOT(read_data()));//有数据就读
    //connect(this,SIGNAL(thread_sig()),this,SLOT(show_slots_id()));
}

void serial_thread::closePort(int portId)
{
    //qDebug() << "LINE:" << __LINE__ << "portId:" << portId;

    if(portId==m_portId)
    {
        //m_readTimer->stop();
     //   m_port->clear();
        m_port->close();
        m_port->deleteLater();

        my_thread->quit();
        my_thread->wait();
        my_thread->deleteLater();
    }
    //qDebug()<<m_portId<<"Thread_id is:" << QThread::currentThreadId();
    emit thread_sig();
}

void serial_thread::read_data()
{
    QString buf;
    buf = QString(m_port->readAll());
    qDebug() << "LINE:" << __LINE__ << "read data:" << buf;
    emit receive_data(buf);
}

void serial_thread::write_data(int portId,QByteArray buff)
{
    qDebug() << "LINE:" << __LINE__ << "write_data!" << endl;
     if(portId==m_portId)
     {
         m_port->write(buff);
     }
//     qDebug()<<m_portId<<" write_id is:"<<QThread::currentThreadId();
//     emit thread_sig();
}

void serial_thread::init_Databit(int Databit)
{
    switch (Databit) {
    case 5:m_port->setDataBits(QSerialPort::Data5);break;
    case 6:m_port->setDataBits(QSerialPort::Data6);break;
    case 7:m_port->setDataBits(QSerialPort::Data7);break;
    case 8:m_port->setDataBits(QSerialPort::Data8);break;
    default:break;
    }
}

void serial_thread::init_Stopbit(QString Stopbit)
{
    if(!QString::compare(Stopbit,QString("1"),Qt::CaseSensitive)){
          m_port->setStopBits(QSerialPort::OneStop);
       }
    if(!QString::compare(Stopbit,QString("2"),Qt::CaseSensitive)){
          m_port->setStopBits(QSerialPort::TwoStop);
       }
}
