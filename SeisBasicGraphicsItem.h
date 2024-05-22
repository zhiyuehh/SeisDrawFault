

#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include <QObject>
#include "../SeisDrawFault/SeisPointItem.h"

#define PI 3.1415926

// 自定义图元 - 基础类
class SeisBasicGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum ItemType {
        Circle = 0,         // 圆
        Ellipse,            // 椭圆
        Concentric_Circle,  // 同心圆
        Pie,                // 饼
        Chord,              // 和弦
        Rectangle,          // 矩形
        Square,             // 正方形
        Polygon,            // 多边形
        Round_End_Rectangle,// 圆端矩形
        Rounded_Rectangle,   // 圆角矩形
		Line,
		DLine
    };

    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }

    QPointF getEdge() { return m_edge; }
    void setEdge(QPointF p)
    {
    	m_edge = p;
    	if(m_type==DLine)
    	{
    		m_center=QPointF((m_edge.x()+m_special.x())/2,(m_edge.y()+m_special.y())/2);
    		m_pointList[1]->setPoint(m_center);
    	}
    }

    QPointF getSpecial() { return m_special; }
    void setSpecial(QPointF p)
    {
    	m_special = p;
    	if(m_type==DLine)
    	{
    		m_center=QPointF((m_edge.x()+m_special.x())/2,(m_edge.y()+m_special.y())/2);
    		m_pointList[1]->setPoint(m_center);
    		m_pointList[2]->setPoint(m_special);
    	}
    }

    ItemType getType() { return m_type; }

protected:
    SeisBasicGraphicsItem(QPointF center, QPointF edge, ItemType type);
    SeisBasicGraphicsItem(ItemType type,QPointF edge, QPointF special);

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

public:
    QPointF m_center;
    QPointF m_edge;
    QPointF m_special;
    ItemType m_type;
    BPointItemList m_pointList;

    QPen m_pen_isSelected;
    QPen m_pen_noSelected;
};

//------------------------------------------------------------------------------
class DLine:public SeisBasicGraphicsItem
{
    Q_OBJECT
public:
	DLine(QPointF pcentter,QPointF pedge, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};

class DWLine:public SeisBasicGraphicsItem
{
    Q_OBJECT
public:
	DWLine(QPointF pleft,QPointF pright, ItemType type);
protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
public slots:
	void SlotPushPoint(QPointF p, QList<QPointF> list);
};

// 椭圆
class BEllipse : public SeisBasicGraphicsItem
{
    Q_OBJECT

public:
    BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BEllipse
{
public:
    BCircle(qreal x, qreal y, qreal radius, ItemType type);

    void updateRadius();

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    qreal m_radius;
};

//------------------------------------------------------------------------------

// 矩形
class BRectangle : public SeisBasicGraphicsItem
{
public:
    BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 多边形
class BPolygon : public SeisBasicGraphicsItem
{
    Q_OBJECT

public:
    BPolygon(ItemType type);

    QPointF getCentroid(QList<QPointF> list);
    void getMaxLength();
    void updatePolygon(QPointF origin, QPointF end);

public slots:
    void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

public:
    qreal m_radius;
    bool is_create_finished;
};


#endif // BQGRAPHICSITEM_H
