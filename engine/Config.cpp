#include "Config.h"

IConfig	*cfg = 0;

//-----------------------------------------------------
//  nsGameVar::nsGameVar:  
//-----------------------------------------------------
nsGameVar::nsGameVar( const char* name, const char* defValue, uint flags )
{
	assert( name != 0 && defValue != 0 );
	m_name = strdup( name );
	m_defValue = strdup( defValue );
	m_currValue = strdup( defValue );
	value = (float)atof( m_currValue );
	m_flags = flags;
}

//-----------------------------------------------------
//  nsGameVar::~nsGameVar:  
//-----------------------------------------------------
nsGameVar::~nsGameVar()
{
	if ( m_name ) free( m_name );
	if ( m_defValue ) free( m_defValue );
	if ( m_currValue ) free( m_currValue );
}

//-----------------------------------------------------
//  nsGameVar::GetString:  
//-----------------------------------------------------
const char* nsGameVar::GetString()
{
	return m_currValue;
}

//-----------------------------------------------------
//  nsGameVar::SetValue:  
//-----------------------------------------------------
void nsGameVar::SetValue( float val )
{
	if ( val == (int)val )
		SetString( StrPrintf( "%i", (int)val) );
	else
		SetString( StrPrintf( "%f", val ) );
}

//-----------------------------------------------------
//  nsGameVar::SetString:  
//-----------------------------------------------------
void nsGameVar::SetString( const char* str )
{
	assert( str != 0 );
	free( m_currValue );
	m_currValue = strdup( str );
	value = (float)atof( m_currValue );
}

//-----------------------------------------------------
//  nsGameVar::GetDefaultValue:  
//-----------------------------------------------------
float nsGameVar::GetDefaultValue()
{
	return (float)atof( m_defValue );
}

//-----------------------------------------------------
//  nsGameVar::GetDefaultString:  
//-----------------------------------------------------
const char* nsGameVar::GetDefaultString()
{
	return m_defValue;
}


//-----------------------------------------------------
//  nsConfig::nsConfig:  
//-----------------------------------------------------
nsConfig::nsConfig()
{

}

//-----------------------------------------------------
//  nsConfig::~nsConfig:  
//-----------------------------------------------------
nsConfig::~nsConfig()
{

}

//-----------------------------------------------------
//  nsConfig::Init:  
//-----------------------------------------------------
bool nsConfig::Init()
{
	if ( !cfg )
	{
		cfg = new nsConfig;
		if ( !cfg ) return false;
	}
	return true;
}

//-----------------------------------------------------
//  nsConfig::Release:  
//-----------------------------------------------------
void nsConfig::Release( bool saveVars )
{
	if ( cfg )
	{
		if ( saveVars )
			cfg->SaveConfig();
		nsConfig	*native = (nsConfig*)cfg;
		delete native;
		cfg = 0;
	}
}

//-----------------------------------------------------
//  nsConfig::FindCommand:  
//-----------------------------------------------------
cmd_t nsConfig::FindCommand( const char* cmdName )
{
	return 0;
}

//-----------------------------------------------------
//  nsConfig::RegisterVariable:  
//-----------------------------------------------------
IGameVar* nsConfig::RegisterVariable( const char* varName, const char* defValue, uint flags )
{
	if ( !varName || !defValue || !strlen( varName ) ) return 0;
	nsGameVar	*var = (nsGameVar*)FindVariable( varName );
	if ( var ) return var;

	var = new nsGameVar( varName, defValue, flags );
	m_varHash.SetData( varName, var );
	return var;
}

//-----------------------------------------------------
//  nsConfig::FindVariable:  
//-----------------------------------------------------
IGameVar* nsConfig::FindVariable( const char* varName )
{
	if ( !varName ) return 0;
	return m_varHash.GetData( varName );
}

//-----------------------------------------------------
//  nsConfig::RegisterCommand:  
//-----------------------------------------------------
void nsConfig::RegisterCommand( cmd_t cmd )
{
}

//-----------------------------------------------------
//  nsConfig::ExecLine:  
//-----------------------------------------------------
void nsConfig::ExecLine( const char* cmdLine )
{
}

//-----------------------------------------------------
//  nsConfig::SaveConfig:  
//-----------------------------------------------------
void nsConfig::SaveConfig( const char* fileName )
{
	if ( !fileName )
		fileName = StrPrintf( "%s.cfg", APP_NAME );

	app->Log( PRN_ALL, "...writing config '%s'\n", fileName );

	FILE*	fp = fopen( fileName, "w" );
	if ( !fp )
	{
		app->Log( PRN_ALL, "WARNING: can't create config file!\n" );
		return;
	}

	if ( m_varHash.FetchBegin() )
	{
		nsGameVar	*var;
		while ( var = m_varHash.FetchNext() )
		{
			fprintf( fp, "%s = \"%s\"\n", var->m_name, var->GetString() );
			delete var;
		}
	}
	m_varHash.FetchEnd();
	m_varHash.DeleteAllKeys();
	
	fclose( fp );
}


