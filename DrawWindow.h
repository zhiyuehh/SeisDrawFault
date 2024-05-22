/*
 * DrawWindow.h
 *
 *  Created on: May 18, 2024
 *      Author: huangsh
 */

#ifndef DRAWLINE_DRAWWINDOW_H_
#define DRAWLINE_DRAWWINDOW_H_

#include <qmainwindow.h>

#include "../SeisDrawFault/SeisDrawGraphicsScene.h"

class DrawWindow: public QMainWindow {
public:
	DrawWindow();
	virtual ~DrawWindow();
private:
	void createToolBar();

private slots:
	void AddFaultline();
	void SlotAddLine();
	void SlotStopAdd();
	void SlotClear();
	void SlotMergeFault();
private:
	SeisDarwGraphicsScene *m_scene;
	QList<SeisBasicGraphicsItem *> m_lineItems;
};

#endif /* DRAWLINE_DRAWWINDOW_H_ */
