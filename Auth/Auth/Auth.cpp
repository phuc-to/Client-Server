#include "Auth.h"
#include <unordered_map>

Auth::Auth()
{
	username;
	password;
	isAdmin = false;
	isConnected = false;
}

Auth::~Auth()
{
}
