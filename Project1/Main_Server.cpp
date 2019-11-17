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

#include "Protocol_Server.h"
#include "Project2.pb.h"

using namespace project2;

struct Client
{
	SOCKET Connection;
	std::string roomName;
	std::string message;
	std::string userName;
	std::string email;
	std::string password;
};

// Client Variables
SOCKET ConnectionClient;
int commandID;
bool run = true;
void ServerClientThread();
void SendToAuthentication(int id, std::string message);

// Server Variables
Client Clients[100];
std::vector<int> lobby;
int clientsCounter = 0;

void HandleClients(int index);
void SendMessageToClient(SOCKET theConnection, int id, std::string message, std::string username);
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

	// Creating client socket to connect to authentication server
	std::cout << "Connecting to authentication server..." << std::endl;
	SOCKADDR_IN addrClient;

	int sizeofadr = sizeof(addrClient);
	inet_pton(AF_INET, "127.0.0.1", &addrClient.sin_addr.s_addr);
	addrClient.sin_port = htons(123456);
	addrClient.sin_family = AF_INET; //IPv4 

	ConnectionClient = socket(AF_INET, SOCK_STREAM, NULL); //Creates connection socket
	if (connect(ConnectionClient, (SOCKADDR*)&addrClient, sizeofadr) != 0)
	{
		MessageBox(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
	}

	std::cout << "Connected!" << std::endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerClientThread, NULL, NULL, NULL); //Create a thread

	// Creating chat server
	std::cout << "Starting chat server..." << std::endl;
	SOCKADDR_IN addr;

	int addrlen = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(1234567);
	addr.sin_family = AF_INET; //IPv4 

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Creates socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // Binds the address to the socket
	listen(sListen, SOMAXCONN); //Listen the socket

	//To accept a connection
	SOCKET newConnection; //Socket to hold the client's connection
	for (int i = 0; i < 10; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0)
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else
		{
			std::cout << "New client Connected!" << std::endl;

			Clients[i].Connection = newConnection;
			lobby.push_back(clientsCounter);
			clientsCounter++;
			std::cout << "Number of Connections: " << lobby.size() << std::endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HandleClients, (LPVOID)(i), NULL, NULL); //Create a thread 
		}
	}


	while (true)
	{
		Protocol* messageSendProtocol = new Protocol();

		std::string input = "";
		std::getline(std::cin, input);
		messageSendProtocol->CreateBuffer(256);
		messageSendProtocol->messageHeader.commandId = commandID;

		std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
		send(ConnectionClient, &packet[0], packet.size(), 0);
		Sleep(10);
	}

	system("pause");
	return 0;
}

