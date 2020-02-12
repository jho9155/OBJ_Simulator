#pragma once

#include "KSCH_Event.h"

class IPS_SCH_EVENT : public KSCH_Event
{
	public:
		IPS_SCH_EVENT(void);
		~IPS_SCH_EVENT(void);

		void Send_PM_Disable_Event( char cPMchar );
		void Send_PM_Enable_Event( char cPMchar );

		bool Send_Run( char* pszName, char* szMsg );
		void Send_Event( char* pszName, char* szMsg );
		bool Set_IO( char* pszName, int ioType, char* szData );

};

