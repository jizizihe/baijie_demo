#include "input_method.h"
#include "ui_input_method.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QScreen>
#include <QDebug>

static QString str;
static int screen_flag = 0;
static int style_num;
static int key_type;
static QScreen *screen;
static int s_width;
static int s_height;

input_method::input_method(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::input_method)
{
        ui->setupUi(this);

        this->setFocusPolicy(Qt::NoFocus);
        screen = qApp->primaryScreen();
        s_width = screen->size().width();
        s_height = screen->size().height();

        if(s_width < s_height)
        {
            screen_flag = 1;
        }

       this->setAttribute(Qt::WA_ShowWithoutActivating,true);
       this->setFocusPolicy(Qt::NoFocus);
}

input_method::~input_method()
{
    delete ui;
}

void input_method::on_btn_0_clicked()
{
        str = "0";
       emit key_str(str);
}

void input_method::on_btn_1_clicked()
{
    str = "1";
    emit key_str(str);
}

void input_method::on_btn_2_clicked()
{
    str = "2";
    emit key_str(str);
}

void input_method::on_btn_3_clicked()
{
    str = "3";
    emit key_str(str);
}

void input_method::on_btn_4_clicked()
{
    str = "4";
    emit key_str(str);
}

void input_method::on_btn_5_clicked()
{
    str = "5";
    emit key_str(str);
}

void input_method::on_btn_6_clicked()
{
    str = "6";
    emit key_str(str);
}

void input_method::on_btn_7_clicked()
{
    str = "7";
    emit key_str(str);
}

void input_method::on_btn_8_clicked()
{
    str = "8";
    emit key_str(str);
}

void input_method::on_btn_9_clicked()
{
    str = "9";
    emit key_str(str);
}

void input_method::on_btn_q_clicked()
{
    if(key_type == 0)
    {
        str = "q";
    }
    else
    {
        str = "Q";
    }
    emit key_str(str);
}

void input_method::on_btn_w_clicked()
{
    if(key_type == 0)
    {
        str = "w";
    }
    else
    {
        str = "W";
    }
    emit key_str(str);
}

void input_method::on_btn_e_clicked()
{
    if(key_type == 0)
    {
        str = "e";
    }
    else
    {
        str = "E";
    }
    emit key_str(str);
}

void input_method::on_btn_r_clicked()
{
    if(key_type == 0)
    {
        str = "r";
    }
    else
    {
        str = "R";
    }
    emit key_str(str);
}

void input_method::on_btn_t_clicked()
{
    if(key_type == 0)
    {
        str = "t";
    }
    else
    {
        str = "T";
    }
    emit key_str(str);
}

void input_method::on_btn_y_clicked()
{
    if(key_type == 0)
    {
        str = "y";
    }
    else
    {
        str = "Y";
    }
    emit key_str(str);
}

void input_method::on_btn_u_clicked()
{
    if(key_type == 0)
    {
        str = "u";
    }
    else
    {
        str = "U";
    }
    emit key_str(str);
}

void input_method::on_btn_i_clicked()
{
    if(key_type == 0)
    {
        str = "i";
    }
    else
    {
        str = "I";
    }
    emit key_str(str);
}

void input_method::on_btn_o_clicked()
{
    if(key_type == 0)
    {
        str = "o";
    }
    else
    {
        str = "O";
    }
    emit key_str(str);
}

void input_method::on_btn_p_clicked()
{
    if(key_type == 0)
    {
        str = "p";
    }
    else
    {
        str = "P";
    }
    emit key_str(str);
}

void input_method::on_btn_a_clicked()
{
    if(key_type == 0)
    {
        str = "a";
    }
    else
    {
        str = "A";
    }
    emit key_str(str);
}

void input_method::on_btn_s_clicked()
{
    if(key_type == 0)
    {
        str = "s";
    }
    else
    {
        str = "S";
    }
    emit key_str(str);
}

