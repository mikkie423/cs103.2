// CS103.2_Group-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Mikkie, Sese, Delear - O for Owesome
//Registr

#include <iostream>
#include <string>
#include <vector> //Vector of Structs
#include <fstream> //Read from File

using namespace std;
//First stage of users entering user and password before donating

//Rego is function for registration and Log and Hospital and Donor is Login Info - Sese
void rego();
void hospital();
void donor();


//User struct by Michaela O'Brien
//Will store all the users personal and relevent medical information
//To be accessed 
struct User
{
	string username, password;
	int permission;

	User(string user = "username", string pw = "password", int p = 3)
	{
		username = user;
		password = pw;
		permission = p;
	}
};

/* Global Variables */
//remember forward dash
string filepathUsersLogin = "File/usersLogin.txt";

string message = "", line;
vector<User> users;// Vector of User struct

/* Function Initialisations */
bool checkLogin();

// CheckLogin function by Michaela O'Brien
// Returns true if the username and password entered match.
// Returns false after 3 wrong attempts.
// Stores users information in User vector/struct
bool checkLogin() {

	//Push back new user created with default constructor.
	users.push_back(User());

	/* Local Variables */
	int tries = 0;
	string input;
	cout << "\n======================================================================================================================";
	cout << "\n\n\t\t\t\t\t\t     LOGIN\n\n";
	cout << "======================================================================================================================\n";

	system("PAUSE");// Makes the program wait until any key is pressed
	system("CLS");// Clears Screen

	ifstream usersFile(filepathUsersLogin);// open file
	if (usersFile.is_open())
	{
		while (tries < 3) // 3 tries max
		{
			tries++;

			cout << message << endl;//potential message output

			cout << "Please enter username: ";
			cin >> input;

			/* read each line in the file one at a time */
			while (getline(usersFile, line))
			{
				string tempName = "";
				string tempPass = "";

				tempName.assign(line, 0, line.find(","));//assign tempName the username from the line

				/* if input matches the tempName */
				if (input == tempName)
				{
					size_t passwordLength = ((line.length()) - (tempName.length()) - 3);

					tempPass.assign(line, tempName.length() + 1, passwordLength); //assign tempPass the password from the line

					cout << "Please enter password: ";
					cin >> input;

					/* if input matches the tempPass */
					if (input == tempPass)
					{
						//preparing for getting permission level
						size_t pos = line.length() - 1;
						string tempPerm = line.substr(pos);

						users.push_back(User());//new User vector
						users[1].username = tempName;
						users[1].password = tempPass;
						users[1].permission = stoi(tempPerm);//string to int


						// TO BE DELETED - FOR TESTING PURPOSES ONLY
						cout << "username: " << users[1].username << endl;
						cout << "password: " << users[1].password << endl;
						cout << "permission level: " << users[1].permission << endl;


						usersFile.close(); // Close usersFile
						return true;
					}//endIf input == tempPass
					else
					{
						message = "\nPassword incorrect\n";
						break;
					}
				}//endIf input == tempName
				else
				{
					message = "\nUsername incorrect\n";
				}
			}//end While getLine
			usersFile.close();
		}//end While tries < 3
		/* Only gets to here if 3 failed attempts*/
		message = "\nSorry, thats 3 failed attempts to login.\nPlease try again later.\n";
		return false;
	} //endIf userFile is open
	else
	{
		message = "\nUnable to open usersLogin file\n";
		return false;
	}
}


int main()
{

	rego();

	ofstream user;
	ifstream password;





	if (checkLogin())
	{
		/* Successful Login */
		switch (users[1].permission)
		{
		case 0:
			cout << "\n\t\t\tWelcome to the Admin Homepage.\n\n";
			system("PAUSE");
			break;
		case 1:
			cout << "\n\t\t\tWelcome to the Hospital Homepage.\n\n";
			system("PAUSE");
			break;
		case 2:
			cout << "\n\t\t\tWelcome to the Donor Homepage.\n\n";
			system("PAUSE");
			break;
		default:
			cout << "\nUser doesn't have permission. Exiting program.\n";
			break;
		}
	}
	else
	{
		/* Failed Login */
		cout << message << endl;//potential message out
		cout << "\nExiting the program now...\n";
		exit(0);
	}

}
//rego code is the switch case for the main menu //Sese Code 
void rego()
{





	int selection = 0;


	cout << "\t\t \t  \t  \t...Welcome to Blood Bank NZ...";
	cout << "\n\n\n Register as Donor [1]";
	cout << "\n Login [2]";
	cout << "\n Press 3 to exit [3] ";
	cout << "\n";
	cin >> selection;
	switch (selection)
	{
	case 1:

		donor();

		break;

	case 2:

		//breaks out of case and program underneath works. 
		break;
	case 3:
		exit(0);
		break;

	default:
		cout << "Closing Program... ";
		system("Pause");
		system("CLS");
		exit(0);
		break;
	}



};
//File Handling for Donor registration part
void donor()
{
	ofstream file;
	string tempName = "";
	string tempPass = "";
	int number;
	file.open("File/usersLogin.txt", ios::app); // this will create file if it's not there, or open. //file mode parameter
	cout << "add user name: ";
	cin >> tempName;
	cout << endl;
	cout << "add password: ";
	cin >> tempPass;




	file << tempName << ",";
	file << tempPass << "," << 2;
	file << endl;
	file.close();
}
//registerhospital within admin perimeters
void hospital()
{
	ofstream file;
	string tempName = "";
	string tempPass = "";
	int number;
	file.open("File/usersLogin.txt", ios::app); // this will create file if it's not there, or open. //file mode parameter
	cout << "add user name: ";
	cin >> tempName;
	cout << endl;
	cout << "add password: ";
	cin >> tempPass;




	file << tempName << ",";
	file << tempPass << "," << 1;
	file << endl;
	file.close();
}
//sese code ^^

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

