#ifndef SSH_WIDGET_H
#define SSH_WIDGET_H

#include <ncurses.h>
#include "sshMenu.h"
#include "sshObject.h"
#include "sshForm.h"
#include "sshInterface.h"

class IWidget : public Object
{
public:
	IWidget(int x, int y, int w, int h);
	~IWidget();
	
	void addMenu(IMenu *menu);
	
	void show();
	void close();
	void setText(const char *str);
	void setText(const char* str, int x, int y);
#define PAGE_DEVICE		0
#define PAGE_COMM		1
#define PAGE_VPN		2
#define PAGE_SNMP		3
#define PAGE_DHCP		4
#define PAGE_NTP		5
#define PAGE_LOGIN		6
#define PAGE_MENU		10
#define PAGE_SELECT		11
	void setCurrentPage(int index);
	void initSelectMenu();
	
	void eventFilter(int value);
	void eventFilter(int value, int type);
	void updateForm(const char *obj, const char* val);
	void finishJob();
	
	void postModify();
	void postModify(int index, const char* buf);
	
	int 	currentPageId(){return m_nMenuIndex;}
	WINDOW *getWindowHandle(){return m_pWindow;}
	IMenu *getMenuHandle(){return m_pMenu;}
	IForm *getFormHandle(){return m_pForm;}
	bool active(){return isActive;}
	
#define MENU_MAIN	1
#define MENU_SUB	2
	int getCurrentMenuId();
	int getActiveItem();
private: //members
	WINDOW 	*m_pWindow;
	IMenu 	*m_pMenu;
	IForm	*m_pForm;
	pthread_t _pThread;
	bool 	isActive;
	
	//which menu page is it.
	int 	m_nMenuIndex;
	
	//interface
	IInterface *m_pInterface;
private: //functions
#define TO_LEFT		20
	void initPageDev();
	void initPageComm();
	void initPageMenu();
	void initPageVpn();
	void initPageSnmp();
	void initPageDhcp();
	void initPageNtp();
	void initPageLogin();
	
	void initNetwork();
	void initNetwork(int type);
};
	

#endif