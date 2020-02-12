#pragma once

#include "KIO.h"

class KIO_String : public KIO
{
	public:
		KIO_String();
		KIO_String(char* pszIOName);
		~KIO_String(void);

		int Find_Index( char* szIOName );

		bool Read(char* szReadData);
		void Write(char* pszWriteStr, int* CommSts);
		int Redefine_IO(char* pszIOName);
};

