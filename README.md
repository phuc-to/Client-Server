# Meal Generator Client-Server Project
The Meal Generator Client Server group project for CPSC 5042 Operating Systems at Seattle University, by Narissa Tsuboi and Phuc To. 

The Meal Generator Client-Server is a socket programming project that returns meal ideas to the client for a user who is indecisive about what they want to eat and looking for ideas to inspire their next meal. This project utilizes stream socket programming and Transmission Control Protocol (TCP) for IP4, where the client and server communicate with continuous streams of characters (buffers), and the server translates each buffer and performs one of the Remote Procedure Calls (RPCs). The focus of this project is to build a client and server that cleanly communicate with each other using basic TCP programming and socket libraries while engaging the user in a fun, randomized meal idea program. The client server architecture supports multithreaded processes.

The program first establishes a listening server socket and waits for a connection request from the client. Through the client, the user can select from the five remote procedure calls (described in detail in the next section). After a successful connection is made, the user can call RPCs as many times as desired. To close the connection, the user will call disconnect. The program includes descriptive error messages for all steps of the client server connection and meal RPCs.

## Release 1.0.0
The server listens on the provided port on local host. 

## Remote Procedure Calls 
1. ProcessConnectRPC
2. ProcessSignupRPC
3. ProcessMealRPC
4. ProcessAddMealRPC
5. ProcessStatusRPC
6. ProcessDisconnectRPC

## Features
- Multi-threaded Server.
- User authentification.

## Build
To build, navigate to the `./Client-Server` folder. 

![Step1](images/step1.JPG)

`$ cmake -S . -B build`

`$ cmake --build build`

## Run the Program
Open two CLI windows. 

CLI Window 1 - Build executables, Run Server: 

`/Client-Server-main $ mkdir build`

`/Client-Server-main $ cmake -S . -B build`

`/Client-Server-main $ cmake -S . -B build`

`/Client-Server-main $ cd build/Server`

`/Client-Server-main/build/Server $ Server`

CLI Window 2 - Run Client: 

`/Client-Server-main/build/Client $ Client`



