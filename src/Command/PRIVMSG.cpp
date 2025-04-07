#include "../../includes/Command/Command.hpp"
#include "../../includes/Command/Messages.hpp"
#include "../../includes/Network/Server.hpp"

//PRIVMSG <receiver> :<message>
//<receiver> could be a user or a channel

static std::string getMessage(const std::string& rawArg) 
{
    std::stringstream ss(rawArg);
    std::string word1, word2;

    ss >> word1 >> word2;
    std::string message;
    std::getline(ss, message);
    if (!message.empty() && message[0] == ' ') 
    {
        message = message.substr(1);
    }
    return message;
}

void handlePrivMsg(Client *client, Server * server)  
{
    const std::vector<std::string>& args = client->getArgs();
    std::string message = getMessage(client->getRawArg());    
    
    std::map<int, Client>& clients = server->getClients();  // Get the client2 map
    
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) 
    {
        std::cout << "Client Nickname: " << it->second.getNickName() << "\n";
    }

    //check args
    if (args.size() < 2) 
    {
        std::string errorMsg = PREFIX_SERVER + ERR_NORECIPIENT("PRIVMSG");
        send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        std::cerr << "Error: No recipient specified for PRIVMSG\n";
        return;
    }

    //cath args

    std::stringstream ss(args[0]);
    std::string receiver;
    std::vector<std::string> receivers;
    while (std::getline(ss, receiver, ',')) 
    {
        receivers.push_back(receiver);
    }

    for (std::vector<std::string>::iterator it = receivers.begin(); it != receivers.end(); ++it)
    {
        std::string receiver = *it;

        //check msg
        if (message.empty()) 
        {
            std::string errorMsg = PREFIX_SERVER + ERR_NOTEXTTOSEND;
            send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
            std::cerr << "Error: No text to send for PRIVMSG\n";
            return;
        }

        Channel *channel = NULL;

        // check channel
        if (receiver[0] == '#') //if it's a channel
        {
            //find channel
            channel = server->findChannel(receiver);
            if (!channel) 
            {
                std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHCHANNEL(client->getNickName(), receiver);
                send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                std::cerr << "Error: No such channel " << receiver << "\n";
                continue;
            }
        

            // and check permission
            if (!channel->isMember(client)) 
            {
                std::string errorMsg = PREFIX_SERVER + ERR_CANNOTSENDTOCHAN(receiver);
                send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                std::cerr << "Error: Cannot send to channel " << receiver << "\n";
                continue;
            }

            // Send MSG to all members
            std::string formattedMsg = ":" + client->getNickName() + " PRIVMSG " + receiver + " :" + message.c_str() + ENDL;
            channel->broadcast(client, formattedMsg);
        }

        else 
        { // or it's a user
            Client *targetClient = server->findClient(receiver);
            if (!targetClient) //if the user doesn't exist
            {
                std::string errorMsg = PREFIX_SERVER + ERR_NOSUCHNICK(receiver);
                send(client->getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
                std::cerr << "Error: No such nick " << receiver << "\n";
                continue;
            }

            // else 
            // {
            //     std::cout << "Found user " << receiver << " to send the message to.\n";
            // }

            // Send MSG to User
            std::string formattedMsg = ":" + client->getNickName() + " PRIVMSG " + receiver + " :" + message.c_str() + ENDL;
            send(targetClient->getSocket(), formattedMsg.c_str(), formattedMsg.size(), 0);
            // std::cout << "Message sent to user " << receiver << ": " << message << "\n";
        }
    }
}