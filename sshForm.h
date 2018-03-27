#ifndef SSH_FORM_H
#define SSH_FORM_H

#include <form.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "sshObject.h"

#include "network/IList.h"
#include "network/IString.h"
#include "network/IMap.h"

using namespace IObject::ISystemBase;
using namespace std;
class IForm
{
public:
	IForm(int count, int id);
	~IForm();
		
	void addForm(const char* name, int frow, int fcol, int type);
	
	void show(WINDOW *p);
	void eventFilter(int value);
	
	void setObjectName(const char *obj);
	void setText(const char *key, const char *val);
	
	int getId(){return m_nId;}
	
	int getModifyForm(char *str);
	
	//check input of form
#define 	CHECK_NONE		0
#define 	CHECK_IP		1		//TYPE_ENUM
#define 	CHECK_CHAR		2		//TYPE_ALNUM
#define 	CHECK_CTRL		3		//TYPE_INTEGER
#define 	CHECK_INT		4		//TYPE_INTEGER
private:
	FIELD 	*m_pField[20];
	FORM 	*m_pForm;
	
	WINDOW  *m_pParent;
	//item count of menu
	int m_nCount;
	//current index of item
	int m_nIndex;
	
	int m_nFcol;
	int m_nFrow;
	
	//id
	int m_nId;
	//lable max length
	int m_nLableLength;
	char m_strName[30];
	
//	std::list<IString > m_pObjNameList;
	IList<IString> m_pObjNameList;
	IMap<IString, IString> m_pObjValueList;
	
	int compareForm(const char *src, const char *dst);
	int checkFormValue(FIELD *form, int type);
	void dealForTextBuffer(char *buf);
};
	

#endif