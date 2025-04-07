#include "../../includes/Command/Command.hpp"
#include "../../includes/Command/Messages.hpp"
#include "../../includes/Network/Server.hpp"

static void getChannelsAndUsers(Client *client, std::vector<std::string> &channels, std::vector<std::string> &users)
{
    std::stringstream streamChannels(client->getArgs()[0]);
    std::stringstream streamUsers(client->getArgs()[1]);
    std::string channel;
    std::string user;
    while (std::getline(streamChannels, channel, ','))
    {
        channels.push_back(channel);
    }
    while (std::getline(streamUsers, user, ','))
    {
        users.push_back(user);
    }
}

static void kickUsersFromChannel(Server *server, Client *client, std::string channelName,
std::vector<std::string> users)
{
    Channel *channel = server->findChannel(channelName);
    if (channel == NULL)
    {
        std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHCHANNEL(client->getNickName(), channelName);
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return ;
    }
    if (!channel->isOperator(client))
    {
        std::string errorMsg = PREFIX_SERVER + ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName());
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); it++)
    {
        Client *user = server->findClient(*it);
        if (user == NULL)
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHNICK_P(client->getNickName(), channelName, *it);
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
            continue;
        }
        if (!channel->isMember(user))
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOTONCHANNEL(user->getNickName(), channelName);
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
            continue;
        }
        channel->removeMember(user);
        std::string kickMsg = PREFIX_SERVER + RPL_KICK(client->getNickName(), channel->getName(), user->getNickName(), client->getLastArg());
        channel->broadcast(NULL, kickMsg);
    }
}

static void kickUserFromChannels(Server *server, Client *client, std::vector<std::string> channels, std::string userName)
{
    Client *user = server->findClient(userName);
    if (user == NULL)
    {
        std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHNICK(client->getNickName());
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
        return ;        
    }
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        Channel *channel = server->findChannel(*it);
        if (channel == NULL)
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHCHANNEL(client->getNickName(), channel->getName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }
        if (!channel->isOperator(client))
        {
            std::string errorMsg = PREFIX_SERVER + ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            continue;           
        }
        if (!channel->isMember(user))
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOTONCHANNEL(user->getNickName(), channel->getName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
            continue;
        }
        channel->removeMember(user);
        std::string kickMsg = PREFIX_SERVER + RPL_KICK(client->getNickName(), channel->getName(), user->getNickName(), client->getLastArg());
        channel->broadcast(NULL, kickMsg);
    }
}

static void kickUsersFromChannels(Server *server, Client *client, std::vector<std::string> channels, std::vector<std::string> users)
{
    std::vector<std::string>::iterator channelIt  = channels.begin();
    std::vector<std::string>::iterator userIt  = users.begin();

    while (channelIt != channels.end() || userIt != users.end())
    {
        Channel *channel = server->findChannel(*channelIt);
        Client  *user = server->findClient(*userIt);
        channelIt++;
        userIt++;
        if (channel == NULL || user == NULL)
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHNICK(client->getNickName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
            continue ;             
        }
        if(!channel->isOperator(client))
        {
            std::string errorMsg = PREFIX_SERVER + ERR_CHANOPRIVSNEEDED(client->getNickName(), channel->getName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            continue;              
        }
        if (!channel->isMember(user))
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOTONCHANNEL(user->getNickName(), channel->getName());
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), MSG_NOSIGNAL);
            continue;            
        }
        channel->removeMember(user);
        std::string kickMsg = PREFIX_SERVER + RPL_KICK(client->getNickName(), channel->getName(), user->getNickName(), client->getLastArg());
        channel->broadcast(NULL, kickMsg);
    }
}

void handleKick(Client *client, Server * server)  
{
    if (client->getArgs().size() < 3)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS_P(client->getNickName(), "KICK");
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }   
    std::vector<std::string> channels;
    std::vector<std::string> users;
    getChannelsAndUsers(client, channels, users);
    if (channels.size() == 1 && users.size() > 1)
        kickUsersFromChannel(server, client, channels[0], users);
    else if (channels.size() > 1 && users.size() == 1)
        kickUserFromChannels(server, client, channels, users[0]);
    else
        kickUsersFromChannels(server, client, channels, users);
}