
#ifndef __PROMPTTIMEELAPSE_H__
#define __PROMPTTIMEELAPSE_H__

class CPromptTimeElapse
{
public:
	CPromptTimeElapse()
	{
		m_dwTick = GetTickCount();
	}
	~CPromptTimeElapse()
	{
		m_dwTick = GetTickCount() - m_dwTick;
		
		char szBuf[256];
		sprintf(szBuf, "¿˙ ± %d ∫¡√Î£°", m_dwTick);
		MessageBox(NULL, szBuf, NULL, MB_OK);
	}
	
	DWORD m_dwTick;
};

#define PROMPTT_TIME_ELAPSE \
	CPromptTimeElapse mte_##name;

#define PROMPTT_TIME_ELAPSE2(name) \
	CPromptTimeElapse mte_##name;

#endif // __PROMPTTIMEELAPSE_H__