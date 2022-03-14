/**
 @file  Client side file for MealTime socket programming project
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 1.0
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <vector>`
#include <iterator>
#include <iostream>

#include "client.h"


using namespace std;

// TCP SETTINGS
const int NAMESPACE = AF_INET;        // internet namespace IP4 
const int STYLE = SOCK_STREAM;        // comm style for TCP programming
const int PROTOCOL = 0;               // 0 for internet 
const int BUFF_SIZE = 1024;           // default buffer size

const char* deli = ";";



int main(int argc, char const* argv[])
{
    
    int cliSocket = 0;                            // init client socket

    const char* logoffRPC = "disconnect;";

    char buffer[BUFF_SIZE] = { 0 };              // init buffer
    const char* serverAddress = argv[1];         // holds server IP address
    //const int PORT = atoi(argv[2]);           // holds port

	// Hard coded port for testing
	const int PORT = 10327;                      

	// Attempt to connect to server
    bool bConnect = client::connectToServer(serverAddress, PORT, cliSocket);

	// Continue if socket successful connected to server. 
    if (bConnect == true)
    {
        
        int valsend;                    // Bytes sent.
        int valread;                    // Bytes rec'd. 
        vector<string> arrayTokens;     // Holds tokens from input string. 
        const int STATUSTOKEN = 0;      // Index of RPC status ("connect", "disconnect", etc).
        const int INFOTOKEN = 1;        // Index of related info from user. 
        
        string msg;                     // Message to be sent to server in buffer. 

		// Display program welcome message and initial options to select from. 
		client::welcome();

        // Begin user login validation and meal processing. 
        while (client::loginValidation(cliSocket, buffer, valsend, valread, arrayTokens, STATUSTOKEN, INFOTOKEN, msg))
            client::mealProcessing(cliSocket, buffer, valsend, valread, arrayTokens, STATUSTOKEN, INFOTOKEN, msg);               
    }
    else
    {
        printf("Connection failed");
    }

    // Terminate connection
    close(cliSocket);

	// TODO: Disconnect Server inside RPC Server. 

    return 0;
}


bool client::loginValidation(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg)
{
    client::optionList();
    bool isValidUser = false;
    while (!isValidUser) {
        int userSelection;
        string username;
        string password;
        string admin;


        // Prompt user to (1) Login, (2) Signup, (3) Exit. 
        cout << "Enter your option: ";
        cin >> userSelection;

        // Handle (0) Server status report
        if (userSelection == 0) {
            msg = "status;";
            const char* curMsg = msg.c_str();
            strcpy(buffer, curMsg);

            // Send socket to server.
            // Buffer example "connect;narissa;mypass", "signup;phuc;mypass". 
            valsend = send(cliSocket, buffer, strlen(buffer), 0);

            cout << "Status request sent.\n\n";

            sleep(2);

            valread = read(cliSocket, buffer, BUFF_SIZE);

            arrayTokens.clear();
            parseTokens(buffer, arrayTokens);
            string error_code = arrayTokens[STATUSTOKEN];
            if (error_code == "active")
                cout << "The server is running.\n\n";
        }
        
        // Handle (1) Login and (2) Signup. 
        else if ((userSelection == 1) || (userSelection == 2)) {
            if (userSelection == 1) {
                msg = "connect;";
                msg += getUsernamePassword();
            }
            else {
                msg = "signup;";
                msg += getUsernamePassword();

                // Assign admin priveledges to account. 
                cout << "Would you like Admin priveledges on your account (Y or N)? ";
                cin >> admin;
                if (admin == "Y" || "y")
                    msg += "Y";
                else
                    msg += "N";
            };

            // Convert msg into char pointer and copy to buffer. 
            const char* curMsg = msg.c_str();
            strcpy(buffer, curMsg);

            // Send socket to server.
            // Buffer example "connect;narissa;mypass", "signup;phuc;mypass". 
            valsend = send(cliSocket, buffer, strlen(buffer), 0);

            if (userSelection == 1)
                cout << "Login message sent\n\n";
            else
                cout << "Signup message sent\n\n";

            sleep(2);

            valread = read(cliSocket, buffer, BUFF_SIZE);

            arrayTokens.clear();
            parseTokens(buffer, arrayTokens);
            string error_code = arrayTokens[STATUSTOKEN];

            // Server returned successful error code upon login. 
            if (error_code == "successful")
            {
                isValidUser = true;
                cout << "Logged In successful.\n";
            }
            // Server returned failed error code upon login.
            else {
                if (userSelection == 1) cout << "Invalid username/password. Try again!\n";
                else cout << "Account already exists. Try login, or use another identity!\n";
                optionList();
            }
            cout << endl;
        }
        // (3) User has selected to exit. Exit gracefully. 
        else if (userSelection == 3)
        {
            isValidUser = true;
            cout << "Goodbye!";
            return false;
        }
        // User entered invalid selection, calls optionList again. 
        else
        {
            cout << "Invalid option. Let's try again!\n";
            optionList();
        }

    }
    return true;
}

bool client::mealProcessing(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg)
{
    const char* logoffRPC = "disconnect;";

    bool isValidUser = false;  // Loop control var

    while (!isValidUser) {
        int userSelection;
        string info;

		// Display options, prompt user, and store selection. 
        mealOptions();
        cout << "Type in your option: ";
        cin >> userSelection;

		// Switch on selction and build msg for buffer.
        switch (userSelection) {
        case 0:
            isValidUser = !isValidUser;
            msg = "logout;";
            break;
        case 1:
            msg = "meal;Random;";
            break;
        case 2:
            msg = "meal;ByTime;";
            cout << "What time of day are you looking at? ";
            cin >> info;
            msg += toLowerCase(info) + ";";
            break;
        case 3:
            msg = "meal;ByCuisine;";
            cout << "What cuisine do you want? ";
            cin >> info;
            msg += toLowerCase(info) + ";";
            break;
        case 4:
            msg = "addMeal;";
            cout << "What is the name of the meal? ";
            cin >> info;
            msg += info + ";";
            cout << "What is the time of the day for " + info;
            cin >> info;
            msg += toLowerCase(info) + ";";
            cout << "Where does it originate from? ";
            cin >> info;
            msg += toLowerCase(info) + ";";
            break;
        default:
            isValidUser = !isValidUser;
            cout << "Invalid option. Let's start again!\n\n";
            userSelection = 5;
        }

		// For all valid selections, convert msg to buffer and send to server. 
        if ((userSelection <= 4) && (userSelection >= 0)) {
            cout << endl;
            const char* curMsg = msg.c_str();
            strcpy(buffer, curMsg);
            valsend = send(cliSocket, buffer, strlen(buffer), 0);

			
            if (userSelection == 0) // Disconnect
                cout << "Logout message sent.\n\n";
            if (userSelection == 4)
                cout << "Your meal adding request has been sent.\n\n";
            else
                cout << "Your meal request has been sent.\n\n";

            sleep(2);

            valread = read(cliSocket, buffer, BUFF_SIZE);

            arrayTokens.clear();
            parseTokens(buffer, arrayTokens);
            string error_code = arrayTokens[STATUSTOKEN];

            if (userSelection == 0)
                cout << "You are logged out.\n\n";
            else {
                if (userSelection != 4) {
					// Buffer rec'd from server, display response. 
                    if (error_code == "successful") {
                        string mealSuggestion = arrayTokens[INFOTOKEN];

                        cout << "The meal for you would be " << mealSuggestion << "!\n";
                        cout << "Do you need another suggestion? 1 if so and 2 to logout: ";
                        cin >> userSelection;

                        // Disconnect. 
                        if (userSelection == 2) {
                            isValidUser = !isValidUser;

                            strcpy(buffer, "logout;");
                            valsend = send(cliSocket, buffer, strlen(buffer), 0);
                            cout << "Logout message sent.\n\n";

                            sleep(2);

                            valread = read(cliSocket, buffer, BUFF_SIZE);
                            cout << "You are logged out.\n\n";
                        }
                    }
                    else
                        cout << "Invalid meal request. Please try again.\n\n";
                }
                else {
                    if (error_code == "successful")
                        cout << "You succesfully added a meal!\n\n";
                    else
                        cout << "The meal you added already exists.\n\n";
                }
            }
        }
    }
}

void client::welcome() {
	string msg = "Welcome to the Client-Server Meal Generator! \nThis program will pass your requests about food\n" 
		"to and from the client and server to fetch you a\ndelicious meal idea!\n";
	cout << msg << "\n";
}

void client::optionList() {
    cout << "Select an account option: \n";
    cout << "0: Server status.\n";
    cout << "1: Login\n";
    cout << "2: Signup\n";
    cout << "3: Exit\n";
}

void client::mealOptions() {
    cout << "What kind of meal are you looking for? 0 to Logout\n";
    cout << "1. A random meal\n";
    cout << "2. A meal by time\n";
    cout << "3. A meal by cuisine\n";    
    cout << "4. Perhaps you want to add a meal by yourself?\n";
}

void client::parseTokens(char* buffer, std::vector<std::string>& a) {
	char* token;
	char* rest = (char*)buffer;

	while ((token = strtok_r(rest, deli, &rest)))
	{
		a.push_back(token);
	}

	return;
}

string client::toLowerCase(string s) {
    string result = "";
    for (char i : s) {
        result += tolower(i);
    }
    return result;
}

/*
	ConnectToServer will connect to the Server based on command line
*/
bool client::connectToServer(const char* serverAddress, int port, int& sock) {
	struct sockaddr_in serv_addr;
	if ((sock = socket(NAMESPACE, STYLE, PROTOCOL)) < 0) {
		printf("\nClient: Socket creation error\n");
		return false;
	}

	serv_addr.sin_family = NAMESPACE;
	serv_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nClient: Connection Failed\n");
		return false;
	}

	return true;
}

string client::getUsernamePassword() {
	string input, username, password;
	cout << "Please provide your username: ";
	cin >> username;
	cout << "Please provide your password: ";
	cin >> password;
	cout << endl;
	input += username + ';' + password + ";";
	return input; 
}