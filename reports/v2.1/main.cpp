//includes
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <ctime>

//namespaces
using namespace boost::asio;
using namespace std;

//function declarations
void login();
void start();
void stop();
void connect();
void logout();
void beep();
string getTimeDateStamp();
int getElapsedSeconds(int startTime);

//global variables
char buff[160];
char option;

time_t rawtime;
struct tm *timeinfo;

io_service service;
ip::tcp::socket sock(service);

//functions
void clearBuffer()
{
    for (int i = 0; i < 161; i++)
    {
        buff[i] = ' '; //clear all elements of the array
    }
}

void connect()
{
    system("clear\n"); //clear console
    cout << "Attempting to establish connection..." << endl << endl;
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 5038); //define the endpoint at the known server address & port

    try
        {
            sock.connect(ep); //attempt to connect to the endpoint, if no exception is thrown the connection is successful
            login(); //login to AMI as admin

            system("clear\n");
            cout << "Start Recoding? [y/n]" << endl << endl;
            while(true) //wait for response
            {
                cin >> option;
                if (option == 'y')
                    {start();}
                else if (option == 'n')
                    {logout();} //close program
                else
                    {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
            }

        }
    catch (boost::system::system_error const& e) //exception was thrown, connection failed
        {
            cout << "Warning: could not " << e.what() << endl << endl;

            cout << "Retry Connection? [y/n]" << endl << endl;
            while(true)
            {
                cin >> option;
                if (option == 'y')
                    {connect();} //restart connection
                else if (option == 'n')
                    {logout();}
                else
                    {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
            }
        }
}

void write(string text)
{
    sock.write_some(buffer(text)); //write to the terminal
    cout << text;                  //echo to console
}

void read()
{
    clearBuffer();
    sock.read_some(buffer(buff)); //read terminal response to the buffer array
    cout << buff << endl;         //echo to console
}

void start()
{
    int oldTime, newTime = 0;
    time_t rawStartTime;

    system("clear\n");            //clear console
    cout << "Initilizing Recording" << endl << endl;

    write("Action: ConfbridgeStartRecord\n"); //start recoring
    write("ActionID: 3\n");
    write("Conference: 100\n");
    write("RecordFile: /var/spool/asterisk/" + getTimeDateStamp() + ".wav\n"); //the only location available for recording
    write("\n");

    int startTime = time(&(rawStartTime));

    while(newTime < 31)
    {
        newTime = getElapsedSeconds(startTime);
        if (newTime > oldTime)
        {
            beep();
            system("clear\n");            //clear console
            cout << "Time Elapsed: " << newTime << "s" << endl;
            oldTime = newTime;
        }
    }
    stop();
}

void stop()
{
    system("clear\n");              //clear console
    cout << "Terminating Recording" << endl << endl;

    write("Action: ConfbridgeStopRecord\n"); //start recoring
    write("ActionID: 4\n");
    write("Conference: 100\n");
    write("\n");
    sleep(1);
    read();

    cout << endl << "--------------------------------" << endl << endl;

    cout << "Begin New Recording? [y/n]" << endl << endl;
    while(true)
    {
        cin >> option;
        if (option == 'y')
            {start();}
        else if (option == 'n')
            {logout();}
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;}
    }
}

void login()
{
    write("Action: login\n"); //Login
    write("ActionID: 1\n");
    write("Username: admin\n");
    write("Secret: admin\n");
    write("\n");
    sleep(1);
    read();

    sleep(1);
    system("clear\n");

    write("Action: Events\n"); //Turn Event Logging Off
    write("ActionID: 2\n");
    write("EventMask: off\n");
    write("\n");
    sleep(1);
    read();
    sleep(1);
}

void logout()
{
    system("clear\n");

    write("Action: Logoff\n");
    write("ActionID: 5\n");
    write("\n");
    sleep(1);
    read();

    exit(0);
}

string getTimeDateStamp()
{
    char stringBuffer[80];
                                    //get date & time as a a string
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(stringBuffer,80,"%d.%m.%Y-%I:%M:%S",timeinfo); //set date & time format

    string dateTime(stringBuffer);        //define date & time string
    return(dateTime);
}

int getElapsedSeconds(int startTime)
{
    time_t currentTime;
    int elapsed = time(&currentTime) - startTime;
    return elapsed;
}

void beep()
{
    write("Action: Originate\n"); //start recoring
    write("ActionID: 6\n");
    write("Channel: LOCAL/101\n");
    write("Application: Playback\n");
    write("Data: beep\n");
    write("\n");
}

void welcome()
{
    cout << "NeXtRAD Asterisk VoIP Controller" << endl;
    cout << "--------------------------------" << endl << endl;
    cout << "Steps:" << endl;
    cout << "1) Establish Connection to Server" << endl;
    cout << "2) Start Recording" << endl;
    cout << "3) Stop Recording" << endl << endl;
    cout << "--------------------------------" << endl << endl;
}

int main()
{
    welcome(); //display welcome screen

    cout << "Establish Connection? [y/n]" << endl << endl;
    while(true) //wait for response
    {
        cin >> option;
        if (option == 'y')
            {connect();} //chose yes - attempt connection
        else if (option == 'n')
            {break;} //chose no - close program
        else
            {cout << "Invalid Response. Please use 'y' or 'n'" << endl;} //request new response
    }
    return 0;
}
