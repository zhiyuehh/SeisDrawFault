


#ifndef BQGRAPHICSSCENE_H
#define BQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "../SeisDrawFault/SeisPointItem.h"
#include "../SeisDrawFault/SeisBasicGraphicsItem.h"

class SeisDarwGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    SeisDarwGraphicsScene(QObject *parent = nullptr);

    void startCreate();
    void StopCreate();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void SigSendPoint(QPointF p, QList<QPointF> list);

protected:
    QList<QPointF> m_list;
    bool is_creating;
};

#endif // BQGRAPHICSSCENE_H
