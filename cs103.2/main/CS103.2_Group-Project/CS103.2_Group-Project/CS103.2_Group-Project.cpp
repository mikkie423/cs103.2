// CS103.2_Group-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Michaela, Sese, Delear - O for Owesome
// Blood Bank Management System

#include <iostream>
#include <string>
#include <vector> //Vector of Structs
#include <fstream> //Read from File
#include <cstdio>

using namespace std;

//PersonalInfo struct by Michaela O'Brien
//Will store all the users personal and relevant medical information
//To be accessed via the User struct.
struct PersonalInfo
{
	string fName, lName, address, phone, location;
	char email[30], bloodGroup[10];
	int date;

};

//User struct by Michaela O'Brien
//Will store the users login and permission information
//And points to the users personal information.
struct User
{
	string username, password;
	int permission;
	PersonalInfo uData;
	User(string user = "default", string pw = "defaultpassword", int p = 3)
	{
		username = user;
		password = pw;
		permission = p;
	}
};	User loggedinUser; //LoggedinUser struct


/* Global Variables */
string filepathUsersLogin = "Files/usersLogin.txt";
string message = "", line;
vector<User> users;// Vector of User struct

int d, e;  //global variables to identify if user already has appointment


/* Function Initialisations */
bool checkLogin(int tries);
void loginRedirection();
void adminMenu();
int getAllLogins();
void viewAllLogins();
void editUser(string input);
bool checkIfUsernameExists(string tempName);

//Rego is function for registration and Log and Hospital and Donor is Login Info - Sese
void rego();
void hospital();
void donor();
void welcome();  //Donor functions by Delear Goolmorade
void eligibilityTest();
void bookAppt();
void viewAppt();
void editAppt();
void cancelAppt();
void deletefile();


int main()
{
	rego();
}


