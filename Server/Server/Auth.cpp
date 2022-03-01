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

bool Auth::UserLookup(const string username, const string password)
{
	string key = username + password;
	bool isAdmin = false; 

	// Check if user already exists.
	if (this->userLookupMap.find(key) == userLookupMap.end()) {
		cout << "New user, creating an account...\n";
		this->userLookupMap.insert({key, true});
		AssignAdmin(key);
		cout << "New user created. Logged in successfully\n";
	}
	else {
		this->userLookupMap[key] = true;
		cout << "Logged in successfully\n";
	}
	return true;
}

bool Auth::AssignAdmin(const string key)
{
	bool isAdmin = false; 

	// Prompt new user if they would like admin status. 
	cout << "Would you like to give this account admin access?: ";
	cin >> isAdmin;

	// Store new user with admin status in map. 
	if (this->adminLookupMap.find(key) == adminLookupMap.end()) {
		this->adminLookupMap.insert({ key, true });
	}
	else {
		cout << "This is an admin account. ";
	}
	return true;
}



