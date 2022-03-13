/**
 @file auth.h
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 1.0
 */

#pragma once

#include <string>
#include <unordered_map>

using namespace std;

/*
 * User authorization class that stores usernames, passwords, and status' of client connections. 
 */
class Auth
{
public:
	/** Auth class constructor*/
	Auth();

	/** Auth class destructor*/
	~Auth();

	/*signUp stores a new username and password in userLookupMap and updates admin status
	@param username
	@param password
	@param status, true for admin, false for normal user
	@param admin, "Y" for admin priveledges
	*/
	bool signUp(const string &username, const string &password, const string &admin);

	/**
	AssignAdmin stores admin status of users stored in userLookupMap
	@param username
	@param admin, "Y" for admin priveledges
	*/
	bool assignAdmin(const string &username);

	/**
	Login checks that the user already exists in the Auth object, then logs
	them into the program. 
	@param username
	@param password
	*/
	bool login(const string &username, const string &password);

	/**
	UserLookup checks if there is a user name and password already saved to the map.
	If not, inits new user message to client, else, checks if admin priveleges should
	be on.
	@param username
	@param password
	@return true for exists
	@pre user doesn't exist or is not logged in
	*/
	bool userLookup(const string &username, const string &password);

private: 
	string username;
	string password;

	// key:usernamepassword; value:true/false for logged in 
	unordered_map<string, string> userLookupMap; 

	// key:username, value:true/false
	unordered_map<string, bool> adminLookupMap;  

};

