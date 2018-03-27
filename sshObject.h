#ifndef SSH_OBJECT_H
#define SSH_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#ifdef DEBUG
void printfLog(const char *strLog, ...);
void initLog();
void exitLog();
#define PRINTF(format,...)  printfLog("[%d](%s):" format,__LINE__,__FUNCTION__,##__VA_ARGS__)
#else
#define PRINTF(format,...)
#endif


class Object
{
	#define SSH_KEY_DONW	0
	#define SSH_KEY_UP		1
	#define SSH_KEY_LEFT	2
	#define SSH_KEY_RIGHT	3
	#define SSH_KEY_ENTER	4
public :
	Object();
	Object(int x, int y, int w, int h)
	{
		m_nX = x;
		m_nY = y;
		m_nWidth = w;
		m_nHeight = h;
	}
	
	virtual void eventFilter(int value) = 0;
	
	int getX(){return m_nX;}
	int getY(){return m_nY;}
	int getWidth(){return m_nWidth;}
	int getHeight(){return m_nHeight;}
private:
	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;
};


#endif