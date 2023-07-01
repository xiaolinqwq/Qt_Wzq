#ifndef __SQLCON_H__
#define __SQLCON_H__
#include <mysql/mysql.h>
#include <string>

class Sqlcon{
public:
	Sqlcon();
	~Sqlcon();
	bool InitDB(std::string host,std::string user,std::string pwd,std::string dbname);
	bool PlaySQL(std::string sql);
    MYSQL_RES* get_res();
	void resfree();
private:
	MYSQL* mysql;
public:
	MYSQL_ROW row;
	MYSQL_RES* res;
    int res_nums;

};

#endif