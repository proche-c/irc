#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <set>
#include <map>
#include <sys/socket.h>
#include <sstream>


class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::set<Client*>	_members;
		std::set<Client*>	_invited;
		std::set<Client*>	_operators;
		bool				_inviteOnlyMode;
		bool				_clientLimitMode;
		bool				_keyMode;
		bool				_protectedTopicMode;
		size_t				_clientLimit;
		std::string			_key;
		static std::map<std::string, Channel*> _channels;
	
	public:
		Channel(const std::string& name);
		~Channel();

		std::string const &getName();
		bool	addMember(Client *client);
		void	removeMember(Client *client);
		void	broadcast(Client *sender, const std::string &message);


		static Channel *getChannel(std::string const &channelName);
		bool isMember(Client *client) const;

		std::set<Client*> &getMembers();

		void 	setKey(std::string const &key);
		std::string const &getKey() const;

		void 	setClienLimitMode(bool mode);
		bool	getClientLimitMode() const;

		void	setClientLimit(size_t limit);
		size_t	getClientLimit() const;

		bool	addOperator(Client *client);
		bool	isOperator(Client *client) const;
		bool	isOperator(const Client &client) const;
		bool	removeOperator(Client *client);

		void	setKeyMode(bool mode);
		bool	getKeyMode() const;

		void 	setTopic(Client &client, const std::string &newTopic);
		std::string const &getTopic() const;

		void	addInvited(Client *client);
		bool	isInvited(Client *client) const;
		std::set<Client *> getInvited(void) const;

		void	setInviteOnlyMode(bool mode);
		bool	getInviteOnlyMode() const;
		std::string stringMembers(void);


		void	setProtectedTopicMode(bool mode);
		bool	getProtectedTopicMode() const;


		void sendTopic(Client &client);


};


#endif