// Receives messages from the authentication server
void ServerClientThread()
{
	std::vector<char> packet(512);
	int packLength;
	while (run)
	{
		if ((packLength = recv(ConnectionClient, &packet[0], packet.size(), NULL)) < 1) {
			std::cout << "Closing connection" << std::endl;
			closesocket(ConnectionClient);
			WSACleanup();
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

			if (command == 0) // Register success
			{
				CreateAccountSuccess* successAccount = new CreateAccountSuccess();
				successAccount->ParseFromString(packetContents);

				std::string username = successAccount->username().c_str();

				std::cout << "New user created\nUsername: [" << username << "]" << std::endl;

				std::string greet = "Hello [" + username + "]!\nEnter a number (1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";

				SendMessageToClient(Clients[successAccount->requestid()].Connection, 5, "Registration Successful", username);
				SendMessageToClient(Clients[successAccount->requestid()].Connection, 2, greet, "");
			}
			else if (command == 1) // Register failure
			{
				CreateAccountFailure* failAccount = new CreateAccountFailure();
				failAccount->ParseFromString(packetContents);

				std::string reason = "";
				if (failAccount->reason() == failAccount->ACCOUNT_ALREADY_EXISTS)
				{
					reason = "Account already exists";
				}
				else if (failAccount->reason() == failAccount->INTERNAL_SERVER_ERROR)
				{
					reason = "Internal server error";
				}

				SendMessageToClient(Clients[failAccount->requestid()].Connection, 6, reason, "");
			}
			else if (command == 2) // Login Success
			{
				AuthenticateSuccess* successAccount = new AuthenticateSuccess();
				successAccount->ParseFromString(packetContents);
				
				std::string username = successAccount->username().c_str();
				
				Clients[successAccount->requestid()].userName = username;

				std::string creationdate = successAccount->creationdate().c_str();

				std::cout << "Username: [" << username << "] logged in, CreationDate: [" << creationdate << "]" << std::endl;

				std::string result = "Authentication successful, account created on on " + creationdate;

				std::string greet = "Hello [" + username + "]!\nEnter a number (1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";

				SendMessageToClient(Clients[successAccount->requestid()].Connection, 5, result, username);
				SendMessageToClient(Clients[successAccount->requestid()].Connection, 2, greet, "");
			}
			else if (command == 3) // Login failure
			{
				AuthenticateFailure* failAccount = new AuthenticateFailure();
				failAccount->ParseFromString(packetContents);

				std::string reason = "";
				if (failAccount->reason() == failAccount->ACCOUNT_DOES_NOT_EXIST)
				{
					reason = "Account does not exist";
				}
				else if (failAccount->reason() == failAccount->INVALID_PASSWORD)
				{
					reason = "Invalid password";
				}
				else if (failAccount->reason() == failAccount->INTERNAL_SERVER_ERROR)
				{
					reason = "Internal server error";
				}

				SendMessageToClient(Clients[failAccount->requestid()].Connection, 6, reason, "");
			}
		}
	}
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
			Protocol* messageProtocol = new Protocol();
			messageProtocol->CreateBuffer(512);

			messageProtocol->buffer->mBuffer = packet;
			messageProtocol->ReadHeader(*messageProtocol->buffer);

			// Create new user
			if (messageProtocol->messageHeader.commandId == 0)
			{
				messageProtocol->ReceiveRegister(*messageProtocol->buffer);
				Clients[index].email = messageProtocol->messageBody.email;
				Clients[index].password = messageProtocol->messageBody.password;
				Clients[index].userName = messageProtocol->messageBody.userName;

				CreateAccount* newAccount = new CreateAccount();

				newAccount->set_requestid(index);
				newAccount->set_email(messageProtocol->messageBody.email.c_str());
				newAccount->set_plaintextpassword(messageProtocol->messageBody.password.c_str());
				newAccount->set_username(messageProtocol->messageBody.userName.c_str());

				std::string serializedAccount = newAccount->SerializeAsString();

				SendToAuthentication(0, serializedAccount);
			}
			// Log in
			if (messageProtocol->messageHeader.commandId == 1)
			{
				messageProtocol->ReceiveLogin(*messageProtocol->buffer);
				Clients[index].email = messageProtocol->messageBody.email;
				Clients[index].password = messageProtocol->messageBody.password;

				Authenticate* loginAccount = new Authenticate();
				
				loginAccount->set_requestid(index);
				loginAccount->set_email(messageProtocol->messageBody.email.c_str());
				loginAccount->set_plaintextpassword(messageProtocol->messageBody.password.c_str());

				std::string serializedAccount = loginAccount->SerializeAsString();

				SendToAuthentication(1, serializedAccount);
			}

			// Join the room
			if (messageProtocol->messageHeader.commandId == 2)
			{
				std::string setRoom = "\nEnter \'LeaveRoom\' to leave room";
				SendMessageToClient(Clients[index].Connection, 2, setRoom, "");

				messageProtocol->JoinRoom(*messageProtocol->buffer);

				if (messageProtocol->messageBody.roomName == "1")
				{
					Clients[index].roomName = "Prequelmemes";
					std::string message = "[" + Clients[index].userName + "] has joined the room Prequelmemes";
					SendMessageToAllInGroup("Prequelmemes", 4, message);
				}
				else if (messageProtocol->messageBody.roomName == "2")
				{
					Clients[index].roomName = "Chonkers";
					std::string message = "[" + Clients[index].userName + "] has joined the room Chonkers";
					SendMessageToAllInGroup("Chonkers", 4, message);
				}
				else if (messageProtocol->messageBody.roomName == "3")
				{
					Clients[index].roomName = "Rarepuppers";
					std::string message = "[" + Clients[index].userName + "] has joined the room Rarepuppers";
					SendMessageToAllInGroup("Rarepuppers", 4, message);
				}
				else {
					std::string setGroup = "\nWrong group number! Try again!\n1 - Prequelmemes, 2 - Chonkers, 3 - Rarepuppers";
					SendMessageToClient(Clients[index].Connection, 2, setGroup, "");
				}
			}

			// Leave the room
			if (messageProtocol->messageHeader.commandId == 3)
			{
				std::string message = "[" + Clients[index].userName + "] left the room";
				SendMessageToAllInGroup(Clients[index].roomName, 4, message);
				Clients[index].roomName = "";

				std::string setRoom = "\nEnter a number(1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";
				SendMessageToClient(Clients[index].Connection, 2, setRoom, "");
			}

			// Send message
			if (messageProtocol->messageHeader.commandId == 4)
			{
				messageProtocol->ReceiveMessage(*messageProtocol->buffer);
				std::cout << "[" << Clients[index].roomName << "][" << messageProtocol->messageBody.userName << "]: " << messageProtocol->messageBody.message << std::endl;
				std::string message = "[" + messageProtocol->messageBody.userName + "]: " + messageProtocol->messageBody.message;
				SendMessageOthersInGroup(index, Clients[index].roomName, 4, message);
			}

			packLength = 0;
			packet.clear();
			delete messageProtocol;
		}
	}
}

