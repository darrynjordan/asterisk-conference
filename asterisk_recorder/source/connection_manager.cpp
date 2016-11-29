#include "connection_manager.hpp"

ConnectionManager::ConnectionManager(void)  : socket(service)
{
	socketEndpoint.address(ip::address::from_string(TELNET_HOST)); 
	socketEndpoint.port(TELNET_PORT);			
}


void ConnectionManager::clearSocketBuffer(void)
{
	for (int i = 0; i < SOCKET_BUFFER_SIZE; i++)
    {
        socketBuffer[i] = ' '; 
    }
}


void ConnectionManager::connectToSocket(void)
{
    cout << "Attempting to establish connection to Asterisk server..." << endl;  
    try
    {
		socket.connect(socketEndpoint); //attempt to connect to the endpoint, if no exception is thrown the connection is successful
		printf("Connection Active!\n\n");
		//readFromSocket(); 
    }
    catch (boost::system::system_error const& e) //exception was thrown, connection failed
    {
		cout << "Exiting: could not " << e.what();		
		exit(0);
		//connectToSocket(); //restart connection
    }
}


void ConnectionManager::loginAMI(void)
{
    writeToSocket("Action: Login"); //Login
    writeToSocket("ActionID: 1");
    writeToSocket("Username: admin");
    writeToSocket("Secret: admin");
    writeToSocket("");
    //readFromSocket();
    writeToSocket("Action: Events"); //Turn Event Logging Off
    writeToSocket("ActionID: 2");
    writeToSocket("EventMask: off");
    writeToSocket("");
    //readFromSocket();
}


void ConnectionManager::beepAMI(void)
{
    writeToSocket("Action: Originate", PRIVATE); //start recoring
    writeToSocket("ActionID: 6", PRIVATE);
    writeToSocket("Channel: LOCAL/101", PRIVATE);
    writeToSocket("Application: Playback", PRIVATE);
    writeToSocket("Data: /home/darryn/tone", PRIVATE);
    writeToSocket("", PRIVATE);    
}


void ConnectionManager::writeToSocket(string message, WritePrivacy privacy)
{
	socket.write_some(buffer(message + (string)"\n")); //write to the socket
	
	if (privacy == PUBLIC)
	{
		if ( message != "")
		{	
			cout << " ~ " << message << endl;   //echo to console
		}
		else
		{
			cout << endl;
		}
	}            		
}


void ConnectionManager::readFromSocket(void)
{
	clearSocketBuffer();
    socket.read_some(buffer(socketBuffer)); //read terminal response to the buffer array
    cout << socketBuffer << endl;         	//echo to console
}


string ConnectionManager::getTimeAndDate(const char *format)
{
	time_t rawtime;                 
    struct tm * timeinfo;           
    char buffer[80];                                     
    time (&rawtime);                
    timeinfo = localtime(&rawtime); 
    strftime(buffer,80,format,timeinfo); 
    string dateTime(buffer);            
    return dateTime;
}


void ConnectionManager::startRecording(time_t startTime, time_t endTime)
{
    printf("Initilizing Recording...\n\n");

    writeToSocket("Action: ConfbridgeStartRecord");
    writeToSocket("ActionID: 3");
    writeToSocket("Conference: 100");
    writeToSocket("RecordFile: /var/spool/asterisk/NeXtRAD/" + getTimeAndDate("%d.%m.%Y-%I:%M:%S") + ".wav"); //the only location available for recording
	
	printf("\nWaiting for Recording to Begin...\n\n");
   
    time_t oldTime;
    time_t currentTime = time(NULL);	
    
    while (currentTime != startTime)
    {
		currentTime = time(NULL);
		
		if (currentTime == startTime)
		{
			cout << startTime - currentTime << "s to start\n";	
			break;
		}
		
		if (currentTime != oldTime)
		{
			cout << startTime - currentTime << "s to start\n";			
		}
		
		oldTime = currentTime;		
	}
	
	printf("\nRecording...\n");	
	writeToSocket("");	

    while (currentTime <= endTime)
    {
        currentTime = time(NULL);
        
        if (currentTime == endTime)
		{
			cout << "Time Elapsed: " << currentTime - startTime << "s" << endl;
			break;
		}
        
        if (currentTime != oldTime)
        {
            beepAMI();       
            cout << "Time Elapsed: " << currentTime - startTime << "s" << endl;
        }
        oldTime = currentTime;
    }    
}

void ConnectionManager::stopRecording(void)
{
    printf("Terminating Recording...\n\n");

    writeToSocket("Action: ConfbridgeStopRecord"); //start recoring
    writeToSocket("ActionID: 4");
    writeToSocket("Conference: 100");
    writeToSocket("");
    sleep(2);
    readFromSocket();
}


void ConnectionManager::logoutAMI()
{
    writeToSocket("Action: Logoff");
    writeToSocket("ActionID: 5");
    writeToSocket("");
    //readFromSocket();
    exit(0);
}






