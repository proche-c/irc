#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sys/socket.h>
#include "../Network/Client.hpp"
#include <cctype>
#include <algorithm>

#define SERVER_VERSION "ircserv-1.0"
#define NETWORK_NAME "MyIRCNetwork"
#define SERVER_NAME "irc.myserver.com"
#define CHANNEL_MODES "oitkl"
#define PARAM_MODES "o,k"
#define PREFIX_SERVER ":irc.myserver.com"

class Server;


void handleInvite(Client *client, Server * server);
void handleJoin(Client *client, Server * server);
void handleKick(Client *client, Server * server);
void handleMode(Client *client, Server * server);
void handleNick(Client *client, Server * server);
void handlePart(Client *client, Server * server);
void handlePrivMsg(Client *client, Server * server);
void handleTopic(Client *client, Server * server);
void handleUser(Client *client, Server * server);


class Command
{
    private:
        std::map<std::string, void(*)(Client *, Server *)> commands;

    public:
        Command();
        ~Command();

        void    executeCommand(const std::string& commandName, Client *client, Server *server);

};

#endif

