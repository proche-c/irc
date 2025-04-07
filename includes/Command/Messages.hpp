#define once

#define ENDL "\r\n"

#define ERR_NEEDMOREPARAMS_P(nick, command) (" 461 " + nick + " " + command + " :Not enough parameters" + ENDL)
#define ERR_CHANOPRIVSNEEDED(nick, channel) (" 482 " + nick + " " +  channel + " :You're not channel operator" + ENDL)
#define ERR_NOSUCHNICK_P(nick, channel, nickname) (" 401 " + nick + " " +  channel + " " + nickname + " :No such nick" + ENDL)

//NICK
#define ERR_NONICKNAMEGIVEN(nickname) (nickname + " :No nickname given" + ENDL)
#define ERR_ERRONEUSNICKNAME(nickname, nick) (nickname + " " + nick + " :Erroneus nickname" + ENDL)
#define ERR_NICKNAMEINUSE(nickname, nick) (nickname + " " + nick + " :Nickname is already in use" + ENDL)
#define RPL_NICK(nickname, nick) (nickname + " changed nickname to " + nick + ENDL)

//JOIN
#define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)" + ENDL)

#define RPL_JOINMSG(nickname, username, hostname, channel) (":" + nickname + "!" + username + "@" + hostname + " JOIN " + channel + ENDL)
#define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)" + ENDL)
#define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)" + ENDL)

#define RPL_TOPIC(nick, channel, topic) (" 332 " + nick + " " + channel + " :" + topic + ENDL)
#define RPL_NOTOPIC(nick, channel) (" 331 " + nick + " " + channel + " :No topic is set" + ENDL)
#define RPL_NAMREPLY(channel, nick, names) (" 353 " + nick + " = " + channel + " :" + names + ENDL)


//MODE
#define ERR_UNKNOWNMODE(nick, channel, char) (" 472 " + nick + " " + " " +  channel + " " + char + " :is unknown mode char to me" + ENDL)
#define ERR_UNKNOWNMODE_PARAM(nick, channel, param) (" 472 " + nick + " " + channel + " " + param + " :Invalid mode parameter" + ENDL)
#define ERR_KEYSET(nick, channel) (" 467 " + nick + " " + channel + " :Channel key already set" + ENDL)
#define RPL_CHANNELMODEIS(nick, channel, modes) (" 324 " + nick + " " + channel + " " + modes + ENDL)

//INVITE
#define ERR_USERONCHANNEL(nick, nickname, channel) (" 443 " + nick + " " + nickname + " " + channel + " :is already on channel" + ENDL)
#define RPL_INVITING(nick, nickname, channel) (" 341 " + nick + " " + nickname + " " + channel + ENDL)
#define RPL_INVITED(nick, nickname, channel) (nick + " :" + nickname + " INVITE to " + channel + ENDL)

//TOPIC
#define ERR_NEEDMOREPARAMS(command) (std::string(" 461 ") + command + " :Not enough parameters" + ENDL)
#define ERR_NOSUCHCHANNEL(nickname, channel) (" 403 " + nickname + " " + channel + " :No such channel" + ENDL)
#define ERR_NOTONCHANNEL(nickname, channel) (" 442 " + nickname + " " + channel + " :Not on that channel" + ENDL)
#define ERR_TOPICNEEDSCOLON(nickname) (" 461 " + nickname + " TOPIC :Not enough parameters" + ENDL)
#define ERR_EMPTYTOPIC(nickname) (" 461 " + nickname + " TOPIC :Topic cannot be empty" + ENDL)

//USER
#define ERR_ALREADYREGISTERED(nickname) ( nickname + " :You may not reregister" + ENDL)
#define RPL_WELCOME(nickname, username, hostname) (nickname + " :Welcome to the " + NETWORK_NAME + " Network, " + nickname + "[!" + username + "@" + hostname + "]" + ENDL)
#define RPL_YOURHOST(nickname) (nickname + " :Your host is " + SERVER_NAME + ", running version " + SERVER_VERSION + ENDL)
#define RPL_CREATED(nickname) (nickname + " :This server was created " + client->getServerCreationTime() + ENDL)
#define RPL_MYINFO(nickname) (nickname + " " + SERVER_VERSION + " " + CHANNEL_MODES + " " + PARAM_MODES + ENDL)

//PRIVMSG
#define ERR_NORECIPIENT(command) (std::string(" 411 ") + command + " :No recipient given" + ENDL)
#define ERR_NOTEXTTOSEND (std::string(" 412 :No text to send") + ENDL)
#define ERR_CANNOTSENDTOCHAN(channel) (" 404 " + channel + " :Cannot send to channel" + ENDL)
#define ERR_NOSUCHNICK(receiver) (" 401 " + receiver + " :No such nick/channel" + ENDL)

//KICK
#define RPL_KICK(nick, channel, user, reason) (" KICK " + channel + " " + user + " :" + reason + ENDL)




