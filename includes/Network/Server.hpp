#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdexcept> //std::runtime_error
#include <sys/types.h> //socket()
#include "../Command/Command.hpp"
#include "Channel.hpp"
#include <cstdlib> //std::atoi
#include <unistd.h> //close()
#include <csignal> //signal()
#include <netinet/in.h> //sockaddr_in and in_addr
#include <poll.h> //poll()
#include <fcntl.h> //fcntl()
#include <string> //std::string
#include <ctime> //time()
#include <arpa/inet.h> //inet_ntoa
#include <cstring> //memset

class Command;
class Channel;


class Server
{
    private:
		int 	_Port;
		std::map<int, Client> clients2;
		int 	SerSocketFd; //server socket file descriptor
	    std::vector <struct pollfd> FD; //file descriptor structure
	    std::string _Password;
	    std::string _ServerCreationTime;
		std::vector<Channel *> channels;

    public:
	    Server(int port, std::string password);
	    ~Server();
		void 					setServerCreationTime(void);
	    std::string 			getServerCreationTime(void) const;
	    void 					SerSocket();
	    static void 			SignalHandler(int signum);
	    void 					ServerInit(int port);
	    void 					CloseFDs();
	    static bool 			Signal;
	    void        			AcceptNewClient(void);
	    void        			RemoveClient(int fd);
	    void        			RecieveData(int fd);
	    void        			ProccessCommand(int fd, std::string line);

		std::vector<Channel *>	&getChannels(void);
		void					addChannel(Channel *channel);

		Channel 				*findChannel(const std::string &channelName);
		Client*					findClient(const std::string& nickname);
		std::map<int, Client> 	&getClients();
};

#endif
