#ifndef SWITCH_CONTROL
#define SWITCH_CONTROL

#include <QWidget>
#include <QTimer>

class SwitchControl : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchControl(QWidget *parent = 0);
    bool isToggled() const; // Return switch status - On: true Off: false
    void setToggle(bool checked); // Setting the switch status
    void setBackgroundColor(QColor color);
    void setCheckedColor(QColor color);
    void setDisbaledColor(QColor color);

protected:   
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;// Draw the switch
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;// Mouse release event - Toggle switch status and transmit toggled() signal
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    void toggled(bool checked);

private slots:
    void onTimeout();   // Produces a sliding effect

private:
    bool Checked;
    QColor backgroundColor;
    QColor checkedColor;
    QColor disabledColor;
    QColor myThumbColor;
    qreal radius;
    qreal X;
    qreal Y;
    qint16 Height;
    qint16 margin;
    QTimer timer;
};

#endif // SWITCH_CONTROL
