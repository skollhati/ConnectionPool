#ifndef _DEBUG
#define _DEBUG(x)
#endif


#pragma comment (lib , "ws2_32.lib" )
#include"MySQLConn.h"
#include <deque>
#include <set>
#include<Windows.h>


using namespace std;

//Pool 상태 알림 구조체
struct POOLSTATE
{
	int size_pool;
	int size_rental;
};

class ConnectionPool
{
public:

	ConnectionPool(unsigned int poolsize, MySQLConn* mysql_conn)
	{
		InitializeCriticalSection(&sCS);
		InitializeCriticalSection(&pCS);
		
		//pool_state.size_pool_size = poolsize;
		
		while (this->pool.size() < poolsize)
		{
			pool.push_back(mysql_conn->CreateConnection());
		}
		pool_state.size_pool = this->pool.size();
	}

	~ConnectionPool()
	{}

	POOLSTATE getPoolState()
	{
		EnterCriticalSection(&sCS);

		POOLSTATE state;
		state.size_pool = pool_state.size_pool;
		state.size_rental = pool_state.size_rental;

		LeaveCriticalSection(&sCS);
		return state;
	}

	MYSQL* getConnection();
	
		
private:
	CRITICAL_SECTION sCS;
	CRITICAL_SECTION pCS;
	POOLSTATE pool_state;
	deque<MYSQL*> pool;
	set<MYSQL*> rental_pool;

	
};

MYSQL* ConnectionPool::getConnection()
{
	EnterCriticalSection(&pCS);
	MYSQL* temp = pool.front();

	pool.pop_front();
	rental_pool.insert(temp);
	LeaveCriticalSection(&pCS);

	return temp;
}