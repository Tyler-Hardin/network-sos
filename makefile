all : SosClientDriver SosServerDriver

.PHONY : clean
clean : 
	rm -f SosClientDriver SosServerDriver

SosClientDriver : SosClientDriver.cpp SosClient.cpp Session.cpp MessageSource.cpp
	clang++ -o $@ -std=c++11 $+ -lpthread -lboost_system
	
SosServerDriver : SosServer.cpp Session.cpp SosServerDriver.cpp Game.cpp SosBoard.cpp \
	MessageSource.cpp
	clang++ -o $@ -std=c++11 $+ -lpthread -lboost_system

