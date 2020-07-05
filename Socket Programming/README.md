# Socket Programming 

### Computer Networks (Background)
- Consists of Machines Interconnected by communication channels
- Machines are Hosts and Routers
    - Hosts run applications
    - Routers forward information among communication channels
- Communication channels is a means of conveying sequences of bytes from one host to another (Ethernet, dial-up, satellite, etc.)

### Packets:
- Sequences of bytes that are constructed and interpreted by programs
- A packet contains
    - Control information:
        - Used by routers to figure out how to forward every packet.
        - e.g. packet destination
    - User data

### Protocol:
- An agreement about the packets exchanged by communicating programs and what they mean.
- A protocol tells
    - how packets are structured
        - where the distention information is located in the packet
        - how big it is
- Protocols are designed to solve specific problems
    - TCP/IP is such collection of solutions (protocol suite or family):
        - IP, TCP, UDP, DNS, ARP, HTTP, and many more
- How can we access the services provided by TCP/IP suite?
    - **Sockets APIs**.

### Addresses:
- Before one program can communicate with another program, it has to tell the network where to find the other program
- In TCP/IP, it takes two piece of information:
    - Internet Address, used by IP (e.g. Company’s main phone number)
    - Port Number, interpreted by TCP & UDP (extension number of an individual in the company)

### Demultiplexing
Convert host-to-host packet delivery service into
a process-to-process communication channel

### Client and server
- Server: passively waits for and responds to clients
- Client: initiates the communication
    - must know the address and the port of the server
- Methods:
    - Socket(): endpoint for communication
    - Bind(): assign a unique number
    - Listen(): wait for a caller
    - Connect(): dial a number
    - Accept(): receive a call
    - Send() and Receive(): Talk
    - Close(): Hang up

Server Side Methods | Clinet Side Methods
---|---
socket() | socket()
bind() |
listen() |
accept() | connect()
recv() | send()
send() | recv()
close() | close()

### Server 
1. Create a TCP socket using socket()
2. Assign a port number to the socket with bind() 
3. Tell the system to allow connections to be made to that port using listen()
4. Repeatedly do the following:
    - Call accept() to get a new socket for each client connection
    - Communicate with the client using send() and recv()
    - Close the client connection using close()
### Client
1. Create a TCP socket using socket()
2. Establish a connection to server using connect()
3. communicate using send() and recv()
4. Close connection using close()

### Why socket programming?
- To build network applications.
    - Firefox, google chrome, etc.
    - Apache Http server
### What is a socket?
- It is an abstraction through which an application may send and receive data
- File is an analogy: read (receive) and write (send)

### Types of sockets
- Stream sockets (TCP): reliable byte-stream service
- Datagram sockets (UDP): best effort datagram service

### What is a socket API?
- An interface between application and network
- Applications access the services provided by TCP and UDP through the sockets API.

### What is a socket?
- Socket: An interface between an application process and transport layer
    - The application process can send/receive messages to/from another application process (local or remote) via a socket
- In Unix jargon, a socket is a file descriptor – an integer associated with an open file
- Types of Sockets: Internet Sockets, unix sockets, X.25 sockets etc
    - Internet sockets characterized by IP Address (4 bytes), port number (2 bytes)

### Types of Internet Sockets
- Stream Sockets (SOCK_STREAM)
    - Connection oriented
    - Rely on TCP to provide reliable two-way connected communication
- Datagram Sockets (SOCK_DGRAM)
    - Rely on UDP
    - Connection is unreliable

### `socket()` - Get the file descriptor
```c
int socket(int domain, int type, int protocol);
```
- domain should be set to AF_INET
- type can be **SOCK_STREAM** or **SOCK_DGRAM**
- set protocol to 0 to have socket choose the correct
protocol based on type
- `socket()` returns a socket descriptor for use in later system calls or -1 on error
```c
int sockfd;
sockfd = socket (PF_INET, SOCK_STREAM, 0);
```

>In some documentation, you'll see mention of a mystical "PF_INET". This is a weird etherial beast that is rarely seen in nature, but I might as well clarify it a bit here. Once a long time ago, it was thought that maybe a address family (what the "AF" in "AF_INET" stands for) might support several protocols that were referenced by their protocol family (what the "PF" in "PF_INET" stands for).

>That didn't happen. Oh well. So the correct thing to do is to use AF_INET in your struct sockaddr_in and PF_INET in your call to socket(). But practically speaking, you can use AF_INET everywhere. And, since that's what W. Richard Stevens does in his book, that's what I'll do here.

### Socket Structures
- `struct sockaddr`: Holds socket address information for many types of sockets
```c
    struct sockaddr {
        unsigned short sa_family; //address family AF_xxx
        unsigned short sa_data[14]; //14 bytes of protocol addr
    }
```
- `struct sockaddr_in`: A parallel structure that makes it easy to reference elements of the socket address
```c
    struct sockaddr_in {
        short int sin_family;           // Set to AF_INET
        unsigned short int sin_port;    // PORT Number
        struct in_addr sin_addr;        // Internet Address
        unsigned char sin_zero[8];      // Set to all zeros
    }
```
- sin_port and sin_addr must be in **Network Byte Order.**

