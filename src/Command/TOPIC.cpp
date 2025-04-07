#include "../../includes/Command/Command.hpp"
#include "../../includes/Network/Server.hpp"
#include "../../includes/Command/Messages.hpp"


//TO SEE THE TOPIC:
//-request to server: TOPIC <channel>
//-answer from server if the channel contains a topic: :<server> 332 <nickname> <channel> :<topic>
//-answer form server without topic: :<server> 331 <nickname> <channel> :No topic is set
//TO DEFINE THE TOPIC:
//-request: TOPIC <channel> :<new_topic>
//-answer: :<nickname>!<user>@<host> TOPIC <channel> :<new_topic>

void handleTopic(Client *client, Server * server) 
{    
     const std::vector<std::string> &args = client->getArgs();

    // Check the args
    if (args.size() < 1 || args[0].empty()) 
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS("TOPIC");
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }


    const std::string &channelName = args[0];

    // search the channel via the server
    Channel *channel = server->findChannel(channelName); 
    if (channel == NULL)
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(client->getNickName(), channelName);
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string newTopic = client->getLastArg();
    if (channel->getProtectedTopicMode() && !channel->isOperator(client))
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(client->getNickName(), channelName);
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    // Check if the client is a member of the channel
    if (!channel->isMember(client)) 
    {
        std::string errorMsg = ERR_NOTONCHANNEL(client->getNickName(), channelName);
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    //sendig topic
    if (args.size() == 1)
    {
        channel->sendTopic(*client);
        return;
    }

    // Ensure that there is no space between ':' and the topic content
    if (newTopic.empty()) 
    {
        std::string errorMsg = ERR_TOPICNEEDSCOLON(client->getNickName());
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        std::cout << "Error: Topic must start with a ':'\n";
        return;
    }

    // Remove the ':' from the start of the topic (it's part of the IRC syntax)
    // newTopic = newTopic.substr(1); // Remove the ':' character

    // Optionally, trim any leading or trailing spaces from the topic string
    newTopic.erase(0, newTopic.find_first_not_of(" \t")); // Trim the spaces at the beginning
    newTopic.erase(newTopic.find_last_not_of(" \t") + 1); // Trim the spaces at the end

    // Check if the topic is empty after trimming spacess
    if (newTopic.empty()) 
    {
        std::string errorMsg = ERR_EMPTYTOPIC(client->getNickName());
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        std::cout << "Error: Topic cannot be empty\n";
        return;
    }

    channel->setTopic(*client, newTopic);
    //std::cout << "Topic chandeg to " << newTopic << std::endl;

}
