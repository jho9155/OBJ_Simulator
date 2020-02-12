#pragma once
#include "KIO.h"

class KIO_Digital : public KIO
{
	public:
		KIO_Digital();
		KIO_Digital(char* pszIOName);
		~KIO_Digital(void);

		int Find_Index( char* szIOName );

		int Read(int* CommSts);
		void Write(int iVal, int* CommSts);
		int Redefine_IO(char* pszIOName);
};