//rego code is the switch case for the main menu //Sese Code 
void rego()
{
	system("CLS");
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
	case 2:
		loginRedirection();
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

// CheckIfUsernameExists() by Michaela
// Checks if the username passed already exists in the usersLogin file.
// Returns true if the username already exists,
// Returns false if the username doesn't exist.
bool checkIfUsernameExists(string tempName)
{
	ifstream usersFile(filepathUsersLogin);// open file
	if (usersFile.is_open())
	{
		/* read each line in the file one at a time */
		while (getline(usersFile, line))
		{
			string checkUsername = "";
			checkUsername.assign(line, 0, line.find(","));//assign checkUsername the username from the line

			/* if checkUsername matches the tempName */
			if (tempName == checkUsername)
			{
				message = "\nUsername exists\n";
				usersFile.close();
				return true;
			}//endIf checkUsername == tempName
		}//end While getLine
		message = "\nUsername doesnt exist\n";
		usersFile.close();
		return false;
	} //endIf userFile is open
	else
	{
		message = "\nUnable to open usersLogin file\n";
		return false;
	}
}


//File Handling for Donor registration part //Sese Code
void donor()
{
	ofstream file;
	string tempName = "";
	string tempPass = "";
	file.open(filepathUsersLogin, ios::app); // this will create file if it's not there, or open. //file mode parameter 
	if (file.is_open())
	{
		do {
			cout << message << endl;
			cout << "add user name: ";
			cin >> tempName;
		} while (checkIfUsernameExists(tempName));
		message = "";
		cout << endl;
		cout << "add password: ";
		cin >> tempPass;

		file << tempName << ",";
		file << tempPass << "," << 2;
		file << endl;
		file.close();
	}
	else
	{
		cout << "File not found!" << endl;
	}

}

//registerhospital within admin perimeters // Sese Code
void hospital()
{
	ofstream file;
	string tempName = "";
	string tempPass = "";
	file.open(filepathUsersLogin, ios::app); // this will create file if it's not there, or open. //file mode parameter
	if (file.is_open())
	{
		do {
			cout << message << endl;
			cout << "add user name: ";
			cin >> tempName;
		} while (checkIfUsernameExists(tempName));
		message = "";
		cout << endl;
		cout << "add password: ";
		cin >> tempPass;




		file << tempName << ",";
		file << tempPass << "," << 1;
		file << endl;
		file.close();
	}
	else
	{
		cout << "File not found!" << endl;
	}

}


// CheckLogin function by Michaela
// Returns true if the username and password entered match.
// Returns false after 3 wrong attempts.
// Stores users information in User vector/struct
bool checkLogin(int tries) {
	system("CLS");// Clears Screen

	/* Local Variables */
	string username, password;
	string tempName = "";
	string tempPass = "";
	string tempPerm = "";

	cout << "\t\tLogin\n\n";

	ifstream usersFile(filepathUsersLogin);// open file
	if (usersFile.is_open())
	{
		if (tries > 2)
		{
			message = "\nSorry, thats 3 failed attempts to login.\nPlease try again later.\n";
			return false;
		}
		else
		{

			cout << message << endl;//potential message output
			message = "";

			cout << "Please enter username: ";
			cin >> username;

			/* read each line in the file one at a time */
			while (getline(usersFile, line))
			{
				if (!usersFile.eof())
				{
					tempName.assign(line, 0, line.find(","));//assign tempName the username from the line

					/* if username matches the tempName */
					while (username == tempName && tries < 3)
					{
						size_t passwordLength = ((line.length()) - (tempName.length()) - 3);

						tempPass.assign(line, tempName.length() + 1, passwordLength); //assign tempPass the password from the line

						cout << message << endl;//potential message output
						message = "";

						cout << "Please enter password: ";
						cin >> password;
						/* if password matches the tempPass */
						if (password == tempPass)
						{
							//preparing for getting permission level
							size_t pos = line.length() - 1;
							tempPerm = line.substr(pos);



							loggedinUser.username = tempName;
							loggedinUser.password = tempPass;
							loggedinUser.permission = stoi(tempPerm);//string to int

							usersFile.close(); // Close usersFile
							return true;
						}
						else
						{
							tries++;
							message = "\nPassword incorrect\n";
						}//endIf password == tempPass
					}//endIf username == tempName
				}
			}//end While getLine
			usersFile.close();


			if (username != tempName && tries < 3)
			{
				tries++;
				message = "\nUsername incorrect\n";
				checkLogin(tries);
			}
		}
	} //endIf userFile is open
	else
	{
		message = "\nUnable to open usersLogin file\n";
		return false;
	}
}


// LoginRedirection function by Michaela
// Depending on the logged in users permission level: 0 - Admin, 1 - Hospital, 2 - Donor;
// Redirects the user to pages they are allowed to access,
// Or boots user out of system if they dont have permission
void loginRedirection()
{
	int tries = 0;

	if (checkLogin(tries))
	{
		/* Successful Login */
		switch (loggedinUser.permission)
		{
		case 0:
			adminMenu();
			break;
		case 1:
			system("CLS");
			cout << "\n\t\t\tWelcome to the Hospital Homepage.\n\n";
			system("PAUSE");
			break;
		case 2:
			system("CLS");
			cout << "\n\t\t\tWelcome to the Donor Homepage.\n\n";
			system("PAUSE");
			welcome();

			break;
		default: /*	 Failed Login  */
			cout << message << endl;//potential message out
			message = ""; //clears old message
			system("PAUSE");
			rego();
		}
	}
}


// AdminMenu function by Michaela
// Alerts Admin if there is any requests to review
// Allows Admin to add new users(donor, hospital), and appointments
// Allows Admin to view, adjust stock and stock requests
void adminMenu()
{
	if (loggedinUser.permission != 0) // if loggedinUser isnt an admin, logs the user out and boots to main screen
	{
		loggedinUser = {};
		rego();
	}


	int numOfRequests = 1; // If there is any requests //TODO actually sort this 

	char input = 'f';


	while (input != 'a' || input != 'b' || input != 'c' || input != 'd')
	{
		system("CLS");
		cout << "\n\t\t\tWelcome to the Admin Homepage.\n\n";
		if (numOfRequests > 0)
		{
			message = "\n\t\t*** There is a new request for you to look at ***\n";
		}
		else
		{
			message = "";
		}
		cout << message << endl;
		message = ""; //clears old message

		cout << "[a] View Requests" << endl;
		cout << "[b] View Appointments" << endl;
		cout << "[c] View All Users" << endl;
		cout << "[d] Log Out" << endl;

		cin >> input;

		switch (input)
		{
		case 'a':
			system("CLS");
			cout << "View Requests" << endl; // View, edit, approve, cancel requests
			system("PAUSE");
			break;
		case 'b':
			system("CLS");
			cout << "View Appointments" << endl; // View, add, edit, cancel appointments
			system("PAUSE");
			break;
		case 'c':
			viewAllLogins();
			break;
		case 'd':
			loggedinUser = {}; //reset the structure as user logs out
			rego();
			break;
		default:
			message = "That was not an option, please try again...\n";
			break;
		}
	}
}

// GetAllLogins function by Michaela
// Gets all the logins from the usersFile
// Sets them all to be a users[] vector
// Returns number of logins
int getAllLogins()
{
	//Push back new user created with default constructor.
	users.push_back(User());

	int i = 0;
	ifstream usersFile(filepathUsersLogin);// read file
	if (usersFile.is_open())
	{
		/* read each line in the file one at a time */
		while (getline(usersFile, line))
		{
			if (!usersFile.eof())
			{
				string tempName = "";
				string tempPass = "";

				tempName.assign(line, 0, line.find(","));//assign tempName the username from the line
				size_t passwordLength = ((line.length()) - (tempName.length()) - 3);
				tempPass.assign(line, tempName.length() + 1, passwordLength); //assign tempPass the password from the line

				//getting permission level
				size_t pos = line.length() - 1;
				string tempPerm = line.substr(pos);


				users.push_back(User());//new User vector

				users[i].username = tempName;
				users[i].password = tempPass;
				users[i].permission = stoi(tempPerm);//string to int

				i++;
			}
		}//end While getLine
		usersFile.close();
		return i;
	} //endIf userFile is open
	else
	{
		message = "\nUnable to open usersLogin file\n";
		return 0;
	}
}


// ViewAllLogins function by Michaela
// Allows an Admin to View all logins in the system
// By making each line in file a user struct
// Admin can enter a username to edit the login
void viewAllLogins()
{
	string input;
	int i = getAllLogins();
	system("CLS");
	cout << "\n\t\t\tView All Users\n" << endl;
	for (int j = 0; j < i; j++)
	{
		if (users[j].username != "default")
		{
			cout << users[j].username << "\t\t" << users[j].password << "\t\t";
			switch (users[j].permission)
			{
			case 0:
				cout << "Admin" << endl;
				break;
			case 1:
				cout << "Hospital" << endl;
				break;
			case 2:
				cout << "Donor" << endl;
				break;
			default:
				cout << "No Permission Level" << endl;
				break;
			}
		}
	}
	cout << "\n[a] To add a new donor user\n[b] To add a new hospital user\n[c] To go back to the Menu\nOtherwise enter the username of the user to edit." << endl;
	cin >> input;
	if (input == "a")
	{
		donor();
	}
	else if (input == "b")
	{
		hospital();
	}
	else if (input == "c")
	{
		return;
	}
	else
	{
		editUser(input);
	}
}


// EditUser function by Michaela
// Gets username from input parameter
// Finds the vector struct with that username and allows user to change password, or delete the user
// Rewrites file with all users usernames and new password
void editUser(string input)
{
	// If there is no vector users, get all the users.
	if (users.empty())
	{
		getAllLogins();
	}
	string user = input;
	char choice = 'f';
	system("CLS");
	cout << "\n\t\t\tEdit User " << user << "\n" << endl;

	ofstream usersFile(filepathUsersLogin);// open file, 
	if (usersFile.is_open())
	{
		for (auto it = users.begin(); it != users.end(); it++)
		{
			if ((*it).username != "default")
			{
				if ((*it).username == user)
				{
					cout << (*it).username << endl;
					cout << (*it).password << endl;
					cout << (*it).permission << endl;
					cout << endl;
					cout << "To change password please enter [a]\nTo delete user please enter [d]\nTo go back to the MainMenu plese enter [b]";
					cin >> choice;
					switch (choice)
					{
					case 'a':
						cout << "\nPlease enter new password: " << endl;
						cin >> (*it).password;
						break;
					case 'd':
						system("CLS");
						cout << "\n\n\n\n\t\t\tAre you sure you want to delete user " << (*it).username << "? [y] yes or [n] no" << endl;
						cin >> choice;
						if (choice == 'y')
						{
							it = users.erase(it);
						}
						break;
					default:
						break;
					}
				}
				usersFile << (*it).username << "," << (*it).password << "," << (*it).permission << endl;
			}
		}
		usersFile.close();
		return;
	}
}

void welcome()

{
	int a;  //local variable 

	cout << "\n-----Welcome-----";  //main menu
	cout << "\nBefore booking an appointment, you must first under-go an eligibility test \nto see if you're eligible to donate!" << endl;
	cout << "\nPress 1 to see if youre eligible to donate";
	cout << "\nPress 2 to book an appointment";
	cout << "\nPress 3 to view, update or cancel an appointment";
	cout << "\nPress 4 to exit" << endl;
	cin >> a;
	switch (a)  //switch case for main menu screen
	{
	case 1: eligibilityTest();

		break;
	case 2: bookAppt();
		break;
	case 3: viewAppt();
		break;
	case 4: exit(0);
		break;
	default: cout << "\nInvalid answer, please choose from the following options: " << endl;
		break;
	}
	while (a != 4);
}
void eligibilityTest()
{
	users.push_back(User());  //global variable
	users.push_back(User());
	char op, a, b;  //local variables
	if (d == 1) //d = 1 means they already booked an appoint
	{
		cout << "\nYou already booked an appointment" << endl;
		cout << "\nif you would like to view or cancel an appointment press y, or press n to go back to the main page: ";
		cin >> op;
		if (op == 'y')
		{
			viewAppt();
		}
		else if (op == 'n')
		{
			welcome();
		}
	}
	e = 0;  //e = 0 means they haven't done the eligibilty test
	string Questions[7] = { {"\nAre you younger than 16 years and older than 71"}, {"\nDo you weigh less than 50kgs"}, {"\nHave you used drugs such as meth and cocaine before"},
		{"\nHave you had any new tattoos or piercings within the last 3months?? "}, {"\nHave you had a cold, sore throat or influenza within the last 28 days"}, {"\nHave you taken any antibiotics within the last 7 days"},
		{"\nHave you or anyone in your household tested positive for covid-19 in the last 14 days"} };  //string array to store the eligibility questions

	cout << "\nAnswer the following questions with y = yes, or n = no" << endl;
	for (int i = 0; i < 7; i++)  //for loop to loop through the questions within the array
	{
		cout << Questions[i] << endl;
		cin >> a;
		if (a == 'n')

		{
			cout << "Success" << endl;
			e = 1;  //e = 1 means they can donate and book an appointment
		}


		else if (a == 'y')
		{
			exit(0);
			e = 0;

		}

		while (a != 'n' && a != 'y')
		{
			cout << "Inavlid answer, please either type y = yes, or n = no: ";
			cin >> a;
		}
	}

	cout << "\nYou are eligible to donate blood \n\nWould you like to book an appointment? y/n: ";
	cin >> b;


	if (b == 'y')
	{
		e = 1;
		cout << "\nThank you for choosing to book an appointment" << endl;
		bookAppt();
	}
	else if (b == 'n')
	{
		cout << "\nYou chose not to book an appointment" << endl;
		welcome();
	}


}
void bookAppt()
{
	//local variables
	char b, c, op;


	if (e == 0 && d == 0)  //d = 0 & e = 0 redirects them to the eligibilty test if they haven't done it already
	{
		cout << "\nYou must be eligible first before you can donate, \nType y to go to the eligibility test \nType n to go back to main page " << endl;
		cin >> c;
		if (c == 'y')
		{
			eligibilityTest();

		}
		else if (c == 'n')
		{
			welcome();
		}
	}

	if (d == 1)  //d = 1 means they already booked an appointment
	{
		cout << "\nYou already booked an appointment" << endl;
		cout << "\nif you would like to view or cancel an appointment press y, or press n to go back to the main page";
		cin >> op;
		if (op == 'y')
		{
			viewAppt();
		}
		else if (op == 'n')
		{
			welcome();
		}
	}
	ofstream file;
	file.open("Booking appointments.txt");
	if (file.is_open())
		cout << "File is open";
	else cout << "Cannot open file";
	cout << "\n\nWhats your first name: ";
	cin.ignore();
	getline(cin, users[1].uData.fName);
	cout << "\nWhat is your blood group,\nPlease write in lower case letters \nIf you don't know just answer n: ";
	cin >> users[1].uData.bloodGroup;


	cout << "\nWhat location would you like to donate, please type it in lower case, choose from the following \nAuckland\nWellington\nChristchurch " << endl;
	cin >> users[1].uData.location;
	if (users[1].uData.location != "auckland" && "wellington" && "christchurch")
	{
		cout << "\nInvalid answer, choose from the folliwng options: ";
		cin >> users[1].uData.location;
		while (users[1].uData.location != "auckland" && "wellington" && "christchurch")
		{
			cout << "\nInvalid answer, choose from the folliwng options: ";
			cin >> users[1].uData.location;
		}
	}
	cout << "\nWhat date would you like to donate, please type in this order: year - month - day: ";
	cin >> users[1].uData.date;
	if (users[1].uData.date <= 20220620 || users[1].uData.date >= 20240620)
	{
		cout << "\nInvalid date, choose current timeline: ";
		cin >> users[1].uData.date;
		while (users[1].uData.date <= 20220620 || users[1].uData.date >= 20240620) //while loop if they keep answering wrong
		{
			cout << "\nInvalid date, choose current timeline: ";
			cin >> users[1].uData.date;
		}
	}

	file << users[1].uData.fName << "\t" << users[1].uData.bloodGroup << "\t" << users[1].uData.location << "\t" << users[1].uData.date << endl;
	file.close();
	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nTo view your details please type y, or type n to go back to the main menu: ";
		cin >> b;
		if (b == 'y')
		{
			cout << "\nThank you for choosing to book an appointment" << endl;
			d = 1;
			viewAppt();
		}
		else if (b == 'n')
		{
			cout << "\nYou chose not to view your details" << endl;
			d = 1;
			welcome();
		}
	} while (b != 'n' && b != 'y');

}
void viewAppt()
{
	char a, b;

	if (e == 0 && d == 0)
	{
		cout << "\nYou must be eligible first before you can donate, \nType y to go to the eligibility test \nOr if you're eligible then type b to book an appointment\nType n to go back to main page " << endl;
		cin >> b;
		if (b == 'y')
		{
			eligibilityTest();

		}
		else if (e == 1 && b == 'b')
		{
			bookAppt();
		}
		else if (b == 'n')
		{
			welcome();
		}
	}



	cout << "\n\n" << users[1].uData.fName;
	cout << "\n" << users[1].uData.bloodGroup;
	cout << "\n" << users[1].uData.location;
	cout << "\n" << users[1].uData.date << endl;

	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nIf you're happy with this and like to donate then type y, \n\nIf you want to edit your info press e\n\nor if you want to cancel your appointment then press n: ";
		cin >> a;
		if (a == 'y')
		{
			cout << "\nThank you for choosing to book an appointment" << endl;
			welcome();
		}
		else if (a == 'n')
		{
			cout << "\nYou chose not to book an appointment" << endl;
			cancelAppt();
		}
		else if (a == 'e')
		{
			cout << "\nYou chose edit an appointment" << endl;
			editAppt();
		}
	} while (a != 'y' && a != 'n' && a != 'e');


}
void editAppt()
{
	cout << "\n\nWhats your first name: ";
	cin.ignore();
	getline(cin, users[1].uData.fName);
	cout << "\nWhat is your blood group,\nPlease write in lower case letters \nIf you don't know just answer n: ";
	cin >> users[1].uData.bloodGroup;

	cout << "\nWhat location would you like to donate, please type it in lower case, choose from the following \nAuckland\nWellington\nChristchurch " << endl;
	cin >> users[1].uData.location;
	if (users[1].uData.location != "auckland" && "wellington" && "christchurch")
	{
		cout << "\nInvalid answer, choose from the folliwng options: ";
		cin >> users[1].uData.location;
		while (users[1].uData.location != "auckland" && "wellington" && "christchurch")
		{
			cout << "\nInvalid answer, choose from the folliwng options: ";
			cin >> users[1].uData.location;
		}
	}
	cout << "\nWhat date would you like to donate, please type in this order: year - month - day: ";
	cin >> users[1].uData.date;
	if (users[1].uData.date <= 20220620 || users[1].uData.date >= 20240620)
	{
		cout << "\nInvalid date, choose current timeline: ";
		cin >> users[1].uData.date;
		while (users[1].uData.date <= 20220620 || users[1].uData.date >= 20240620) //while loop if they keep answering wrong
		{
			cout << "\nInvalid date, choose current timeline: ";
			cin >> users[1].uData.date;
		}
	}
	ofstream updateFile;  //fstream file to over-write the previous ofstream file
	updateFile.open("booking appointments.txt");
	if (updateFile.is_open())
		cout << "File is open";
	else cout << "Cannot open file";
	updateFile << users[1].uData.fName << "\t" << users[1].uData.bloodGroup << "\t" << users[1].uData.location << "\t" << users[1].uData.date << endl;

	cout << " ** Info updated !!!!\n";
	users.clear();



	char a;  //local variable
	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nIf you're happy with this and like to donate then type y, \nOr if you want to cancel your appointment then press n: ";
		cin >> a;
		if (a == 'y')
		{
			cout << "\nThank you for choosing to book an appointment" << endl;
			welcome();
		}
		else if (a == 'n')
		{
			cout << "\nYou chose not to book an appointment" << endl;
			cancelAppt();
		}
	} while (a != 'y' && a != 'n');
}

