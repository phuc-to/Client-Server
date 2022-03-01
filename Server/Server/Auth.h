/**
 @file auth.h
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

class Auth
{
public:
	/** Auth class constructor*/
	Auth();

	/** Auth class destructor*/
	~Auth();

	/**
	UserLookup checks if there is a user name and password already saved to the map. 
	If not, inits new user message to client, else, checks if admin priveleges should 
	be on.
	@param username
	@param password
	@return true for logged in
	@pre user doesn't exist or is not logged in
	@post user is logged in
	*/
	bool UserLookup(const string username, const string password);

	/**
	AssignAdmin stores admin status of users stored in userLookupMap
	@param key, "usernamepassword"
	@param status, true for admin, false for normal user
	*/
	bool AssignAdmin(const string key);

private: 
	string username;
	string password;

	// key:usernamepassword; value:true/false for logged in 
	unordered_map<string, bool> userLookupMap; 
	// key:username, value:true/false
	unordered_map<string, bool> adminLookupMap;  

};

