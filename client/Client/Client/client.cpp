// Client side C/C++ program to demonstrate Socket programming
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
const char deli = ';';
// This is a C Program. No classes. You may turn this into an Object Oriented C++ program if you wish

void welcome();
void optionList();
void mealOptions();


void ParseTokens(char* buffer, std::vector<std::string>& a)
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
bool ConnectToServer(const char* serverAddress, int port, int& sock)
{
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, serverAddress, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return false;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return false;
    }

    return true;
}

int main(int argc, char const* argv[])
{
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* logoffRPC = "disconnect;";
    char buffer[1024] = { 0 };
    const char* serverAddress = argv[1];
    const int port = atoi(argv[2]);
    const int BUFF_SIZE = 1024;

    bool bConnect = ConnectToServer(serverAddress, port, sock);

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
            string option;
            string username;
            string password;

            cin >> option;
            if ((option == "1") || (option == "2")) {
                if (option == "1") {
                    msg = "connect;";
                }
                else msg = "signup;";
                cout << "Please provide your username: ";
                cin >> username;
                cout << "Please provide your password: ";
                cin >> password;
                msg += username + ';' + password;
                const char* curMsg = msg.c_str();
                strcpy(buffer, curMsg);
                valsend = send(sock, buffer, strlen(buffer), 0);
                valread = read(sock, buffer, BUFF_SIZE);
                string returnMsg = buffer;
                string error_code = returnMsg.substr(0, returnMsg.find(deli));
                if (error_code == "successful")
                {
                    valid = true;
                    cout << "You successfully logged in" << endl << endl;
                }
                else {
                    if (option == "failed") cout << "Invalid username/password. Try again!" << endl;
                    else cout << "Account already exists. Try login, or use another identity!" << endl;
                    optionList();
                }
            }
        }



        valid = false;
        while (!valid) {
            int meal;
            string info;

            mealOptions();
            cin >> meal;
            switch (meal) {
            case 1:
                msg = "mealRandom;";
                break;
            case 2:
                msg = "mealByTime;";
                cout << "What time of day are you looking at? ";
                cin >> info;
                msg += info;
                break;
            case 3:
                msg = "mealByCuisine;";
                cout << "What cuisine do you want? ";
                cin >> info;
                msg += info;
                break;
            default:
                valid = !valid;
                cout << "Invalid option. Let's start again!" << endl;
            }
            valid = !valid;
            const char* curMsg = msg.c_str();
            strcpy(buffer, curMsg);
            valsend = send(sock, buffer, strlen(buffer), 0);
            valread = read(sock, buffer, BUFF_SIZE);
            cout << "The meal for you would be " << buffer << endl << endl;
        }



    }
    else
    {
        printf("Exit without calling RPC");
    }


    // Do a Disconnect Message

    if (bConnect == true)
    {
        strcpy(buffer, logoffRPC);
        int nlen = strlen(buffer);
        buffer[nlen] = 0;   // Put the null terminator
        int valwrite = send(sock, buffer, strlen(buffer) + 1, 0);

        printf("DisConnect message sent with %d bytes\n", valwrite);

        int valread = read(sock, buffer, BUFF_SIZE);
        printf("Return response = %s with valread=%d\n", buffer, valread);
    }
    else
    {
        printf("Exit without calling RPC");
    }

    // Terminate connection
    close(sock);

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