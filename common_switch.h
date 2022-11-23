#ifndef SWITCH_CONTROL
#define SWITCH_CONTROL

#include <QWidget>
#include <QTimer>

class common_switch : public QWidget
{
    Q_OBJECT

public:
    explicit common_switch(QWidget *parent = 0);
    bool isToggled() const;        // Return switch status - On: true Off: false
    void setToggle(bool checked);  // Setting the switch status
    void setBackgroundColor(QColor color);
    void setCheckedColor(QColor color);
    void setDisbaledColor(QColor color);

protected:   
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;        // Draw the switch
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE; // Mouse release event - Toggle switch status and transmit toggled() signal
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    void toggled(bool checked);

private slots:
    void changeToggled();   // Produces a sliding effect

private:
    bool g_checked;
    QColor g_backgroundColor;
    QColor g_checkedColor;
    QColor g_disabledColor;
    QColor g_myThumbColor;
    qreal g_radius;
    qreal g_x;
    qreal g_y;
    qint16 g_height;
    qint16 g_margin;
};

#endif // COMMON_SWITCH_H
