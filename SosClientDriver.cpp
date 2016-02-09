/**
 * Tyler Hardin
 * Networking Project 3
 * 12/01/14
 *
 * Client driver implementation.
 */

#include "SosClient.hpp"

#include <iostream>
#include <memory>

class SosClientDriver{
	/** the client */
	std::shared_ptr<SosClient> s;
	
public:
	SosClientDriver() : s(nullptr) {}

	/**
	 * Prints a usage error and exits.
	 */
	void usage_error(){
		std::cerr << "SosClientDriver [hostname] [port] [name]" << std::endl;
		std::exit(1);
	}
	
	/**
	 * Main function.
	 *
	 * @param argc		the arg count
	 * @param argv		the arg vector
	 */
	int main(int argc, char** argv){
		// 0 is not a normal port arg, so print usage. (This is useful b/c 0 also happens to be 
		// the value returned when the arg is not a number.)
		if(argc != 4 || atoi(argv[2]) == 0)
			usage_error();
		
		int port = atoi(argv[2]);
		
		s.reset(new SosClient(argv[1], port, argv[3]));
		
		try{
			s->run();
		}
		catch(boost::system::system_error e){
			// Print the standard system error. It's cleaner that way.
			std::cerr << e.what() << std::endl;
			exit(2);
		}
		return 0;
	}
};

int main(int argc, char** argv){
	SosClientDriver s;
	return s.main(argc, argv);
}
