#include "../../includes/Command/Command.hpp"
#include "../../includes/Command/Messages.hpp"
#include "../../includes/Network/Client.hpp"
#include "../../includes/Network/Server.hpp"

//NICK <nickname>

static	bool validateNickname(const std::string &nickname)
{
	if (isdigit(nickname[0]))
		return false;
	for (size_t i = 0; i < nickname.size(); i++)
	{
		if (std::isspace(nickname[i]) || nickname[i] == '#' || nickname[i] == ':' || nickname[i] == '&')
			return false;
	}
	return true;
}

void handleNick(Client *client, Server * server) 
{ 
	if (client->getArgs().size() != 1 || client->getArgs()[0].empty()) // Nick receives only one arg 
	{
		std::string errorMsg = ERR_NONICKNAMEGIVEN(client->getNickName());
		send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}

	std::string newNick = client->getArgs()[0];

	std::map<int, Client>::iterator it;
    for (it = server->getClients().begin(); it != server->getClients().end(); ++it) 
	{
        if (it->second.getNickName() == newNick) 
		{
            std::string errorMsg = ERR_NICKNAMEINUSE(client->getNickName(), newNick);
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }

	if (!validateNickname(newNick)) // if the nickname is invalid
	{
		std::string errorMsg = ERR_ERRONEUSNICKNAME(client->getNickName(), client->getArgs()[0]);
		send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
    std::string successMsg = RPL_NICK(client->getNickName(), client->getArgs()[0]);
	client->setNickName(client->getArgs()[0]);// reset the NickName
    send(client->getSocket(), successMsg.c_str(), successMsg.size(), 0);
	client->setNickSet(true);
	if (client->isRegistered() == false && client->isUserSet() == true) // if the client is not registered and the user is set
	{
		client->setRegistered(true);
        std::string successMsg1 = RPL_WELCOME(client->getNickName(), client->getUsername(), client->getHostname());
        send(client->getSocket(), successMsg1.c_str(), successMsg1.size(), 0);
        std::string successMsg2 = RPL_YOURHOST(client->getNickName());
        send(client->getSocket(), successMsg2.c_str(), successMsg2.size(), 0);
        std::string successMsg3 = RPL_CREATED(client->getNickName());
        send(client->getSocket(), successMsg3.c_str(), successMsg3.size(), 0);
        std::string successMsg4 = RPL_MYINFO(client->getNickName());
        send(client->getSocket(), successMsg4.c_str(), successMsg4.size(), 0);
	}
	return;
}