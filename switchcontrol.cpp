#include "switchcontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

SwitchControl::SwitchControl(QWidget *parent)
    : QWidget(parent),
      Checked(true),
      backgroundColor(98, 200, 182,180),
      checkedColor(0, 160, 230),
      disabledColor(98, 200, 182,100),
      myThumbColor(98, 200, 182,255),
      radius(20.0),
      Height(46),
      margin(4)
{
    setCursor(Qt::PointingHandCursor);
    Checked = true;
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void SwitchControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity;
    if (isEnabled()) {
        if (Checked) {
            background = checkedColor;
            thumbColor = checkedColor;
            dOpacity = 0.600;
        }
        else {
            background = backgroundColor;
            thumbColor = myThumbColor;
            dOpacity = 0.800;
        }
    } else {
        background = backgroundColor;
        //dOpacity = 0.260;
        dOpacity = 0.500;
        thumbColor = disabledColor;
    }
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(margin, margin, width() - 2 * margin, height() - 2 * margin), radius, radius);
    painter.drawPath(path.simplified());

    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(X - (Height/2), Y - (Height/2), height(), height()));
}

void SwitchControl::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            Checked = !Checked;
            emit toggled(Checked);
            //timer.start();
            onTimeout();
        } else {
            event->ignore();
        }
    }
}

void SwitchControl::resizeEvent(QResizeEvent *event)
{
    X = width() - Height;
    Y = Height / 2;
    QWidget::resizeEvent(event);
}

QSize SwitchControl::sizeHint() const
{
    return minimumSizeHint();
}

QSize SwitchControl::minimumSizeHint() const
{
    return QSize(2 * (Height + margin), Height + 2 * margin);
}

void SwitchControl::onTimeout()
{
    if (Checked) {
        // X += 1;
        // if (X >= width() - Height)
        //     timer.stop();
        X = width() - Height;
    } else {
        //X -= 1;
        // if (X <= Height / 2)
        //    timer.stop();
        X = Height / 2;
    }
    update();
}

bool SwitchControl::isToggled() const
{
    return Checked;
}

void SwitchControl::setToggle(bool checked)
{
    emit toggled(Checked);
    Checked = checked;
    onTimeout();
    // timer.start(1);
}

void SwitchControl::setBackgroundColor(QColor color)
{
    backgroundColor = color;
}

void SwitchControl::setCheckedColor(QColor color)
{
    checkedColor = color;
}

void SwitchControl::setDisbaledColor(QColor color)
{
    disabledColor = color;
}
