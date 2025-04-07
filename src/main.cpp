#include "../includes/Network/Server.hpp"
#include "../includes/Command/Command.hpp"

int main (int argc, char **argv)
{
    if(argc != 3)
    {
        std::cerr << "Error: Invalid arguments." << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    if (port <= 0 || port > 65535) 
    { 
        std::cerr << "Error: Port must be between 1 and 65535." << std::endl;
        std::cerr << "Common Port for personnal server are between 1024 and 49151" << std::endl;
        return 1;
    }

    Command command; //create the command object
    Server server(port, password); //create the server object
    
    try
    {
        signal(SIGINT, Server::SignalHandler); //catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //catch the signal (ctrl + \)
		signal(SIGTSTP, Server::SignalHandler); //catch the signal (ctrl + z)
        server.ServerInit(port); //initialize the server
    }

	catch(const std::exception& e)
    {
        std::cout << "Entro aqui" << std::endl;
		server.CloseFDs(); //close the file descriptors
		std::cerr << e.what() << std::endl;
	}

    return 0;
}
