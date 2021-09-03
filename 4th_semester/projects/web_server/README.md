## Model Web Server

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

How to build a server:
```bash
make PROG=SERVER
```

How to build a client:
```bash
make PROG=CLIENT
```

How to launch a server:
```bash
./prog_server
```

How to launch a client:
```bash
./prog_client 
```

### Problem statement:
- A model web server implementation that supports the HTTP protocol subset described below.
- Implementation of Common Gateway Interface (CGI) support in the developed web server.
- Implementation of the interpreter of the model script description language for writing CGI scripts. Implementation language - C++.

<b>Implementation of the first stage of the task:</b>

The goal of the first stage of the task is to implement a static web server that supports a subset of the HTTP protocol. <br>
First program is a "pseudo-server", the purpose of which is to record real requests sent by web clients (for example, various web browsers). The server accepts the request, writes it to a log file, and immediately closes the connection. Saved queries can be used to debug the server. <br>

<b>Implementation of the second stage of the task:</b>

The goal of the second stage of the task is to add CGI support to the previously developed model web server. <br>
The simplest CGI implementation scheme implies that server runs a separate child process to process the CGI program. Output of the program is redirected to a temporary file for later inclusion of this file in response to the request. The temporary file is used because the program may terminate incorrectly (and then its results should be ignored without being passed to the client), also to find out the length of the response and form the `Content-length` header. After CGI program completes correctly, the server generates a response based on the output of the program.
When implementing CGI, the disadvantages of the first server implementation scheme are particularly evident (when the server does not start processing the next request until it completes the current one). Indeed, in this case, the server is idle waiting for the completion of a CGI program, which can run arbitrarily long.
