#include "source/includes.hpp"
#include "source/parameters.hpp"
#include "source/connection_manager.hpp"

ConnectionManager connectionManager;

int main(int argc, char **argv)
{    
    printf("NeXtRAD Asterisk Recorder\n");
    printf("-------------------------\n");
    
    if (argc == 3)
    {
		connectionManager.connectToSocket();   
	    connectionManager.loginAMI();	
	    //connectionManager.stopRecording();  //safety precaution
	    //connectionManager.startRecording(atoi(argv[1]), atoi(argv[2]));
		connectionManager.startRecording(time(NULL) + atoi(argv[1]), time(NULL) + atoi(argv[1]) + atoi(argv[2]));
		connectionManager.stopRecording();
		connectionManager.logoutAMI();
		return 0;
	}
	else
	{
		printf("Not enough launch parameters!\nMake sure start and end time have been supplied.");
		exit(0);
	}
}
