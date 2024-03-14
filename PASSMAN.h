#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

const string server = "tcp://127.0.0.1:3306";
const string db_username = "db-username";
const string db_password = "db-password";
extern sql::Driver* driver;
extern sql::Connection* con;

class Database
{
public:
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;

	void set_schema();
	void create_table();
};

class User : public Database
{
	string password;

	void login();
	void signup();

public:
	string username;

	void user_menu();
};

class Credentials : public Database
{
	string current_user;
	string site_name;
	string site_username;
	string site_password;
	vector<vector<string>> credentials;

	void getall_credentials();
	void get_site_credentials(int index);

public:
	Credentials(string curr_user);
	void credentials_menu();
	void saved_credentials();
	void add_credentials();
};
