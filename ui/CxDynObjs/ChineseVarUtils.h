
// ChineseVarUtils.h: 中文变量支持函数
//
//////////////////////////////////////////////////////////////////////

#ifndef __CHINESEVARUTILS_H__
#define __CHINESEVARUTILS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 含有中文字符的文本转换为只有ASCII字符的文本
inline LPTSTR ChineseToASCII(LPCSTR lpChinese, LPSTR lpASCII)
{
	static const TCHAR szHex[] = _T("0123456789ABCDEF");

	BYTE* p;
	BYTE* p1 = (BYTE*)lpChinese;
	BYTE* p2 = (BYTE*)lpASCII;
	
	BOOL bString = FALSE;
	BOOL bComment = FALSE;
	BOOL bChinese = FALSE;

	while (*p1 != '\0')
	{
		if (*p1 == '"')
			bString = !bString;
		else if (!bString && *p1 == '\'')
			bComment = TRUE;

		if (!bString && !bComment)
		{
			if (*p1 >= 0xb0 && *p1 <= 0xf7)
			{
				p = p1 + 1;
				if (*p > 0xa0 && *p < 0xff)
				{
					if (!bChinese)
					{
						bChinese = TRUE;

						*p2++ = 'C';
						*p2++ = 'N';
					}

					// 转换成 16 进制字符
					*p2++ = szHex[*p1 >> 4];
					*p2++ = szHex[*p1 & 0xf];
					p1++;

					*p2++ = szHex[*p1 >> 4];
					*p2++ = szHex[*p1 & 0xf];
					p1++;

					continue;
				}
			}

			if (bChinese)
			{
				bChinese = FALSE;

				*p2++ = 'C';
				*p2++ = 'N';
			}

			if (*p1 == 'C' || *p1 == 'c')
			{
				p = p1 + 1;
				if (*p == 'N' || *p == 'n')
				{
					*p2++ = *p1;
					*p2++ = *p;
					*p2++ = *p1;
					*p2++ = *p;

					p1 += 2;

					continue;
				}
			}
		}

		if (*p1 == '\n')
		{
			bString = FALSE;
			bComment = FALSE;
		}

		*p2++ = *p1++;
	}

	if (bChinese)
	{
		bChinese = FALSE;
		
		*p2++ = 'C';
		*p2++ = 'N';
	}

	*p2 = '\0';

	return lpASCII;
}

// 只有ASCII字符的文本转换为含有中文字符的文本
inline LPSTR ASCIIToChinese(LPCSTR lpASCII, LPSTR lpChinese)
{
	BYTE* p;
	BYTE* p1 = (BYTE*)lpASCII;
	BYTE* p2 = (BYTE*)lpChinese;

	BOOL bString = FALSE;
	BOOL bComment = FALSE;
	BOOL bChinese = FALSE;
	
	while (*p1 != '\0')
	{
		if (*p1 == '"')
			bString = !bString;
		else if (!bString && *p1 == '\'')
			bComment = TRUE;
		
		if (!bString && !bComment)
		{
			if (*p1 == 'C' || *p1 == 'c')
			{
				p = p1 + 1;
				if (*p == 'N' || *p == 'n')
				{
					p1 += 2;

					if (*p1 == 'C' || *p1 == 'c')
					{
						p = p1 + 1;
						if (*p == 'N' || *p == 'n')
						{
							*p2++ = *p1++;
							*p2++ = *p1++;
							continue;
						}
					}

					bChinese = !bChinese;
				}
			}

			// 字符转换为 16 进制数
			if (bChinese)
			{
				BYTE bValue = 0;
				if (*p1 >= 'A')
					bValue |= *p1 - 'A' + 10;
				else
					bValue |= *p1 - '0';
				bValue <<= 4;
				p1++;

				if (*p1 >= 'A')
					bValue |= *p1 - 'A' + 10;
				else
					bValue |= *p1 - '0';
				p1++;

				*p2++ = bValue;

				continue;
			}
		}
		
		if (*p1 == '\n')
		{
			bString = FALSE;
			bComment = FALSE;
		}
		
		*p2++ = *p1++;
	}
	
	*p2 = '\0';

	return lpChinese;
}


#define USES_CHINESEVAR_CONVERSION int _convertT; _convertT; LPCSTR _lpaT; _lpaT; LPCWSTR _lpwT; _lpwT;

#define VARC2A(lpa) (\
	((_lpaT = lpa) == NULL) ? NULL : (\
		_convertT = lstrlen(_lpaT) * 4 + 1,\
		ChineseToASCII(_lpaT, (LPSTR)alloca(_convertT))))

#define VARA2C(lpa) (\
	((_lpaT = lpa) == NULL) ? NULL : (\
		_convertT = lstrlen(_lpaT) + 1,\
		ASCIIToChinese(_lpaT, (LPSTR)alloca(_convertT))))

inline BOOL IsValidVBScriptVar(LPCSTR lpszName)
{
	USES_CHINESEVAR_CONVERSION;
	
	lpszName = VARC2A(lpszName);
	
	if (!isalpha(*lpszName++))
		return FALSE;
	
	while (*lpszName != 0)
	{
		if (!isalnum(*lpszName) && *lpszName != '_')
			return FALSE;
		lpszName++;
	}
	
	return TRUE;
}

inline BOOL IsValidVBScriptVarW(LPCWSTR lpszName)
{
	USES_CONVERSION;
	
	LPCSTR lpszNameT = W2A(lpszName);
	return IsValidVBScriptVar(lpszNameT);
}

#endif // __CHINESEVARUTILS_H__