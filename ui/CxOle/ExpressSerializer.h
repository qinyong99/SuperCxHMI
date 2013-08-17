// ExpressSerializer.h: interface for the CExpressSerializer class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __EXPRESSSERIALIZER_H__
#define __EXPRESSSERIALIZER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct DefaultValue
{
	VARTYPE vt;
	union 
	{	
        double						dblVal;						// VT_R8   
        LPSTR						strVal;						// VT_LPSTR
    };
};

class CDefaultValue : DefaultValue
{
	struct StringData
	{
		long nRefs;     // reference count
		
		char* data()
		{ return (char*)(this + 1); }
	};

	static int rgInitData[];
	static StringData* tmpDataNil;
	static LPSTR tmpPchNil;

public:
	CDefaultValue()
	{
		vt = VT_NULL;
	}
	CDefaultValue(int iSrc)
	{
		vt = VT_R8;
		dblVal = (double)iSrc;
	}
	CDefaultValue(long lSrc)
	{
		vt = VT_R8;
		dblVal = (double)lSrc;
	}
	CDefaultValue(unsigned long ulSrc)
	{
		vt = VT_R8;
		dblVal = (double)ulSrc;
	}
	CDefaultValue(float fltSrc)
	{
		vt = VT_R8;
		dblVal = (double)fltSrc;
	}
	CDefaultValue(double dblSrc)
	{
		vt = VT_R8;
		dblVal = dblSrc;
	}
	CDefaultValue(LPCSTR lpszSrc)
	{
		vt = VT_LPSTR;
		strVal = tmpPchNil;
		
		int nLen = (lpszSrc != NULL) ? (int)strlen(lpszSrc) : 0;
		if (nLen != 0)
		{
			if (AllocBuffer(nLen))
				memcpy((void*)strVal, lpszSrc, nLen);
		}
	}
	CDefaultValue(LPCWSTR lpszSrc)
	{
		vt = VT_LPSTR;
		strVal = tmpPchNil;
		
		int nLen = (lpszSrc != NULL) ? (int)wcslen(lpszSrc) : 0;
		if (nLen != 0)
		{
			if (AllocBuffer(nLen * 2))
			{
				_wcstombsz(strVal, lpszSrc, (nLen * 2) + 1);

				int nNewLength = strlen(strVal);   // zero terminated
				strVal[nNewLength] = '\0';
			}
		}
	}
	BOOL AllocBuffer(int nLen)
	{
		if (nLen == 0)
		{
			strVal = tmpPchNil;
		}
		else
		{
			StringData* pData = (StringData*)new BYTE[sizeof(StringData) + (nLen + 1) * sizeof(TCHAR)];
			if(pData == NULL)
				return FALSE;
			
			pData->nRefs = 1;
			pData->data()[nLen] = _T('\0');
			strVal = pData->data();
		}
		
		return TRUE;
	}
	
	static int __cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count)
	{
		if (count == 0 && mbstr != NULL)
			return 0;
		
		int result = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, mbstr, (int)count, NULL, NULL);
		ATLASSERT(mbstr == NULL || result <= (int)count);
		if (result > 0)
			mbstr[result - 1] = 0;
		return result;
	}

	void InternalCopy(const CDefaultValue& vSrc)
	{
		vt = vSrc.vt;
		dblVal = vSrc.dblVal;
		
		if (vSrc.vt == VT_LPSTR)
		{
			StringData* pData = (StringData*)strVal - 1;
			if (pData != tmpDataNil)
				pData->nRefs++;
		}
	}

	CDefaultValue(const CDefaultValue& vSrc)
	{
		InternalCopy(vSrc);
	}
	CDefaultValue& operator=(const CDefaultValue& vSrc)
	{
		InternalCopy(vSrc);
		return *this;
	}
	bool operator==(const CDefaultValue& vSrc)
	{
		if (vt != vSrc.vt)
			return false;

		if (vt == VT_NULL)
			return true;
		if (vt == VT_LPSTR)
			return strcmp(strVal, vSrc.strVal) == 0;
		
		return dblVal == (double)vSrc.dblVal;
	}
	bool operator!=(const CDefaultValue& vSrc)
	{
		return !operator==(vSrc);
	}
	virtual ~CDefaultValue()
	{
		if (vt == VT_LPSTR)
		{
			StringData* pData = (StringData*)strVal - 1;
			if (pData != tmpDataNil)
			{
				ATLASSERT(pData->nRefs != 0);
				if (InterlockedDecrement(&pData->nRefs) <= 0)
					delete[] (BYTE*)pData;
				vt = VT_EMPTY;
			}
		}
	}
};

_declspec(selectany) int CDefaultValue::rgInitData[] = { -1, 0 };
_declspec(selectany) CDefaultValue::StringData* CDefaultValue::tmpDataNil = (StringData*)&rgInitData;
_declspec(selectany) LPSTR CDefaultValue::tmpPchNil = (LPSTR)(((BYTE*)&rgInitData) + sizeof(StringData));

