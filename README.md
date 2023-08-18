A simple file transfer program written in the C language.

Example usage:

Client.c accepts two parameters, a port number, the server's IP address,
and the file you wish to transfer to the server.

Compiling and using Client.c:
    gcc -o Client.exe Client.c
    ./Client.exe "192.168.1.230" 50300 test.txt

    Port number:      50300
    File to transfer: test.txt

Server.c accepts three parameters: 
a port number, and the file name that the transferred file will be stored as.
If there is no file created already, the Server.c program will create that file for you.

Compiling and using Sever.c:
    gcc -o Server.exe Server.c
    ./Server.exe 50300 transferred_file.txt

    Port number:       50300
    File to save to:   transferred_file.txt

NOTE: For this to work, both programs need to have the same port number specified.