#include "sshForm.h"

IForm::IForm(int count, int id) : 
	m_nIndex(0),
	m_pParent(NULL),
	m_nFrow(0),
	m_nFcol(0),
	m_nLableLength(0)
{
	m_nCount = count;
	m_nId = id;
	m_pField[count*2] = NULL;
	memset(m_strName, 0 , sizeof(m_strName));
}

IForm::~IForm()
{
	m_pObjNameList.clear();
	m_pObjValueList.clear();
	
	unpost_form(m_pForm);
	free_form(m_pForm);
	
	for(int i = 0; i < m_nCount*2; i++)
	{
		PRINTF("~IForm : index = %d", i);
		free_field(m_pField[i]);
	}
}

/**
 *	add form into current window.
 *	frow : y
 *	fcol : x
 * 	type : form input type
 *  the lable of form MUST after 
 */
void IForm::addForm(const char* name, int frow, int fcol, int type)
{
	if(m_nIndex >= m_nCount)
		return;
	//set max length of lable.
	if(m_nLableLength < strlen(name))
		m_nLableLength = strlen(name);
		
	//new edit form
	m_pField[m_nIndex] = new_field(1, 32, frow, fcol, 0, 0);
	set_field_back(m_pField[m_nIndex], A_UNDERLINE);
	field_opts_off(m_pField[m_nIndex], O_AUTOSKIP);
	mvprintw(frow+4, 2, name);

	checkFormValue(m_pField[m_nIndex], type);
	
	//set font color
//	init_pair(1, COLOR_WHITE, COLOR_BLUE);
//	set_field_fore(m_pField[m_nIndex],COLOR_PAIR(1));
	
	//new edit lable
	m_pField[m_nCount+m_nIndex] = new_field(1, 20, frow, fcol-20, 0, 0);
	set_field_buffer(m_pField[m_nCount+m_nIndex], 0, name);
	//disable lable form area.
	set_field_opts(m_pField[m_nCount+m_nIndex], (int) ((unsigned) field_opts(m_pField[m_nCount+m_nIndex]) & ~O_ACTIVE));
	
	refresh();
	m_nIndex ++;
}

void IForm::setObjectName(const char *obj)
{
	m_pObjNameList.append(IString(obj));
	PRINTF("%d %s", m_pObjNameList.size(), m_pObjNameList.last().data());
	m_pObjValueList.append(obj, "");
//	memcpy(m_strName, obj, strlen(obj));
}

void IForm::setText(const char *key, const char *val)
{
	for(int i=0; i< m_pObjNameList.size(); i++)
	{
		if(strcmp(key, m_pObjNameList.at(i).data()) == 0)
		{
			PRINTF("set %s -> %s", key, val);
			set_field_buffer(m_pField[i], 0, val);
			wrefresh(m_pParent);
			m_pObjValueList.setValue(key, val);
#if 0
			char *t = field_buffer(m_pField[i], 0);
			for(int i=0; i<strlen(t); i++)
			{
				PRINTF("buf[%d] = %c 0x%x ", i,t[i] ,t[i]);
			}
#endif
		}
	}
}

