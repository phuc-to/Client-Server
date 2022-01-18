# Client-Server
Client Server group project for CPSC 5042 Operating Systems. 

The main objective is to create two pieces of software (Client and Server) that will exchange RPC's (Remote Procedure Calls).

Milestone 1 - Design and Create simple Client/Server Applications.

Requirements:

Design five RPC's between the Client and the Server using the template below as an example. Try to make them relevant. Two of the five have to be:
int Connect (char *userName , char *password)
int Disconnect()

You will design at least three more. You have thre freedom to do whatever you want, but it would be interesting to make some sort of a game, calculator, or something relevant.
 
Create a "client" and a "server" on CS1. The server will open a new socket in which the client will make a call. When the server gets a new connection request from client, it will create a new thread. That new thread will then be solely responsible for talking to the client that just called it. The server will then to continue listening for more connection requests
The data that is transferred between the Client and Server should be string. It is beyond the scope of this class to pass integers in their raw form.The client program should
establish a connection to the server, execute the connect call, sleep for a random amount of time between 1 and 10 seconds, and execute the Disconnect call
The three other RPC's can be coded and tested later
You will put the Client code in "client.cpp" and the Server code in "server.cpp"
You will have a make file that will build these components
You will put some debug messages into both client and server code that will end up going to stdout
