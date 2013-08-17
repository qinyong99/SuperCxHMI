// AmbientProperty.h: interface for the CAmbientProperty class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AMBIENTPROPERTY_H__
#define __AMBIENTPROPERTY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAmbientProperty :
   public CObject
{
   DECLARE_SERIAL( CAmbientProperty );

public:
   CAmbientProperty();
   CAmbientProperty( DISPID dispid, LPCTSTR pszName, const VARIANT& var,
	  int vti, BOOL tStock = FALSE );

   void Enable( BOOL tEnabled );
   DISPID GetID() const;
   CString GetName() const;
   const COleVariant& GetValue() const;
   int GetVTI() const;
   BOOL IsEnabled() const;
   BOOL IsStock() const;
   void SetValue( const VARIANT& varValue, int vti = -1 );

   void Serialize( CArchive& ar );

protected:
   DISPID m_dispid;
   CString m_strName;
   COleVariant m_varValue;
   int m_vti;
   BOOL m_tEnabled;
   BOOL m_tStock;
};

#endif // !defined(AFX_AMBIENTPROPERTY_H__A71485C7_F42E_11D4_8C60_5254AB20ED22__INCLUDED_)
