#include "common_switch.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

common_switch::common_switch(QWidget *parent)
    : QWidget(parent),
      g_checked(true),
      g_backgroundColor(98, 200, 182,180),
      g_checkedColor(0, 160, 230),
      g_disabledColor(98, 200, 182,100),
      g_myThumbColor(98, 200, 182,255),
      g_radius(20.0),
      g_height(46),
      g_margin(4)
{
    setCursor(Qt::PointingHandCursor);
    g_checked = true;
}

void common_switch::paintEvent(QPaintEvent *event)
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
        if (g_checked) {
            background = g_checkedColor;
            thumbColor = g_checkedColor;
            dOpacity = 0.600;
        }
        else {
            background = g_backgroundColor;
            thumbColor = g_myThumbColor;
            dOpacity = 0.800;
        }
    } else {
        background = g_backgroundColor;
        //dOpacity = 0.260;
        dOpacity = 0.500;
        thumbColor = g_disabledColor;
    }
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(g_margin, g_margin, width() - 2 * g_margin, height() - 2 * g_margin), g_radius, g_radius);
    painter.drawPath(path.simplified());

    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(g_x - (g_height/2), g_y - (g_height/2), height(), height()));
}

void common_switch::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void common_switch::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            g_checked = !g_checked;
            emit toggled(g_checked);
            changeToggled();
        } else {
            event->ignore();
        }
    }
}

void common_switch::resizeEvent(QResizeEvent *event)
{
    g_x = width() - g_height;
    g_y = g_height / 2;
    QWidget::resizeEvent(event);
}

QSize common_switch::sizeHint() const
{
    return minimumSizeHint();
}

QSize common_switch::minimumSizeHint() const
{
    return QSize(2 * (g_height + g_margin), g_height + 2 * g_margin);
}

void common_switch::changeToggled()
{
    if (g_checked) {
        g_x = width() - g_height;
    } else {
        g_x = g_height / 2;
    }
    update();
}

bool common_switch::isToggled() const
{
    return g_checked;
}

void common_switch::setToggle(bool g_Checked)
{
    emit toggled(g_Checked);
    g_Checked = g_Checked;
    changeToggled();
}

void common_switch::setBackgroundColor(QColor color)
{
    g_backgroundColor = color;
}

void common_switch::setCheckedColor(QColor color)
{
    g_checkedColor = color;
}

void common_switch::setDisbaledColor(QColor color)
{
    g_disabledColor = color;
}
