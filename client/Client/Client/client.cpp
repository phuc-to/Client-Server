/**
 @file  Client side = file for MealTime socket programming project
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
                valsend = send(sock, buffer, strlen(buffer), 0);                
                cout << "Connect message sent" << endl;

                valread = read(sock, buffer, BUFF_SIZE);                

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
                cout << "What day of time are you looking at? ";
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
            default:
                valid = !valid;
                cout << "Invalid option. Let's start again!" << endl;
            }
            if ((meal <= 4) && (meal > 0))
            {

                const char* curMsg = msg.c_str();
                strcpy(buffer, curMsg);
                valsend = send(sock, buffer, strlen(buffer), 0);
                if (meal == 4)
                    printf("Disconnect message sent with %d bytes\n", valsend);
                else
                    cout << "Your meal request has been sent" << endl;
                sleep(2);
                valread = read(sock, buffer, BUFF_SIZE);
                if (meal == 4)
                {
                    cout << "You are disconnected" << endl;
                    printf("Return response = %s with valread=%d\n", buffer, valread);
                }
                else
                    cout << "The meal for you would be " << buffer << endl << endl;


                cout << "Do you need another suggestion? 1 if so and 2 to logout: ";
                cin >> meal;
                if (meal == 2)
                {
                    valid = !valid;

                    strcpy(buffer, logoffRPC);
                    valsend = send(sock, buffer, strlen(buffer), 0);
                    printf("Disconnect message sent with %d bytes\n", valsend);
                    sleep(2);
                    valread = read(sock, buffer, BUFF_SIZE);
                    cout << "You are disconnected" << endl;
                }
            }

        }



    }
    else
    {
        printf("Exit without calling RPC");
    }

#if 0
    // Do a Disconnect Message

    if (bConnect == true)
    {
        strcpy(buffer, logoffRPC);
        int nlen = strlen(buffer);
        buffer[nlen] = 0;   // Put the null terminator
        int valwrite = send(sock, buffer, strlen(buffer) + 1, 0);

        printf("Disconnect message sent with %d bytes\n", valwrite);

        int valread = read(sock, buffer, BUFF_SIZE);
        printf("Return response = %s with valread=%d\n", buffer, valread);
    }
    else
    {
        printf("Exit without calling RPC");
    }
#endif
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
    cout << "3: Exit" << endl;
}

void mealOptions()
{
    cout << "What kind of meal are you looking for? 4 if you want to log out" << endl;
    cout << "1. A random one" << endl;
    cout << "2. A meal by time" << endl;
    cout << "3. A meal by cuisine" << endl;    
}