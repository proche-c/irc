#include "../../includes/Network/Channel.hpp"

std::map<std::string, Channel*> Channel::_channels; // to check why


Channel::Channel(const std::string& name): _name(name), _topic(""),
_inviteOnlyMode(0), _clientLimitMode(0), _keyMode(0), _protectedTopicMode(1),
_clientLimit(0), _key("")

{
    std::cout << "Channel object created" << std::endl;
}

Channel::~Channel() 
{
    std::cout << "Channel object destroyed" << std::endl;
}

 std::string const &Channel::getName()
{
    return _name;
}

bool Channel::addMember(Client *client)
{
    if (_members.find(client) != _members.end())
    {
        return false; //already member
    }
    _members.insert(client);
    return true;
}

void Channel::removeMember(Client *client)
{
    _members.erase(client);
}

void Channel::broadcast(Client *sender, const std::string &message)
{
    for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it != sender)
        {
            send((*it)->getSocket(), message.c_str(), message.size(), 0);
        }
    }
}

Channel* Channel::getChannel(const std::string& channelName)
{
    static std::map<std::string, Channel*> channels;
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
    {
        return (it->second);  
    }
    return NULL;  // Channel not found
}

bool Channel::isMember(Client *client) const 
{
    for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) //set<Client*> is a collection of pointers to Client objects, it's a loop walking to every element Client* of _members
    {
        if ((*it)->getSocket() == client->getSocket()) //(*it) dereferences the iterator to accss to the Client* pointer inside of _members
                                                      //it means that if the socket from the client comming from arg is the same as a client in _members, it returns true      
        {
            return true;
        }
    }
    return false;
}

std::set<Client*> &Channel::getMembers()
{
    return _members;
}

void Channel::setKey(std::string const &key)
{
    _key = key;
}

std::string const &Channel::getKey() const
{
    return _key;
}

void Channel::setClienLimitMode(bool mode)
{
    _clientLimitMode = mode;
}

bool Channel::getClientLimitMode() const
{
    return _clientLimitMode;
}

void Channel::setClientLimit(size_t limit)
{
    _clientLimit = limit;
}

size_t Channel::getClientLimit() const
{
    return _clientLimit;
}

bool Channel::addOperator(Client *client)
{

    if (_operators.find(client) != _operators.end())
    {
        return false; //already member
    }
    _operators.insert(client);
    return true;
}

bool Channel::isOperator(Client *client) const
{
    for (std::set<Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if (*it == client)
        {
            return true;
        }
    }
    return false;
}

bool Channel::removeOperator(Client *client)
{

    if (_operators.find(client) == _operators.end())
    {
        return false; //not operator
    }
    _operators.erase(client);
    return true;
}

void Channel::setKeyMode(bool mode)
{
    _keyMode = mode;
}

bool Channel::getKeyMode() const
{
    return _keyMode;
}

void Channel::setTopic(Client &client, const std::string &newTopic)
{
    // Check if the channel is protected (+t) and if the client is an operator
    if (this->_protectedTopicMode && !this->isOperator(client))
    {
        std::string errorMsg = "482 " + client.getNickName() + " " + this->_name + " :You're not a channel operator\r\n";
        send(client.getSocket(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Define the topic
    this->_topic = newTopic;

    // Notify all users
    std::string notification = ":" + client.getNickName() + " TOPIC " + this->_name + " :" + this->_topic + "\r\n";
    for (std::set<Client *>::iterator it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        send((*it)->getSocket(), notification.c_str(), notification.size(), 0);
    }
    //std::cout << "Topic changed" << std::endl;
}

std::string const &Channel::getTopic() const
{
    return _topic;
}

void Channel::addInvited(Client *client)
{
    _invited.insert(client);
}

bool Channel::isInvited(Client *client) const
{
    for (std::set<Client*>::const_iterator it = _invited.begin(); it != _invited.end(); ++it)
    {
        if (*it == client)
        {
            return true;
        }
    }
    return false;
}

std::set<Client *> Channel::getInvited(void) const
{
    return _invited;
}

void Channel::setInviteOnlyMode(bool mode)
{
    _inviteOnlyMode = mode;
}

bool Channel::getInviteOnlyMode() const
{
    return _inviteOnlyMode;
}

std::string Channel::stringMembers(void)
{
    std::ostringstream names;
    for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        names << (*it)->getNickName() << " ";
    }
    return names.str();
}


void Channel::setProtectedTopicMode(bool mode)
{
    _protectedTopicMode = mode;
}

bool Channel::getProtectedTopicMode() const
{
    return _protectedTopicMode;
}

void Channel::sendTopic(Client &client) 
{
    std::string response;

    if (this->_topic.empty()) 
    { // if no topic is tefined
        response = ":" + client.getNickName() + " 331 " + client.getNickName() + " " + this->_name + " :No topic is set\r\n";
    } 
    else // if topic is defined
    { 
        response = ":" + client.getNickName() + " 332 " + client.getNickName() + " " + this->_name + " :" + this->_topic + "\r\n";
    }
    send(client.getSocket(), response.c_str(), response.size(), 0);
}

bool Channel::isOperator(const Client &client) const
{
    return this->_operators.find(const_cast<Client*>(&client)) != this->_operators.end();
}