#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "includes.hpp"
#include "parameters.hpp"

enum WritePrivacy {PUBLIC, PRIVATE};

class ConnectionManager
{
	private:	
		char socketBuffer[SOCKET_BUFFER_SIZE];
		io_service service;
		ip::tcp::socket socket;
		ip::tcp::endpoint socketEndpoint;
		string password;
		string ipCameraAddress;		
		string cameraOverlayConfig;
		
	public:
		ConnectionManager(void);
		void writeToSocket(string, WritePrivacy = PUBLIC);
		void readFromSocket(void);
		void clearSocketBuffer(void);
		void connectToSocket(void);
		void configureVideoStream(void);
		void startRecording(time_t startTime, time_t endTime);
		void stopRecording(void);
		void loginAMI(void);
		void logoutAMI(void);
		void beepAMI(void);
		string getTimeAndDate(const char *);
};

#endif
