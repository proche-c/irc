#include "../../includes/Network/Server.hpp"
#include "../../includes/Command/Command.hpp"

bool Server::Signal = false;

std::string toUpperCase(const std::string& str) 
{
    std::string result = str;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = std::toupper(result[i]);
    }
    return result;
}


Server::Server(int port, std::string password): _Port(port), _Password(password)
{
    this->_Port = port;
    std::cout << "Server object created on port " << port << " with password: " << password << std::endl;
}

Server::~Server()
{
    close(SerSocketFd);
    std::cout << "Server object destroyed" << std::endl;
}

void    Server::setServerCreationTime(void)
{
    std::time_t now = std::time(0);
    struct std::tm *timeinfo = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, 80, "%a %b %d %H:%M:%S %Y", timeinfo);
    _ServerCreationTime = std::string(buffer);
}

std::string  Server::getServerCreationTime(void) const
{
    return _ServerCreationTime;
}

void Server::SignalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    Signal = true; //to stop the main loop
}

void Server::ServerInit(int port)
{
	this->_Port = port;   //-*-*-*-*--* to check, maybe has to be fix 
	SerSocket(); //create the server socket
    setServerCreationTime();

	std::cout <<"Server " << SerSocketFd << " Connected" << std::endl;
	std::cout << "Waiting to accept a connection...\n";

    while(Server::Signal == false) //main loop
    {
        if ((poll(&FD[0], FD.size(), -1) == -1) && Server::Signal == false)
        {
            //&FD[0] is the address of the first element of the vector
            //-1 means wait indefinitely for an event
            throw std::runtime_error("poll() failed");
        }

        for(size_t i =0; i < FD.size(); i++) //iterate through the vector of pollfd structures
        {
            if (FD[i].revents & POLLIN) // if there is revents(returned events). Are flags writen in the FD after the call to poll()
                {
                if (FD[i].fd == SerSocketFd)  //.fd comes from poll.h
					{
                    //std::cout << "Accepting new client function" << std::endl;
                    AcceptNewClient(); //new connexion
                    }
                else
                    {
                    //std::cout << "Receiving new data function" << std::endl;
                    // ReceiveNewData(FD[i].fd); //data from a connected client
                    RecieveData(FD[i].fd);
                    }
                }   
        }
    }
    for (std::vector<Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
    {
        delete *it;
    }
    //std::cout << "Entro aqui en serverInit " << std::endl;
    CloseFDs();
}


void Server::SerSocket()
{
    struct sockaddr_in addressIPv4;
    addressIPv4.sin_family = AF_INET;
    addressIPv4.sin_port = htons(this->_Port); //htons converts the unsigned short integer hostshort from host byte order to network byte order
    addressIPv4.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY allows the server to accept a client connection on any interface
    
    SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //create a socket, AF_INET is the address family for IPv4, SOCK_STREAM is the type of socket, 0 is the protocol (default protocol related to the chosen type of socket)
    if (SerSocketFd == -1)
    {
        throw std::runtime_error("Failed to create a socket");
    }

    int reuseSocket = 1;
    if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuseSocket, sizeof(reuseSocket)) == -1)
    {
        //int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);  add options to a socket
        //SOL_SOCKET is the level of the protocol,in this cse it means socket options apply to the socket itself
        //SO_REUSEADDR allows other sockets to bind to an address even if it is already in use or in the TIME_WAIT state
        //optval is a pointer to the value to be set
        //optlen is the size of the value to be set in bytes
        throw std::runtime_error("Failed to set socket options");
    }

    if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
    {
        //int fcntl(int fd, int cmd, ... /* arg */ ); manipulate file descriptor
        //fd is the file descriptor on which to operate
        //F_SETFL set the file status flags to the value specified by arg
        //O_NONBLOCK sets the file status flags and enables nonblocking mode, usful in case of poll() function to avoid blocking while waiting for events
        throw std::runtime_error("Failed to set socket to non-blocking mode");
    }

    if (bind(SerSocketFd, (struct sockaddr *)&addressIPv4, sizeof(addressIPv4)) == -1)
    {
        //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); soket function is just a communication endpoint, bind assigns the address specified by addr to the socket
        //sockfd is the file descriptor of the socket
        //addr is a pointer to a sockaddr structure containing the address to bind to, 0 will automatically assign the address to an available port 
        //addrlen is the size of the address structure
        throw std::runtime_error("Failed to bind the socket");
    }

    if (listen(SerSocketFd, SOMAXCONN) == -1)
    {
        //int listen(int sockfd, int backlog); listen for connections on a socket and make it a passive socket
        //sockfd is the file descriptor of the socket
        //backlog is the maximum length of the queue of pending connections
        //SOMAXCONN is the maximum value for backlog
        throw std::runtime_error("Failed to listen on the socket");
    }

    struct pollfd NewPoll; //pollfd structure is used to describe a set of file descriptors to be monitored by the poll() function
                           //int poll(struct pollfd fds[], nfds_t nfds, int timeout)
                           //poll() function waits for one of a set of file descriptors to become ready to perform I/O
    NewPoll.fd = SerSocketFd; //FD to be monitored
    NewPoll.events = POLLIN; //POLLIN set the event to read operation
    NewPoll.revents = 0; // revents is an output parameter, filled by the kernel with the events that actually occurred
    FD.push_back(NewPoll); //add the new pollfd structure to the vector of pollfd structures
}

