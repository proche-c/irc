#include "../../includes/Command/Command.hpp"

Command::Command()
{
    std::cout << "Command object created" << std::endl;
    commands["INVITE"] = handleInvite;
    commands["JOIN"] = handleJoin;
    commands["KICK"] = handleKick;
    commands["MODE"] = handleMode;
    commands["NICK"] = handleNick;
    commands["PRIVMSG"] = handlePrivMsg;
    commands["TOPIC"] = handleTopic;
    commands["USER"] = handleUser;
}

Command::~Command()
{
    std::cout << "Command object destroyed" << std::endl;
}


void Command::executeCommand(const std::string& commandName, Client *client, Server *server)
{

    if (commands.find(commandName) != commands.end()) //if the command is in the map
    {
        commands[commandName](client, server); //execute the function associated with the command
    }
    else
    {
        std::cerr << commandName << ": Command not found" << std::endl;
    }
}