void input_method::on_btn_d_clicked()
{
    if(key_type == 0)
    {
        str = "d";
    }
    else
    {
        str = "D";
    }
    emit key_str(str);
}

void input_method::on_btn_f_clicked()
{
    if(key_type == 0)
    {
        str = "f";
    }
    else
    {
        str = "F";
    }
    emit key_str(str);
}

void input_method::on_btn_g_clicked()
{
    if(key_type == 0)
    {
        str = "g";
    }
    else
    {
        str = "G";
    }
    emit key_str(str);
}

void input_method::on_btn_h_clicked()
{
    if(key_type == 0)
    {
        str = "h";
    }
    else
    {
        str = "H";
    }
    emit key_str(str);
}

void input_method::on_btn_j_clicked()
{
    if(key_type == 0)
    {
        str = "j";
    }
    else
    {
        str = "J";
    }
    emit key_str(str);
}

void input_method::on_btn_k_clicked()
{
    if(key_type == 0)
    {
        str = "k";
    }
    else
    {
        str = "K";
    }
    emit key_str(str);
}

void input_method::on_btn_l_clicked()
{
    if(key_type == 0)
    {
        str = "l";
    }
    else
    {
        str = "L";
    }
    emit key_str(str);
}

void input_method::on_btn_case_clicked()
{
    if(ui->btn_case->text() == "cap")
    {
        key_type = 1;
        ui->btn_case->setText("low");
        ui->btn_q->setText("Q");
        ui->btn_w->setText("W");
        ui->btn_e->setText("E");
        ui->btn_r->setText("R");
        ui->btn_t->setText("T");
        ui->btn_y->setText("Y");
        ui->btn_u->setText("U");
        ui->btn_i->setText("I");
        ui->btn_o->setText("O");
        ui->btn_p->setText("P");
        ui->btn_a->setText("A");
        ui->btn_s->setText("S");
        ui->btn_d->setText("D");
        ui->btn_f->setText("F");
        ui->btn_g->setText("G");
        ui->btn_h->setText("H");
        ui->btn_j->setText("J");
        ui->btn_k->setText("K");
        ui->btn_l->setText("L");
        ui->btn_z->setText("Z");
        ui->btn_x->setText("X");
        ui->btn_c->setText("C");
        ui->btn_v->setText("V");
        ui->btn_b->setText("B");
        ui->btn_n->setText("N");
        ui->btn_m->setText("M");

    }
    else
    {
        key_type = 0;
        ui->btn_case->setText("cap");
        ui->btn_q->setText("q");
        ui->btn_w->setText("w");
        ui->btn_e->setText("e");
        ui->btn_r->setText("r");
        ui->btn_t->setText("t");
        ui->btn_y->setText("y");
        ui->btn_u->setText("u");
        ui->btn_i->setText("i");
        ui->btn_o->setText("o");
        ui->btn_p->setText("p");
        ui->btn_a->setText("a");
        ui->btn_s->setText("s");
        ui->btn_d->setText("d");
        ui->btn_f->setText("f");
        ui->btn_g->setText("g");
        ui->btn_h->setText("h");
        ui->btn_j->setText("j");
        ui->btn_k->setText("k");
        ui->btn_l->setText("l");
        ui->btn_z->setText("z");
        ui->btn_x->setText("x");
        ui->btn_c->setText("c");
        ui->btn_v->setText("v");
        ui->btn_b->setText("b");
        ui->btn_n->setText("n");
        ui->btn_m->setText("m");
    }
}

void input_method::on_btn_z_clicked()
{
    if(key_type == 0)
    {
        str = "z";
    }
    else
    {
        str = "Z";
    }
    emit key_str(str);
}

void input_method::on_btn_x_clicked()
{
    if(key_type == 0)
    {
        str = "x";
    }
    else
    {
        str = "X";
    }
    emit key_str(str);
}

