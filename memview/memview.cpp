#include "memview.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    resize(200,200);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

}

Widget::~Widget()
{
    delete ui;

}

void Widget::sendPoint(int pro_pos,unsigned long addr)
{
    sem_wait(Empty);
        sem_wait(Mutex);

        lseek(file, pro_pos * sizeof(int), SEEK_SET);        /*���ļ�ָ���ƶ��������ߴ�*/
        write(file, &addr, sizeof(int));                     /*������д��item*/

        sem_post(Mutex);
        sem_post(Full);
        return;
}

unsigned long Widget::getPoint()
{
        int pos, newpos;
        unsigned long item;
        sem_wait(Full);
        sem_wait(Mutex);

        lseek(file, 10 * sizeof(int), SEEK_SET);     /*���ļ�ָ��ָ���ļ��ĵ�11��int��֮ǰ*/
        read(file, &pos, sizeof(int));               /*���ļ��е�11����������������pos����Ϊ������ָ��*/
        newpos = (pos + 1) % 10;                     /*��������ָ���1ģ10��д���ļ���11������*/
        lseek(file, 10 * sizeof(int), SEEK_SET);
        write(file, &newpos, sizeof(int));

        lseek(file, pos * sizeof(int), SEEK_SET);    /*���ļ�ָ���ƶ��������ߴ�*/
        read(file, &item, sizeof(int));              /*�����߶���item*/

        sem_post(Mutex);         /*�����ź���*/
        sem_post(Empty);         /*���л�����Դ*/
        return item;
}

QPoint Widget::handle(unsigned long undealedpoint)
{
    int x,y;

    if(flag==0)
    {
        x = undealedpoint/1000+1;
        y = undealedpoint-(x-1)*1000;
        flag = 1;
    }
    else
    {
        x = undealedpoint/1000+1;
        y = undealedpoint-(x-1)*1000;
        flag = 0;
    }
    QPoint handle[1]={QPoint(x,y)};
    return handle;
}

void Widget::paintEvent(QPaintEvent *)
{
    Empty = sem_open("Empty", 10);
    Mutex = sem_open("Mutex", 1);
    Full = sem_open("Full", 0);

    int x;
    int y;

    int
    int pos;
    unsigned long addrpoint;

    file1 = open("buffer", O_RDWR | O_CREAT | O_TRUNC, 0777);
    file2 = open(FILENAME_MAX,O_RDONLY);
    char *fgets(receive,5000,file2);

    lseek(file, 10 * sizeof(int), SEEK_SET);                      /*���ļ�ָ���ƶ�����11��int���������ʼ��Ϊ0*/
    write(file, &pos, sizeof(int));


    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    //paint.setPen(Qt::blue);
    //paint.drawPoint(100,100);
    //paint.setBrush(Qt::blue);

    for(int i=0; i<2; i++)
            if(!fork())
            {
                while(1)
                {
                    addrpoint = getPoint( );
                }
            }
    for (int j = 0;j<sizeof(receive);j++)
    {
        sendPoint(pos,receive[j]);
        pos = (pos + 1) % 10;
    }

    QPoint dealedPoint[1] = handle(addrpoint);

    paint.setPen(Qt::blue);
    for(int i = 0;i<1000;i+=3)
    {
        for(int j = 0;j<1000;j+=3)
        {
            paint.drawPoint(i,j);
        }
    }

    if(flag==0)
    {
        for(;dealedPoint.y<1000;y++)
        {
            paint.setPen(Qt::yellow);
            paint.drawPoint(dealedPoint.x,dealedpoint.y);
            x = dealedPoint.x;
        }
    }

    else
    {
        paint.setPen(Qt::yellow);
        for(;x<dealedPoint.x-x-1;x++)
        {
            for(int j = 0;j<1000;j++)
            {
                paint.drawPoint(x,j);
            }
        }
        for(int z= 0;z<dealedPoint.y;z++)
        {
            paint.drawPoint(dealedPoint.x,z);
        }
    }


}
