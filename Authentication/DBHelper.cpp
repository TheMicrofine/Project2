//// DBHelper.cpp
//// Jenny Moon & Ryan O'Donnell
//
//#include "DBHelper.h"
//
//#include "Project2.pb.h"
//
//DBHelper::DBHelper(void)
//{
//
//}
//
//DBHelper::~DBHelper(void)
//{
//
//}
//
//int DBHelper::ConnectToDatabase(
//	const std::string& host,
//	const std::string& user,
//	const std::string& pass,
//	const std::string& db)
//{
//	int status = 0;
//
//	try
//	{
//		// Step #1 Create our driver
//		sql::Driver* driver = get_driver_instance();
//
//		// Step #2 Attempt to connect to the database
//		conn = driver->connect(host.c_str(), user.c_str(), pass.c_str());
//
//		// Step #3 Select our schema
//		conn->setSchema(db.c_str());
//
//		// Step #4 Prepare our statements
//		PrepareStatements();
//	}
//	catch (sql::SQLException e)
//	{
//		// Catch any error, and output here
//		printf("Failed to connect to database with error code: %d\n", e.getErrorCode());
//		printf(e.what());
//		printf("\n");
//		status = 1;
//	}
//
//	return status;
//}
//
//void DBHelper::PrepareStatements(void)
//{
//	mCreateUser = conn->prepareStatement("INSERT INTO user (id, last_login, creation_date) VALUES (?, ?, ?);");
//	mRegisterUser = conn->prepareStatement("INSERT INTO web_auth (id, email, salt, hashed_password, userId) VALUES (?, ?, ?, ?, ?);");
//}
//
//void CheckSQLException()
//{
//	// TODO:
//}
//
//sql::ResultSet* DBHelper::Query(const std::string& query)
//{
//	sql::SQLString sql(query.c_str());
//	sql::PreparedStatement* pStmt = conn->prepareStatement(sql);
//
//	std::cout << query << std::endl;
//	try
//	{
//		return pStmt->executeQuery();
//	}
//	catch (sql::SQLException e)
//	{
//		printf("SelectFromDB error code: %d\n", e.getErrorCode());
//		printf(e.what());
//		printf("\n");
//	}
//
//	return 0;
//}
//
//int DBHelper::QueryUpdate(const std::string& query)
//{
//	sql::SQLString sql(query.c_str());
//	sql::PreparedStatement* pStmt = conn->prepareStatement(sql);
//	try
//	{
//		return pStmt->executeUpdate();
//	}
//	catch (sql::SQLException e)
//	{
//		printf("SelectFromDB error code: %d\n", e.getErrorCode());
//		printf(e.what());
//		printf("\n");
//	}
//
//	return 0;
//}
//
//bool DBHelper::QueryExecute(const std::string& query)
//{
//	sql::SQLString sql(query.c_str());
//	sql::PreparedStatement* pStmt = conn->prepareStatement(sql);
//
//	try
//	{
//		return pStmt->execute();
//	}
//	catch (sql::SQLException e)
//	{
//		printf("SelectFromDB error code: %d\n", e.getErrorCode());
//		printf(e.what());
//		printf("\n");
//	}
//
//	return false;
//}
//
//const std::string currentDateTime() {
//	time_t     now = time(0);
//	struct tm  tstruct;
//	char       buf[80];
//	localtime_s(&tstruct, &now);
//
//	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//
//	return buf;
//}
//
//void DBHelper::RegisterUser(const std::string& id, const std::string& email, const std::string& salt, const std::string& password, const std::string& userId)
//{
//	std::string src_str = password;
//	std::string hash_hex_str;
//
//	sql::ResultSet* s = Query("SELECT * FROM web_auth WHERE email = '" + email + "';");
//
//	if (s->rowsCount() > 0)
//	{
//	//	CreateAccountWebFailure();
//		std::cout << "Account already exists" << std::endl;
//		return;
//	}
//
//	std::string dateTime = currentDateTime();
//
//	mCreateUser->setString(1, userId.c_str());
//	mCreateUser->setDateTime(2, dateTime);
//	mCreateUser->setDateTime(3, dateTime);
//
//	mRegisterUser->setBigInt(1, id.c_str());
//	mRegisterUser->setString(2, email.c_str());
//	mRegisterUser->setString(3, salt.c_str());
//	mRegisterUser->setString(4, hash_hex_str);
//
//	try
//	{
//		mCreateUser->executeUpdate();
//		sql::ResultSet* s = Query("SELECT id FROM user WHERE creation_date = '" + dateTime + "';");
//		s->next();
//		std::cout << s->getInt64("id") << std::endl;
//
//		mRegisterUser->setString(5, std::to_string(s->getInt64("id")));
//
//		mRegisterUser->executeUpdate();
//	}
//	catch (sql::SQLException e)
//	{
//		printf("Register user exception error code: %d\n", e.getErrorCode());
//		printf(e.what());
//		printf("\n");
//	}
//
//	//CreateAccountWebSuccess();
//	std::cout << "Account successfully created" << std::endl;
//}
//
//void DBHelper::AuthenticateUser(const std::string& email, const std::string& password)
//{
//	std::string src_str = password;
//	std::string hash_hex_str;
//
//	sql::ResultSet* s = Query("SELECT * FROM web_auth WHERE email = '" + email + "' AND hashed_password = '" + hash_hex_str + "';");
//	s->next();
//
//	if (s->rowsCount() < 1)
//	{
//		//AuthenticateAccountWebFailure();
//		std::cout << "Account not found" << std::endl;
//		return;
//	}
//
//	std::cout << s->getInt64("userId") << std::endl;
//	mAuthenticateUser = conn->prepareStatement("UPDATE user SET last_login = '" + currentDateTime() + "' WHERE id = " + std::to_string(s->getInt64("userId")) + ";");
//
//	mAuthenticateUser->executeUpdate();
//
//	std::cout << "Account successfully authenticated" << std::endl;
//
//
//}
