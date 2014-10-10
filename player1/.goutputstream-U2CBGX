// Defines a Peer-to-Peer network class using c-style strings.
// Sockets are non-blocking IO to allow other functionalities to run.
// This is an implementation of an overly simplified Peer-to-Peer network.
// An important assumption is the connection will only be between two clients.
//
// You do not need to modify this file but welcome to do so.
//	C.P.
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <unistd.h>     // for close()
#include <fcntl.h>		// for fctnl()
#include <cstring>
#include <cstdlib>

#define MAXPENDING 1
#define RCVBUFSIZE 256

class NetP2P
{
private:
	int listeningSock;
	struct sockaddr_in ourAddr;

	int targetSock;
	struct sockaddr_in targetAddr;

	char buffer[RCVBUFSIZE + 1];
	int bufLen;

	unsigned short port;

	// Error handler that outputs useful error message.
	// Currently terminates program when error occurs. However, this may
	// be changed by removing the exit statement.
	void dieWithError(char *errorMessage)
	{
		perror(errorMessage);
		exit(1);
	}
public:
	// Constructor to set a listener for incoming connections
	NetP2P(unsigned short port)
		: port(port)
	{
		initialize();
		listenP2P();
	}

	// Constructor to set a connection to a listener with the target IP
	NetP2P(unsigned short port, char* targetIP)
		: port(port)
	{	
		initialize();
		connectP2P(targetIP);
	}

	// Returns: Number of bytes read.
	//				0	: if the connection is closed
	//				-1	: if no new bytes are available (Due to non-blocking socket)
	//				< -1: an error has occurred
	// 
	// Wrapper for the recv(...) function.
	//
	// Reads from the buffer for any incoming messages. The read
	// characters are copied to the str pointer for use. 
	// Assumes str is big enough to contain the contents of buffer.
	int readBuffer(char* str)
	{
		int result = recvBufferP2P();
		strcpy(str, buffer);	
		return result;
	}

	// Returns: Number of bytes written.
	//
	// Wrapper for the recv(...) function.
	// 
	// Reads from str pointer to copy into the buffer. The buffer is
	// then sent as an outgoing message. The size of str is limited
	// to RCVBUFSIZE.
	int writeBuffer(char* str)
	{
		bufLen = strlen(str);
		if (bufLen <= RCVBUFSIZE)
			strcpy(buffer, str);
		else
		{
			memcpy(buffer, str, RCVBUFSIZE);
			bufLen = RCVBUFSIZE;
			buffer[RCVBUFSIZE] = '\0';
		}
		int result = sendBufferP2P();
		return result;
	}

	// Terminates the connection to the other Peer.
	void shutdown()
	{
		closeP2P();
	}
private:
	// Initializes local address struct and clears buffer.
	void initialize()
	{
		// Construct local address structure
		memset(&ourAddr, 0, sizeof(ourAddr));
		ourAddr.sin_family = AF_INET;
		ourAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		ourAddr.sin_port = htons(port);

		// Zero out buffer
    	memset(buffer, '\0', RCVBUFSIZE + 1);
	}

	// Returns: Number of bytes read.
	//				0	: if the connection is closed
	//				-1	: if no new bytes are read (Due to non-blocking socket)
	//				< -1: an error has occurred
	int recvBufferP2P()
	{
		bufLen = recv(targetSock, buffer, RCVBUFSIZE, 0);
		if (bufLen < -1)
			dieWithError("recv() failed");
		else if (bufLen == -1)
			buffer[0] = '\0';
		else if (bufLen == 0)
		{
			closeP2P();
			buffer[0] = '\0';
		}
		else
			buffer[bufLen] = '\0';
		return bufLen;
	}

	// Returns: Number of bytes written.
	int sendBufferP2P()
	{
		int result = send(targetSock, buffer, bufLen, 0);
		if (result != bufLen)
			dieWithError("send() failed");

		return result;
	}
	
	// Initializes a TCP connection socket.
	// Initializes target address struct using port and target IP address.
	// The function then attempts to connect to the target.
	//
	// target IP = ###.###.###.###
	void connectP2P(char* targetIP)
	{
		printf("----- Creating Socket - "); fflush(stdout);
		// Create a reliable, stream socket using TCP
		if ((targetSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			dieWithError("socket() failed");
		printf("done\n");

		printf("----- Initializing Target Address - "); fflush(stdout);
		memset(&targetAddr, 0, sizeof(targetAddr));
		targetAddr.sin_family      = AF_INET;
		targetAddr.sin_addr.s_addr = inet_addr(targetIP);
		targetAddr.sin_port        = htons(port);	
		printf("done\n");

		printf("----- Connecting - "); fflush(stdout);
		// Establish the connection to the target
		if (connect(targetSock, (struct sockaddr*) &targetAddr, sizeof(targetAddr)) < 0)
			dieWithError("connect() failed");
		printf("done\n");

		// Set Non-Blocking Socket
		int flags = fcntl(targetSock, F_GETFL);
		if (flags == -1)
			dieWithError("first fntcl() failed");
		if (fcntl(targetSock, F_SETFL, flags | O_NONBLOCK) == -1)
			dieWithError("second fntcl() failed");
	}

	// Initializes a TCP listening socket.
	// The function then binds the listening socket.
	// The function is set to listen for connection requests using the socket.
	// The function then waits to accept a connection.
	//
	// target IP = ###.###.###.###
	void listenP2P()
	{
		printf("----- Creating Socket - "); fflush(stdout);
		// Create a reliable, stream socket using TCP
		if ((listeningSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			dieWithError("socket() failed");
		printf("done\n");

		printf("----- Binding - "); fflush(stdout);
		// Bind to the local address
		if (bind(listeningSock, (struct sockaddr*) &ourAddr, sizeof(ourAddr)) < 0)
			dieWithError("bind() failed");
		printf("done\n");

		printf("----- Listening - "); fflush(stdout);
		// Mark the socket so it will listen for incoming connections
		if (listen(listeningSock, MAXPENDING) < 0)
			dieWithError("listen() failed");

		acceptP2P();
		printf("done\n");
	}

	// The function waits and accepts an incoming connection using the listening socket.
	// The target address struct is initialized as well as the TCP connection socket.
	void acceptP2P()
	{
		unsigned int targetLen; // Length of client address data structure

		// Set the size of the in-out parameter
		targetLen = sizeof(targetAddr);

		// Wait for a client to connect
		if ((targetSock = accept(listeningSock, (struct sockaddr*) &targetAddr, &targetLen)) < 0)
			dieWithError("accept() failed");

		// Set Non-Blocking Socket
		int flags = fcntl(targetSock, F_GETFL);
		if (flags == -1)
			dieWithError("first fntcl() failed");
		if (fcntl(targetSock, F_SETFL, flags | O_NONBLOCK) == -1)
			dieWithError("second fntcl() failed");
	}

	// Closes both the TCP connection socket and listening socket.
	void closeP2P()
	{
		close(targetSock);
		close(listeningSock);
	}
};
