#ifndef _NSCONFIG_H_
#define _NSCONFIG_H_

#include "elocal.h"

typedef void (*cmd_t)( const char *argv[], int argc );

//-----------------------------------------------------
//  class nsGameVar:  
//-----------------------------------------------------
class nsGameVar : public IGameVar
{
friend class nsConfig;

public:
	virtual const char* GetString();
	virtual void		SetValue( float val );
	virtual void		SetString( const char* str );

	virtual float		GetDefaultValue();
	virtual const char*	GetDefaultString();

private:
	char	*m_name;
	char	*m_defValue;
	char	*m_currValue;
	uint	m_flags;

private:
	nsGameVar( const char* name, const char* defValue, uint flags );
	virtual ~nsGameVar();
};

//-----------------------------------------------------
//  class nsConfig:  
//-----------------------------------------------------
class nsConfig : public IConfig  
{
public:
	static bool Init();
	static void Release( bool saveVars );

private:
	cmd_t				FindCommand( const char* cmdName );

private:
	virtual IGameVar*	RegisterVariable( const char* varName, const char* defValue, uint flags = 0 );
	virtual IGameVar*	FindVariable( const char* varName );
	virtual void		RegisterCommand( cmd_t cmd );
	virtual void		ExecLine( const char* cmdLine );
	virtual void		SaveConfig( const char* fileName = 0 );

	nsConfig();
	virtual ~nsConfig();

	nsHashMap<nsGameVar>	m_varHash;
};

extern	IConfig	*cfg;

#endif
