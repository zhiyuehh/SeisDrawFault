/*
 * DrawWindow.cpp
 *
 *  Created on: May 18, 2024
 *      Author: huangsh
 */

#include "DrawWindow.h"

#include <string>
#include <QAction>
#include <QToolBar>
#include <QDebug>

#include "SeisDrawGraphicsView.h"

DrawWindow::DrawWindow() {
	// TODO Auto-generated constructor stub
    setWindowTitle(tr("自定义图形"));
//    setMaximumSize(1024, 768);
//    setMinimumSize(1027, 768);
    m_scene=new SeisDarwGraphicsScene(this);
    m_scene->setBackgroundBrush(Qt::gray);

    SeisDrawGraphicsView *view = new SeisDrawGraphicsView();
    view->setScene(m_scene);
//    view->show();
    setCentralWidget(view);
    createToolBar();
}

DrawWindow::~DrawWindow() {
	// TODO Auto-generated destructor stub
}

void DrawWindow::createToolBar()
{
	QToolBar *toolBar = addToolBar("Toolbar");


    QAction *AddFaultline = new QAction("AddFault", this);
    QAction *AddLineact = new QAction("AddLine", this);
    QAction *ActStopAdd = new QAction("Stop", this);
    QAction *ActClear = new QAction("Clear", this);
    QAction *ActMergeFault = new QAction("MergeFault", this);

    toolBar->addAction(AddFaultline);
    toolBar->addAction(AddLineact);
    toolBar->addAction(ActStopAdd);
    toolBar->addAction(ActClear);
    toolBar->addAction(ActMergeFault);

    connect(AddFaultline, &QAction::triggered, this, &DrawWindow::AddFaultline);
    connect(AddLineact, &QAction::triggered, this, &DrawWindow::SlotAddLine);
    connect(ActStopAdd, &QAction::triggered, this, &DrawWindow::SlotStopAdd);
    connect(ActClear, &QAction::triggered, this, &DrawWindow::SlotClear);
    connect(ActMergeFault, &QAction::triggered, this, &DrawWindow::SlotMergeFault);
}

void DrawWindow::AddFaultline()
{
    m_scene->startCreate();
    connect(m_scene, &SeisDarwGraphicsScene::SigSendPoint, this, [&](QPointF pt, QList<QPointF> list)
    		{
    	qDebug()<<"listsize=="<<list.size();
    			if(list.size()<2)
    				{
    					DWLine *lineitem=new DWLine(pt,pt,SeisBasicGraphicsItem::ItemType::DLine);
    					m_scene->addItem(lineitem);
    					m_lineItems.push_back(lineitem);
    					qDebug()<<"m_lineItems.push_back(lineitem);"<<m_lineItems.size();
    				}
    			else
				{
    				DWLine *curitem=dynamic_cast<DWLine *>(m_lineItems[m_lineItems.size()-1]);
    				curitem->setSpecial(pt);
    				curitem->update();
    				qDebug()<<"setSpecial;"<<m_lineItems.size();
				}
    			m_scene->update();
    		});
}

void DrawWindow::SlotAddLine()
{
	qDebug()<<"SlotAddLine";
    DLine *lineitem=new DLine(QPointF(100,100),QPointF(200,200),SeisBasicGraphicsItem::ItemType::Line);
    m_scene->addItem(lineitem);
}

void DrawWindow::SlotClear()
{
	qDebug()<<"clear...";
    m_scene->clear();
    m_scene->StopCreate();
    m_lineItems.clear();
}

void DrawWindow::SlotStopAdd()
{
	qDebug()<<"stop...";
    m_scene->StopCreate();
}

void DrawWindow::SlotMergeFault()
{
//	m_scene->StopCreate();

	qDebug()<<"fault num=="<<m_lineItems.size();

	for(int i=0;i<m_lineItems.size();i++)
	{
		DWLine *item=dynamic_cast<DWLine *>(m_lineItems[i]);
		qDebug()<<item->m_center;
	}

	m_scene->clear();
	m_lineItems.clear();

}