void input_method::on_btn_c_clicked()
{
    if(key_type == 0)
    {
        str = "c";
    }
    else
    {
        str = "C";
    }
    emit key_str(str);
}

void input_method::on_btn_v_clicked()
{
    if(key_type == 0)
    {
        str = "v";
    }
    else
    {
        str = "V";
    }
    emit key_str(str);
}

void input_method::on_btn_b_clicked()
{
    if(key_type == 0)
    {
        str = "b";
    }
    else
    {
        str = "B";
    }
    emit key_str(str);
}

void input_method::on_btn_n_clicked()
{
    if(key_type == 0)
    {
        str = "n";
    }
    else
    {
        str = "N";
    }
    emit key_str(str);
}

void input_method::on_btn_m_clicked()
{
    if(key_type == 0)
    {
        str = "m";
    }
    else
    {
        str = "M";
    }
    emit key_str(str);
}

void input_method::on_btn_other_clicked()
{
    str = "back";
    emit key_str(str);
}

void input_method::on_btn_style_clicked()
{
    if(style_num == 1)
    {
         this->setStyleSheet("QPushButton{background-color: rgba(133, 133, 133,180);"
    "border-style: outset; "
    "border-width:  1px; border-radius: 5px; border-color: rgba(0, 0, 0, 100);"
    "color:rgba(0, 0, 0, 100);padding: 6px; outline: none; }"
    " QPushButton:pressed { "
    "background-color:rgba(133, 133, 133,70);"
    "border-color: rgba(0, 0, 0, 100);border-style: inset;color:rgba(0, 0, 0, 100);}");
        style_num=0;
    }
    else if(style_num == 0)
    {
        this->setStyleSheet("QPushButton{background-color: rgba(100, 225, 100, 130);"
                            "border-style: outset; "
                            "border-width:  1px; border-radius: 5px; border-color: rgba(100, 225, 110, 250);"
                            "color:rgba(0, 0, 0, 100);padding: 6px; outline: none; }"
                            " QPushButton:pressed { "
                            "background-color:rgba(100,255,100, 200);"
                            "border-color: rgba(255, 225, 255, 30);border-style: inset;color:rgba(0, 0, 0, 100);}");
        style_num=1;
   }
}

void input_method::on_btn_close_clicked()
{
    str = "close";
    emit key_str(str);
}

void input_method::input_mfont()
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
   ui->btn_0->setFont(font);
   ui->btn_1->setFont(font);
   ui->btn_2->setFont(font);
   ui->btn_3->setFont(font);
   ui->btn_4->setFont(font);
   ui->btn_5->setFont(font);
   ui->btn_6->setFont(font);
   ui->btn_7->setFont(font);
   ui->btn_8->setFont(font);
   ui->btn_9->setFont(font);
   ui->btn_e->setFont(font);
   ui->btn_r->setFont(font);
   ui->btn_t->setFont(font);
   ui->btn_y->setFont(font);
   ui->btn_u->setFont(font);
   ui->btn_i->setFont(font);
   ui->btn_o->setFont(font);
   ui->btn_p->setFont(font);
   ui->btn_a->setFont(font);
   ui->btn_s->setFont(font);
   ui->btn_d->setFont(font);
   ui->btn_f->setFont(font);
   ui->btn_g->setFont(font);
   ui->btn_h->setFont(font);
   ui->btn_j->setFont(font);
   ui->btn_k->setFont(font);
   ui->btn_l->setFont(font);
   ui->btn_z->setFont(font);
   ui->btn_x->setFont(font);
   ui->btn_c->setFont(font);
   ui->btn_v->setFont(font);
   ui->btn_b->setFont(font);
   ui->btn_n->setFont(font);
   ui->btn_m->setFont(font);
   ui->btn_case->setFont(font);
   ui->btn_close->setFont(font);
   ui->btn_other->setFont(font);
   ui->btn_style->setFont(font);
}
