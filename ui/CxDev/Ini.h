// Ini.h: interface for the CIni class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __INI_H__
#define __INI_H__

#pragma once

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

//#ifndef __ATLSTR_H__
//#define __ATLSTR_H__
//#endif //!__ATLSTR_H__

//namespace WTL
//{

#define SER_GET(bGet,value) SerGet(bGet,value,#value)
#define SER_ARR(bGet,value,n) SerGet(bGet,value,n,#value)
#define SER_GETD(bGet,value,default) SerGet(bGet,value,#value,NULL,default)
#define SER_ARRD(bGet,value,n,default) SerGet(bGet,value,n,#value,default)

//##ModelId=3DC4E1610078
class CIni  
{
public:

#ifdef __NEVER_DEFINED__
   // MAKRO: SerGet(bGet,value,#value)
	//##ModelId=3DC4E1610121
   int SER_GET(BOOL bGet,int value);
   // MAKRO: SerGet(bGet,value,n,#value)
	//##ModelId=3DC4E161015D
   int SER_ARR(bGet,int* value,int n);
#endif
	// If the IniFilename contains no path,
	// the module-directory will be add to the FileName,
	// to avoid storing in the windows-directory
	// bModulPath=TRUE: ModulDir, bModulPath=FALSE: CurrentDir
	//##ModelId=3DC4E161017B
	static void AddModulPath(CString& strFileName,BOOL bModulPath = TRUE);
	//##ModelId=3DC4E16101A3
	static CString GetDefaultSection();
	//##ModelId=3DC4E16101CB
	static CString GetDefaultIniFile(BOOL bModulPath = TRUE);

	//##ModelId=3DC4E16101E9
	CIni( BOOL bModulPath = TRUE);
	//##ModelId=3DC4E1610211
	CIni(CIni const& Ini, BOOL bModulPath = TRUE);
	//##ModelId=3DC4E161022F
	CIni(CString const& strFileName, BOOL bModulPath = TRUE);
	//##ModelId=3DC4E1610257
	CIni(CString const& strFileName, CString const& strSection, BOOL bModulPath = TRUE);
	//##ModelId=3DC4E161027F
	~CIni();

