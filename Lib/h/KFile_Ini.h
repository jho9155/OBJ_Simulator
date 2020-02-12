#pragma once
class KFile_Ini
{
	private :
		char	m_szFileName[MAX_PATH];

	public:
		KFile_Ini(char* pszFile);
		~KFile_Ini(void);

		bool Read( char* pszSection, char* pszItem, char* pszRetStr, int iMaxSize );
		bool Read( char* pszSection, char* pszItem, int* piRetVal );
		bool Read( char* pszSection, char* pszItem, float* pdbRetVal );
		bool Read( char* pszSection, char* pszItem, double* pdbRetVal );

		void Write( char* pszSection, char* pszItem, char* pszValue );
		void Write( char* pszSection, char* pszItem, int iValue );
		void Write( char* pszSection, char* pszItem, float dbValue, int dev );
		void Write( char* pszSection, char* pszItem, double dbValue, int dev );


		void Set_Object( char* pszFileName );
		char* Get_Object();
};

