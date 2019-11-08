//// DBHelper.h
//// Jenny Moon & Ryan O'Donnell
//
//#ifndef DBHELPER_HG
//#define DBHELPER_HG
//
//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>
//
//#include <string>
//#include <iostream>
//
//
//
//class DBHelper
//{
//public:
//	DBHelper(void);
//	virtual ~DBHelper(void);
//
//	int ConnectToDatabase(const std::string& host, const std::string& user, const std::string& pass, const std::string& db);
//	sql::ResultSet* Query(const std::string& query);
//	int QueryUpdate(const std::string& query);
//	bool QueryExecute(const std::string& query);
//
//	void RegisterUser(const std::string& id, const std::string& email, const std::string& salt, const std::string& password, const std::string& userId);
//	void AuthenticateUser(const std::string& email, const std::string& password);
//private:
//	void PrepareStatements(void);
//
//	sql::Connection* conn;
//
//	sql::PreparedStatement* mCreateUser;
//	sql::PreparedStatement* mRegisterUser;
//	sql::PreparedStatement* mAuthenticateUser;
//};
//
//
//#endif
