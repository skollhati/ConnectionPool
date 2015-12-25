#include<WinSock2.h>
#include<mysql.h>
#include<iostream>

typedef MYSQL* ConnPtr;
using namespace std;

class MySQLConn
{
public :
	MySQLConn();

	MySQLConn(char* _Serv, char* _id,char* _db, char* _passwd, unsigned int _port)
	{
		Serv = _Serv;
		id = _id;
		passwd = _passwd;
		port = _port;
		db = _db;
	}

	~MySQLConn()
	{}
	//커넥션 만드는 함수
	MYSQL* CreateConnection();

	//커넥션을 얻고 쿼리문 수행하는 함수
	BOOL QueryDataBase(char* str,const char* command, MYSQL* conn_object);

private:
	char* Serv;
	char* id;
	char* passwd;
	char* db;
	unsigned int port;
	
};


MYSQL* MySQLConn::CreateConnection()
{
	ConnPtr conn_object = new MYSQL();
	
	mysql_init(conn_object);
	
	if (!mysql_real_connect(conn_object, Serv, id, passwd, db, port, NULL, NULL))
		return NULL;
	
	return conn_object;
}

BOOL MySQLConn::QueryDataBase(char* str,const char* command, MYSQL* conn_object)
{
	mysql_query(conn_object, "set session character_set_connection=euckr;");
	mysql_query(conn_object, "set session character_set_results=euckr;");
	mysql_query(conn_object, "set session character_set_client=euckr;");
	mysql_select_db(conn_object, "test");
	if (mysql_query(conn_object, command))
	{
		cout << "error" << endl;
		return false;
	}
	MYSQL_RES *res =  mysql_store_result(conn_object);
	//MYSQL_RES *res = mysql_store_result(conn_object);
	//*str = mysql_num_fields(res);
	MYSQL_ROW sql_row;
	while ((sql_row=mysql_fetch_row(res))!=NULL)
			cout << "셀렉  결과 :" << sql_row[1]<<endl;

		return true;
}


