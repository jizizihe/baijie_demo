#include "keythread.h"

using namespace std;

keythread::keythread(QObject *parent) : QThread(parent)
{

}
keythread::~keythread()
{

}

char * keytest_scan(char* keypath)
{
    int fd,i = 0;
    static char path[64];
    char buf[64];

    for(i = 0;i < 10; i++)
    {
        bzero(path,sizeof(path));
        snprintf(path,sizeof(path),"/sys/class/input/input%d/name",i);
        fd = open(path, O_RDONLY);

        if(fd < 0)
        {
            printf("open failed");
            return NULL;
        }
        bzero(buf,sizeof(buf));
        read(fd,buf,sizeof(buf));
        QString str1 = QString(buf);
        qDebug() << "buf" <<str1;
        bzero(path,sizeof(path));
        snprintf(path,sizeof(path),"%s\n",keypath);
        QString str2 = QString(path);
        qDebug() << "path" <<str2;
        if(strcmp(buf,path) == 0)
        {
            snprintf(path,sizeof(path),"/dev/input/event%d",i);
            return path;
        }

        close(fd);
    }
    QString str = QString(path);
    qDebug() << path;
    return NULL;
}

/*1--pressed 0--not pressed*/
int key_test(char * event)
{
    int buttons_fd;
    int i=0,count;

    struct input_event ev_key;
    buttons_fd = open(event, O_RDWR);

    if (buttons_fd < 0) {
        printf("open input device error\n");
        return 0;
    }

    for (;;)
    {
        count = read(buttons_fd,&ev_key,sizeof(struct input_event));
        //qDebug() << ev_key.type << ev_key.code-1 << ev_key.value;
        for(i=0; i<(int)count/sizeof(struct input_event); i++){
            if(EV_KEY==ev_key.type)
            {
                qDebug("type:%d,code:%d,value:%d\n", ev_key.type,ev_key.code-1,ev_key.value);
                if(ev_key.value == 1)
                {
                    return 1;
                }
            }
            if(EV_SYN==ev_key.type) {
                //printf("---touch screen test ok\n");
                //qDebug() << "-------KEY PRESS------\n";
            }
        }
   }
   close(buttons_fd);

   return 0;
}

void keythread::run()
{
    char* path;
    char* keypath = "sunxi-keyboard";

    path = keytest_scan(keypath);
    if(path == NULL)
    {
        qDebug() << "not find this device!";
    }
    else
    {
        for(;;)
        {
            QString str = QString(path);
            //qDebug() << str;

            int key_status = key_test(path);
            if(key_status == 1)
            {
                str = "-------KEY PRESS------";
                //qDebug() << str;
                emit message(str);
            }
        }
    }
}
