/**
 * @file Client-side code for Meal Time socket programming project
 * @authors Phuc T, Narissa T, Kristen K
 * @date 2/6/22
 * @version 1.0
 * @reference https://www.geeksforgeeks.org/socket-programming-cc/
 *
 */

// Kristen says hello
#include <stdio.h>
#include <iostream>       // C++ IO
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 10328  // Per spec. 

using namespace std;

const string deli = ";";

int main(int argc, char const *argv[])
{
	// START CLIENT SOCKET INIT //////////////////////////////////////////////////////////////


	// client socket and buffer init 
	const int NAMESPACE = AF_INET;        // internet namespace IP4 
	const int STYLE = SOCK_STREAM;        // comm style for TCP programming
	const int PROTOCOL = 0;               // 0 for internet 
	const int BUFF_SIZE = 1024;           // default buffer size
	
	char buffer[BUFF_SIZE] = { 0 };       // init empty character buffer 
	int cli_socket = 0;                   // set client socket to true
	int valread;                          // holds communication from server

	// server socket address init 
	struct sockaddr_in address;           // server socket address struct
	address.sin_family = NAMESPACE;       // server socket's address format is INET for IP4
	address.sin_port = htons(PORT);       // assign server's port and convert to network bytes

	// create client socket, exit if error creating socket, else print success msg
	// socket function: socket(int namespace, int style, int protocol)
	if ((cli_socket = socket(NAMESPACE, STYLE, PROTOCOL)) < 0) {
		cerr << "\nClient: Socket creation error\n";
		return -1;
	}
	else
		cout << "\nClient: Socket creation successful\n"; 

	 // connect to server, exit if error connecting, else print success msg
	if (connect(cli_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cerr << "\nClient: Connection to server failed, please try again.\n";
		return -1;
	}
	else
		cout << "\nClient: Client server connection established\n"; 

	// test connection (the code up to this point allows Nars to work on the server)
	// send function: send(int socket, buffer, size of buffer, flags)
	const char *testMsg = "Testing from the client!";
	send(cli_socket, testMsg, strlen(testMsg), 0); 

	// read return msg from server and print
	// read function: read(int socket, buffer, buffer size)
	valread = read(cli_socket, buffer, BUFF_SIZE); 
	cout << "Server: " << buffer << "\n";

    //Main program
    char *msg;
    welcome();
    optionList();

    //Validate account

    bool valid = false;
    while (!valid) {
        string option;
        string username;
        string password;

        cin >> option;
        if ((option == "1") || (option == "2")) {
            if (option == "1"){
                msg = "connect;";
            }
            else msg = "signup;";
            cout << "Please provide your username: ";
            cin >> username;
            cout << "Please provide your password: ";
            cin >> password;
            msg += "username;password";
            send(cli_socket, msg, strlen(msg), 0);
            valread = read(cli_socket, buffer, BUFF_SIZE);
            string returnMsg = buffer;
            error_code = returnMsg.substr(0, returnMsg.find(deli));
            if (error_code = 0) valid = true;
            else {
                if (option == "1") cout << "Invalid username/password. Try again!" << endl;
                else cout << "Account already exists. Try login, or use another identity!" << endl;
                optionList();
            }
        }
    }



    valid = false;
    while (!valid) {
        string meal;

        mealOptions();
        cin >> meal;
        switch(meal) {
            case "1":
                msg = "mealRandom";
                break;
            case "2":
                msg = "mealByTime";
                break;
            case "3":
                msg = "mealByCuisine";
                break;
            default:
                valid = !valid;
                cout << "Invalid option. Let's start again!" << endl;
        }
        valid = !valid;
        send(cli_socket, msg, strlen(msg), 0);
        valread = read(cli_socket, buffer, BUFF_SIZE);
        cout << "The meal for you would be " << buffer << endl;
    }

	return 0;
}

void welcome()
{
    cout << "Welcome to your Meal Generator." << endl;
}

void optionList()
{
    cout << "These are the account options: " << endl;
    cout << "1: Login" << endl;
    cout << "2: Signup" << endl;
}

void mealOptions()
{
    cout << "What kind of meal are you looking for?" << endl;
    cout << "1. A random one" << endl;
    cout << "2. A meal by time" << endl;
    cout << "3. A meal by cuisine" << endl;
}