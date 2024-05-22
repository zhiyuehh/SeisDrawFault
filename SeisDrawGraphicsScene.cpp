#include "../SeisDrawFault/SeisDrawGraphicsScene.h"

#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>
SeisDarwGraphicsScene::SeisDarwGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
	is_creating = false;
}

void SeisDarwGraphicsScene::startCreate()
{
	is_creating = true;
    m_list.clear();
}

void SeisDarwGraphicsScene::StopCreate()
{
	is_creating = false;
    m_list.clear();
}

void SeisDarwGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug()<<"mousePressEvent";
    if (is_creating)
    {
        QPointF p(event->scenePos().x(), event->scenePos().y());
        qDebug()<<event->scenePos().x()<<event->scenePos().y();

        switch ( event->buttons() )
        {
			case Qt::LeftButton:
			{
				m_list.push_back(p);
				emit SigSendPoint(p, m_list);
				if(m_list.size()==2)
				{
					m_list.clear();
				}
			} break;
			case Qt::RightButton:
			{
			} break;
			default: break;
        }
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}
