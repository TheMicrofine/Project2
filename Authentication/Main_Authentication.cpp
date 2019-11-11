//#include "bcrypt/BCrypt.hpp"
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

//#include <iostream>
//#include <string>
//#include "Project2.pb.h"
//
//using namespace project2;
//
//void CreateNewAccount(
//	CreateAccount* newAccount,
//	std::string email,
//	std::string password,
//	std::string username)
//{
//	newAccount->set_email(email);
//	newAccount->set_plaintextpassword(password);
//	newAccount->set_username(username);
//}
//
//int main(int argc, char** argv)
//{
//	CreateAccount* newAccount1 = new CreateAccount();
//
//	CreateNewAccount(
//		newAccount1,
//		"l_gustafson@fanshaweonline.ca",
//		"12345",
//		"Lukas");
//
//	// Serialize the data into a string
//	std::string serializedAccount = newAccount1->SerializeAsString();
//	std::cout << serializedAccount << std::endl;
//	// This is the info you would send over the network
//
//	// Receive the string/data, and deserialize
//	CreateAccount* received = new CreateAccount();
//	received->ParseFromString(serializedAccount);
//
//	// Check data
//	printf("Password: %s\n", received->plaintextpassword().c_str());
//
//	// pause
//	system("Pause");
//
//	return 0;
//}
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
///*
//$(SolutionDir)dev\include;
//$(SolutionDir)dev\lib\$(Platform)\$(Configuration);
//
//lib files, libcrypto.lib, libss.lib, mysqlcppconn.lib*/
//
//sql::Driver* driver;
//sql::Connection* con;
//sql::Statement* stmt;
//sql::PreparedStatement* pstmt;
//sql::ResultSet* rs;
//
//std::string server = "127.0.0.1:3306";
//std::string username = "root";
//std::string password = "root";
//std::string schema = "info6016";
//
//
//
//void InsertExample()
//{
//
//	//sql::PreparedStatement* addEmployee =
//	//con->prepareStatement("INSERT INTO employees (Employee, Department_ID) VALUES($1, $2);");
//	// Easy example: Run a insert command, and create an employee.
//	try
//	{
//		// #1 Prepare a statement
//		//pstmt = con->prepareStatement("INSERT INTO employees (Employee, Department_ID) VALUES('Lukas', 5);");
//
//		pstmt = con->prepareStatement("INSERT INTO employees (Employee, Department_ID) VALUES('potatop', ?);");
//		pstmt->setInt(1, 5.0);
//
//		// #2 Execute the statement
//		int result = pstmt->executeUpdate();
//
//		// #3 Check the result
//		printf("%d row(s) affected.\n", result);
//	}
//	catch (sql::SQLException & exception)
//	{
//		std::cout << "# ERR: SQLException in " << __FILE__;
//		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
//		std::cout << "# ERR: " << exception.what();
//		std::cout << " (MySQL error code: " << exception.getErrorCode();
//		std::cout << ", SQLState: " << exception.getSQLState() << ")" << std::endl;
//		system("Pause");
//	}
//}
//
//int main(int argc, char** argv)
//{
//	try
//	{
//		driver = get_driver_instance();
//		con = driver->connect(server, username, password);
//		printf("Successfully connected to our database!\n");
//
//		con->setSchema(schema);
//		printf("Successfully set our schema!\n");
//	}
//	catch (sql::SQLException & exception)
//	{
//		std::cout << "# ERR: SQLException in " << __FILE__;
//		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
//		std::cout << "# ERR: " << exception.what();
//		std::cout << " (MySQL error code: " << exception.getErrorCode();
//		std::cout << ", SQLState: " << exception.getSQLState() << ")" << std::endl;
//		system("Pause");
//		return 1;
//	}
//
//	InsertExample();
//
//	// Medium example: Retrieve the employees table
//	// SELECT * FROM employees;
//	try
//	{
//		// #1 Prepare our statement
//		pstmt = con->prepareStatement("SELECT * FROM employees;");
//
//		// #2 Execute our statement
//		rs = pstmt->executeQuery();
//
//		// #3 Show the result
//		if (rs->rowsCount() > 0)
//		{
//			printf("%d rows returned!\n", (int)rs->rowsCount());
//
//			printf("Employee List:\n");
//			while (rs->next())
//			{
//				std::string name = rs->getString(1);
//				int department_id = rs->getInt(2);
//
//				std::cout << "Name: " << name << std::endl;
//				std::cout << "Department ID: " << department_id << std::endl;
//			}
//		}
//		else
//		{
//			printf("No rows returned!\n");
//		}
//	}
//	catch (sql::SQLException & exception)
//	{
//		std::cout << "# ERR: SQLException in " << __FILE__;
//		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
//		std::cout << "# ERR: " << exception.what();
//		std::cout << " (MySQL error code: " << exception.getErrorCode();
//		std::cout << ", SQLState: " << exception.getSQLState() << ")" << std::endl;
//		system("Pause");
//		return 1;
//	}
//
//
//	system("Pause");
//	return 0;
//}


