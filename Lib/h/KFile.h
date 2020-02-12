#pragma once

class KFile
{
	private:
		char	m_szFileName[MAX_PATH];
		FILE*	m_fi;
		unsigned int m_uLineMaxSize;

	public:
		KFile(char* pszFileName, unsigned int uiLineMaxsize);
		~KFile(void);

		char* GetName();
		void SetName( char* pszFile, unsigned int uiMaxSize );

		bool Read();
		virtual bool Readline( char* pszline );
};


