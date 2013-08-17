// SimpleArray2.h: interface for the CSimpleArray2 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SIMPLEARRAY2_H_
#define __SIMPLEARRAY2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class CSimpleArray2 : public CSimpleArray< T >  
{
public:
	int Insert(int nIndex, T& t)
	{
		if (nIndex < 0 || nIndex > m_nSize)
			nIndex = m_nSize;

		if (m_nSize == m_nAllocSize)
		{
			T* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
			if(aT == NULL)
				return -1;
			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}
		m_nSize++;

		memmove((void*)&m_aT[nIndex + 1], (void*)&m_aT[nIndex], (m_nSize - (nIndex + 1)) * sizeof(T));
		SetAtIndex(nIndex, t);

		return nIndex;
	}
};

#endif // __SIMPLEARRAY2_H_