	//##ModelId=3DC4E1610293
	void SetFileName(CString const& strFileName);
	//##ModelId=3DC4E16102BB
	void SetModuleFile(HMODULE hModule);
	//##ModelId=3DC4E16102D9
	void SetSection(CString const& strSection);
	//##ModelId=3DC4E16102F7
	CString const& GetFileName() const;
	//##ModelId=3DC4E1610315
	CString const& GetSection() const;
private:
	//##ModelId=3DC4E1610333
	void Init(LPCSTR strIniFile, LPCSTR strSection = NULL);
public:
	//##ModelId=3DC4E161035B
	CString	GetString(CString strEntry,	LPCSTR strDefault=NULL,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1610384
	double GetDouble(CString strEntry,	double fDefault = 0.0, LPCSTR strSection = NULL);
	//##ModelId=3DC4E16103AC
	float GetFloat(CString strEntry, float fDefault = 0.0, LPCSTR strSection = NULL);
	//##ModelId=3DC4E16103D4
	int	GetInt(CString strEntry, int nDefault = 0, LPCSTR strSection = NULL);
	//##ModelId=3DC4E162006E
	DWORD GetDWORD(CString strEntry, DWORD nDefault = 0,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620096
	BOOL GetBool(CString strEntry, BOOL bDefault = FALSE, LPCSTR strSection = NULL);
	//##ModelId=3DC4E16200B4
	CPoint GetPoint(CString strEntry, CPoint ptDefault = CPoint(0,0),	LPCSTR strSection = NULL);
	//##ModelId=3DC4E16200DC
	CRect GetRect(CString strEntry, CRect rectDefault = CRect(0,0,0,0),	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620104
	COLORREF GetColRef(CString strEntry,	COLORREF crDefault = RGB(128,128,128), LPCSTR strSection = NULL);

	//##ModelId=3DC4E162012C
	void WriteString(CString strEntry,CString str, LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620154
	void WriteDouble(CString strEntry,double f,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E162017C
	void WriteFloat(CString strEntry,float f, LPCSTR strSection = NULL);
	//##ModelId=3DC4E162019A
	void WriteInt(CString strEntry,int n, LPCSTR strSection = NULL);
	//##ModelId=3DC4E16201C2
	void WriteDWORD(CString strEntry,DWORD n, LPCSTR strSection = NULL);
	//##ModelId=3DC4E16201EA
	void WriteBool(CString strEntry,BOOL b,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620208
	void WritePoint(CString strEntry,CPoint	pt,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620230
	void WriteRect(CString strEntry,CRect rect,	LPCSTR strSection = NULL);
	//##ModelId=3DC4E1620259
	void WriteColRef(CString strEntry,COLORREF cr, LPCSTR strSection = NULL);

	//##ModelId=3DC4E1620277
	void SerGetString(BOOL bGet,CString& str, CString strEntry,	LPCSTR strSection = NULL, LPCSTR strDefault=NULL);
	//##ModelId=3DC4E16202A9
	void SerGetDouble(BOOL bGet,double& f, CString strEntry, LPCSTR strSection = NULL, double fDefault = 0.0);
	//##ModelId=3DC4E16202D1
	void SerGetFloat(BOOL bGet, float& f, CString strEntry, LPCSTR strSection = NULL, float fDefault = 0.0);
	//##ModelId=3DC4E1620303
	void SerGetInt(BOOL bGet,int& n, CString strEntry,	LPCSTR strSection = NULL, int nDefault = 0);
	//##ModelId=3DC4E162032B
	void SerGetDWORD(BOOL bGet,DWORD& n, CString strEntry, LPCSTR strSection = NULL, DWORD nDefault = 0);
	//##ModelId=3DC4E1620353
	void SerGetBool(BOOL bGet,BOOL& b,	CString strEntry, LPCSTR strSection = NULL,	BOOL bDefault = FALSE);
	//##ModelId=3DC4E1620385
	void SerGetPoint(BOOL bGet,CPoint& pt, CString strEntry, LPCSTR strSection = NULL, CPoint ptDefault = CPoint(0,0));
	//##ModelId=3DC4E16203AD
	void SerGetRect(BOOL bGet,CRect& rect, CString strEntry, LPCSTR strSection = NULL, CRect rectDefault = CRect(0,0,0,0));
	//##ModelId=3DC4E16203DF
	void SerGetColRef(BOOL bGet,COLORREF& cr, CString strEntry, LPCSTR strSection = NULL, COLORREF crDefault = RGB(128,128,128));

	//##ModelId=3DC4E163001F
	void SerGet(BOOL bGet, CString& str, CString strEntry,	LPCSTR strSection = NULL, LPCSTR strDefault=NULL);
	//##ModelId=3DC4E1630051
	void SerGet(BOOL bGet, double& f, CString strEntry,	LPCSTR strSection = NULL, double fDefault = 0.0);
	//##ModelId=3DC4E1630079
	void SerGet(BOOL bGet, float& f, CString strEntry, LPCSTR strSection = NULL, float fDefault = 0.0);
	//##ModelId=3DC4E16300A1
	void SerGet(BOOL bGet, int& n,	CString strEntry, LPCSTR strSection = NULL,	int nDefault = 0);
	//##ModelId=3DC4E16300D3
	void SerGet(BOOL bGet, short& n, CString strEntry, LPCSTR strSection = NULL, int nDefault = 0);
	//##ModelId=3DC4E16300FB
	void SerGet(BOOL bGet, DWORD& n, CString strEntry, LPCSTR strSection = NULL, DWORD nDefault = 0);
	//##ModelId=3DC4E1630124
	void SerGet(BOOL bGet, WORD	& n, CString strEntry, LPCSTR strSection = NULL, DWORD nDefault = 0);
	//##ModelId=3DC4E1630156
	void SerGet(BOOL bGet, CPoint& pt, CString strEntry, LPCSTR strSection = NULL, CPoint ptDefault = CPoint(0,0));
	//##ModelId=3DC4E163017E
	void SerGet(BOOL bGet, CRect& rect,	CString strEntry, LPCSTR strSection = NULL,	CRect rectDefault = CRect(0,0,0,0));
   
//ARRAYs
	//##ModelId=3DC4E16301A6
	void SerGet(BOOL bGet,CString* str,	int nCount, CString strEntry, LPCSTR strSection = NULL,	LPCSTR strDefault=NULL);
	//##ModelId=3DC4E16301D8
	void SerGet(BOOL bGet,double* f, int nCount, CString strEntry, LPCSTR strSection = NULL, double fDefault = 0.0);
	//##ModelId=3DC4E163020A
	void SerGet(BOOL bGet,float* f,	int nCount, CString strEntry, LPCSTR strSection = NULL,	float fDefault = 0.0);
	//##ModelId=3DC4E163023C
	void SerGet(BOOL bGet,int* n, int nCount, CString strEntry,	LPCSTR strSection = NULL, int nDefault = 0);
	//##ModelId=3DC4E1630264
	void SerGet(BOOL bGet,short* n,	int nCount, CString strEntry, LPCSTR strSection = NULL,	int nDefault = 0);
	//##ModelId=3DC4E1630296
	void SerGet(BOOL bGet,DWORD* n,	int nCount, CString strEntry, LPCSTR strSection = NULL,	DWORD nDefault = 0);
	//##ModelId=3DC4E16302C8
	void SerGet(BOOL bGet,WORD* n, int nCount, CString strEntry, LPCSTR strSection = NULL,	DWORD nDefault = 0);
	//##ModelId=3DC4E16302F0
	void SerGet(BOOL bGet,CPoint* pt, int nCount, CString strEntry,	LPCSTR strSection = NULL, CPoint ptDefault = CPoint(0,0));
	//##ModelId=3DC4E1630322
	void SerGet(BOOL bGet,CRect* rect, int nCount, CString strEntry, LPCSTR strSection = NULL, CRect rectDefault = CRect(0,0,0,0));
  
//
	//##ModelId=3DC4E1630354
	int GetSectionNames(CString** pparrSection);
	//##ModelId=3DC4E1630368
	BOOL DeleteSection(CString const& strSection);

private:
	//##ModelId=3DC4E1630386
	char* GetLPCSTR(CString strEntry, LPCSTR strSection, LPCSTR strDefault);
	//##ModelId=3DC4E16303AE
	BOOL m_bModulPath;  //TRUE: Filenames without path take the Modulepath
                        //FALSE: Filenames without path take the CurrentDirectory

#define MAX_INI_BUFFER 256
	//##ModelId=3DC4E164007B
	char m_chBuffer[MAX_INI_BUFFER];
	//##ModelId=3DC4E1640099
	CString m_strFileName;
	//##ModelId=3DC4E16400B7
	CString m_strSection;

public:
	//##ModelId=3DC4E16400FD
	static CString Read(CString const& strFileName, CString const& strSection, CString const& strEntry, CString const& strDefault);
	//##ModelId=3DC4E1640125
	static void	Write(CString const& strFileName, CString const& strSection, CString const& strEntry, CString const& strValue);
};

//////////////////////////////////////////////////////////////////////////////////
//功 能：
//描 述：If the IniFilename contains no path,
//the module-directory will be add to the FileName,
//to avoid storing in the windows-directory
//////////////////////////////////////////////////////////////////////////////////
//##ModelId=3DC4E161017B
inline void CIni::AddModulPath(CString& strFileName, BOOL bModulPath /*= TRUE*/)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath(strFileName, drive, dir, fname, ext);
	if (!drive[0])
	{
		//PathCanonicalize(..) doesn't work with for all Plattforms !
		CString strModule;
		if (bModulPath)
			GetModuleFileName(NULL, strModule.GetBuffer(MAX_INI_BUFFER), MAX_INI_BUFFER);
		else
			GetCurrentDirectory(MAX_INI_BUFFER, strModule.GetBuffer(MAX_INI_BUFFER));
		strModule.ReleaseBuffer();
		_splitpath(strModule, drive, dir, fname, ext);
		strModule = drive;
		strModule += dir;
		strModule += strFileName;
		strFileName = strModule;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//功 能：得到缺省区
//////////////////////////////////////////////////////////////////////////////////
//##ModelId=3DC4E16101A3
inline CString CIni::GetDefaultSection()
{
	return CString("Default");
//	return AfxGetAppName();
}

//////////////////////////////////////////////////////////////////////////////////
//功 能：得到缺省初始化配置信息文件
//////////////////////////////////////////////////////////////////////////////////
//##ModelId=3DC4E16101CB
inline CString CIni::GetDefaultIniFile(BOOL bModulPath /*= TRUE*/)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTemp;
	CString strApplName;
	GetModuleFileName(NULL, strTemp.GetBuffer(MAX_INI_BUFFER), MAX_INI_BUFFER);
	strTemp.ReleaseBuffer();
	_splitpath(strTemp, drive, dir, fname, ext);
	strTemp = fname; //"ApplName"
	strTemp += ".ini";  //"ApplName.ini"
	
	//是否使用模块所在路径
	if (bModulPath)
	{
		strApplName  = drive;
		strApplName += dir;
		strApplName += strTemp;
	}
	else
	{
		GetCurrentDirectory(MAX_INI_BUFFER, strApplName.GetBuffer(MAX_INI_BUFFER));
		strApplName.ReleaseBuffer();
//		strApplName.TrimRight('\\');
//		strApplName.TrimRight('/');
		strApplName += "\\";
		strApplName += strTemp;
	}
	return strApplName;
}

//////////////////////////////////////////////////////////////////////
//构造/析构函数
//////////////////////////////////////////////////////////////////////

//##ModelId=3DC4E16101E9
inline CIni::CIni(BOOL bModulPath /*= TRUE*/):
	m_bModulPath(bModulPath)
{
	m_strFileName = GetDefaultIniFile(m_bModulPath);
	m_strSection  = GetDefaultSection();
}

//##ModelId=3DC4E1610211
inline CIni::CIni(CIni const& Ini, BOOL bModulPath /*= TRUE*/):
	m_strFileName(Ini.m_strFileName),
	m_strSection(Ini.m_strSection),
	m_bModulPath(Ini.m_bModulPath)
{
	if (m_strFileName.IsEmpty())
		m_strFileName = GetDefaultIniFile(m_bModulPath);
	AddModulPath(m_strFileName,m_bModulPath);

	if (m_strSection.IsEmpty())
		m_strSection = GetDefaultSection();
}

//##ModelId=3DC4E161022F
inline CIni::CIni(CString const& strFileName, BOOL bModulPath /*= TRUE*/):
	m_strFileName(strFileName),
	m_bModulPath(bModulPath)
{
	if(m_strFileName.IsEmpty())
		m_strFileName = GetDefaultIniFile(m_bModulPath);
	AddModulPath(m_strFileName,bModulPath);

	m_strSection = GetDefaultSection();
}
//##ModelId=3DC4E1610257
inline CIni::CIni(CString const& strFileName, CString const& strSection, BOOL bModulPath /*= TRUE*/):
	m_strFileName(strFileName),
	m_strSection(strSection),
	m_bModulPath(bModulPath)
{
	if (m_strFileName.IsEmpty())
		m_strFileName = GetDefaultIniFile(m_bModulPath);
	AddModulPath(m_strFileName,bModulPath);

	if (m_strSection.IsEmpty())
		m_strSection = GetDefaultSection();
}

//##ModelId=3DC4E161027F
inline CIni::~CIni()
{
}

//////////////////////////////////////////////////////////////////////
// 操作
//////////////////////////////////////////////////////////////////////

//##ModelId=3DC4E1610293
inline void CIni::SetFileName(CString const& strFileName)
{
	m_strFileName = strFileName;
	AddModulPath(m_strFileName);
}

//##ModelId=3DC4E16102BB
inline void CIni::SetModuleFile(HMODULE hModule)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	CString strTemp;
	CString strApplName;
	GetModuleFileName(hModule, strTemp.GetBuffer(MAX_INI_BUFFER), MAX_INI_BUFFER);
	strTemp.ReleaseBuffer();
	_splitpath(strTemp, drive, dir, fname, ext);
	strTemp = fname; //"ApplName"
	strTemp += ".ini";  //"ApplName.ini"
	
	strApplName  = drive;
	strApplName += dir;
	strApplName += strTemp;

	SetFileName(strApplName);
}

//##ModelId=3DC4E16102D9
inline void CIni::SetSection(CString const& strSection)
{
	m_strSection = strSection;
}

//##ModelId=3DC4E16102F7
inline CString const& CIni::GetFileName() const
{
	return m_strFileName;
}

//##ModelId=3DC4E1610315
inline CString const& CIni::GetSection() const
{
	return m_strSection;
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

//##ModelId=3DC4E1610333
inline void CIni::Init(LPCSTR strFileName, LPCSTR strSection/* = NULL*/)
{
	if (strSection != NULL)
		m_strSection = strSection;
	if (strFileName != NULL)
		m_strFileName = strFileName;
}

//##ModelId=3DC4E161035B
inline CString CIni::GetString(CString strEntry, LPCSTR strDefault/*=NULL*/, LPCSTR strSection/* = NULL*/)
{
	if (strDefault == NULL)
		return CString(GetLPCSTR(strEntry, strSection, ""));
	else
		return CString(GetLPCSTR(strEntry, strSection, strDefault));
}

//##ModelId=3DC4E1610384
inline double CIni::GetDouble(CString strEntry, double fDefault/* = 0.0*/, LPCSTR strSection/* = NULL*/)
{
	CString strDefault;
	strDefault.Format("%g", fDefault);
	GetLPCSTR(strEntry, strSection, strDefault);

	return atof(m_chBuffer);
}

//##ModelId=3DC4E16103AC
inline float CIni::GetFloat(CString strEntry, float fDefault/* = 0.0*/, LPCSTR strSection/* = NULL*/)
{
	CString strDefault;
	strDefault.Format("%g", fDefault);
	GetLPCSTR(strEntry, strSection, strDefault);

	return (float)atof(m_chBuffer);
}

//##ModelId=3DC4E16103D4
inline int CIni::GetInt(CString strEntry, int nDefault/* = 0*/, LPCSTR strSection/* = NULL*/)
{
	CString strDefault;
	strDefault.Format("%d", nDefault);
	GetLPCSTR(strEntry, strSection, strDefault);

	return atoi(m_chBuffer);
}

//##ModelId=3DC4E162006E
inline DWORD CIni::GetDWORD(CString strEntry, DWORD nDefault/* = 0*/, LPCSTR strSection/* = NULL*/)
{
	CString strDefault;
	strDefault.Format("%d", nDefault);
	GetLPCSTR(strEntry, strSection, strDefault);

	return (DWORD)atoi(m_chBuffer);
}

//##ModelId=3DC4E1620096
inline BOOL CIni::GetBool(CString strEntry,BOOL bDefault/* = FALSE*/,LPCSTR strSection/* = NULL*/)
{
	CString strDefault;
	strDefault.Format("%d", bDefault);
	GetLPCSTR(strEntry, strSection, strDefault);

	return (atoi(m_chBuffer) != 0);
}

//##ModelId=3DC4E16200B4
inline CPoint CIni::GetPoint(CString strEntry,	CPoint ptDefault, LPCSTR strSection)
{
	CPoint ptReturn = ptDefault;

	CString strDefault;
	strDefault.Format("(%d,%d)", ptDefault.x, ptDefault.y);

	CString strPoint = GetString(strEntry, strDefault);
	sscanf(strPoint, "(%d,%d)", &ptReturn.x, &ptReturn.y);

	return ptReturn;
}

//##ModelId=3DC4E16200DC
inline CRect CIni::GetRect(CString strEntry, CRect rectDefault, LPCSTR strSection)
{
	CRect rectReturn=rectDefault;

	CString strDefault;
	strDefault.Format("(%d,%d,%d,%d)", rectDefault.top, rectDefault.left,
		rectDefault.bottom, rectDefault.right);

	CString strRect = GetString(strEntry, strDefault);
	sscanf(strRect,"(%d,%d,%d,%d)", &rectReturn.top,&rectReturn.left, 
		&rectReturn.bottom, &rectReturn.right);

	return rectReturn;
}

//##ModelId=3DC4E1620104
inline COLORREF CIni::GetColRef(CString strEntry, COLORREF crDefault, LPCSTR strSection)
{
	int temp[3] = {	
		GetRValue(crDefault),
		GetGValue(crDefault),
		GetBValue(crDefault)};

	CString strDefault;
	strDefault.Format("RGB(%hd,%hd,%hd)", temp[0], temp[1], temp[2]);

	CString strColRef = GetString(strEntry, strDefault);
	sscanf(strColRef, "RGB(%d,%d,%d)", temp, temp + 1, temp + 2);

	return RGB(temp[0], temp[1], temp[2]);
}
	
//##ModelId=3DC4E162012C
inline void CIni::WriteString(CString strEntry,CString	str, LPCSTR strSection/* = NULL*/)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	WritePrivateProfileString(m_strSection, strEntry, str, m_strFileName);
}

//##ModelId=3DC4E1620154
inline void CIni::WriteDouble(CString strEntry,double f, LPCSTR strSection/*= NULL*/)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("%g", f);
		WritePrivateProfileString(m_strSection, strEntry, strBuffer, m_strFileName);
}

//##ModelId=3DC4E162017C
inline void CIni::WriteFloat(CString strEntry,float f, LPCSTR strSection/* = NULL*/)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("%g", f);
		WritePrivateProfileString(m_strSection, strEntry, strBuffer, m_strFileName);
}

//##ModelId=3DC4E162019A
inline void CIni::WriteInt(CString strEntry, int n, LPCSTR strSection/* = NULL*/)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("%d", n);
		WritePrivateProfileString(m_strSection, strEntry, strBuffer, m_strFileName);
}

//##ModelId=3DC4E16201C2
inline void CIni::WriteDWORD(CString strEntry, DWORD n, LPCSTR strSection/* = NULL*/)
{
	if(strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("%d", n);
		WritePrivateProfileString(m_strSection, strEntry, strBuffer, m_strFileName);
}

//##ModelId=3DC4E16201EA
inline void CIni::WriteBool(CString strEntry,BOOL b, LPCSTR strSection/* = NULL*/)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("%d", b);
		WritePrivateProfileString(m_strSection, strEntry, strBuffer, m_strFileName);
}

//##ModelId=3DC4E1620208
inline void CIni::WritePoint(CString strEntry, CPoint pt, LPCSTR strSection)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("(%d,%d)", pt.x, pt.y);
	Write(m_strFileName, m_strSection, strEntry, strBuffer);
}

//##ModelId=3DC4E1620230
inline void CIni::WriteRect(CString strEntry,CRect rect, LPCSTR strSection)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("(%d,%d,%d,%d)", rect.top, rect.left, rect.bottom,rect.right);
	Write(m_strFileName, m_strSection, strEntry, strBuffer);
}

//##ModelId=3DC4E1620259
inline void CIni::WriteColRef(CString strEntry, COLORREF cr, LPCSTR strSection)
{
	if (strSection != NULL) 
		m_strSection = strSection;
	CString strBuffer;
	strBuffer.Format("RGB(%d,%d,%d)", GetRValue(cr), GetGValue(cr), GetBValue(cr));
	Write(m_strFileName, m_strSection, strEntry, strBuffer);
}

//##ModelId=3DC4E1630386
inline char* CIni::GetLPCSTR(CString strEntry, LPCSTR strSection, LPCSTR strDefault)
{
	// evtl Section neu setzen
	if (strSection != NULL)
		m_strSection = strSection;

	CString temp;
	if (strDefault == NULL)
		temp = Read(m_strFileName, m_strSection, strEntry, CString());
	else
		temp = Read(m_strFileName, m_strSection, strEntry, strDefault);

	return (char*)memcpy(m_chBuffer, (LPCTSTR)temp, temp.GetLength()+1);// '+1' damit die Null am Ende mit kopiert wird
}

//##ModelId=3DC4E1620277
inline void CIni::SerGetString(BOOL bGet, CString&	str, CString strEntry, LPCSTR strSection, LPCSTR strDefault)
{
	if (bGet)
		str = GetString(strEntry, strDefault/*=NULL*/, strSection/* = NULL*/);
	else
		WriteString(strEntry, str, strSection/* = NULL*/);
}

//##ModelId=3DC4E16202A9
inline void CIni::SerGetDouble(BOOL bGet, double& f, CString strEntry, LPCSTR strSection/* = NULL*/, double fDefault/* = 0.0*/)
{
	if (bGet)
		f = GetDouble(strEntry, fDefault/*=NULL*/, strSection/* = NULL*/);
	else
		WriteDouble(strEntry, f, strSection/* = NULL*/);
}

//##ModelId=3DC4E16202D1
inline void CIni::SerGetFloat(	BOOL bGet,float	&	f,	CString strEntry, LPCSTR strSection/* = NULL*/,float fDefault/* = 0.0*/)
{
	if (bGet)
		f = GetFloat(strEntry,fDefault/*=NULL*/,strSection/* = NULL*/);
	else
		WriteFloat(strEntry,f, strSection/* = NULL*/);
}

//##ModelId=3DC4E1620303
inline void CIni::SerGetInt(BOOL bGet,int& n, CString strEntry, LPCSTR strSection/* = NULL*/, int nDefault/* = 0*/)
{
	if (bGet)
		n = GetInt(strEntry, nDefault/*=NULL*/, strSection/* = NULL*/);
	else
		WriteInt(strEntry, n, strSection/* = NULL*/);
}

//##ModelId=3DC4E162032B
inline void CIni::SerGetDWORD(BOOL bGet, DWORD& n,	CString strEntry, LPCSTR strSection/* = NULL*/, DWORD nDefault/* = 0*/)
{
	if (bGet)
		n = (DWORD)GetInt(strEntry, nDefault/*=NULL*/, strSection/* = NULL*/);
	else
		WriteInt(strEntry, n, strSection/* = NULL*/);
}

//##ModelId=3DC4E1620353
inline void CIni::SerGetBool(BOOL bGet, BOOL& b, CString strEntry, LPCSTR strSection/* = NULL*/, BOOL bDefault/* = FALSE*/)
{
	if(bGet)
		b = GetBool(strEntry, bDefault/*=NULL*/, strSection/* = NULL*/);
	else
		WriteBool(strEntry, b, strSection/* = NULL*/);
}

//##ModelId=3DC4E1620385
inline void CIni::SerGetPoint(BOOL bGet, CPoint& pt, CString strEntry,	LPCSTR strSection,	CPoint ptDefault)
{
	if (bGet)
		pt = GetPoint(strEntry, ptDefault, strSection);
	else
		WritePoint(strEntry, pt, strSection);
}

//##ModelId=3DC4E16203AD
inline void CIni::SerGetRect(BOOL bGet, CRect& rect, CString strEntry,	LPCSTR strSection, CRect rectDefault)
{
	if (bGet)
		rect = GetRect(strEntry, rectDefault, strSection);
	else
		WriteRect(strEntry, rect, strSection);
}

//##ModelId=3DC4E16203DF
inline void CIni::SerGetColRef(BOOL bGet, COLORREF& cr, CString strEntry, LPCSTR strSection, COLORREF crDefault)
{
	if (bGet)
		cr = GetColRef(strEntry, crDefault, strSection);
	else
		WriteColRef(strEntry, cr, strSection);
}
////////////////////////////////////////////////////////////////////////////////////////////////////77
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//##ModelId=3DC4E163001F
inline void CIni::SerGet(BOOL bGet, CString& str, CString strEntry, LPCSTR strSection/*= NULL*/, LPCSTR strDefault/*= NULL*/)
{
	SerGetString(bGet,str,strEntry,strSection,strDefault);
}

//##ModelId=3DC4E1630051
inline void CIni::SerGet(BOOL bGet,double& f, CString strEntry, LPCSTR strSection/*= NULL*/, double fDefault/* = 0.0*/)
{
	SerGetDouble(bGet,f,strEntry,strSection,fDefault);
}

//##ModelId=3DC4E1630079
inline void CIni::SerGet(BOOL bGet, float& f, CString strEntry, LPCSTR strSection/*= NULL*/, float fDefault/* = 0.0*/)
{
	SerGetFloat(bGet,f,strEntry,strSection,fDefault);
}

//##ModelId=3DC4E16300A1
inline void CIni::SerGet(BOOL bGet,int& n,	CString strEntry, LPCSTR strSection/*= NULL*/, int nDefault/* = 0*/)
{
	SerGetInt(bGet,n,strEntry,strSection,nDefault);
}

//##ModelId=3DC4E16300D3
inline void CIni::SerGet(BOOL bGet, short& n, CString strEntry, LPCSTR strSection/*= NULL*/, int nDefault/* = 0*/)
{
	int nTemp = n;
	SerGetInt(bGet, nTemp, strEntry, strSection, nDefault);
	n = nTemp;
}

//##ModelId=3DC4E16300FB
inline void CIni::SerGet(BOOL bGet,DWORD& n, CString strEntry,	LPCSTR strSection/*= NULL*/, DWORD nDefault/* = 0*/)
{
	SerGetDWORD(bGet, n, strEntry, strSection, nDefault);
}

//##ModelId=3DC4E1630124
inline void CIni::SerGet(BOOL bGet, WORD& n, CString strEntry, LPCSTR strSection/*= NULL*/,	DWORD nDefault/* = 0*/)
{
	DWORD dwTemp = n;
	SerGetDWORD(bGet, dwTemp, strEntry, strSection, nDefault);
	n = (WORD)dwTemp;
}

//##ModelId=3DC4E1630156
inline void CIni::SerGet(BOOL bGet,CPoint& pt, CString strEntry, LPCSTR strSection/*= NULL*/, CPoint ptDefault/* = CPoint(0,0)*/)
{
	SerGetPoint(bGet, pt, strEntry, strSection, ptDefault);
}

//##ModelId=3DC4E163017E
inline void CIni::SerGet(BOOL bGet,CRect& rect, CString strEntry, LPCSTR strSection/*= NULL*/,	CRect rectDefault/* = CRect(0,0,0,0)*/)
{
	SerGetRect(bGet, rect, strEntry, strSection, rectDefault);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 数组
//////////////////////////////////////////////////////////////////

//##ModelId=3DC4E16301A6
inline void CIni::SerGet(BOOL bGet, CString* ar, int nCount, CString strEntry, LPCSTR strSection/*=NULL*/,	LPCSTR Default/*=NULL*/)
{
   CString strBuffer;
   for (int i = 0 ; i < nCount ; i++)
   {
		strBuffer.Format("_%i",i);
		strBuffer = strEntry + strBuffer;
		SerGet(bGet,ar[i],strBuffer,strSection,Default);
   }
}

//##ModelId=3DC4E16301D8
inline void CIni::SerGet(BOOL bGet, double* ar, int nCount, CString strEntry,	LPCSTR strSection/*=NULL*/,	double Default/* = 0.0*/)
{
	CString strBuffer;
	for (int i = 0 ; i < nCount ; i++)
	{
		strBuffer.Format("_%i", i);
		strBuffer = strEntry + strBuffer;
		SerGet(bGet, ar[i], strBuffer, strSection, Default);
	}
}

//##ModelId=3DC4E163020A
inline void CIni::SerGet(	BOOL bGet,float	* ar, int nCount, CString strEntry,	LPCSTR strSection/*=NULL*/,	float Default/* = 0.0*/)
{
   CString strBuffer;
   for (int i=0 ; i<nCount ; i++)
   {
      strBuffer.Format("_%i",i);
      strBuffer = strEntry + strBuffer;
      SerGet(bGet,ar[i],strBuffer,strSection,Default);
   }
}

//##ModelId=3DC4E163023C
inline void CIni::SerGet(BOOL bGet, int* ar, int nCount, CString strEntry,	LPCSTR strSection/*=NULL*/,	int Default/* = 0*/)
{
	CString strBuffer;
	for (int i = 0 ; i < nCount ; i++)
	{
		strBuffer.Format("_%i", i);
		strBuffer = strEntry + strBuffer;
		SerGet(bGet, ar[i], strBuffer, strSection, Default);
	}
}

//##ModelId=3DC4E1630264
inline void CIni::SerGet(BOOL bGet, short* ar,	int nCount, CString strEntry, LPCSTR strSection/*=NULL*/, int Default/* = 0*/)
{
	CString strBuffer;
	for (int i = 0 ; i < nCount ; i++)
	{
		strBuffer.Format("_%i", i);
		strBuffer = strEntry + strBuffer;
		SerGet(bGet, ar[i], strBuffer, strSection, Default);
	}
}

//##ModelId=3DC4E1630296
inline void CIni::SerGet(BOOL bGet,DWORD* ar, int nCount, CString strEntry, LPCSTR strSection/*=NULL*/, DWORD Default/* = 0*/)
{
	CString strBuffer;
	for (int i = 0; i < nCount; i++)
	{
		strBuffer.Format("_%i", i);
		strBuffer = strEntry + strBuffer;
		SerGet(bGet, ar[i], strBuffer, strSection, Default);
	}
}

//##ModelId=3DC4E16302C8
inline void CIni::SerGet(BOOL bGet, WORD* ar, int nCount, CString strEntry, LPCSTR strSection/*=NULL*/, DWORD Default/* = 0*/)
{
   CString strBuffer;
   for (int i = 0 ; i < nCount ; i++)
   {
      strBuffer.Format("_%i", i);
      strBuffer = strEntry + strBuffer;
      SerGet(bGet, ar[i], strBuffer, strSection, Default);
   }
}

//##ModelId=3DC4E16302F0
inline void CIni::SerGet(BOOL bGet, CPoint* ar, int nCount, CString strEntry, LPCSTR strSection/*=NULL*/, CPoint Default/* = CPoint(0,0)*/)
{
   CString strBuffer;
   for (int i = 0; i < nCount; i++)
   {
      strBuffer.Format("_%i", i);
      strBuffer = strEntry + strBuffer;
      SerGet(bGet, ar[i], strBuffer, strSection, Default);
   }
}

//##ModelId=3DC4E1630322
inline void CIni::SerGet(BOOL bGet, CRect	* ar, int nCount, CString strEntry,	LPCSTR strSection/*=NULL*/,	CRect Default/* = CRect(0,0,0,0)*/)
{
   CString strBuffer;
   for (int i = 0; i < nCount; i++)
   {
      strBuffer.Format("_%i", i);
      strBuffer = strEntry + strBuffer;
      SerGet(bGet, ar[i], strBuffer, strSection, Default);
   }
}

#define MAX_SECTIONS_STR_LENTH 1024
//##ModelId=3DC4E1630354
inline int CIni::GetSectionNames(CString** pparrSection)
{
	TCHAR chSections[MAX_SECTIONS_STR_LENTH];
	DWORD dwSize = GetPrivateProfileSectionNames(chSections, MAX_SECTIONS_STR_LENTH, m_strFileName);
	if (dwSize == 0)
		return 0;

	TCHAR* pCh = chSections;
	//得到数目
	int nCount = 0;
	while (1)
	{
		while (*pCh != _T('\0'))
		{
			pCh++;
		}
		nCount++;
		pCh++;
		if (*pCh == _T('\0'))
			break;
	}

	*pparrSection = new CString[nCount];
	pCh = chSections;
	for (int i = 0; i < nCount; i++)
	{
		(*pparrSection)[i] = pCh;
		while (*pCh != _T('\0'))
		{
			pCh++;
		}
		pCh++;
		if (*pCh == _T('\0'))
			break;
	} 

	return nCount;
}
/////////////////////////////////////////////////////////////////////////////////
//功 能：删除区
/////////////////////////////////////////////////////////////////////////////////
//##ModelId=3DC4E1630368
inline BOOL CIni::DeleteSection(CString const& strSection)
{
	return WritePrivateProfileString(strSection, NULL, NULL, m_strFileName);
}

//////////////////////////////////////////////////////////////////////
//静态函数
//////////////////////////////////////////////////////////////////////

//##ModelId=3DC4E16400FD
inline CString CIni::Read(CString const& strFileName, CString const& strSection, CString const& strEntry, CString const& strDefault)
{
	CString strReturn;
	GetPrivateProfileString(strSection,
							strEntry,
							strDefault,
							strReturn.GetBufferSetLength(MAX_INI_BUFFER),
							MAX_INI_BUFFER,
							strFileName);
	strReturn.ReleaseBuffer();
	return strReturn;
}

//##ModelId=3DC4E1640125
inline void CIni::Write(CString const& strFileName, CString const& strSection, CString const& strEntry, CString const& strValue)
{
	WritePrivateProfileString(strSection,
							strEntry,
							strValue,
							strFileName);
}

//}; //namespace WTL

#endif // __INI_H__