// Main_Server.cpp
// Jenny Moon & Ryan O'Donnell
// Non-blocking server using threads

#define WIN32_LEAN_AND_MEAN // Strips out rarely used calls

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>

#include "Protocol_Authentication.h"
#include "Project2.pb.h"

using namespace project2;

struct Client
{
	SOCKET Connection;
};

Client Clients[100];
std::vector<int> lobby;

int clientsCounter = 0;
void HandleClients(int index);
void SendMessageToClient(SOCKET theConnection, int id, std::string message);
void SendMessageToAllInGroup(std::string groupName, int id, std::string message);
void SendMessageOthersInGroup(int clientIndex, std::string groupName, int id, std::string message);

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
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HandleClients, (LPVOID)(i), NULL, NULL); //Create a thread 
		}
	}

	system("pause");
	return 0;
}

void HandleClients(int index)
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
				// Check data
				std::cout << "Email: [" << newAccount->email().c_str() << "]\nPassword: [" << newAccount->plaintextpassword().c_str() << "]\nUsername: [" << newAccount->username().c_str() << "]" << std::endl;
			}
			else if (command == 1)
			{
				Authenticate* loginAccount = new Authenticate();
				loginAccount->ParseFromString(packetContents);
				// Check data
				std::cout << "Email: [" << loginAccount->email().c_str() << "]\nPassword: [" << loginAccount->plaintextpassword().c_str() << "]" << std::endl;
			}
		


			//	
			//// Create name
			//if (messageProtocol->messageHeader.commandId == 0)
			//{
			//
			//	

			/*	messageProtocol->ReceiveName(*messageProtocol->buffer);
				Clients[index].name = messageProtocol->messageBody.name;
				std::string greet = "Hello [" + messageProtocol->messageBody.name + "]!\nEnter a number (1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";
				SendMessageToClient(Clients[index].Connection, 2, greet);*/
			//}

			//// Join the room
			//if (messageProtocol->messageHeader.commandId == 2)
			//{
			//	std::string setRoom = "\nEnter \'LeaveRoom\' to leave room";
			//	SendMessageToClient(Clients[index].Connection, 2, setRoom);

			//	messageProtocol->JoinRoom(*messageProtocol->buffer);

			//	if (messageProtocol->messageBody.roomName == "1")
			//	{
			//		Clients[index].room = "Prequelmemes";
			//		std::string message = "[" + Clients[index].name + "] has joined the room Prequelmemes";
			//		SendMessageToAllInGroup("Prequelmemes", 4, message);
			//	}
			//	else if (messageProtocol->messageBody.roomName == "2")
			//	{
			//		Clients[index].room = "Chonkers";
			//		std::string message = "[" + Clients[index].name + "] has joined the room Chonkers";
			//		SendMessageToAllInGroup("Chonkers", 4, message);
			//	}
			//	else if (messageProtocol->messageBody.roomName == "3")
			//	{
			//		Clients[index].room = "Rarepuppers";
			//		std::string message = "[" + Clients[index].name + "] has joined the room Rarepuppers";
			//		SendMessageToAllInGroup("Rarepuppers", 4, message);
			//	}
			//	else {
			//		std::string setGroup = "\nWrong group number! Try again!\n1 - Prequelmemes, 2 - Chonkers, 3 - Rarepuppers";
			//		SendMessageToClient(Clients[index].Connection, 2, setGroup);
			//	}
			//}

			//// Leave the room
			//if (messageProtocol->messageHeader.commandId == 3)
			//{
			//	std::string message = "[" + Clients[index].name + "] left the room";
			//	SendMessageToAllInGroup(Clients[index].room, 4, message);
			//	Clients[index].room = "";

			//	std::string setRoom = "\nEnter a number(1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";
			//	SendMessageToClient(Clients[index].Connection, 2, setRoom);
			//}

			//// Send message
			//if (messageProtocol->messageHeader.commandId == 4)
			//{
			//	messageProtocol->ReceiveMessage(*messageProtocol->buffer);
			//	std::cout << "[" << Clients[index].room << "][" << messageProtocol->messageBody.name << "]: " << messageProtocol->messageBody.message << std::endl;
			//	std::string message = "[" + messageProtocol->messageBody.name + "]: " + messageProtocol->messageBody.message;
			//	SendMessageOthersInGroup(index, Clients[index].room, 4, message);
			//}
			
			packLength = 0;
			packet.clear();
			//delete messageProtocol;
		}
	}
}

// Sends message to current client
//void SendMessageToClient(SOCKET theConnection, int id, std::string message)
//{
//	Protocol* messageSendProtocol = new Protocol();
//	messageSendProtocol->CreateBuffer(512);
//	messageSendProtocol->messageHeader.commandId = id;
//	messageSendProtocol->messageBody.message = message;
//	messageSendProtocol->SendMessages(*messageSendProtocol->buffer, id);
//
//	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
//	send(theConnection, &packet[0], packet.size(), 0);
//
//	delete messageSendProtocol;
//}
