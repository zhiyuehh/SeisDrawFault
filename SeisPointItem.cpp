#include "../SeisDrawFault/SeisPointItem.h"

#include <QDebug>

#include "../SeisDrawFault/SeisBasicGraphicsItem.h"
SeisPointItem::SeisPointItem(QAbstractGraphicsShapeItem* parent, QPointF p, PointType type)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(p)
    , m_type(type)
{
    this->setPos(m_point);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);

    switch (type) {
    case Center:
        this->setCursor(Qt::OpenHandCursor);
        break;
    case Edge:
        this->setCursor(Qt::PointingHandCursor);
        break;
    case Special:
        this->setCursor(Qt::PointingHandCursor);
        break;
    default: break;
    }
}

QRectF SeisPointItem::boundingRect() const
{
    return QRectF(-4, -4, 8, 8);
}

void SeisPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());
    this->setPos(m_point);

    switch (m_type) {
    case Center:
        painter->drawEllipse(-4, -4, 8, 8);
        break;
    case Edge:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    case Special:
        painter->drawRect(QRectF(-4, -4, 8, 8));
        break;
    default: break;
    }
}

void SeisPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton )
    {
    	qreal dx = event->scenePos().x() - event->lastScenePos().x();
    	qreal dy = event->scenePos().y() - event->lastScenePos().y();

    	SeisBasicGraphicsItem* item = static_cast<SeisBasicGraphicsItem *>(this->parentItem());
    	SeisBasicGraphicsItem::ItemType itemType = item->getType();

    	switch (m_type)
    	{
			case Center:
			{
				item->moveBy(dx, dy);
				this->scene()->update();
			} break;
			case Edge:
			{
				m_point = this->mapToParent( event->pos() );
				this->setPos(m_point);
				this->scene()->update();

				switch (itemType) {
				case SeisBasicGraphicsItem::ItemType::DLine: {
					DWLine *lineitem = dynamic_cast<DWLine *>(item);
					lineitem->setEdge(m_point);
				} break;
				case SeisBasicGraphicsItem::ItemType::Line: {
					DLine *lineitem = dynamic_cast<DLine *>(item);
					lineitem->setEdge(m_point);
				} break;
				default: break;
				}
			} break;
			case Special:
			{
				m_point = this->mapToParent( event->pos() );
				this->setPos(m_point);
				this->scene()->update();

				switch (itemType)
				{
					case SeisBasicGraphicsItem::ItemType::DLine: {
						DWLine *lineitem = dynamic_cast<DWLine *>(item);
						lineitem->setSpecial(m_point);
					} break;
					default: break;
				}
			}break;
    	}
    }
}

//------------------------------------------------------------------------------

void BPointItemList::setRandColor()
{
    this->setColor(QColor(qrand()%256, qrand()%256, qrand()%256));
}

void BPointItemList::setColor(const QColor color)
{
    for (auto &temp : *this)
    {
        temp->setBrush(QBrush(color));
    }
}

void BPointItemList::setVisible(bool visible)
{
    for (auto &temp : *this)
    {
        temp->setVisible(visible);
    }
}
