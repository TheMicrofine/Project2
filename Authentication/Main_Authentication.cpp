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
#include "Project2.pb.h"

using namespace project2;

void CreateNewAccount(
	CreateAccount* newAccount,
	std::string email,
	std::string password,
	std::string username)
{
	newAccount->set_email(email);
	newAccount->set_plaintextpassword(password);
	newAccount->set_username(username);
}

int main(int argc, char** argv)
{
	CreateAccount* newAccount1 = new CreateAccount();

	CreateNewAccount(
		newAccount1,
		"l_gustafson@fanshaweonline.ca",
		"12345",
		"Lukas");

	// Serialize the data into a string
	std::string serializedAccount = newAccount1->SerializeAsString();
	std::cout << serializedAccount << std::endl;
	// This is the info you would send over the network

	// Receive the string/data, and deserialize
	CreateAccount* received = new CreateAccount();
	received->ParseFromString(serializedAccount);

	// Check data
	printf("Password: %s\n", received->plaintextpassword().c_str());

	// pause
	system("Pause");

	return 0;
}