void Server::CloseFDs()
{
    close(SerSocketFd);
    std::cout << "Server socket closed" << std::endl;
}

//active and passsive sockets
//active socket is the client socket, it initiates the connection to the server
//passive socket is the server socket, it waits for the client to initiate the connection
//when a connection request is received by the server socket, it creates a new socket for the client

void Server::AcceptNewClient(void)
{
    Client              client; //Create a new client
    int                 clientFd; // File description for socket created when accepting conection
    struct sockaddr_in  clientAddress; //Struc with client address information
    struct pollfd       NewPoll; //Struct pollfd for new socket information
    socklen_t           length = sizeof(clientAddress);

    length = sizeof(clientAddress);
    //Accept() bloks until receive a conection and return a file descriptor representing
    //the connection with the client
    clientFd = accept(this->SerSocketFd, (sockaddr *)&(clientAddress), &length);
    if (clientFd == -1)
    {
        std::cout << "Fail accepting new client"  << std::endl;
        return ;
    }
    //Set the new socket to non-blocking
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "Fail te set connection socket to non-blocking" << std::endl;
        return ;
    }
    //Set values for NewPoll with connection socket information
    NewPoll.fd  = clientFd;
    NewPoll.events = POLLIN;
    NewPoll.revents = 0;

    //Set the values of client
    client.setSocket(clientFd);
    client.setAddress(inet_ntoa(clientAddress.sin_addr));

    //Add the new client to the clients vector
    clients2[clientFd] = client;

    //Add NewPoll to the FD vector
    FD.push_back(NewPoll);

    std::cout << "Client with fd: " << clientFd << " has been connected successfully" << std::endl;
}

void Server::RemoveClient(int fd)
{
    // Delete client from channels
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        if (this->channels[i]->isMember(&(clients2[fd])))
            this->channels[i]->removeMember(&(clients2[fd]));
        if (this->channels[i]->getMembers().size() == 0)
        {
            delete this->channels[i];
            this->channels.erase(this->channels.begin() + i);
        }
    }
    // Delete the pollfd associated to the client
    for (size_t i = 0; i < FD.size(); ++i) 
    {
        if (FD[i].fd == fd) {
            FD.erase(FD.begin() + i);
            break;
        }
    }

    // Delete the client from the map clients2
    std::map<int, Client>::iterator it = clients2.find(fd);
    if (it != clients2.end()) {
        clients2.erase(it);
        std::cout << "Client with fd: " << fd << " disconnected and deleted.\n";
    } else {
        std::cerr << "Error : Client with fd " << fd << " not found in the clients2 map.\n";
    }
}


