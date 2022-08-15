#ifndef INPUT_METHOD_H
#define INPUT_METHOD_H

#include <QMainWindow>

namespace Ui {
class input_method;
}

class input_method : public QMainWindow
{
    Q_OBJECT

public:
    explicit input_method(QWidget *parent = 0);
    ~input_method();
    void input_mfont();

private slots:
    void on_btn_0_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_q_clicked();

    void on_btn_w_clicked();

    void on_btn_e_clicked();

    void on_btn_r_clicked();

    void on_btn_t_clicked();

    void on_btn_y_clicked();

    void on_btn_u_clicked();

    void on_btn_i_clicked();

    void on_btn_o_clicked();

    void on_btn_p_clicked();

    void on_btn_a_clicked();

    void on_btn_s_clicked();

    void on_btn_d_clicked();

    void on_btn_f_clicked();

    void on_btn_g_clicked();

    void on_btn_h_clicked();

    void on_btn_j_clicked();

    void on_btn_k_clicked();

    void on_btn_l_clicked();

    void on_btn_case_clicked();

    void on_btn_z_clicked();

    void on_btn_x_clicked();

    void on_btn_c_clicked();

    void on_btn_v_clicked();

    void on_btn_b_clicked();

    void on_btn_n_clicked();

    void on_btn_m_clicked();

    void on_btn_other_clicked();

    void on_btn_style_clicked();

    void on_btn_close_clicked();

signals:
    void key_str(QString);

private:
    Ui::input_method *ui;
};

#endif // INPUT_METHOD_H
