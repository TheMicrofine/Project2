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

// Server Variables
Client Clients[100];
std::vector<int> lobby;

int clientsCounter = 0;
void HandleClients(int index);
void SendMessageToClient(SOCKET theConnection, int id, std::string message);
void SendMessageToAllInGroup(std::string groupName, int id, std::string message);
void SendMessageOthersInGroup(int clientIndex, std::string groupName, int id, std::string message);

void CreateNewAccount(
	CreateAccount* newAccount,
	std::string email,
	std::string password,
	std::string username)
{
	newAccount->set_requestid(0);
	newAccount->set_email(email);
	newAccount->set_plaintextpassword(password);
	newAccount->set_username(username);
}

void CreateAuthenticate(
	Authenticate* login,
	std::string email,
	std::string password)
{
	login->set_requestid(1);
	login->set_email(email);
	login->set_plaintextpassword(password);
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
	for (int i = 0; i < 100; i++)
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

		//if (commandID == 2)
		//{
		//	messageSendProtocol->messageBody.roomName = input.c_str();
		//	messageSendProtocol->JoinRoom(*messageSendProtocol->buffer);
		//}
		//else if (commandID == 4)
		//{
		//	if (input == "LeaveRoom")
		//	{
		//		messageSendProtocol->LeaveRoom(*messageSendProtocol->buffer);
		//	}
		//	else
		//	{
		//		messageSendProtocol->messageBody.name = nameProtocol->messageBody.name;
		//		messageSendProtocol->messageBody.message = input.c_str();
		//		messageSendProtocol->SendMessages(*messageSendProtocol->buffer);
		//	}
		//}

		std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
		send(ConnectionClient, &packet[0], packet.size(), 0);
		Sleep(10);
	}


	system("pause");
	return 0;
}

// Handle the client thread and receives messages from the server
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
			Protocol* messageProtocol = new Protocol();
			messageProtocol->CreateBuffer(512);

			messageProtocol->buffer->mBuffer = packet;
			messageProtocol->ReadHeader(*messageProtocol->buffer);

			messageProtocol->buffer->ResizeBuffer(messageProtocol->messageHeader.packetLength);

			messageProtocol->ReceiveMessage(*messageProtocol->buffer);
			std::cout << messageProtocol->messageBody.message << std::endl;
			commandID = messageProtocol->messageHeader.commandId;

			delete messageProtocol;
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

				CreateNewAccount(
					newAccount,
					messageProtocol->messageBody.email.c_str(),
					messageProtocol->messageBody.password.c_str(),
					messageProtocol->messageBody.userName.c_str());

				std::string serializedAccount = newAccount->SerializeAsString();

				// Packet -> [command][size][contents]
				std::vector<char> packet;
				packet.push_back(0);
				packet.push_back(serializedAccount.length());

				const char* temp = serializedAccount.c_str();
				for (int i = 0; i < serializedAccount.length(); i++)
				{
					packet.push_back(temp[i]);
				}

				send(ConnectionClient, &packet[0], packet.size(), 0);
				Sleep(10);
			}
			// Log in
			if (messageProtocol->messageHeader.commandId == 1)
			{
				messageProtocol->ReceiveLogin(*messageProtocol->buffer);
				Clients[index].email = messageProtocol->messageBody.email;
				Clients[index].password = messageProtocol->messageBody.password;

				Authenticate* loginAccount = new Authenticate();

				CreateAuthenticate(
					loginAccount,
					messageProtocol->messageBody.email.c_str(),
					messageProtocol->messageBody.password.c_str());

				std::string serializedAccount = loginAccount->SerializeAsString();

				// Packet -> [command][size][contents]
				std::vector<char> packet;
				packet.push_back(1);
				packet.push_back(serializedAccount.length());

				const char* temp = serializedAccount.c_str();
				for (int i = 0; i < serializedAccount.length(); i++)
				{
					packet.push_back(temp[i]);
				}

				send(ConnectionClient, &packet[0], packet.size(), 0);
				Sleep(10);
			}
			//// Create name
			//if (messageProtocol->messageHeader.commandId == 1)
			//{
			//	messageProtocol->ReceiveName(*messageProtocol->buffer);
			//	Clients[index].userName = messageProtocol->messageBody.userName;
			//	std::string greet = "Hello [" + messageProtocol->messageBody.userName + "]!\nEnter a number (1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";
			//	SendMessageToClient(Clients[index].Connection, 2, greet);
			//}

			// Join the room
			if (messageProtocol->messageHeader.commandId == 2)
			{
				std::string setRoom = "\nEnter \'LeaveRoom\' to leave room";
				SendMessageToClient(Clients[index].Connection, 2, setRoom);

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
					SendMessageToClient(Clients[index].Connection, 2, setGroup);
				}
			}

			// Leave the room
			if (messageProtocol->messageHeader.commandId == 3)
			{
				std::string message = "[" + Clients[index].userName + "] left the room";
				SendMessageToAllInGroup(Clients[index].roomName, 4, message);
				Clients[index].roomName = "";

				std::string setRoom = "\nEnter a number(1 - 3) to join a room!\n1 - Prequel Memes, 2 - Chonkers, 3 - Rare puppers";
				SendMessageToClient(Clients[index].Connection, 2, setRoom);
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

// Sends message to current client
void SendMessageToClient(SOCKET theConnection, int id, std::string message)
{
	Protocol* messageSendProtocol = new Protocol();
	messageSendProtocol->CreateBuffer(512);
	messageSendProtocol->messageHeader.commandId = id;
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
