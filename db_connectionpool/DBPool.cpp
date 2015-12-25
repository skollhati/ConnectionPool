#include"DBPool.h"

DBPool::DBPool()
{
	cs = NULL;
	m_iPoolsize = 0;
	m_iNow = 0;
	m_myConn = NULL;
	m_bInUse = NULL;
}

DBPool* DBPool::getInstance()
{
	static DBPool* Inst = new DBPool();
	return Inst;
}

int DBPool::Initialize(unsigned int argPoolSize, char *srv, char *id, char *pass, unsigned int port)
{
	m_iPoolsize = argPoolSize;
	m_iNow = 0;

	cs = new CRITICAL_SECTION[m_iPoolsize];
	m_myConn = new MYSQL[m_iPoolsize];
	m_bInUse = new bool[m_iPoolsize];

	for (unsigned int i = 0; i<m_iPoolsize; i++)
	{
		m_bInUse[i] = false;
		InitializeCriticalSection(&cs[i]);
		mysql_init(&m_myConn[i]);
		mysql_real_connect(&m_myConn[i], srv, id, pass, "CamMeeting", port, 0, 0);
	}

	return 0;
}

MYSQL* DBPool::GetConnection()
{
	int i = m_iNow;
	while (true)
	{
		i %= m_iPoolsize;
		if (!m_bInUse[i])
		{
			if (TryEnterCriticalSection(&cs[i]) != 0)
			{
				m_bInUse[i] = true;
				LeaveCriticalSection(&cs[i]);
				return &m_myConn[i];
			}
		}
		i++;
	}
}
void DBPool::ReturnConnection(MYSQL* argSQL)
{
	for (unsigned int i = 0; i<m_iPoolsize; i++)
	{
		if (argSQL == &m_myConn[i])
		{
			m_bInUse[i] = false;
			break;
		}
	}
}

void DBPool::Destroy()
{
	if (m_bInUse != NULL)
	{
		delete[] m_bInUse;
		m_bInUse = NULL;
	}
	if (m_myConn != NULL)
	{
		for (unsigned int i = 0; i<m_iPoolsize; i++)
		{
			mysql_close(&m_myConn[i]);
		}
		delete[] m_myConn;
		m_myConn = NULL;
	}

	if (cs != NULL)
	{
		for (unsigned int i = 0; i<m_iPoolsize; i++)
			DeleteCriticalSection(&cs[i]);

		delete[] cs;
		cs = NULL;
	}
	m_iNow = 0;
	m_iPoolsize = 0;
}