void cancelAppt()
{

	char a; //local variable


	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nAre you sure you want to cancel your appointment? y/n: ";
		cin >> a;
		if (a == 'y')
		{
			deletefile();

		}

		else if (a == 'n')
		{
			d = 1;
			cout << "\nYou chose not to cancel your appointment." << endl;
		}
	} while (a != 'y' && a != 'n');
}



void deletefile()  //delete file completely deletes the txt file
{
	e = 0;  //resets the eligibilty criteria to 0 so they must re-do the test again
	d = 0;  //resets the booking to 0 so they must book again if theyre eligible
	string line;
	ifstream file("booking appointments.txt"); // create a file stream and open the file for reading 
	size_t recno = 0;
	while (getline(file, line)) // count all the lines from the file
	{
		++recno;
	}

	string* records = new string[recno]; // Dynamically allocated array for holding strings

	// we're now past the end of the file, in this case, I think it's simpler to close the file and start afresh
	file.close(); // close the file
	file.open("booking appointments.txt"); // open the file
	for (size_t i = 0; getline(file, line); ++i) // read all the lines from the file and store in records array
	{
		cout << line << endl;
		records[i] = line;
	}

	file.close(); // close the file

	cout << "\nThese are the records" << endl;
	for (size_t i = 0; i < recno; ++i) // display all the lines read
	{
		cout << i << ": " << records[i] << endl;
	}

	size_t recno1 = 0;
	cout << "\nWhich record would you like to delete?" << endl;
	cin >> recno1; // get the user to pick a record

	if (recno1 < 0 || recno1 >= recno) // validate their choice
	{
		cout << "Invalid choice" << endl;
	}
	else
	{
		cout << "You have chosen to delete record " << recno1 << ": " << records[recno1] << endl;
		records[recno1].erase(); // blank out users choice 
	}

	ofstream records_output("booking appointments.txt"); // create a file stream and open the file for output
	cout << "These are the records" << endl;
	for (size_t i = 0; i < recno; ++i) // write out the new records to screen
	{
		if (!records[i].empty())
		{
			cout << i << ": " << records[i] << endl;
			records_output << records[i] << endl;// also overwrite the original file with the new records
		}
	}

	records_output.close(); // close the file
	cout << endl << "Canceled successfully. Goodbye" << endl;


}


