#include "switchcontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
SwitchControl::SwitchControl(QWidget *parent)
    : QWidget(parent),
      m_bChecked(true),
    //  m_background(Qt::black),
      m_background(98, 200, 182,180),
      m_checkedColor(0, 160, 230),
      m_disabledColor(190, 190, 190),
    //  m_thumbColor(Qt::white),
      m_thumbColor(98, 200, 182,255),
      m_radius(20.0),
      m_nHeight(43),
      m_nMargin(4)
{
    // 鼠标滑过光标形状 - 手型
    setCursor(Qt::PointingHandCursor);

    m_bChecked = true;
    // 连接信号槽
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

}

// 绘制开关
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
    if (isEnabled()) { // 可用状态
        if (m_bChecked) { // 打开状态
            background = m_checkedColor;
            thumbColor = m_checkedColor;
            dOpacity = 0.600;
        } else { //关闭状态
            background = m_background;
            thumbColor = m_thumbColor;
            dOpacity = 0.800;
        }
    } else {  // 不可用状态
        background = m_background;
        dOpacity = 0.260;
        thumbColor = m_disabledColor;
    }
    // 绘制大椭圆
    painter.setBrush(background);
    painter.setOpacity(dOpacity);

   // path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);


    //QRect textRect(0, 0,width()-10, height()-20); painter.setPen(QPen(QColor(0, 0, 0)));
    //painter.drawText(textRect, Qt::AlignRight, "On");painter.setPen(QPen(background));
    painter.drawPath(path.simplified());
   // painter.setPen(QPen(thumbColor));

    // 绘制小椭圆
    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
   // painter.drawEllipse(QRectF(m_nX - (m_nHeight / 2), m_nY - (m_nHeight / 2), height(), height()));
    painter.drawEllipse(QRectF(m_nX - (m_nHeight/2), m_nY - (m_nHeight/2), height(), height()));

}

// 鼠标按下事件
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

// 鼠标释放事件 - 切换开关状态、发射toggled()信号
void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            m_bChecked = !m_bChecked;
            emit toggled(m_bChecked);
            //m_timer.start();
            onTimeout();
        } else {
            event->ignore();
        }
    }
}

// 大小改变事件
void SwitchControl::resizeEvent(QResizeEvent *event)
{
   // m_nX = m_nHeight / 2;
    m_nX = width() - m_nHeight;
    m_nY = m_nHeight / 2;
    QWidget::resizeEvent(event);
}

// 默认大小
QSize SwitchControl::sizeHint() const
{
    return minimumSizeHint();
    //qDebug() << "line:" << __LINE__ << "Height:" << m_nHeight;
}

// 最小大小
QSize SwitchControl::minimumSizeHint() const
{
    return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
}

// 切换状态 - 滑动
void SwitchControl::onTimeout()
{
    if (m_bChecked) {
       // m_nX += 1;
       // if (m_nX >= width() - m_nHeight)
       //     m_timer.stop();
        m_nX = width() - m_nHeight;
    } else {
        //m_nX -= 1;
       // if (m_nX <= m_nHeight / 2)
        //    m_timer.stop();
        m_nX = m_nHeight / 2;
    }
    update();
}

// 返回开关状态 - 打开：true 关闭：false
bool SwitchControl::isToggled() const
{
    return m_bChecked;
}

// 设置开关状态
void SwitchControl::setToggle(bool checked)
{
    emit toggled(m_bChecked);
    m_bChecked = checked;
    onTimeout();
   // m_timer.start(1);
}

// 设置背景颜色
void SwitchControl::setBackgroundColor(QColor color)
{
    m_background = color;
}

// 设置选中颜色
void SwitchControl::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

// 设置不可用颜色
void SwitchControl::setDisbaledColor(QColor color)
{
    m_disabledColor = color;
}
