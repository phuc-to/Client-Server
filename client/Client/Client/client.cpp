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
#include <vector>`
#include <iterator>
#include <iostream>

#include "client.h"


using namespace std;

/* UTILITY FUNCTION PROTOTYPES */

/** Displays welcome message and program instructions to console. */
void welcome();

/** Converts an input string to lowercase */
string toLowerCase(string s);

/** Prompts user for username and password and collects console input */
string getUsernamePassword();

const char* DELI = ";"; // for parsing 


// TCP SETTINGS
const int NAMESPACE = AF_INET;        // internet namespace IP4 
const int STYLE = SOCK_STREAM;        // comm style for TCP programming
const int PROTOCOL = 0;               // 0 for internet 
const int BUFF_SIZE = 1024;           // default buffer size


/** Client-side driver. */
int main(int argc, char const* argv[])
{
    
    int cliSocket = 0;                 // init client socket

    const char* logoffRPC = "disconnect;";

    char buffer[BUFF_SIZE] = { 0 };      // init buffer
    const char* serverAddress = argv[1]; // holds IP address
    //const int PORT = atoi(argv[2]);    // holds port

	// Hard coded port for testing.
	const int PORT = 10327;                      

	// Attempt to connect to server.
    bool bConnect = client::connectToServer(serverAddress, PORT, cliSocket);

	// Continue if socket successful connected to server. 
    if (bConnect == true) {
        
        int valsend;                    // Bytes sent.
        int valread;                    // Bytes rec'd. 
        vector<string> arrayTokens;     // Holds tokens from input string. 
        const int STATUSTOKEN = 0;      // Index of RPC status ("connect", "disconnect", etc).
        const int INFOTOKEN = 1;        // Index of related info from user. 
        
        string msg;                     // Message to be sent to server in buffer. 

		// Display program welcome message and initial options to select from. 
		welcome();
		client::optionList();

        // Begin user login validation and meal processing. 
        if (client::loginValidation(cliSocket, buffer, valsend, valread, arrayTokens, STATUSTOKEN, INFOTOKEN, msg))
            client::mealProcessing(cliSocket, buffer, valsend, valread, arrayTokens, STATUSTOKEN, INFOTOKEN, msg);               
    }
    else { printf("Client>Connection failed."); }

    // Terminate connection
    close(cliSocket);

	// TODO: Disconnect Server inside RPC Server. 

    return 0;
}


/* CLIENT CLASS FUNCTION DEFINITIONS, SEE CLIENT.H FOR PROTOTYPES */

void client::optionList() {
	cout << "Select an account option: " << endl;
	cout << "1: Login" << endl;
	cout << "2: Signup" << endl;
	cout << "3: Exit" << endl;
}

