// ApexGis.h: interface for the CApexGis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APEXGIS_H__A3941307_300C_46B4_A632_198403E9B13A__INCLUDED_)
#define AFX_APEXGIS_H__A3941307_300C_46B4_A632_198403E9B13A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CApexGis  
{
public:
	CApexGis();
	virtual ~CApexGis();

	void DoDraw(HDC hdcDest);
};

#endif // !defined(AFX_APEXGIS_H__A3941307_300C_46B4_A632_198403E9B13A__INCLUDED_)
