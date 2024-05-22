#include "../SeisDrawFault/SeisBasicGraphicsItem.h"

#include <algorithm>
#include <cmath>
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>
SeisBasicGraphicsItem::SeisBasicGraphicsItem(QPointF center, QPointF edge, ItemType type)
    : m_center(center), m_edge(edge), m_type(type)
{
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
}

SeisBasicGraphicsItem::SeisBasicGraphicsItem(ItemType type,QPointF edge, QPointF special)
: m_edge(edge), m_special(special), m_type(type)
{
	m_center=QPointF((m_edge.x()+m_special.x())/2,(m_edge.y()+m_special.y())/2);
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
}

void SeisBasicGraphicsItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_isSelected);
}

void SeisBasicGraphicsItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_noSelected);
}


DLine::DLine(QPointF pcentter,QPointF pedge, ItemType type):SeisBasicGraphicsItem(pcentter, pedge, type)
{
    SeisPointItem *point = new SeisPointItem(this, m_edge, SeisPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new SeisPointItem(this, m_center, SeisPointItem::Center));
    m_pointList.setRandColor();
}

QRectF DLine::boundingRect() const
{
	qreal bleft=qMin(m_center.x(),m_edge.x());
	qreal btop=qMin(m_center.y(),m_edge.y());
	qreal bwidth=abs(m_center.x()-m_edge.x());
	qreal bheight=abs(m_center.y()-m_edge.y());
    return QRectF(bleft,btop,bwidth,bheight);
}

void DLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    painter->drawLine(m_center,m_edge);
}

DWLine::DWLine(QPointF pleft,QPointF pright, ItemType type):SeisBasicGraphicsItem(type,pleft, pright)
{
    SeisPointItem *point = new SeisPointItem(this, m_edge, SeisPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new SeisPointItem(this, m_center, SeisPointItem::Center));

    point = new SeisPointItem(this, m_special, SeisPointItem::Special);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.setRandColor();
}

QRectF DWLine::boundingRect() const
{
	qreal bleft=qMin(m_special.x(),m_edge.x());
	qreal btop=qMin(m_special.y(),m_edge.y());
	qreal bwidth=abs(m_special.x()-m_edge.x());
	qreal bheight=abs(m_special.y()-m_edge.y());
    return QRectF(bleft,btop,bwidth,bheight);
}

void DWLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    painter->drawLine(m_center,m_edge);
    painter->drawLine(m_center,m_special);
}

void DWLine::SlotPushPoint(QPointF p, QList<QPointF> list)
{
	qDebug()<<p;
	if(list.size()<2)
	{
		this->setEdge(p);
	}
	else
		this->setSpecial(p);
	this->update();
}
//------------------------------------------------------------------------------

BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : SeisBasicGraphicsItem(QPointF(x,y), QPointF(x+width/2,y+height/2), type)
{
    SeisPointItem *point = new SeisPointItem(this, m_edge, SeisPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new SeisPointItem(this, m_center, SeisPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BEllipse::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void BEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawEllipse(ret);
}

void BEllipse::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{}

//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type)
    : BEllipse(x, y, radius*sqrt(2), radius*sqrt(2), type)
{
    updateRadius();
}

void BCircle::updateRadius()
{
    m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF BCircle::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawEllipse(ret);
}

void BCircle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* radius_spinBox = new QSpinBox(menu);
    radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius_spinBox->setRange(0, 1000);
    radius_spinBox->setPrefix("r: ");
    radius_spinBox->setSuffix(" mm");
    radius_spinBox->setSingleStep(1);
    radius_spinBox->setValue(m_radius);
    connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_radius = v;

        if (m_edge.x() < 0) {
            m_edge.setX(m_center.x() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setX(m_center.x() + m_radius * sqrt(2)/2);
        }

        if (m_edge.y() < 0) {
            m_edge.setY(m_center.y() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setY(m_center.y() + m_radius * sqrt(2)/2);
        }

        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius_spinBox);
    menu->addAction(radius_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}


BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : SeisBasicGraphicsItem(QPointF(x,y), QPointF(width/2,height/2), type)
{
    SeisPointItem *point = new SeisPointItem(this, m_edge, SeisPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new SeisPointItem(this, m_center, SeisPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BRectangle::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void BRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawRect(ret);
}

void BRectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BPolygon::BPolygon(ItemType type)
    : SeisBasicGraphicsItem(QPointF(0,0), QPointF(0,0), type),m_radius(0)
{
    is_create_finished = false;
}

void BPolygon::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!is_create_finished) {
        m_center = getCentroid(list);
        getMaxLength();

        if (isCenter) {
            m_pointList.append(new SeisPointItem(this, m_center, SeisPointItem::Center));
            m_pointList.setRandColor();
            is_create_finished = true;
        } else {
            SeisPointItem *point = new SeisPointItem(this, p, SeisPointItem::Edge);
            point->setParentItem(this);
            m_pointList.append(point);
            m_pointList.setColor(QColor(0, 255, 0));
        }

        this->update();
    }
}

QPointF BPolygon::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}

void BPolygon::getMaxLength()
{
    QVector<qreal> vec;
    for (auto &temp : m_pointList)
    {
        qreal dis = sqrt(pow(m_center.x() - temp->x(), 2) + pow(m_center.y() - temp->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    m_radius = ret;
}

void BPolygon::updatePolygon(QPointF origin, QPointF end)
{
    QList<QPointF> list;

    for (auto &temp : m_pointList) {
        if (temp->getPoint() == origin) {
            temp->setPoint(end);
        }
        list.append(temp->getPoint());
    }

    m_center = getCentroid(list);
    m_pointList.at(m_pointList.size()-1)->setPoint(m_center);

    getMaxLength();
}

QRectF BPolygon::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    if (is_create_finished) {
        for (int i = 1; i < m_pointList.size() - 1; i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }

        painter->drawLine(m_pointList.at(m_pointList.size()-2)->getPoint(), m_pointList.at(0)->getPoint());
    } else {
        for (int i = 1; i < m_pointList.size(); i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }
    }
}