void    Server::RecieveData(int fd)
{
    char buffer[1024];
    ssize_t readBytes;

    //Set buffer to 0
    memset(buffer, 0, sizeof(buffer));

    //Receive a message from a connected socket
    readBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    //Check if the client has desconnected
    if (readBytes <= 0)
    {
        std::cout << "Client with fd: " << fd << " is desconnected" << std::endl;
        RemoveClient(fd);
        close(fd);
    }
    else
    {
        buffer[readBytes] = '\0';
        // std::cout << "Client with fd: " << fd << " sent the message: "  << buffer << std::endl;
        clients2[fd].setBuffer(buffer);
        std::string toProccess = clients2[fd].getBuffer();
        // std::cout << "Client with fd: " << fd << " sent the message: "  << toProccess << std::endl;

        if (toProccess.size() > 1  && toProccess[toProccess.size() - 1] == '\n')
        {
            clients2[fd].cleanBuffer();
            // std::cout << "2   Client with fd: " << fd << " sent the message: "  << toProccess << std::endl;
            std::istringstream stream(toProccess);
            std::string line;
            while(std::getline(stream, line, '\n'))
            {
                // std::cout << "*******Command: " << commandName << std::endl;
                if (!line.empty() && line[line.length() - 1] == '\r')
                    line.erase(line.length() - 1);
                ProccessCommand(fd, line);
                // clients2[fd].setBuffer("");
            }
        }
    }
    return ;
}

void    Server::ProccessCommand(int fd, std::string line)
{
    Command command;
    std::string commandName;
    std::istringstream stream(line);


    stream >> commandName;
    std::vector<std::string> args;
    std::string arg;
    while(stream >> arg)
    {
        args.push_back(arg);
    }

    commandName = toUpperCase(commandName);
    // std::cout << "Processed command: " << commandName << std::endl;

    // std::cout << "Arguments: ";
    for (size_t i = 0; i < args.size(); i++)
    {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;;
    std::map<int, Client>::iterator it = clients2.find(fd);
    if (it == clients2.end()) {
        std::cerr << "Error : Client with fd " << fd << " not found.\n";
        return;
    }

    // client catch
    Client& client = it->second;
    client.setLastArg(line);
    client.setRawArg(line);
    client.setArgs(args);
    client.setServerCreationTime(_ServerCreationTime);

    // command validation
    if (commandName == "PASS" && !client.isAuthenticated())
    {
        std::string password = args[0];
        if (password == _Password)
        {
            client.setAuthenticated(true);
            std::cout << "Client with fd: " << fd << " authenticated successfully" << std::endl;
        }
        else
        {
            std::cerr << "Client with fd: " << fd << " failed to authenticate" << std::endl;
            return;
        }
    }
    if (client.isAuthenticated())
    {
        if (commandName != "INVITE" && commandName != "JOIN" && commandName != "KICK" &&
            commandName != "MODE" && commandName != "NICK" &&
            commandName != "PRIVMSG" && commandName != "TOPIC" && commandName != "USER") {
            // std::cerr << "Command not found" << std::endl;
            return;
        }

        // client inscritpoin verification
        if (!client.isRegistered() && commandName != "NICK" && commandName != "USER") {
            // std::cerr << "Client not registered" << std::endl;
            return;
        }

        // command execution
        command.executeCommand(commandName, &client, this);
    }
}

std::vector<Channel *>   &Server::getChannels(void)
{
    return channels;
}

void	Server::addChannel(Channel *channel)
{
    channels.push_back(channel);
}

Channel* Server::findChannel(const std::string &channelName) {
    for (size_t i = 0; i < this->channels.size(); ++i) 
    {
        Channel* channel = this->channels[i];
        if (channel->getName() == channelName) 
        {
            return channel; // channel found
        }
    }
    return NULL; // channel not found
}

Client* Server::findClient(const std::string& nickname) 
{
    std::map<int, Client>::iterator it;

    // walk in the map 
    for (it = clients2.begin(); it != clients2.end(); ++it) 
    {
        if (it->second.getNickName() == nickname) 
        {
            return &(it->second); // returns a pointer to the Client
        }
    }
    return NULL; // No client found
}

std::map<int, Client> &Server::getClients()
{
    return clients2;
}



