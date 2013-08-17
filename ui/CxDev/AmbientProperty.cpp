// AmbientProperty.cpp: implementation of the CAmbientProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL( CAmbientProperty, CObject, 1 );

CAmbientProperty::CAmbientProperty() :
   m_dispid( 0 ),
   m_tEnabled( FALSE )
{
}

CAmbientProperty::CAmbientProperty( DISPID dispid, LPCTSTR pszName,
   const VARIANT& var, int vti, BOOL tStock ) :
   m_dispid( dispid ),
   m_strName( pszName ),
   m_varValue( var ),
   m_vti( vti ),
   m_tEnabled( TRUE ),
   m_tStock( tStock )
{
}

void CAmbientProperty::Enable( BOOL tEnabled )
{
   m_tEnabled = tEnabled;
}

DISPID CAmbientProperty::GetID() const
{
   return( m_dispid );
}

CString CAmbientProperty::GetName() const
{
   return( m_strName );
}

const COleVariant& CAmbientProperty::GetValue() const
{
   return( m_varValue );
}

int CAmbientProperty::GetVTI() const
{
   return( m_vti );
}

BOOL CAmbientProperty::IsEnabled() const
{
   return( m_tEnabled );
}

BOOL CAmbientProperty::IsStock() const
{
   return( m_tStock );
}

void CAmbientProperty::SetValue( const VARIANT& varValue, int vti )
{
   m_varValue = varValue;
   if( vti == -1 )
   {
	  m_vti = VTToVTI( varValue.vt );
   }
   else
   {
	  ASSERT( VTIToVT( vti ) == varValue.vt );
	  m_vti = vti;
   }
}

void CAmbientProperty::Serialize( CArchive& ar )
{
   CObject::Serialize( ar );

   if( ar.IsStoring() )
   {
	  ar<<m_dispid;
	  ar<<m_strName;
	  ar<<m_varValue;
	  ar<<m_vti;
	  ar<<m_tEnabled;
	  ar<<m_tStock;
   }
   else
   {
	  ar>>m_dispid;
	  ar>>m_strName;
	  ar>>m_varValue;
	  ar>>m_vti;
	  ar>>m_tEnabled;
	  ar>>m_tStock;
   }
}
