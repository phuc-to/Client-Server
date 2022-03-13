/**
 @file auth.cpp
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#pragma once

#include "Auth.h"
#include <unordered_map>

Auth::Auth() {}

Auth::~Auth() {}

bool Auth::signUp(const string &username, const string &password, const string &admin) {

	// New user, create new account and assign admin priveledges. 
	if (this->userLookupMap.find(username) == userLookupMap.end()) {
		this->userLookupMap.insert({ username, password});  // Add new user.
	}
	// User already exists. 
	else
		return false;

	// Check for admin priveledges and assign. 
	if (admin == "Y")
		this->assignAdmin(username);
	return true;
}

bool Auth::assignAdmin(const string & username)
{
	if (this->userLookupMap.find(username) != userLookupMap.end()) {
		return false;
		// Store new user with admin status in map. 
		if (this->adminLookupMap.find(username) == adminLookupMap.end()) {
			this->adminLookupMap.insert({ username, true });
		}
	}
	return true;
}

bool Auth::login(const string & username, const string & password) {
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

bool Auth::userLookup(const string& username, const string& password) {
	if (this->userLookupMap.find(username) == userLookupMap.end())
		return true;
	return false;
}
