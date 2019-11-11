// Buffer_Authentication.h
// Jenny Moon & Ryan O'Donnell
// Serialization and deserialization of basic types

#ifndef BUFFER_AUTHENTICATION_HG
#define BUFFER_AUTHENTICATION_HG

#include <vector>

class Buffer {
public:
	Buffer(size_t size);
	std::vector<char> mBuffer;

	void ResizeBuffer(size_t size);

	void WriteInt32LE(int value);
	int ReadInt32LE(void);

	void WriteShort16LE(short value);
	short ReadShort16LE(void);

private:
	int mReadIndex;
	int mWriteIndex;
};

#endif
