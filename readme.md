

#To Run: 
	Run the following command to compile the server and client executables:
	`gcc TCPEchoServer.c -o server && gcc TCPEchoClient.c -o client`

	In order to test the program start the server with a port arg.  Ex. ./server 7
	Start the client by giving it an ip followed by a port.  Ex. ./client 127.0.0.1 7


#References/sources

	The bulk of the code was given on canvas.  It looks like it was compiled from the following book : "TCP/IP Sockets in C: Practical Guide for Programmers, Second Edition (ISBN: 978-0-12-374540-8) by Kenneth L. Calvert and Michael J. Donahoo 
	Originally published as: The Pocket Guide to TCP/IP Sockets: C Version (ISBN: 1-55860-686-6)"

	Various other code snippets located here: http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/code/AcceptTCPConnection.c

	I used both the following two github repositories as references for program structure
		*https://github.com/mplewis/csci4061/blob/master/assignment5/TCPEchoClientServer/TCPEchoClient.c
		*github.com/awbradle 

	Additionally I utilized various stackoverflow posts for help with c syntax, and programming in C book https://www.amazon.com/Programming-3rd-Stephen-G-Kochan/dp/0672326663

	I also utilized Beejs guide to network programming for learning about some basic best practices in C http://beej.us/guide/bgnet/

#File Overview
	
	HandleTCPClient.h is a file that contains the server side code that handles a TCP client.  It does things like loads the news file to be read as well as handles searching through the news file

	TCPEchoServer.c is the server side code, handling the port bindings etc and processing of server side stuff

	TCPEchoClient.c is the client side code, giving a basic CLI interface for a user to query for the news

	DieWithError.h is a file that handles an abortive shutdown