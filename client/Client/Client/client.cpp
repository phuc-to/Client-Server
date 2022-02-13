/**
 @file  Client side file for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/10/22
 @version 1.0
 @reference https://www.geeksforgeeks.org/socket-programming-cc/
 @reference https://www.gnu.org/software/libc/manual/html_node/Sockets.html

 GNU C functions
 setsockopt function: setsockopt(int socket, int level, int optname, int optval, socklen_t optlen)
 bind function: bind(int socket, struct sockaddr*address, socklen_t length)
 listen function: listen(int socket, int number of connection requests allowed in queue)
 read function: read(int socket, buffer, buffer size)
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <vector>
#include <iterator>
#include <iostream>


using namespace std;

// TCP SETTINGS
const int NAMESPACE = AF_INET;        // internet namespace IP4 
const int STYLE = SOCK_STREAM;        // comm style for TCP programming
const int PROTOCOL = 0;               // 0 for internet 
const int BUFF_SIZE = 1024;           // default buffer size

const char deli = ';';

// TODO: PT - Function prototype documenation, follow style in server.h for consistency. 
void welcome();
void optionList();
void mealOptions();
void parseTokens(char * buffer, std::vector<std::string>& a);
bool connectToServer(const char * serverAddress, int port, int & sock);

int main(int argc, char const* argv[])
{
    
    int cliSocket = 0;                            // init client socket
    // struct sockaddr_in serv_addr; // defined later in connectToServer

    const char* logoffRPC = "disconnect;";

    char buffer[BUFF_SIZE] = { 0 };              // init buffer
    const char* serverAddress = argv[1];         // holds server IP address
    //const int PORT = atoi(argv[2]);              // holds port

	// hard coded port for testing
	const int PORT = 10328;                      

	// Attempt to connect to server
    bool bConnect = connectToServer(serverAddress, PORT, cliSocket);

    if (bConnect == true)
    {
        
        int valsend;
        int valread;
        
        //Main program
        string msg;
        welcome();
        optionList();

        //Validate account

        bool valid = false;
        while (!valid) {
            int option;
            string username;
            string password;

            cout << "Type your option: ";
            cin >> option;
            if ((option == 1) || (option == 5)) {
                if (option == 1) {
                    msg = "connect;";
                }
                else msg = "signup;";
                cout << "Please provide your username: ";
                cin >> username;
                cout << "Please provide your password: ";
                cin >> password;
                cout << endl;
                msg += username + ';' + password;

                const char* curMsg = msg.c_str();
                strcpy(buffer, curMsg);
                valsend = send(cliSocket, buffer, strlen(buffer), 0);                
                cout << "Connect message sent" << endl;

                valread = read(cliSocket, buffer, BUFF_SIZE);                

                sleep(2);

                string returnMsg = buffer;
                string error_code = returnMsg.substr(0, returnMsg.find(deli));
                if (error_code == "successful")
                {
                    valid = true;
                    cout << "You successfully logged in" << endl;
                }
                else {
                    if (option == 1) cout << "Invalid username/password. Try again!" << endl;
                    else cout << "Account already exists. Try login, or use another identity!" << endl;
                    optionList();
                }
                cout << endl;
            }
            else if (option == 3)
            {
                valid = true;
                cout << "Goodbye!";
            }
            else
            {
                cout << "Invalid option. Let's try again!" << endl;
                optionList();

            }
                
        }

        valid = false;
        while (!valid) {
            int meal;
            string info;

            mealOptions();
            cout << "Type in your option: ";
            cin >> meal;
            switch (meal) {
            case 1:
                msg = "mealRandom;";
                break;
            case 2:
                msg = "mealByTime;";
                cout << "What time of day are you looking at? ";
                cin >> info;
                msg += info + ";";
                break;
            case 3:
                msg = "mealByCuisine;";
                cout << "What cuisine do you want? ";
                cin >> info;
                msg += info + ";";
                break;
            case 4:
                valid = !valid;
                msg = "disconnect;";
                break;
            default:
                valid = !valid;
                cout << "Invalid option. Let's start again!" << endl;
                meal = 5;
            }
            if ((meal <= 4) && (meal > 0))
            {
                cout << endl;
                const char* curMsg = msg.c_str();
                strcpy(buffer, curMsg);
                valsend = send(cliSocket, buffer, strlen(buffer), 0);
                if (meal == 4)
                    cout << "Disconnect message sent" << endl;
                else
                    cout << "Your meal request has been sent" << endl;
                sleep(2);
                valread = read(cliSocket, buffer, BUFF_SIZE);
                if (meal == 4)
                {
                    cout << "You are disconnected" << endl;
                }
                else
                {
                    cout << "The meal for you would be " << buffer << endl << endl;

                    cout << "Do you need another suggestion? 1 if so and 2 to logout: ";
                    cin >> meal;
                    if (meal == 2)
                    {
                        valid = !valid;

                        strcpy(buffer, logoffRPC);
                        valsend = send(cliSocket, buffer, strlen(buffer), 0);
                        printf("Disconnect message sent");
                        sleep(2);
                        valread = read(cliSocket, buffer, BUFF_SIZE);
                        cout << "You are disconnected" << endl;
                    }
                }
            }
        }
    }
    else
    {
        printf("Exit without calling RPC");
    }

    // Terminate connection
    close(cliSocket);

    return 0;
}

//TODO: PT Build out program description and instructions. 
void welcome()
{
    cout << "Welcome to your Meal Generator." << endl;
}

void optionList()
{
    cout << "Select an account option: " << endl;
    cout << "1: Login" << endl;
    cout << "2: Signup" << endl;
    cout << "3: Exit" << endl;
}

void mealOptions()
{
    cout << "What kind of meal are you looking for? 4 to Exit" << endl;
    cout << "1. A random meal" << endl;
    cout << "2. A meal by time" << endl;
    cout << "3. A meal by cuisine" << endl;    
}

void parseTokens(char* buffer, std::vector<std::string>& a)
{
	char* token;
	char* rest = (char*)buffer;

	while ((token = strtok_r(rest, ";", &rest)))
	{
		printf("%s\n", token);
		a.push_back(token);
	}

	return;
}

/*
	ConnectToServer will connect to the Server based on command line
*/
bool connectToServer(const char* serverAddress, int port, int& sock)
{
	struct sockaddr_in serv_addr;
	if ((sock = socket(NAMESPACE, STYLE, PROTOCOL)) < 0)
	{
		printf("\nClient: Socket creation error\n");
		return false;
	}

	serv_addr.sin_family = NAMESPACE;
	serv_addr.sin_port = htons(port);

	// TODO: PT SEG FAULT AT INET_PTON
	 //Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(NAMESPACE, serverAddress, &serv_addr.sin_addr) <= 0)
	{
		printf("\nClient: Invalid address/Address not supported \n");
		return false;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nClient: Connection Failed\n");
		return false;
	}

	return true;
}