class CExpressSerializer;

typedef BOOL (CExpressSerializer::*SerializeFunc)(IStream* pStream, void* pValue, const CDefaultValue& defaultValue, BYTE bIndex);
typedef BOOL (CExpressSerializer::*DeserializeFunc)(IStream* pStream, void* pValue);

class CSerializerEntry
{
public:
	CSerializerEntry()
	{
		nOffset = -1;
	}
	
	static CSerializerEntry CreateSerializerEntry(int nOffset, int nSize, SerializeFunc pfnSerialize, DeserializeFunc pfnDeserialize, CDefaultValue defaultValue)
	{
		CSerializerEntry e;
		e.pfnSerialize = pfnSerialize;
		e.pfnDeserialize = pfnDeserialize;
		e.nOffset = nOffset;
		e.nSize = nSize;
		e.defaultValue = defaultValue;
		
		return e;
	}
	
	SerializeFunc pfnSerialize;
	DeserializeFunc pfnDeserialize;
	int nOffset;
	int nSize;
	CDefaultValue defaultValue;
};

class CExpressSerializer  
{
public:
	CExpressSerializer();
	virtual ~CExpressSerializer();

	BOOL Serialize_Bool(IStream* pStream, BOOL* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(*pValue), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_Bool(IStream* pStream, BOOL* pValue)
	{
		pStream->Read(pValue, sizeof(*pValue), NULL);
		return TRUE;
	}

	BOOL Serialize_Int(IStream* pStream, int* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(int), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_Int(IStream* pStream, int* pValue)
	{
		pStream->Read(pValue, sizeof(int), NULL);
		return TRUE;
	}

	BOOL Serialize_Long(IStream* pStream, long* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(long), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_Long(IStream* pStream, long* pValue)
	{
		pStream->Read(pValue, sizeof(long), NULL);
		return TRUE;
	}

	BOOL Serialize_ULong(IStream* pStream, unsigned long* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(unsigned long), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_ULong(IStream* pStream, unsigned long* pValue)
	{
		pStream->Read(pValue, sizeof(unsigned long), NULL);
		return TRUE;
	}

	BOOL Serialize_Float(IStream* pStream, float* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(float), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_Float(IStream* pStream, float* pValue)
	{
		pStream->Read(pValue, sizeof(float), NULL);
		return TRUE;
	}

	BOOL Serialize_Double(IStream* pStream, double* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);
			pStream->Write(pValue, sizeof(double), NULL);
			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_Double(IStream* pStream, double* pValue)
	{
		pStream->Read(pValue, sizeof(double), NULL);
		return TRUE;
	}

	BOOL Serialize_BSTR(IStream* pStream, BSTR* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		if (CDefaultValue(*pValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);

			CComBSTR bstr;
			bstr.Attach(*pValue);
			bstr.WriteToStream(pStream);
			bstr.Detach();

			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_BSTR(IStream* pStream, BSTR* pValue)
	{
		CComBSTR bstr;
		bstr.Attach(*pValue);
		bstr.Empty();

		bstr.ReadFromStream(pStream);
		*pValue = bstr.Detach();

		return TRUE;
	}

	BOOL Serialize_CString(IStream* pStream, CString* pValue, const CDefaultValue& defaultValue, BYTE bIndex)
	{
		LPCTSTR lpszValue = *pValue;
		if (CDefaultValue(lpszValue) != defaultValue)
		{
			pStream->Write(&bIndex, sizeof(BYTE), NULL);

			if (pValue->GetLength() < 255)
			{
				BYTE b = pValue->GetLength();
				pStream->Write(&b, sizeof(BYTE), NULL);
			}
			else if (pValue->GetLength() < 0xfffe)
			{
				BYTE b = 0xff;
				pStream->Write(&b, sizeof(BYTE), NULL);

				WORD w = pValue->GetLength();
				pStream->Write(&w, sizeof(WORD), NULL);
			}
			else
			{
				BYTE b = 0xff;
				pStream->Write(&b, sizeof(BYTE), NULL);
				
				WORD w = 0xffff;
				pStream->Write(&w, sizeof(WORD), NULL);

				DWORD dw = pValue->GetLength();
				pStream->Write(&dw, sizeof(DWORD), NULL);
			}
			
			//LPCTSTR psz = (LPCTSTR)(*pValue);
			pStream->Write((LPCTSTR)(*pValue), pValue->GetLength()*sizeof(TCHAR), NULL);

			return TRUE;
		}
		
		return FALSE;
	}
	
	BOOL Deserialize_CString(IStream* pStream, CString* pValue)
	{
		ATLASSERT(pStream != NULL && pValue != NULL);
		
		UINT nLen;

		BYTE b;
		pStream->Read(&b, sizeof(BYTE), NULL);
		if (b < 0xff)
		{
			nLen = b;
		}
		else
		{
			WORD w;
			pStream->Read(&w, sizeof(WORD), NULL);

			if (w < 0xfffe)
			{
				nLen = w;
			}
			else
			{
				DWORD dw;
				pStream->Read(&dw, sizeof(DWORD), NULL);
				nLen = dw;
			}
		}

		LPTSTR pszData = pValue->GetBufferSetLength(nLen);
		pStream->Read(pszData, nLen*sizeof(TCHAR), NULL);
		
		return TRUE;
	}

	static BOOL Serialize_Blob(IStream* pStream, void* pValue, int nSize, BYTE bIndex)
	{
		pStream->Write(&bIndex, sizeof(BYTE), NULL);
		pStream->Write(pValue, nSize, NULL);
		return TRUE;
	}
	
	static BOOL Deserialize_Blob(IStream* pStream, void* pValue, int nSize)
	{
		pStream->Read(pValue, nSize, NULL);
		return TRUE;
	}

	static void SerializeEntries(IStream* pStream, const CSerializerEntry* pEntries, void* pThis)
	{
		for (int nIndex = 0; ; nIndex++)
		{
			const CSerializerEntry& entry = pEntries[nIndex];
			if (entry.nOffset == -1)
				break;

			if (entry.nSize != 0)
				Serialize_Blob(pStream, (BYTE*)pThis + entry.nOffset, entry.nSize, nIndex);
			else
				(((CExpressSerializer*)NULL)->*entry.pfnSerialize)(pStream, (BYTE*)pThis + entry.nOffset, entry.defaultValue, nIndex);
		}
		
		BYTE b = 0xff;
		pStream->Write(&b, sizeof(BYTE), NULL);
	}
	
	static void DeserializeEntries(IStream* pStream, const CSerializerEntry* pEntries, void* pThis)
	{
		BYTE b;
		pStream->Read(&b, sizeof(BYTE), NULL);
		while (b != 0xff)
		{
			const CSerializerEntry& entry = pEntries[b];
			
			if (entry.nSize != 0)
				Deserialize_Blob(pStream, (BYTE*)pThis + entry.nOffset, entry.nSize);
			else
				(((CExpressSerializer*)NULL)->*entry.pfnDeserialize)(pStream, (BYTE*)pThis + entry.nOffset);

			pStream->Read(&b, sizeof(BYTE), NULL);
		}
	}
};

#define BEGIN_SERIALIZE_MAP(x) public: \
	typedef x theClass; \
	const CSerializerEntry* WINAPI _GetSerializeEntries(int* pnEntries = NULL) { \
static const CSerializerEntry _entries[] = {

#define SERIALIZE_ENTRY_BOOL(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_Bool, (DeserializeFunc)&CExpressSerializer::Deserialize_Bool, CDefaultValue((BOOL)defaultValue)) },
#define SERIALIZE_ENTRY_INT(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_Int, (DeserializeFunc)&CExpressSerializer::Deserialize_Int, CDefaultValue((int)defaultValue)) },
#define SERIALIZE_ENTRY_LONG(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_Long, (DeserializeFunc)&CExpressSerializer::Deserialize_Long, CDefaultValue((long)defaultValue)) },
#define SERIALIZE_ENTRY_ULONG(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_ULong, (DeserializeFunc)&CExpressSerializer::Deserialize_ULong, CDefaultValue((unsigned long)defaultValue)) },
#define SERIALIZE_ENTRY_FLOAT(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_Float, (DeserializeFunc)&CExpressSerializer::Deserialize_Float, CDefaultValue((float)defaultValue)) },
#define SERIALIZE_ENTRY_DOUBLE(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_Double, (DeserializeFunc)&CExpressSerializer::Deserialize_Double, CDefaultValue((double)defaultValue)) },
#define SERIALIZE_ENTRY_BSTR(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_BSTR, (DeserializeFunc)&CExpressSerializer::Deserialize_BSTR, CDefaultValue(defaultValue)) },
#define SERIALIZE_ENTRY_CSTRING(memberName, defaultValue) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), 0, (SerializeFunc)&CExpressSerializer::Serialize_CString, (DeserializeFunc)&CExpressSerializer::Deserialize_CString, CDefaultValue(defaultValue)) },
#define SERIALIZE_ENTRY_BLOB(memberName) { CSerializerEntry::CreateSerializerEntry(offsetof(theClass, memberName), sizeof(memberName), NULL, NULL, CDefaultValue()) },

#define END_SERIALIZE_MAP()   {CSerializerEntry()}}; \
	if (pnEntries) *pnEntries = sizeof(_entries)/sizeof(CSerializerEntry) - 1; \
		return _entries;}

#define EXPRESS_SERIALIZE(stream) CExpressSerializer::SerializeEntries(stream, _GetSerializeEntries(), this)
#define EXPRESS_DESERIALIZE(stream) CExpressSerializer::DeserializeEntries(stream, _GetSerializeEntries(), this)


#endif // __EXPRESSSERIALIZER_H__
