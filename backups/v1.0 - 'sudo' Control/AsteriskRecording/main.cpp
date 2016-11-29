//includes
#include <iostream>
#include <string>
#include <stdlib.h>

//namespaces
using namespace std;

//global variables
string confNumber;
string command;
string option;

//functions
void welcome();
void stop();
void start();

void start()
{
    system("clear\n"); //clear console
    cout << "~$ asterisk -rx \"confbridge record start " + confNumber + "\"" << endl << endl; //print command to terminal
    command = "sudo asterisk -rx \"confbridge record start " + confNumber + "\""; //create string of command
    system(command.c_str()); //run command in terminal

    cout << "\nType 'stop' to end the recording" << endl;

    while(true)
    {
        cin >> option;
        if (option == "stop")
        {stop();}
    }
}

void stop()
{
    system("clear\n"); //clear console
    cout << "~$ asterisk -rx 'confbridge record stop " + confNumber + "'" << endl << endl; //print command to terminal
    command = "sudo asterisk -rx \"confbridge record stop " + confNumber + "\""; //create string of command
    system(command.c_str()); //run command in terminal

    cout << "\nOptions: " << endl;
    cout << "- 'open' to open the location of the recording" << endl;
    cout << "- 'start' to start a new recording" << endl;
    cout << "- 'exit' to close the program" << endl;

    while(true)
    {
        cin >> option;
        if (option == "start")
        {start();} else
        if (option == "exit")
        {exit(0);} else
        if (option == "open")
        {system("cd /var/spool/asterisk");}  //unable to gain access
    }
}

void welcome()
{
    system("clear\n");
    cout << "NeXtRad Confbridge Recorder" << endl;
    cout << "---------------------------" << endl << endl;

    cout << "Please enter the conference extension: (default 100)" << endl;
    cin >> confNumber;
}

int main()
{
    welcome();
    start();

    return 0;
}
