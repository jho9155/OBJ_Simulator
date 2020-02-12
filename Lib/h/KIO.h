#pragma once
#include "KSeqUtil.h"

class KIO
{
	protected:
		char	m_szIOName[128];
		int		m_ioIdx;
		int		m_ioType;
		KStr*	m_pStr;

	public:
		KIO(char* pszIOName, int ioType);
		~KIO(void);

		int Get_Index();
		char* Get_IOName();
		int Get_IO_Type();

		int Redefine_IO( char* pszIOName, int ioType );
		int ReIndexing();

		bool Write_IO( char* pszvalue );
};

