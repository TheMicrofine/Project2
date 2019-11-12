#include "bcrypt/BCrypt.hpp"
//#include <iostream>
//
//int main() {
//	std::string right_password = "right_password";
//	std::string wrong_password = "wrong_password";
//
//	std::cout << "generate salt... " << std::flush;
//	std::string salt = BCrypt::generateSalt(12);
//	std::cout << "done." << std::endl;
//
//	std::cout << "salt is: " << salt.c_str() << std::endl;
//
//	std::cout << "generate hash... " << std::flush;
//	std::string hash = BCrypt::generateHash(right_password, salt);
//	std::cout << "done." << std::endl;
//
//	std::cout << "hash is: " << hash.c_str() << std::endl;
//
//	std::cout << "checking right password: " << std::flush
//		<< BCrypt::validatePassword(right_password, hash) << std::endl;
//
//	std::cout << "checking wrong password: " << std::flush
//		<< BCrypt::validatePassword(wrong_password, hash) << std::endl;
//
//
//	std::cout << "generate salt... " << std::flush;
//	salt = BCrypt::generateSalt(12);
//	std::cout << "done." << std::endl;
//
//	std::cout << "salt is: " << salt.c_str() << std::endl;
//
//	std::cout << "generate new hash... " << std::flush;
//	hash = BCrypt::generateHash(wrong_password, salt);
//	std::cout << "done." << std::endl;
//
//	std::cout << "hash is: " << hash.c_str() << std::endl;
//
//	std::cout << "checking right password: " << std::flush
//		<< BCrypt::validatePassword(right_password, hash) << std::endl;
//
//	std::cout << "checking wrong password: " << std::flush
//		<< BCrypt::validatePassword(wrong_password, hash) << std::endl;
//
//	system("pause");
//	return 0;
//}


// Main_Authentication.cpp
// Jenny Moon & Ryan O'Donnell
// Authenticates logins and registers with the use of a database

#define WIN32_LEAN_AND_MEAN // Strips out rarely used calls

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "Project2.pb.h"

using namespace project2;

struct Client
{
	SOCKET Connection;
};

Client Clients[100];
std::vector<int> lobby;

int clientsCounter = 0;
void HandleClient(int index);
void SendToClient(SOCKET theConnection, int id, std::string message);
//void SendMessageToAllInGroup(std::string groupName, int id, std::string message);
//void SendMessageOthersInGroup(int clientIndex, std::string groupName, int id, std::string message);

sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* rs;

std::string server = "127.0.0.1:3306";
std::string username = "root";
std::string password = "root";
std::string schema = "project_2";

std::string loginUsername;
std::string creationDate;

const std::string currentDateTime()
{
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

		// TODO: Valid password before adding it into database
		// if not valid return 2;

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
			return 0;
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
		return 3;
	}
}

int AuthenticateAccount(std::string email, std::string password, std::string userName)
{
	// TODO: Get username based from the table and creation date
	loginUsername = "";
	creationDate = "";

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
			return 0;
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
		return 2;
	}
}

int main(void)
{
	//Winsock Startup
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBox(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	std::cout << "Starting authentication server..." << std::endl;

	// Socket addres info
	SOCKADDR_IN addr;

	int addrlen = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(123456);
	addr.sin_family = AF_INET; //IPv4 

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Creates socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // Binds the address to the socket
	listen(sListen, SOMAXCONN); //Listen the socket

	//To accept a connection
	SOCKET newConnection; //Socket to hold the client's connection
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0)
		{
			std::cout << "Failed to accept the chat server's connection." << std::endl;
		}
		else
		{
			std::cout << "Chat server connected!" << std::endl;

			Clients[i].Connection = newConnection;
			lobby.push_back(clientsCounter);
			clientsCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HandleClient, (LPVOID)(i), NULL, NULL); //Create a thread 
		}
	}

	system("pause");
	return 0;
}

