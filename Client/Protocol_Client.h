// Protocol_Client.h
// Jenny Moon & Ryan O'Donnell
// Creates and reads the formatting of the packets needed for sending and receiving

#ifndef PROTOCOL_CLIENT_HG
#define PROTOCOL_CLIENT_HG

#include "Buffer_Client.h"
#include <string>

struct Body {
	std::string roomName;
	std::string message;
	std::string userName;
	std::string email;
	std::string password;
};

struct Header {
	size_t packetLength;
	short commandId;
};

class Protocol {
public:
	Protocol();
	~Protocol();

	void CreateBuffer(size_t index);
	void ReadHeader(Buffer &myBuffer);

	void SendRegister(Buffer& myBuffer);
	void SendLogin(Buffer& myBuffer);

	void JoinRoom(Buffer &myBuffer);
	void LeaveRoom(Buffer &myBuffer);

	void ReceiveMessage(Buffer &myBuffer);
	void ReceiveUsername(Buffer &myBuffer);
	void SendMessages(Buffer &myBuffer, int id);
	void SendMessages(Buffer &myBuffer);

	Header messageHeader;
	Body messageBody;
	Buffer* buffer;
};
#endif
