#include<iostream>
#include"CPool.h"

void main()
{
	MySQLConn mysql_conn("localhost","root","test","1234",3306);
	ConnectionPool cPooling(100, &mysql_conn);

	cout << "»ý¼ºµÈ pool :"<<cPooling.getPoolState().size_pool << endl;
	char* str=nullptr;
	mysql_conn.QueryDataBase(str,"select * from users;",cPooling.getConnection());
	while (1)
	{

	}
}