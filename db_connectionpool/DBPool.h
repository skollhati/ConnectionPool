#include<WinSock2.h>
#include<mysql.h>
#include<Windows.h>

class DBPool{

private:
	DBPool();
	CRITICAL_SECTION *cs;
	unsigned int m_iPoolsize; 
	unsigned int m_iNow;
	MYSQL *m_myConn;
	bool *m_bInUse;


public: 
	static DBPool * getInstance();  
	int Initialize(unsigned int, char*, char*, char*, unsigned int);
	MYSQL* GetConnection(); 
	void ReturnConnection(MYSQL*); 
	void Destroy();
};
unsigned int __stdcall PoolProc(void*);
