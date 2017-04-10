
Run the following command to compile the server and client executables:
`gcc TCPEchoServer.c -o server && gcc TCPEchoClient.c -o client`

In order to test the program start the server with a port arg.  Ex. ./server 7
Start the client by giving it an ip followed by a port.  Ex. ./client 127.0.0.1 7


References/sources

The bulk of the code was given on canvas and looks like it is compiled from 	

"TCP/IP Sockets in C: Practical Guide for Programmers, Second Edition (ISBN: 978-0-12-374540-8) by Kenneth L. Calvert and Michael J. Donahoo 
Originally published as: The Pocket Guide to TCP/IP Sockets: C Version (ISBN: 1-55860-686-6)"

Snippets located here: http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/code/AcceptTCPConnection.c

I used github.com/awbradle as a reference for program architecting

Additionally I utilized various stackoverflow posts for help with c syntax, and a programming in C book for help with c syntax