### Byte Ordering
- Two types of “ Byte ordering
    - Network Byte Order: High-order byte of the number is stored in memory at the lowest address
    - Host Byte Order: Low-order byte of the number is stored in memory at the lowest address
- Network stack (TCP/IP) expects Network Byte Order Conversions:

    Methods | What it means?
    |---|---
    htons() | Host to Network Short
    htonl() | Host to Network Long
    ntohs() | Network to Host Short
    ntohl() | Network to Host Long

### Dealing with IP Addresses
```c
struct in_addr {
    unsigned long s_addr; // that's a 32­bit long, or 4 bytes
};
```
```c
int inet_aton(const char *cp, struct in_addr *inp);
```
```c
    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    inet_aton(“10.0.0.5”,&(my_addr.sin_addr));
    memset(&(my_addr.sin_zero),'\0',8);
```
- `inet_aton()` gives non-zero on success; zero on failure
- To convert binary IP to string: `inet_ntoa()`
    - `printf(“ %s” ,inet_ntoa(my_addr.sin_addr));`

### Specifying Addresses
- Applications need to be able to specify Internet address and Port number. How?
- Use **Address Structure**
    1. `sockaddr` : Generic data type
    2. `in_addr` : Internet address
    3. `sockaddr_in` : Another view of Sockaddr
```c
struct sockaddr_in {
    unsigned short sin_family;  /* Internet protocol (AF_INET) */
    unsigned short sin_port;    /* Address port (16 bits) */
    struct in_addr sin_addr;    /* Internet address (32 bits) */
    char sin_zero[8];           /* Not used */
}
```
### bind() - what port am I on?
- Used to associate a socket with a port on the local machine
    - The port number is used by the kernel to match an incoming packet to a process
- `int bind(int sockfd, struct sockaddr *my_addr, int addrlen)`
    - `sockfd` is the socket descriptor returned by socket()
    - `my_addr` is pointer to struct sockaddr that contains information about your IP address and port.
    - `addrlen` is set to `sizeof(struct sockaddr)`
    - returns -1 on error
    - `my_addr.sin_port = 0;` Choose an unused port at random
    - `my_addr.sin_addr.s_addr = INADDR_ANY;` Use my IP adr

### Example
```c
// Socket - File Descriptor Creator
int sockfd;
sockfd = socket(PF_INET, SOCK_STREAM, 0);

struct sockaddr_in my_addr;
my_addr.sin_family = AF_INET;           // host byte order
my_addr.sin_port = htons(MYPORT);       // short, network byte order
my_addr.sin_addr.s_addr = inet_addr("172.28.44.57");
memset(&(my_addr.sin_zero), '\0', 8);   // zero the rest of the struct
bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
```

### connect() - Hello!
- Connects to a remote host
- `int connect(int sockfd, struct sockaddr *serv_addr, int addrlen)`
    - `sockfd` is the socket descriptor returned by socket()
    - `serv_addr` is pointer to struct sockaddr that contains information on destination IP address and port
    - `addrlen` is set to sizeof(struct sockaddr)
    - returns -1 on error
- No need to bind(), kernel will choose a port.

### Example
```c
#define DEST_IP "172.28.44.57"
#define DEST_PORT 5000
main() {
    int sockfd;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in dest_addr; // will hold the destination addr
    dest_addr.sin_family = AF_INET; // host byte order
    dest_addr.sin_port = htons(DEST_PORT); // network byte order
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    memset(&(dest_addr.sin_zero), '\0', 8); // zero the rest of the
    struct connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
}
```

### listen() - Call me please!
- Waits for incoming connections
- `int listen(int sockfd, int backlog);`
    - `sockfd` is the socket file descriptor returned by socket()
    - `backlog` is the number of connections allowed on the incoming queue
    - listen() returns -1 on error
    - Need to call bind() before you can listen()
        - socket()
        - bind()
        - listen()
        - accept()

### accept() - Thank you for calling !
- accept() gets the pending connection on the port you are listen()ing on
- `int accept(int sockfd, void *addr, int *addrlen);`
    - `sockfd` is the listening socket descriptor 
    - information about incoming connection is stored in `addr` which is a pointer to a local `struct sockaddr_in`
    - `addrlen` is set to `sizeof(struct sockaddr_in)`
    - accept returns a new socket file descriptor to use for this accepted connection and -1 on error

### send() and recv() - Let's talk!
- The two functions are for communicating over stream sockets or connected datagram sockets.
- `int send(int sockfd, const void *msg, int len, int flags);`
    - `sockfd` is the socket descriptor you want to send data to (returned by socket() or got from accept())
    - `msg` is a pointer to the data you want to send
    - `len` is the length of that data in bytes
    - set flags to 0 for now
    - sent() returns the number of bytes actually sent (may be less than the number you told it to send) or -1 on error.
- `int recv(int sockfd, void *buf, int len, int flags);`
    - `sockfd` is the socket descriptor to read from
    - `buf` is the buffer to read the information into
    - `len` is the maximum length of the buffer
    - set flags to 0 for now
    - `recv()` returns the number of bytes actually read into the buffer or -1 on error
    - If `recv()` returns 0, the remote side has closed connection on you










