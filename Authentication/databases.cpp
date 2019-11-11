const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

int RegisterAccount(std::string email, std::string password, std::string userName)
{
	try
	{
		// Check if email already exists
		pstmt = con->prepareStatement("SELECT * FROM web_authentication WHERE email = '" + email + "';");
		rs = pstmt->executeQuery();

		if (rs->rowsCount() > 0)
		{
			std::cout << "Account already exists" << std::endl;
			return 1;
		}
		else
		{
			// Add account into user
			pstmt = con->prepareStatement("INSERT INTO user(Last_Login, Creation_Date, User_Name) VALUES(? , ? , ? );");
			pstmt->setDateTime(1, currentDateTime());
			pstmt->setDateTime(2, currentDateTime());
			pstmt->setString(3, userName);
			rs = pstmt->executeQuery();

			// Get user name from user table for userID in web_authentication
			pstmt = con->prepareStatement("SELECT * FROM user WHERE User_Name = '" + userName + "'");
			rs = pstmt->executeQuery();
			rs->next();
			int userID = rs->getInt64("ID");

			std::string salt = BCrypt::generateSalt(12);
			// Add account into web_authentication
			pstmt = con->prepareStatement("INSERT INTO web_authentication (email, salt, hashed_password, userID) VALUES(? , ? , ? , ? );");
			pstmt->setString(1, email);
			pstmt->setString(2, salt);
			pstmt->setString(3, BCrypt::generateHash(password, salt));
			pstmt->setBigInt(4, std::to_string(userID));
			rs = pstmt->executeQuery();

			std::cout << "Account Created." << std::endl;
		}
	}
	catch (sql::SQLException & exception)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << exception.what();
		std::cout << " (MySQL error code: " << exception.getErrorCode();
		std::cout << ", SQLState: " << exception.getSQLState() << ")" << std::endl;
		system("Pause");
		return 1;
	}
}

int AuthenticateAccount(std::string email, std::string password, std::string userName)
{
	// Confirm if email already exists
	try
	{
		pstmt = con->prepareStatement("SELECT * FROM web_authentication WHERE email = '" + email + "';");
		rs = pstmt->executeQuery();

		if (rs->rowsCount() == 0)
		{
			std::cout << "Account does not exist" << std::endl;
			return 1;
		}
		else
		{
			// Update user's login time
			pstmt = con->prepareStatement("UPDATE user SET Last_Login = '" + currentDateTime() + "' WHERE User_Name = '" + userName + "'");
			rs = pstmt->executeQuery();

			std::cout << "Sign in logged succesfully." << std::endl;
		}
	}
	catch (sql::SQLException & exception)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << exception.what();
		std::cout << " (MySQL error code: " << exception.getErrorCode();
		std::cout << ", SQLState: " << exception.getSQLState() << ")" << std::endl;
		system("Pause");
		return 1;
	}
}
