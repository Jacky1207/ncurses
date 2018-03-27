#include "sshMenu.h"
#include "sshWindow.h"

extern IWindow *g_pWindow;
IMenu::IMenu(int count) :
	m_pItems(NULL),
	m_pMenu(NULL),
	m_pParent(NULL),
	m_nIndex(0),
	m_nId(0)
{
	m_nCount = count;
	m_pItems = (ITEM **)calloc(count + 1, sizeof(ITEM *));
	m_pItems[count] = (ITEM *)NULL;
	keypad(stdscr, TRUE);
}

IMenu::~IMenu()
{
	unpost_menu(m_pMenu);
	free_menu(m_pMenu);
	for(int i = 0; i < m_nCount ;i++)
	{
		PRINTF("~IMenu : index = %d", i);
		free_item(m_pItems[i]);
	}
}

void IMenu::addItem(const char* name)
{
	if(m_nIndex >= m_nCount)
		return;
	m_pItems[m_nIndex] = new_item(name, "");
	//set user func
//	set_item_userptr(m_pItems[m_nIndex],(void*)NULL);
	m_nIndex++;
}

void IMenu::show(WINDOW* win, int x, int y, int w, int h)
{
	m_pMenu = new_menu((ITEM **)m_pItems);
	
	set_menu_win(m_pMenu,win);
	set_menu_sub(m_pMenu,derwin(win, h, w, y, x));
//	set_menu_mark(m_pMenu," * ");
	post_menu(m_pMenu);
	refresh();
	
	m_pParent = win;
}

void IMenu::setMenuFormat(int row, int col)
{
	set_menu_format(m_pMenu, row, col);
}

int IMenu::eventFilter(int value)
{
	switch(value)
	{
	case KEY_DOWN:	//258
		menu_driver(m_pMenu,REQ_DOWN_ITEM);
		break;
	case KEY_UP:	//259
		menu_driver(m_pMenu,REQ_UP_ITEM);
		break;
	case KEY_LEFT:	//260
		menu_driver(m_pMenu,REQ_LEFT_ITEM);
		break;
	case KEY_RIGHT:	//261
		menu_driver(m_pMenu,REQ_RIGHT_ITEM);
		break;
	case 10://enter
		int index = getActiveMenuItem();
		PRINTF("index = %d", index);
		g_pWindow->setCurrentIndex(index);
		return 1;
	}
	wrefresh(m_pParent);
	return 0;
}

int IMenu::getActiveMenuItem()
{
	ITEM *cur;
	cur = current_item(m_pMenu);
	pos_menu_cursor(m_pMenu);
	int index = item_index(cur);
	return index;
}
