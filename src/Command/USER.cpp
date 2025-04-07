#include "../../includes/Command/Command.hpp"
#include "../../includes/Command/Messages.hpp"

//USER <username> <hostname> <servername> :<realname>
///USER testuser localhost irc.example.com :Test User

void handleUser(Client *client, Server * server) 
{
    (void)server; 
    const std::vector<std::string>& args = client->getArgs();

    if (client->isRegistered())
    {
        std::string errorMsg = PREFIX_SERVER + ERR_ALREADYREGISTERED(client->getNickName());
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        std::cerr << "Client " << client->getSocket() << errorMsg << std::endl;
        return;
    }
    
    if (args.size() < 4)
    {
        std::string errorMsg = PREFIX_SERVER + ERR_NEEDMOREPARAMS("USER");
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        std::cerr << "Client " << client->getSocket() << ": Invalid USER command\n";
        return;
    }

    

    std::string username = args[0];
    std::string hostname = args[1];
    std::string servername = args[2];
    std::string realname = args[3];

    client->setUsername(username);
    client->setHostname(hostname);
    client->setServername(servername);
    client->setRealname(realname);
    client->setUserSet(true);

    if (!client->isRegistered() && client->isNickSet())
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
