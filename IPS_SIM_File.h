#pragma once
#include "kSeqUtil.h"
#include "KFile.h"

class IPS_SIM_File : public KFile
{
	private:
		kUtlFile*		m_pkFile;

	public:
		IPS_SIM_File(char* pszFile, unsigned int uiMaxLine);
		~IPS_SIM_File(void);

		bool Readline( char* pszline );
};

