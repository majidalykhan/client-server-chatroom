# client-server-chatroom

<b>Description</b><br>
In this project, we have studied the synchronization problems, filing, threading and inter-process communication. We have implemented a server program and a client program. Server was responsible for getting requests from client and processing them. We used a shared memory to implement the communication between client programs and the server program. We achieved to solve the problem by using semaphores. Both the client and server programs lock the critical sections before they use and unlocks after being used. We have implemented filing so messages of server and client are saved at runtime.

<b>Language</b><br>
Linux C Programming

<b>How to Run in Linux</b>
In terminal<br>
Write Command: gcc -lpthread -o s server.c (For Server),  gcc -lpthread -o c client.c (For Client)