bool client::loginValidation(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg) {
    bool isValidUser = false;  // Loop control var. 

	// Prompt user for login, signup, or exit until they are succesfully logged in or exit. 
    while (!isValidUser) {
        int userSelection;
        string username, password, admin;

        // Prompt user to (1) Login, (2) Signup, (3) Exit. 
        cout << "Enter your option: ";
        cin >> userSelection;

        // Handle (1) Login and (2) Signup. 
        if ((userSelection == 1) || (userSelection == 2)) {
            if (userSelection == 1) {
                msg = "connect;";
                msg += getUsernamePassword();
            }
            else {
                msg = "signup;";
                msg += getUsernamePassword();

                // Assign admin priveledges to account. 
                cout << "Would you like Admin priveledges on your account (Y or N)?\n";
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
                cout << "Login message sent" << endl << endl;
            else
                cout << "Signup message sent" << endl << endl;

            sleep(2);

            valread = read(cliSocket, buffer, BUFF_SIZE);

            arrayTokens.clear();
            parseTokens(buffer, arrayTokens);
            string error_code = arrayTokens[STATUSTOKEN];

            // Server returned successful error code upon login. 
            if (error_code == "successful") {
                isValidUser = true;
                cout << "Logged In successful." << endl;
            }
            // Server returned failed error code upon login.
            else {
                if (userSelection == 1) cout << "Invalid username/password. Try again!" << endl;
                else cout << "Account already exists. Try login, or use another identity!" << endl;
                optionList();
            }
            cout << endl;
        }
        // (3) User has selected to exit. Exit gracefully. 
        else if (userSelection == 3) {
            isValidUser = true;
            cout << "Goodbye!";
            return false;
        }
        // User entered invalid selection, calls optionList again. 
        else {
            cout << "Invalid option. Let's try again!" << endl;
            optionList();
        }

    }
    return true;
}

bool client::mealProcessing(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, int INFOTOKEN, string msg) {
    const char* logoffRPC = "disconnect;";

    bool isValidUser = false;
    while (!isValidUser) {
        int meal;
        string info;

        mealOptions();
        cout << "Type in your option: ";
        cin >> meal;
        switch (meal) {
        case 0:
            isValidUser = !isValidUser;
            msg = "disconnect;";
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
            cout << "Invalid option. Let's start again!" << endl;
            meal = 5;
        }
        if ((meal <= 4) && (meal >= 0)) {
            cout << endl;
            const char* curMsg = msg.c_str();
            strcpy(buffer, curMsg);
            valsend = send(cliSocket, buffer, strlen(buffer), 0);
            if (meal == 4)
                cout << "Disconnect message sent" << endl << endl;
            else
                cout << "Your meal request has been sent" << endl << endl;

            sleep(2);

            valread = read(cliSocket, buffer, BUFF_SIZE);

            arrayTokens.clear();
            parseTokens(buffer, arrayTokens);
            string error_code = arrayTokens[STATUSTOKEN];

            if (meal == 0)
                cout << "You are disconnected" << endl;
            else {
                if (meal != 4) {
                    if (error_code == "successful") {
                        string mealSuggestion = arrayTokens[INFOTOKEN];
                        cout << "The meal for you would be " << mealSuggestion << endl << endl;

                        cout << "Do you need another suggestion? 1 if so and 2 to logout: ";
                        cin >> meal;
                        if (meal == 2) {
                            isValidUser = !isValidUser;

                            strcpy(buffer, logoffRPC);
                            valsend = send(cliSocket, buffer, strlen(buffer), 0);
                            cout << "Disconnect message sent" << endl << endl;

                            sleep(2);

                            valread = read(cliSocket, buffer, BUFF_SIZE);
                            cout << "You are disconnected" << endl;
                        }
                    }
                    else {
                        //string error = arrayTokens[INFOTOKEN];
                        //cout << "An Error Occured: " << arrayTokens[INFOTOKEN] << endl;
                        cout << "Please try again" << endl << endl;
                    }
                }
                else
                    cout << "You succesfully added a meal" << endl << endl;
            }
        }
    }
}

void client::mealOptions() {
    cout << "What kind of meal are you looking for? 0 to Exit" << endl;
    cout << "1. A random meal" << endl;
    cout << "2. A meal by time" << endl;
    cout << "3. A meal by cuisine" << endl;    
    cout << "4. Perhaps you want to add a meal by yourself?" << endl;
}

void client::parseTokens(char* buffer, std::vector<std::string>& a) {
	char* token;
	char* rest = (char*)buffer;

	while ((token = strtok_r(rest, DELI, &rest)))
	{
		a.push_back(token);
	}

	return;
}

bool client::connectToServer(const char* serverAddress, int port, int& sock) {
	struct sockaddr_in serv_addr;
	if ((sock = socket(NAMESPACE, STYLE, PROTOCOL)) < 0) {
		printf("\nClient: Socket creation error\n");
		return false;
	}

	serv_addr.sin_family = NAMESPACE;
	serv_addr.sin_port = htons(port);

	// TODO: PT SEG FAULT AT INET_PTON
	 //Convert IPv4 and IPv6 addresses from text to binary form
	//if (inet_pton(NAMESPACE, serverAddress, &serv_addr.sin_addr) <= 0)
	//{
	//	printf("\nClient: Invalid address/Address not supported \n");
	//	return false;
	//}

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nClient: Connection Failed\n");
		return false;
	}

	return true;
}

/* UTILITY FUNCTION DEFINITIONS (MAIN)*/

void welcome() {
	cout << "Welcome to your Meal Generator." << endl;
}

string getUsernamePassword() {
	string input, username, password;
	cout << "Please provide your username: ";
	cin >> username;
	cout << "Please provide your password: ";
	cin >> password;
	cout << endl;
	input += username + ';' + password + ";";
	return input;
}

string toLowerCase(string s) {
	string result = "";
	for (char i : s) {
		result += tolower(i);
	}
	return result;
}