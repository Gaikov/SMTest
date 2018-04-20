#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "utils.h"

//-----------------------------------------------------
//  class nsHashMap:  
//-----------------------------------------------------
template <class type>
class nsHashMap
{
public:
	nsHashMap();
	virtual ~nsHashMap();

	bool	SetData( const char* key, type* data );	//false - ���� ����� ���� ��� ����
	type*	GetData( const char* key ) const;		//NULL - not found
	
	bool	DeleteKey( const char* key );
	bool	DeleteAllKeys();	//����� ������� ������ ����� ��������� �����/��

	//���������������� ��������� (�������) ���������
	bool	FetchBegin() const;
	type*	FetchNext() const;
	void	FetchEnd() const;

private:
	typedef struct hashItem_s
	{
		type*		userData;
		char*		key;
		hashItem_s	*prev, *next;
	}
	hashItem_t;

	hashItem_t	*m_hash[256];
	
	mutable bool		m_fetch;
	mutable int			m_fetchList;
	mutable hashItem_t	*m_fetchItem;

private:
	nsHashMap( const nsHashMap& hashMap ) {}
	nsHashMap& operator = ( const nsHashMap& hashMap ) { return *this; }

	hashItem_t* FindItem( const char* key ) const;
};

//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------
//  nsHashMap::nsHashMap:  
//-----------------------------------------------------
template <class type>
nsHashMap<type>::nsHashMap()
{
	memset( m_hash, 0, sizeof(m_hash) );
	m_fetch = false;
}

//-----------------------------------------------------
//  ~nsHashMap::nsHashMap:  
//-----------------------------------------------------
template <class type>
nsHashMap<type>::~nsHashMap()
{
	DeleteAllKeys();
}

//-----------------------------------------------------
//  nsHashMap::SetData:  
//-----------------------------------------------------
template <class type>
inline bool nsHashMap<type>::SetData( const char* key, type* data )
{
	if ( GetData( key ) ) return false;
	
	hashItem_t	*item = (hashItem_t*)malloc( sizeof(hashItem_t) );
	item->key = strdup( key );
	item->userData = data;
	item->prev = item->next = 0;

	AddToList( &m_hash[ StrHash( key ) ], item );
	return true;
}

//-----------------------------------------------------
//  nsHashMap::GetData:  
//-----------------------------------------------------
template <class type>
inline type* nsHashMap<type>::GetData( const char* key ) const
{
	hashItem_t	*item = FindItem( key );
	if ( item ) return item->userData;
	return 0;
}

//-----------------------------------------------------
//  nsHashMap::DeleteKey:  
//-----------------------------------------------------
template <class type>
inline bool nsHashMap<type>::DeleteKey( const char* key )
{
	assert( !m_fetch );	//������ ������� ���� � ������ �������
	hashItem_t	*item = FindItem( key );
	if ( !item ) return false;
	
	DelFromList( &m_hash[ StrHash( key ) ], item );
	free( item->key );
	free( item );
	return true;
}

//-----------------------------------------------------
//  nsHashMap::DeleteAllKeys:  
//-----------------------------------------------------
template <class type>
bool nsHashMap<type>::DeleteAllKeys()
{
	assert( !m_fetch );	//������ ������� ����� � ������ �������

	for ( int i = 0; i < 256; i++ )
	{
		hashItem_t	*item = m_hash[i];
		while ( item )
		{
			hashItem_t	*del = item;
			item = item->next;
			free( del->key );
			free( del );
		}
	}

	memset( m_hash, 0, sizeof(m_hash) );
	return true;
}

//-----------------------------------------------------
//  nsHashMap<type>::FetchBegin:  
//-----------------------------------------------------
template <class type>
bool nsHashMap<type>::FetchBegin() const
{
	m_fetch = true;
	m_fetchList = 0;
	m_fetchItem = 0;
	return true;
}

//-----------------------------------------------------
//  nsHashMap<type>::FetchNext:  
//-----------------------------------------------------
template <class type>
inline type* nsHashMap<type>::FetchNext() const
{
	assert( m_fetch );	//����� ������� FetchNext ����� ������� FetchBegin

	if ( m_fetchItem )
		m_fetchItem = m_fetchItem->next;
	
	while ( m_fetchList < 256 && !m_fetchItem )
	{
		m_fetchItem = m_hash[ m_fetchList ];
		m_fetchList ++;
	}

	if ( !m_fetchItem ) return 0;
	return m_fetchItem->userData;
}

//-----------------------------------------------------
//  nsHashMap<type>::FetchEnd:
//-----------------------------------------------------
template <class type>
inline void nsHashMap<type>::FetchEnd() const
{
	m_fetch = false;
	m_fetchItem = 0;
}

//-----------------------------------------------------
//  nsHashMap::FindItem:  
//-----------------------------------------------------
template <class type>
inline nsHashMap<type>::hashItem_t* nsHashMap<type>::FindItem( const char* key ) const
{
	for ( hashItem_t *item = m_hash[StrHash( key )]; item; item = item->next )
	{
		if ( StrEqual( key, item->key ) )
			return item;
	}
	return 0;
}

#endif