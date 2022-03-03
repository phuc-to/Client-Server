/**
 @file auth.cpp
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#include "Auth.h"
#include <unordered_map>

Auth::Auth()
{

}

Auth::~Auth()
{
}

bool Auth::SignUp(const string &username, const string &password, const string &admin)
{

	// New user, create new account and assign admin priveledges. 
	if (this->userLookupMap.find(username) == userLookupMap.end()) {
		this->userLookupMap.insert({ username, true });  // Add new user.
	}
	// User already exists. 
	else if (this->userLookupMap.find(username) != userLookupMap.end()) {
		return false; 
	}

	// Check for admin priveledges and assign. 
	if (admin == "Y")
		this->AssignAdmin(username);
	return true;
}

bool Auth::AssignAdmin(const string & username)
{
	if (this->userLookupMap.find(username) != userLookupMap.end()) {
		return false;
		// Store new user with admin status in map. 
		if (this->adminLookupMap.find(username) == adminLookupMap.end()) {
			this->adminLookupMap.insert({ username, true });
		}

		return true;
	}
}

bool Auth::Login(const string & username, const string & password)
{
	string storedPassword; 

	// Check that user already exists. If not, return false. 
	// User doesn't exist
	if (this->userLookupMap.find(username) == userLookupMap.end())
		return false;
	// User exists
	else {
		// Check passwords match. 
		storedPassword = userLookupMap[username]; 
		if (storedPassword != password)
			return false; 
	}

	return true;
}
;