void HandleClient(int index)
{
	int packLength;
	bool run = true;

	while (run)
	{
		std::vector<char> packet(512);
		if ((packLength = recv(Clients[index].Connection, &packet[0], packet.size(), NULL)) < 1)
		{
			closesocket(Clients[index].Connection);
			run = false;
		}
		else
		{
			int command = packet[0];
			int length = packet[1];
			std::string packetContents;

			for (int i = 2; i <= length + 1; i++)
			{
				packetContents += packet[i];
			}

			// Create New Account
			if (command == 0)
			{
				CreateAccount* newAccount = new CreateAccount();
				newAccount->ParseFromString(packetContents);

				std::cout << "Email: [" << newAccount->email().c_str() << "]\nPassword: [" << newAccount->plaintextpassword().c_str() << "]\nUsername: [" << newAccount->username().c_str() << "]" << std::endl;

				//TODO: FIX THIS
				//int result = RegisterAccount(newAccount->email().c_str(), newAccount->plaintextpassword().c_str(), newAccount->username().c_str());

				int result = 0;
				std::cout << "Result: " << result << std::endl;

				// 0 = Success, 1 = Account exists, 2 = Invalid password, 3 = Server error
				if (result == 0)
				{
					CreateAccountSuccess* successAccount = new CreateAccountSuccess();
					successAccount->set_requestid(newAccount->requestid());
					successAccount->set_username(newAccount->username());

					std::string serializedResult = successAccount->SerializeAsString();
					SendToClient(Clients[index].Connection, 0, serializedResult);
				}
				else
				{
					CreateAccountFailure* failAccount = new CreateAccountFailure();
					failAccount->set_requestid(newAccount->requestid());

					if (result == 1)
					{
						failAccount->set_reason(failAccount->ACCOUNT_ALREADY_EXISTS);
					}
					else if (result == 2)
					{
						failAccount->set_reason(failAccount->INVALID_PASSWORD);
					}
					else if (result == 3)
					{
						failAccount->set_reason(failAccount->INTERNAL_SERVER_ERROR);
					}

					std::string serializedResult = failAccount->SerializeAsString();
					SendToClient(Clients[index].Connection, 1, serializedResult);
				}
			}
			else if (command == 1)
			{
				Authenticate* loginAccount = new Authenticate();
				loginAccount->ParseFromString(packetContents);
				// Check data
				std::cout << "Email: [" << loginAccount->email().c_str() << "]\nPassword: [" << loginAccount->plaintextpassword().c_str() << "]" << std::endl;

				//TODO: FIX THIS
				//int result = AuthenticateAccount(newAccount->email().c_str(), newAccount->plaintextpassword().c_str());

				int result = 0;
				std::cout << "Result: " << result << std::endl;

				// 0 = Success, 1 = Account doesnt exist, 2 = Server error
				if (result == 0)
				{
					// Placeholders
					loginUsername = loginAccount->email().c_str();
					creationDate = "2019-11-12";

					AuthenticateSuccess* successAccount = new AuthenticateSuccess();
					successAccount->set_requestid(loginAccount->requestid());
					successAccount->set_username(loginUsername);
					successAccount->set_creationdate(creationDate);

					std::string serializedResult = successAccount->SerializeAsString();
					SendToClient(Clients[index].Connection, 2, serializedResult);
				}
				else
				{
					AuthenticateFailure* failAccount = new AuthenticateFailure();
					failAccount->set_requestid(loginAccount->requestid());

					if (result == 1)
					{
						failAccount->set_reason(failAccount->INVALID_CREDENTIALS);
					}
					else if (result == 2)
					{
						failAccount->set_reason(failAccount->INTERNAL_SERVER_ERROR);
					}

					std::string serializedResult = failAccount->SerializeAsString();
					SendToClient(Clients[index].Connection, 3, serializedResult);
				}
			}
			packLength = 0;
			packet.clear();
		}
	}
}

// Sends message to client
void SendToClient(SOCKET connection, int id, std::string serializedString)
{
	// Packet -> [requestId][contentSize][content]
	std::vector<char> packet;
	packet.push_back(id);
	packet.push_back(serializedString.length());

	const char* temp = serializedString.c_str();
	for (int i = 0; i < serializedString.length(); i++)
	{
		packet.push_back(temp[i]);
	}

	send(connection, &packet[0], packet.size(), 0);
	Sleep(10);
}
