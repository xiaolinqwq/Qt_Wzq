#include "sqlcon.h"
#include <iostream>
Sqlcon::Sqlcon()
{
    mysql = mysql_init(NULL);
    if(mysql == NULL) {
        std::cout << "ERROR: " << mysql_error(mysql);
        exit(-1);
    }
}

Sqlcon::~Sqlcon()
{
    if(!mysql){
        mysql_close(mysql);
    }
}
bool Sqlcon::InitDB(std::string host,std::string user,std::string pwd,std::string dbname)
{
    if(!mysql_real_connect(mysql,host.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),0,NULL,0)) {
		std::cout << "connect fial: "<< mysql_error(mysql);
		exit(-1);
	}
	return true;
}

bool Sqlcon::PlaySQL(std::string sql)
{
    if(mysql_query(mysql, sql.c_str())){
        std::cout << "ERROR: " << mysql_error(mysql);
        return false;
    }
    res = mysql_store_result(mysql);
    return true;
}

void Sqlcon::resfree()
{
    mysql_free_result(res);
}

MYSQL_RES* Sqlcon::get_res()
{
    return res;
}