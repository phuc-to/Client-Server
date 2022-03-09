/**
 @file  Client side header file for MealTime socket programming project
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 2.0
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> // UNIX socket structs
#include <stdlib.h>
#include <netinet/in.h> // For constants and structs for INTERNET domain addresses  
#include <string.h>
#include <vector>
#include <iterator>

using namespace std; 

class client
{
	public:
		static bool loginValidation(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg);
		static bool mealProcessing(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg);
		static void welcome();
		static void optionList();
		static void mealOptions();
		static void parseTokens(char * buffer, std::vector<std::string>& a);
		static string toLowerCase(string s);
		static bool connectToServer(const char * serverAddress, int port, int & sock);
		static string getUsernamePassword();

	private:


};
