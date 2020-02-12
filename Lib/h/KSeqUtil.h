#pragma once

#define FILE_STR  "Datalog/Seq/%s"

enum { 
	   TIME_HMSM_COLON, TIME_HMSM_HYPEN, TIME_HMSM_SLASH, TIME_HMSM_CONTINUE,
	   TIME_HMS_COLON, TIME_HMS_HYPEN, TIME_HMS_SLASH, TIME_HMS_CONTINUE,
	   TIME_HM_COLON,  TIME_HM_HYPEN,  TIME_HM_SLASH,  TIME_HM_CONTINUE
 };

enum {
		DATE_YYYYMMDD_SLASH, DATE_YYYYMMDD_HYPEN, DATE_YYYYMMDD_CONTINUE, DATE_YYYYMMDD_PERIOD,
		DATE_YYMMDD_SLASH,   DATE_YYMMDD_HYPEN,   DATE_YYMMDD_CONTINUE, DATE_YYMMDD_PERIOD
};


class KSeqUtil
{
	public:
		KSeqUtil(void);
		~KSeqUtil(void);
		int Get_Int_Array_MinValue_Index( int* piArray, int iSize, int* piMinValue );			// return ���� Min Index��.
		int Get_Int_Array_MaxValue_Index( int* piArray, int iSize, int* piMaxValue );			// return ���� Max Index��.
		bool Get_Int_Array_Min2Max_Sort( int* piArray, int iSize, int* piResultArray );			// ������������ Sorting��.
		bool Get_Int_Array_Max2Min_Sort( int* piArray, int iSize, int* piResultArray );			// ������������ Sorting��.
};

class kUtlFile
{
	public:
		kUtlFile(void);
		~kUtlFile(void);

		int kaDirectory_Exist_Check(char* pszfolder, bool bAttachCrtDir);
		bool kaDirectory_Make_at_Sub(char* DirectoryName, long line);
		int kaRelPath_To_FullPath( char* pszRelPath, char* pszFullPath, unsigned int uiSize );
};


class KLog
{
	private :
		// ���ϰ��� Util Class
		kUtlFile* m_UtilFile;

	public:
		KLog(void);
		~KLog(void);

		void LogWrite( int iColor, bool bErrDisp, long line, char* pszCategory, char* pszStr, ... );
};

class KStr
{
	private:
		KLog* m_plog;

	public:
		KStr(void);
		~KStr(void);

		bool NumStr_Entirety_Check(char* szStr);
		bool REMOVE_STR_TAIL_BLANK(char* pszData);
		int	 Char_Find_and_Replace( char* pszData, char chFind, char chchanged );
		bool Append_Quotes_Mark( char* pszData, int iArraySize );

		bool Str_Right_Shift( char* pszData, int iArraySize );


		int Str_Extract_With_Seperator( char* pszText, char sep, int iNo, char* pszRetVal, int iMaxData );
		int Str_Extract_With_Seperator( char* pszText, char sep, int iNo, int*  piRetVal );
		int Str_Extract_With_Seperator( char* pszText, char sep, int iNo, double* pdbRetVal );

		bool Check_Valid_Number_String( char* pstr );		// ���� kaStr_to_NUM_Valid_Check
		int Check_Integer_And_Return( char* szBuffer, int iLength, BOOL *bResult );
		double Check_Float_And_Return( char* szBuffer, int iLength, BOOL *bResult ); 

		int Str_Has_Char( char* pszStr, char ch );																// string�� Character�� ������ �ִ��� Ȯ���Ѵ�. ���ϰ��� char�� ��ġ
		int Get_Char_Count_In_Str( char* pszStr, char ch, int iSize );											// string ���� �ش� character�� ��� �ִ����� Ȯ���Ѵ�.

		int	Get_IO_Type_From_String( char* pszStr );
		void GetTimeData( char* szData, int dMode );
		void GetDateData( char* szData, int dMode );

		int Get_Module_Number( char* pszHeader, char* pszMdlStr ); 												// 0 : Not match Header, 1 ~ ?? PM Module�� Number�� �´� 

};

