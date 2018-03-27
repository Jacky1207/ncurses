#ifndef SSH_MENU_H
#define SSH_MENU_H

#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "sshObject.h"

class IMenu
{
public:
	IMenu(int count);
	~IMenu();
		
	void addItem(const char* name);
	
	//callback for jump page
	typedef void(* selectPageFun)(int page);
	void registerPageFun(selectPageFun fun){ _readyReadFun = fun;}
	
	void onPaint();
	void show(WINDOW* win, int x, int y, int w, int h);
	void setMenuFormat(int row, int col);
	//return if 
	int eventFilter(int value);
	
	void setMenuId(int id){m_nId = id;}
	int getMenuId(){return m_nId;}
	
	int getActiveMenuItem();
private:
	ITEM **	m_pItems;
	MENU *m_pMenu;
	//father window
	WINDOW* m_pParent;
	//item count of menu
	int m_nCount;
	//current index of item
	int m_nIndex;
	
	int m_nId;
	
	//
	selectPageFun _readyReadFun;
};
	

#endif