int IForm::getModifyForm(char *str)
{
	int ret = 0;
	PRINTF("form id = %d", m_nId);
	if(m_nId == 13)	//SSHLOGIN
	{
		//just conform the input are same.
		char *old = field_buffer(m_pField[0], 0);
		char *set = field_buffer(m_pField[1], 0);
		PRINTF("old - %s:new - %s", old , set);
		dealForTextBuffer(old);
		dealForTextBuffer(set);
		if(strcmp(old, set) == 0 && strlen(old) != 0)
		{
			char tmp[100] = {0};
			sprintf(tmp, "ssh_password=%s\n", set);
			strcat(str, tmp);
			return 1;
		}
		return 0;
	}
	for(int i=0;i < m_pObjValueList.size(); i++ )
	{
		IString value = m_pObjValueList.value(m_pObjNameList.at(i));
		PRINTF("value = %s", value.data());
		
		// field buffer len is likly always be length of form.
		char *buf = field_buffer(m_pField[i], 0);
		int state = field_status(m_pField[i]);
		dealForTextBuffer(buf);
		PRINTF("text = %s | %d:%d   %d", buf, strlen(buf), strlen(value.data()), state);
		if(strcmp(value.data(), buf) != 0)
		{
			char tmp[100] = {0};
			sprintf(tmp, "%s=%s\n",m_pObjNameList.at(i).data(), buf);
			strcat(str, tmp);
			PRINTF("%s", str);
			ret = 1;
		}			
	}
	
	return ret;
}
//the form buff need transform before use
void IForm::dealForTextBuffer(char *buf)
{
	for(int i=0; i< strlen(buf); i++)
	{
		if(buf[i] == 0x20)
			buf[i] = 0x0;
	}
}

int IForm::compareForm(const char *src, const char *dst)
{
	int i=0;
	for(; i< strlen(src); i++)
	{
		if(src[i] != dst[i])
		{
			return 0;
		}
	}
	if(dst[i] != 0x20)
	{
		return 0;
	}
	return 1;
}

void IForm::show(WINDOW *p)
{
	m_pForm = new_form(m_pField);
	scale_form(m_pForm, &m_nFrow, &m_nFcol);
	
	PRINTF("row = %d, col = %d", m_nFrow, m_nFcol);
	/* 设置主窗口和子窗口*/
	set_form_win(m_pForm, p);
	set_form_sub(m_pForm, derwin(p, m_nFrow, m_nFcol, 2, 2));
//	set_current_field(m_pForm,m_pField[0]);
	post_form(m_pForm);
	
//	form_driver(m_pForm,REQ_END_LINE);
	refresh();
	
	m_pParent = p;
}

void IForm::eventFilter(int value)
{
	switch(value)
	{
	case KEY_DOWN:	//258
		PRINTF("FORM NEXT");
		form_driver(m_pForm,REQ_NEXT_FIELD);
		form_driver(m_pForm,REQ_END_LINE);
		break;
	case KEY_UP:	//259
		PRINTF("FORM PREV");
		form_driver(m_pForm,REQ_PREV_FIELD);
		form_driver(m_pForm,REQ_END_LINE);
		break;
	case KEY_BACKSPACE:
		PRINTF("FORM BACK");
		form_driver(m_pForm,REQ_DEL_PREV);
		form_driver(m_pForm,REQ_END_LINE);
		break;
	default:
		/* 如果输入的是普通字符, 就把它打印出来*/
		form_driver(m_pForm,value);
		break;
	}
	//must refresh parent window.
	wrefresh(m_pParent);
}

int IForm::checkFormValue(FIELD *form, int type)
{
	switch(type)
	{
		case CHECK_IP:
		{
//			char reg[] = "^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9]).){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])$";
			char reg[] = "^((0|1[0-9]{0,2}|2[0-9]{0,1}|2[0-4][0-9]|25[0-5]|[3-9][0-9]{0,1})\\.){3}(0|1[0-9]{0,2}|2[0-9]{0,1}|2[0-4][0-9]|25[0-5]|[3-9][0-9]{0,1})(?(\\/)\\/([0-9]|[1-2][0-9]|3[0-2])|)$";
//			char reg[] = "^(([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+))$";
			set_field_type(form, TYPE_REGEXP, reg);
			break;
		}
		case CHECK_CHAR:
		{
			set_field_type(form, TYPE_ALNUM ,0);
			break;
		}
		case CHECK_CTRL:
		{
			set_field_type(form, TYPE_INTEGER, 1, 0, 1);
			break;
		}
		case CHECK_INT:	//form port
		{
			set_field_type(form, TYPE_INTEGER, 0, 1000, 65535);
			break;
		}
		default:
			break;
	}
}