#ifndef _COMM_CONFIG_H_
#define _COMM_CONFIG_H_

#include "../nsLib/nsLib.h"

#define	GVF_SAVABLE		1	//����������� � config.cfg, ���� cfg_savevars 1 ����������� ���...
#define GVF_READONLY	2	//���������� �� ��������������� � �������
#define GVF_INTERNAL	4	//���������� ��� ����������� ������������� (��� �������� � ��������, ��������...)

//-----------------------------------------------------
//  IGameVar:  
//-----------------------------------------------------
struct IGameVar
{
	float				value;
	
	virtual const char* GetString() = 0;
	
	virtual void		SetValue( float val ) = 0;
	virtual void		SetString( const char* str ) = 0;

	virtual float		GetDefaultValue() = 0;
	virtual const char*	GetDefaultString() = 0;
};

//-----------------------------------------------------
//  IConfig:  
//-----------------------------------------------------
struct IConfig
{
	virtual IGameVar*	RegisterVariable( const char* varName, const char* defValue, uint flags = 0 ) = 0;
	virtual IGameVar*	FindVariable( const char* varName ) = 0;

	virtual void		RegisterCommand( void (*cmd)( const char *argv[], int argc ) ) = 0;

	virtual void		ExecLine( const char* cmdLine ) = 0;

	virtual void		SaveConfig( const char* fileName = 0 ) = 0;
};

#endif