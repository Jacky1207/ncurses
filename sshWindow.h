#ifndef SSH_WINDOW_H
#define SSH_WINDOW_H

#include "sshWidget.h"
#include "sshObject.h"

class IWindow
{
public:
	IWindow();
	~IWindow();
	
	void onPaint();
	void init();
	void show();
	
#define BTM_HEIGHT		3
#define HEAD_HEIGHT		4
#define ADD_WIDTH		25
	
#define MAINMENU	0
#define SUBMENU		1
	void startEventFilter(int type, int value);
	void setCurrentIndex(int index);
	
	void setFormText(const char *key, const char *val);
	void clientFinish();
private:
	//star head
	IWidget *m_pHead;
	//main area 
	IWidget *m_pMain;
	//select buttons
	IWidget *m_pBottom;
	//
	IWidget *m_pAddition;
	//current index of page
	int m_nIndex;
	
	void jumpPage(int index);
	void moveToCurrentItem();
	void forceEvent();
};
	

#endif