void SendToAuthentication(int id, std::string serializedString)
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

	send(ConnectionClient, &packet[0], packet.size(), 0);
	Sleep(10);
}

// Sends message to current client
void SendMessageToClient(SOCKET theConnection, int id, std::string message, std::string username)
{
	Protocol* messageSendProtocol = new Protocol();
	messageSendProtocol->CreateBuffer(512);
	messageSendProtocol->messageHeader.commandId = id;
	if (id == 5)
	{
		messageSendProtocol->messageBody.userName = username;
	}
	messageSendProtocol->messageBody.message = message;
	messageSendProtocol->SendMessages(*messageSendProtocol->buffer, id);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
	send(theConnection, &packet[0], packet.size(), 0);

	delete messageSendProtocol;
}

// Broadcasts to all (Join/Leave)
void SendMessageToAllInGroup(std::string groupName, int id, std::string message)
{
	Protocol* messageSendProtocol = new Protocol();
	messageSendProtocol->CreateBuffer(512);

	messageSendProtocol->messageHeader.commandId = id;
	messageSendProtocol->messageBody.message = message;

	messageSendProtocol->SendMessages(*messageSendProtocol->buffer, id);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
	for (int i = 0; i < clientsCounter; i++)
	{
		if (Clients[i].roomName == groupName)
		{
			send(Clients[i].Connection, &packet[0], packet.size(), 0);
		}
	}
	delete messageSendProtocol;
}

// Messages others in the room 
void SendMessageOthersInGroup(int clientIndex, std::string roomName, int id, std::string message)
{
	Protocol* messageSendProtocol = new Protocol();
	messageSendProtocol->CreateBuffer(512);
	messageSendProtocol->messageHeader.commandId = id;

	messageSendProtocol->messageBody.message = message;
	messageSendProtocol->SendMessages(*messageSendProtocol->buffer, id);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
	for (int i = 0; i < clientsCounter; i++)
	{
		if (clientIndex == i)
		{
			continue;
		}
		if (Clients[i].roomName == roomName)
		{
			send(Clients[i].Connection, &packet[0], packet.size(), 0);
		}
	}

	delete messageSendProtocol;
}
