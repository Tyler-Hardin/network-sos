/* Tyler Hardin
 * Networking Project 3
 * 12/01/14
 * Driver for the server. As with the server, I removed "Multiuser" b/c it's redundant and long.
 */
 
#include "SosServer.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
 /* Driver for the server. */
class SosServerDriver{
	/** the server */
	std::shared_ptr<SosServer> s;

public:
	/** Constructor. */
	SosServerDriver() : s(nullptr) {}

	/**
	* Prints a usage error and exits.
	*/
	void usage_error(){
		std::cerr << "SosServerDriver [port] [board size]" << std::endl;
		std::exit(1);
	}

	/**
	* Main function.
	* @param argc          the arg count
	* @param argv          the arg vector
	*/
	int main(int argc, char** argv){
		// 0 is not a normal port arg and is invalid for board size, so print usage. (This is 
		// useful b/c 0 also happens to be the value returned when the arg is not a number.)
		if(argc != 3 || atoi(argv[1]) == 0 || atoi(argv[2]) == 0)
			usage_error();
		int port = atoi(argv[1]);
		int size = atoi(argv[2]);
		s.reset(new SosServer(port, size));
		try{
			s->listen();
		}
		catch(boost::system::system_error e){
			// Print the standard system error. It's cleaner that way.
			std::cerr << e.what() << std::endl;
			exit(2);
		}
		return 0;
	}
};

/* Main function.
 * @param argc          the arg count
 * @param argv          the arg vector
 */
int main(int argc, char** argv){
	SosServerDriver driver;
	return driver.main(argc, argv);
}





