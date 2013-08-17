// TagPaser.h : 标签解析

#ifndef __TAGPASER_H_
#define __TAGPASER_H_

class CTagPaser
{
public:
	CComBSTR m_bstrText;


	CTagPaser()
	{
	}

	CTagPaser(BSTR bstrText)
	{
		m_bstrText = bstrText;
	}

	void PaserText(BSTR bstrText)
	{
		m_bstrText = bstrText;
	}
	static BOOL ReadStrictTag(BSTR bstrText, BSTR* pbstrNode, BSTR* pbstrBlock, BSTR* pbstrProp)
	{
		OLECHAR* p = bstrText;

		//过滤空格
		while (*p == OLECHAR(' '))
			p++;

		if ((p[0] == OLECHAR('T') || p[0] == OLECHAR('t')) &&
			(p[1] == OLECHAR('A') || p[1] == OLECHAR('a')) &&
			(p[2] == OLECHAR('G') || p[2] == OLECHAR('g')) &&
			(p[3] == OLECHAR('S') || p[3] == OLECHAR('s')) &&
			p[4] == OLECHAR('.'))
		{
			p = p + 5;
			int n = ScanNodeName(p);
			if (n > 0)
			{
				//读入变量名称
				if (*pbstrBlock = ScanDotString(p))
				{
					*pbstrProp = ScanDotString(p);
				}
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL ReadTag(OLECHAR*& p, BSTR* pbstrNode, BSTR* pbstrBlock, BSTR* pbstrProp)
	{
		OLECHAR* p1;
		int nTotal = ::SysStringLen(m_bstrText) - (p - m_bstrText.m_str);
		while (nTotal)
		{
			if ((p[0] == OLECHAR('T') || p[0] == OLECHAR('t')) &&
				(p[1] == OLECHAR('A') || p[1] == OLECHAR('a')) &&
				(p[2] == OLECHAR('G') || p[2] == OLECHAR('g')) &&
				(p[3] == OLECHAR('S') || p[3] == OLECHAR('s')) &&
				p[4] == OLECHAR('.'))
			{
				p1 = p + 5;
				int n = ScanNodeName(p1);
				if (n > 0)
				{
					//读入变量名称
					if (*pbstrBlock = ScanDotString(p1))
					{
						*pbstrProp = ScanDotString(p1);
					}
					p = p1;
					return TRUE;
				}
				p++;
				nTotal--;
			}
		}

		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////////
	//功 能：读入结点名称
	//参 数：字符串指针
	//返 回：结点名长度
	/////////////////////////////////////////////////////////////////////////
	static int ScanNodeName(OLECHAR*& p)
	{
		if ((p[0] == OLECHAR('T') || p[0] == OLECHAR('t')) &&
			(p[1] == OLECHAR('H') || p[1] == OLECHAR('h')) &&
			(p[2] == OLECHAR('I') || p[2] == OLECHAR('i')) &&
			(p[3] == OLECHAR('S') || p[3] == OLECHAR('s')) &&
			(p[4] == OLECHAR('N') || p[4] == OLECHAR('n')) &&
			(p[5] == OLECHAR('O') || p[5] == OLECHAR('o')) &&
			(p[6] == OLECHAR('D') || p[6] == OLECHAR('d')) &&
			(p[7] == OLECHAR('E') || p[7] == OLECHAR('e')))
		{
			p += 8;
			return 8;
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////////////////
	//功 能：读入以点开头的字符串
	//参 数：字符串指针
	//返 回：不包括.号的的合法变量字符串
	/////////////////////////////////////////////////////////////////////////
	static BSTR ScanDotString(OLECHAR*& p)
	{
		if (*p == OLECHAR('.'))
		{
			p++;
			if (iswalpha(*p))
			{
				int iLength = 1;
				while (iswalnum(*(p + iLength)) || *(p + iLength) == OLECHAR('_'))
				{
					iLength++;
				}
				p += iLength;
				return SysAllocStringLen(p - iLength, iLength);
			}
		}
		return NULL;
	}

};

#endif //__TAGPASER_H_