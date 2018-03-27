#include "sshWidget.h"

void *eventFilterThread(void *param);

IWidget::IWidget(int x, int y, int w, int h) :
	Object(x ,y ,w ,h),
	m_pMenu(NULL),
	m_pForm(NULL),
	isActive(true),
	m_pWindow(NULL),
	m_pInterface(NULL),
	m_nMenuIndex(PAGE_MENU)
{
	PRINTF("width = %d, height = %d", w, h);
	m_pWindow = newwin(h, w, y, x);
	/* 0, 0 是字符默认的行列起始位置*/
	box(m_pWindow, 0 , 0);
	keypad(m_pWindow,TRUE);
	PRINTF("IWidget() finish");
	
//	pthread_create(&_pThread,NULL,eventFilterThread,(void *)this);
}

IWidget::~IWidget()
{
	isActive = false;
	if(m_pWindow != NULL)
		close();
	wborder(m_pWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(m_pWindow);
	delwin(m_pWindow);
	m_pWindow = NULL;
}

void IWidget::setText(const char* str)
{
	mvwprintw(m_pWindow, getHeight()/2, getWidth()/2 - strlen(str)/2, str);
}

void IWidget::setText(const char* str, int x, int y)
{
	mvwprintw(m_pWindow, y, x, str);
}

void IWidget::show()
{
	/*刷新窗口缓冲，显示box */
	wrefresh(m_pWindow);
}

void IWidget::close()
{
	if(m_pMenu != NULL)
	{
		PRINTF("close Menu!");
		delete m_pMenu;
		m_pMenu = NULL;
	}
	if(m_pForm != NULL)
	{
		PRINTF("close Form!");
		delete m_pForm;
		m_pForm = NULL;
	}
	finishJob();
}

void IWidget::eventFilter(int value)
{
	if(m_pMenu != NULL)
	{
		m_pMenu->eventFilter(value);
	}
	if(m_pForm != NULL)
	{
		m_pForm->eventFilter(value);
	}
}

/* this filter used for forceEvent */
void IWidget::eventFilter(int value, int type)
{
	if(m_pForm != NULL)
	{
		m_pForm->eventFilter(value);
	}
}


void IWidget::finishJob()
{
	if(m_pInterface != NULL)
	{
		delete m_pInterface;
		m_pInterface = NULL;
		PRINTF("close socket!");
	}
}

void IWidget::setCurrentPage(int index)
{
	close();
	if(index < PAGE_MENU &&index != PAGE_LOGIN)
	{
		//connect net.
		initNetwork();
	}
	switch(index)
	{
		case PAGE_MENU:
		{
			initPageMenu();
			break;
		}
		case PAGE_DEVICE:
		{
			initPageDev();
			break;
		}
		case PAGE_COMM:
		{
			initPageComm();
			break;
		}
		case PAGE_VPN:
		{
			initPageVpn();
			break;
		}
		case PAGE_SNMP:
		{
			initPageSnmp();
			break;
		}
		case PAGE_DHCP:
		{
			initPageDhcp();
			break;
		}
		case PAGE_NTP:
		{
			initPageNtp();
			break;
		}
		case PAGE_LOGIN:
		{
			initPageLogin();
			break;
		}
		default:
			break;
	}
	show();
}

void IWidget::initNetwork()
{
	m_pInterface = new IInterface();
	m_pInterface->init();
}

void IWidget::initNetwork(int type)
{
	finishJob();
	m_pInterface = new IInterface(type);
}

void IWidget::initSelectMenu()
{
	m_pMenu = new IMenu(2);
	m_pMenu->addItem("Select");
	m_pMenu->addItem("Exit");
	m_pMenu->setMenuFormat(1, 2);
	m_pMenu->show(m_pWindow, 4, 1, 60, 1);
	m_nMenuIndex = PAGE_SELECT;
}

void IWidget::initPageMenu()
{
	PRINTF("Menu PAGE");
	int count = 7;
	m_pMenu = new IMenu(count);
	m_pMenu->addItem("Device Info");
	m_pMenu->addItem("Communication");
	m_pMenu->addItem("VPN Service");
	m_pMenu->addItem("SNMP Service");
	m_pMenu->addItem("DHCP Service");
	m_pMenu->addItem("NTP Service");
	m_pMenu->addItem("Login");
	m_pMenu->setMenuFormat(count, 1);
	m_pMenu->show(m_pWindow,  4, (getHeight()-7)/2, 45, count);
	m_pMenu->setMenuId(MENU_MAIN);
	PRINTF("Menu PAGE END!");
	m_nMenuIndex = PAGE_MENU;
}

void IWidget::initPageDev()
{
	PRINTF("DEV PAGE");
	m_pForm = new IForm(1, LOGIC);
	m_pForm->addForm("Logic Dev Name:", 1, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("logic_name");
	m_pForm->show(m_pWindow);
	m_pMenu = new IMenu(3);
	m_pMenu->addItem("Dcu Hardinit");
	m_pMenu->addItem("Dcu Datainit");
	m_pMenu->addItem("Dcu paraminit");
	m_pMenu->setMenuFormat(3, 1);
	m_pMenu->show(m_pWindow,  2, 5, 45, 3);
	
	m_pInterface->getRequest(LOGIC);
	PRINTF("DEV PAGE END!");
	m_nMenuIndex = PAGE_DEVICE;
}

void IWidget::initPageComm()
{
	PRINTF("COMM PAGE");
	m_pForm = new IForm(8, COMM);
	m_pForm->addForm("Master IP:", 1, TO_LEFT, CHECK_IP);
	//set obj name
	m_pForm->setObjectName("hes_master_ip");
	m_pForm->addForm("Master Port:", 3, TO_LEFT, CHECK_INT);
	m_pForm->setObjectName("hes_master_port");
	m_pForm->addForm("Slave IP:", 5, TO_LEFT, CHECK_IP);
	m_pForm->setObjectName("hes_slave_ip");
	m_pForm->addForm("Slave Port:", 7, TO_LEFT, CHECK_INT);
	m_pForm->setObjectName("hes_slave_port");
	m_pForm->addForm("APN:", 9, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("apn");
	m_pForm->addForm("PPP User:", 11, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("ppp_usr");
	m_pForm->addForm("PPP Pass:", 13, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("ppp_password");
	m_pForm->addForm("Heart circle:", 15, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("heart_circle");
	
	m_pInterface->getRequest(COMM);
	m_pForm->show(m_pWindow);
	PRINTF("COMM PAGE END!");
	m_nMenuIndex = PAGE_COMM;
}

void IWidget::initPageVpn()
{
	PRINTF("VPN PAGE");
	m_pForm = new IForm(4, VPN);
	m_pForm->addForm("VPN Service:", 1, TO_LEFT, CHECK_CTRL);
	m_pForm->setObjectName("vpn_ctrl");
	m_pForm->addForm("VPN Address:", 3, TO_LEFT, CHECK_IP);
	m_pForm->setObjectName("vpn_server_ip");
	m_pForm->addForm("VPN Subnet:", 5, TO_LEFT, CHECK_NONE);
	m_pForm->setObjectName("vpn_subnet");
	m_pForm->addForm("Pre-Shared:", 7, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("vpn_pre_shared");
	m_pInterface->getRequest(VPN);
	
	m_pForm->show(m_pWindow);
	PRINTF("VPN PAGE END!");
	m_nMenuIndex = PAGE_VPN;
}

void IWidget::initPageSnmp()
{
	PRINTF("SNMP PAGE");
	m_pForm = new IForm(4, SNMP);
	m_pForm->addForm("SNMP Service:", 1, TO_LEFT, CHECK_CTRL);
	m_pForm->setObjectName("snmp_ctrl");
	m_pForm->addForm("V3 SNMP User:", 3, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("snmp_user");
	m_pForm->addForm("V3 Password:", 5, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("snmp_authentication");
	m_pForm->addForm("V3 DES Key:", 7, TO_LEFT, CHECK_CHAR);
	m_pForm->setObjectName("snmp_encryption");
	m_pInterface->getRequest(SNMP);
	
	m_pForm->show(m_pWindow);
	PRINTF("SNMP PAGE END!");
	m_nMenuIndex = PAGE_SNMP;
}

void IWidget::initPageDhcp()
{
	PRINTF("DHCP PAGE");
	m_pForm = new IForm(4, DHCP);
	m_pForm->addForm("DHCP Service:", 1, TO_LEFT, CHECK_CTRL);
	m_pForm->setObjectName("dhcp_ctrl");
	m_pForm->addForm("DCU IP :", 3, TO_LEFT, CHECK_IP);
	m_pForm->setObjectName("dcu_ip");
	m_pForm->addForm("DCU Mask:", 5, TO_LEFT, CHECK_IP);
	m_pForm->setObjectName("dcu_subnet");
	m_pForm->addForm("DCU Gateway:", 7, TO_LEFT, CHECK_IP);
	m_pForm->setObjectName("dcu_gateway");
	m_pInterface->getRequest(DHCP);
	
	m_pForm->show(m_pWindow);
	PRINTF("DHCP PAGE END!");
	m_nMenuIndex = PAGE_DHCP;
}

void IWidget::initPageNtp()
{
	m_pForm = new IForm(2, NTP);
	m_pForm->addForm("NTP Service:", 1, TO_LEFT,CHECK_CTRL);
	m_pForm->setObjectName("ntp_ctrl");
	m_pForm->addForm("NTP Server:", 3, TO_LEFT,CHECK_CHAR);
	m_pForm->setObjectName("ntp_server_name");
	m_pInterface->getRequest(NTP);
	
	m_pForm->show(m_pWindow);
	m_nMenuIndex = PAGE_NTP;
}

void IWidget::initPageLogin()
{
	m_pForm = new IForm(2, SSHLOGIN);
	m_pForm->addForm("New Password:", 1, TO_LEFT, CHECK_CHAR);
//	m_pForm->setObjectName("user_name");
	m_pForm->addForm("Confirm Password:", 3, TO_LEFT, CHECK_CHAR);
//	m_pForm->setObjectName("user_pass");
//	m_pInterface->getRequest(SSHLOGIN);
	
	m_pForm->show(m_pWindow);
	m_nMenuIndex = PAGE_LOGIN;
}

int IWidget::getCurrentMenuId()
{
	return m_pMenu->getMenuId();
}

int IWidget::getActiveItem()
{
	if(m_pMenu != NULL)
		return m_pMenu->getActiveMenuItem();
}

void IWidget::updateForm(const char *obj, const char* val)
{
	if(m_pForm != NULL)
	{
		m_pForm->setText(obj, val);
	}
}

void IWidget::postModify()
{
	if(m_pForm != NULL)
	{
		char buf[1024] = {0};
		if(m_pForm->getModifyForm(buf) == 1)
		{
			PRINTF("POST %s",buf);
			initNetwork(1);
			int id = m_pForm->getId();
			m_pInterface->postRequest(id, buf, strlen(buf));
		}
	}
	finishJob();
}

void IWidget::postModify(int index, const char* buf)
{
	initNetwork(1);
	m_pInterface->postRequest(index, buf, strlen(buf));
	finishJob();
}

