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

#include <iostream>
#include <string>
#include <bitset>
#include "addressbook.pb.h"

using namespace example;

void CreatePerson(
	Person* newPerson,
	std::string name,
	int id,
	std::string email,
	Person_PhoneType type,
	std::string number)
{
	newPerson->set_name(name);
	newPerson->set_id(id);
	newPerson->set_email(email);

	// Need to call add_phones to allocate the memory, the function returns
	// a pointer to the new PhoneNumber to allow you to modify the data.
	Person::PhoneNumber* phoneNumber = newPerson->add_phones();
	phoneNumber->set_number(number);

	phoneNumber->set_type(type);
}

//void EndianExample()
//{
//	int32_t value1 = 1;
//	int32_t value2 = -1;
//
//	char c1 = value1;
//	char c2 = value2;
//
//	std::bitset<32>(value1);
//	std::bitset<32>(value2);
//	std::bitset<8>(c1);
//	std::bitset<8>(c2);
//}

int main(int argc, char** argv)
{
	//	EndianExample();
	//	system("Pause");
		// Verify headers and lib files are the same version
		// GOOGLE_PROTOBUF_VERIFY_VERSION; might be deprecated.

	AddressBook addressBook;

	Person* newPerson1 = addressBook.add_people();
	CreatePerson(
		newPerson1,
		"Lukas",
		1,
		"l_gustafson@fanshaweonline.ca",
		Person_PhoneType::Person_PhoneType_MOBILE,
		"123-456-1234");

	Person* newPerson2 = addressBook.add_people();
	CreatePerson(
		newPerson2,
		"Jane",
		2,
		"",
		Person_PhoneType::Person_PhoneType_WORK,
		"123-456-56789");

	// Serialize the data into a string
	std::string serializedAddressBook = addressBook.SerializeAsString();
	std::cout << serializedAddressBook << std::endl;
	// This is the info you would send over the network

	// Receive the string/data, and deserialize
	AddressBook received;
	received.ParseFromString(serializedAddressBook);

	// Check data
	int numPeople = received.people_size();
	printf("Received %d people in our address book!\n", numPeople);

	// pause
	system("Pause");

	return 0;
}

