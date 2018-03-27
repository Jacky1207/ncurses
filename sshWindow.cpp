#include "sshWindow.h"
#include <unistd.h>
IWindow::IWindow() : 
		m_nIndex(0)
{
	
}

IWindow::~IWindow()
{
	
}

void IWindow::init()
{
	m_pHead = new IWidget(0, 0, COLS, HEAD_HEIGHT);
	m_pHead->setText("|-- Star Dcu --|");

	m_pMain = new IWidget(0, HEAD_HEIGHT, COLS-ADD_WIDTH, LINES-HEAD_HEIGHT-BTM_HEIGHT);
	m_pMain->setCurrentPage(PAGE_MENU);
	m_pMain->show();	
	
	m_pBottom = new IWidget(0, LINES-BTM_HEIGHT, COLS, BTM_HEIGHT);
	m_pBottom->initSelectMenu();
	
	m_pAddition = new IWidget(COLS-ADD_WIDTH, HEAD_HEIGHT, ADD_WIDTH, LINES-HEAD_HEIGHT-BTM_HEIGHT);
	m_pAddition->setText("service:", 2, 2);
	m_pAddition->setText("0  ->   close", 2, 3);
	m_pAddition->setText("1  ->   open", 2, 4);
	
	m_pAddition->setText("description:", 2, 8);
	m_pAddition->setText("<Enter> select submenu", 2, 9);
	m_pAddition->setText("<Ctrl+C>  will exit", 2, 10);
	show();
}

void IWindow::setCurrentIndex(int index)
{
	int selectId = m_pBottom->getActiveItem();
	int menuId = m_pMain->getActiveItem();
	PRINTF("%d - %d", menuId, selectId);
	if(selectId == 0)
	{
		if(m_pMain->currentPageId() == PAGE_MENU)
		{
			jumpPage(menuId);
			m_nIndex = menuId;
			
			//set curse in right position.
			forceEvent();

		}
		else if(m_pMain->currentPageId() == PAGE_DEVICE)
		{
			if(menuId == 0)
				m_pMain->postModify(HARDWARE, "hardinit");
			else if(menuId == 1)
				m_pMain->postModify(DATAINIT, "datainit");
			else if(menuId == 2)
				m_pMain->postModify(PARAMINIT, "paraminit");

			PRINTF("jumppage to MENU from  DEV");
			jumpPage(PAGE_MENU);
		}
	}
	else if(selectId == 1)
	{
		clientFinish();
		/**
		 *	check if has modified.
		 */
		m_pMain->postModify();
		PRINTF("jumppage to MENU");
		jumpPage(PAGE_MENU);
		moveToCurrentItem();
	}
}

void IWindow::show()
{
	m_pHead->show();
	m_pMain->show();
	m_pBottom->show();
	m_pAddition->show();
}

void IWindow::moveToCurrentItem()
{
	for(int i=0;i<m_nIndex;i++)
	{
		m_pMain->eventFilter(KEY_DOWN);
	}
}

void IWindow::startEventFilter(int type, int value)
{
	//start event
	/**
	 *	the form will update its field buffer when
	 *	the cursor leaved.
	 *	so we need to update it by hand.
	 */
	int ret = 0; 
	#if 0 
	if(type == SUBMENU && m_pMain->currentPageId() != PAGE_MENU)
	{
		m_pBottom->eventFilter(value);
		ret = 1;
	}
	else if(type == MAINMENU)
	{
		//main menu donot deal with enter
		if(value != 10)
			m_pMain->eventFilter(value);
	}
	
	if(ret == 1)
	{
		forceEvent();
	}
	
	if(value == 10 )		//enter
	{
		m_pBottom->eventFilter(value);
		//mv curse to the first item
		m_pBottom->eventFilter(KEY_LEFT);
	}
	#endif
	if(type == SUBMENU )
	{
		if(m_pMain->currentPageId() != PAGE_MENU)
			forceEvent();
		m_pBottom->eventFilter(value);
		//mv curse to the first item
		if(value == 10)
			m_pBottom->eventFilter(KEY_LEFT);
	}
	else if(type == MAINMENU)
	{
		m_pMain->eventFilter(value);
	}
}

void IWindow::forceEvent()
{
	PRINTF("force update!");
	/* force update the buffer.
	 *
	 *	just use in form. menu doesn't support.
	 */
	m_pMain->eventFilter(KEY_DOWN, 0);
	m_pMain->eventFilter(KEY_UP, 0);
}

void IWindow::setFormText(const char *key, const char *val)
{
	m_pMain->updateForm(key, val);
}

void IWindow::clientFinish()
{
	m_pMain->finishJob();
}

/**
 *	jump page 
 *	repaint main area
 */
void IWindow::jumpPage(int index)
{
	delete m_pMain;
	m_pMain = NULL;
	m_pMain = new IWidget(0, HEAD_HEIGHT, COLS-ADD_WIDTH, LINES-HEAD_HEIGHT-BTM_HEIGHT);
	m_pMain->setCurrentPage(index);
	show();
}