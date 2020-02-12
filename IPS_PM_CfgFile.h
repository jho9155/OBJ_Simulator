#pragma once

#include "KFile_Ini.h"

class IPS_PM_CfgFile : public KFile_Ini
{
	private:
		int		m_PortIdx;

	public:
		IPS_PM_CfgFile( int iPortIdx, char* pszFile );
		~IPS_PM_CfgFile(void);
		bool ReadData( char* pszFileName );

		void Set_Port_Index( int idx );
		int  Get_Port_index();
};

