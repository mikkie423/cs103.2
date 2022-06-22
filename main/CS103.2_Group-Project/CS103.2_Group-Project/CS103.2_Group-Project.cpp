// CS103.2_Group-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Blood Bank Management System
// By Michaela, Delear, and Sese


#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Read from File
#include <cstdio> // EOF

using namespace std;

//PersonalInfo struct by Michaela
//Will store all the users personal and relevant medical information
//To be accessed via the User struct.
struct BookingInformation
{
	string location;
	int date;
	int d, e;
};

//User struct by Michaela
//Will store the users login and permission information
//And points to the users personal information.
struct User
{
	string username, password;
	int permission;
	string bloodGroup;
	BookingInformation booking;

}; User* loggedinUser;

//Stock struct by Michaela
//Will store the stocks blood type and amount information
struct Stock
{
	string bloodGroup;
	int available;
};

//Request struct by Michaela
//Will store the requests by referencing stock, and users that ordered it
//Status variable for admin to change.
struct Request
{
	User user;
	Stock stock;
	int status;
};

vector<User> users;// Vector of User struct
vector<Stock> totals; // Vector of Stock struct
vector<Request>requests; // Vector of Request struct

/* Global Variables */
string filepathUsersLogin = "Files/usersLogin.txt";
string filepathBookings = "Files/bookingAppointments.txt";
string filepathStock = "Files/stock.txt";
string filepathRequests = "Files/requests.txt";
string message = "", line;
int d, e;  //global variables to identify if user already has appointment and has passed the eligibilty test
bool toApprove; //variable for if there are requests to be checked

/* Function Initialisations */
//Michaela's functions
bool login(int tries);
void loginRedirection();
void adminMenu();
void getAllLogins();
void viewAllLogins();
void editUser(string user);
bool checkIfUsernameExists(string tempName);
bool checkIfBloodGroupExists(string tempBlood);
void getAllBookings(); //for admin
void viewAllBookings(); //for admin
void hospitalMenu();
void getAllStock();
void viewAllStock();
void requestStock();
void viewAllRequests();
void getAllRequests();

//Rego is function for registration and Log and Hospital and Donor is Login Info - Sese
void rego();
void hospital();
void donor();

//Donor functions by Delear Goolmorade
void welcome();  
void eligibilityTest();
void bookAppt();
void viewAppt();
void editAppt();
void deletefile(string user);

// Program execution begins and ends here.
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
		getAllLogins();
		donor();
	case 2:
		getAllLogins();
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

//File Handling for Donor registration part //Sese Code
void donor()
{
	ofstream file;
	string tempName = "";
	string tempPass = "";
	string tempBlood = "";

	do {
		cout << message << endl;
		cout << "add user name: ";
		cin >> tempName;
	} while (checkIfUsernameExists(tempName) == true);
	cin.clear();
	message = "";
	cout << endl;
	cout << "add password: ";
	cin >> tempPass;
	cout << endl;
	do {
		cout << message << endl;
		cout << "Enter Blood Type or if you dont know enter na: ";
		cin >> tempBlood;
	} while (checkIfBloodGroupExists(tempBlood) == false);
	message = "";


	file.open(filepathUsersLogin, ios::app); // this will create file if it's not there, or open. //file mode parameter 
	if (file.is_open())
	{
		file << "\n" << tempName << "\n" << tempPass << "\n" << tempBlood << "\n" << 2;
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

		file << "\n" << tempName << "\n" << tempPass << "\n" << "na" << "\n" << 1;
		file.close();
	}
	else
	{
		cout << "File not found!" << endl;
	}

}

// CheckIfUsernameExists() by Michaela
// Checks if the username passed already exists in the usersLogin file.
// Returns true if the username already exists,
// Returns false if the username doesn't exist.
bool checkIfUsernameExists(string tempName)
{
	bool ans;
	for (auto it = users.begin(); it != users.end(); it++)
	{
		/* if checkUsername matches the tempName */
		if (tempName == (*it).username)
		{
			message = "\nUsername exists\n";
			return true;
		}
		else
		{
			message = "\nUsername doesnt exist\n";
			ans = false;
		}
	}
	return ans;
}

// CheckIfBloodGroupExists() by Michaela
// Checks if the bloodgroup passed exists in the stock file or matches na
// Returns true if the bloodgroup already exists or matches na,
// Returns false if the bloodgroup doesn't exist.
bool checkIfBloodGroupExists(string tempBlood)
{
	getAllStock();
	bool ans = false;
	for (auto it = totals.begin(); it != totals.end(); it++)
	{
		/* if checkUsername matches the tempName */
		if (tempBlood == "na")
		{
			return true;
		}
		else if (tempBlood == (*it).bloodGroup)
		{
			return true;
		}
		else
		{
			message = "\nBlood Group doesnt exist\n";
			ans = false;
		}
	}
	return ans;
}

// Login function by Michaela
// Returns true if the username and password entered match.
// Returns false after 3 wrong attempts.
// References users information with loggedinUser pointer
bool login(int tries)
{
	string username, password;
	size_t i = 0;

	system("CLS");
	cout << "\n\t\tLogin\n" << endl;

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

		for (size_t i = 0; i < users.size(); i++)
		{
			/* if username matches the tempName */
			while (username == users[i].username && tries < 3)
			{
				cout << message << endl;//potential message output
				message = "";

				cout << "Please enter password: ";
				cin >> password;
				/* if password matches the tempPass */

				if (password == users[i].password)
				{
					loggedinUser = &users[i]; //loggedinUser pointer to the address of the vector User struct that is the user logged in

					return true;
				}
				else
				{

					tries++;
					message = "\nPassword incorrect\n";
				}//endIf password == tempPass
			}//endIf username == tempName
		}
	}

	if (username != users[i].username && tries < 3)
	{
		cout << users[i].username << "\ttries: " << tries << endl;
		tries++;
		message = "\nUsername incorrect\n";
		login(tries);
	}
	return false;
}

// LoginRedirection function by Michaela
// Depending on the logged in users permission level: 0 - Admin, 1 - Hospital, 2 - Donor;
// Redirects the user to pages they are allowed to access,
// Or boots user out of system if they dont have permission
void loginRedirection()
{
	int tries = 0;

	if (login(tries))
	{
		/* Successful Login */
		switch (loggedinUser->permission)
		{
		case 0:
			adminMenu();
			break;
		case 1:
			hospitalMenu();
			break;
		case 2:
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
// Allows Admin to view stock and stock requests
// Allows Admin to view, edit and delete appointments
// Allows Admin to view, edit and delete users
void adminMenu()
{
	getAllRequests();
	if (loggedinUser->permission != 0) // if loggedinUser isnt an admin, logs the user out and boots to main screen
	{
		loggedinUser = {};
		rego();
	}

	char input = 'f';

	while (input != 'a' || input != 'b' || input != 'c' || input != 'd')
	{
		system("CLS");
		cout << "\n\t\t\tWelcome to the Admin Homepage.\n\n";
		if (toApprove == true)
		{
			message = "\n\t\t*** There are new requests for you to approve ***\n";
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
			viewAllStock();
			break;
		case 'b':
			viewAllBookings();
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
// Gets all the logins from the filepathUsersLogin
// Sets them all to be a users[] vector
void getAllLogins()
{
	if (!users.empty())
	{
		users.clear();
	}
	string temp;
	ifstream usersFile(filepathUsersLogin);// open file
	if (usersFile.is_open())
	{
		User temp_user;

		while (!usersFile.eof())
		{
			getline(usersFile, temp_user.username);
			getline(usersFile, temp_user.password);
			getline(usersFile, temp_user.bloodGroup);
			getline(usersFile, temp);
			temp_user.permission = stoi(temp);
			users.push_back(temp_user);
		}

	}
	else
	{
		cout << "Users File not found" << endl;
	}
	usersFile.close();
	getAllBookings();
}

// GetAllBookings function by Michaela
// Gets all the bookings from the filepathBookings
// Checks if the name in the booking matches a users name
// If it matches, adds booking details to users[] vector
void getAllBookings()
{
	if (users.empty())
	{
		getAllLogins();
	}
	string temp;
	string tempName;
	ifstream bookingsFile(filepathBookings);// open file
	if (bookingsFile.is_open())
	{
		while (!bookingsFile.eof())
		{
			getline(bookingsFile, tempName);
			for (size_t i = 0; i < users.size(); i++)
			{
				if (tempName != users[i].username && users[i].booking.d != 1)
				{
					users[i].booking.d = 0;
					users[i].booking.e = 0;
				}
				else if (tempName == users[i].username)
				{
					users[i].booking.d = 1;
					users[i].booking.e = 1;
					getline(bookingsFile, users[i].bloodGroup);
					getline(bookingsFile, users[i].booking.location);
					getline(bookingsFile, temp);
					users[i].booking.date = stoi(temp);
				}
			}
		}
		bookingsFile.close();
	}
	else
	{
		cout << "Bookings File not found" << endl;
	}
}

// GetAllStock function by Michaela
// Gets all the stock from the filepathStock
// Sets them all to be a totals[] vector
void getAllStock()
{
	if (!totals.empty())
	{
		totals.clear();
	}
	string temp;
	ifstream file(filepathStock);// open file
	if (file.is_open())
	{
		Stock temp_stock;

		while (!file.eof())
		{
			getline(file, temp_stock.bloodGroup);
			getline(file, temp);
			temp_stock.available = stoi(temp);
			totals.push_back(temp_stock);
		}
		file.close();

	}
	else
	{
		cout << "Stock File not found" << endl;
	}
}

// GetAllRequests function by Michaela
// Gets all the requests from the filepathRequests
// Sets them all to be a requests[] vector
void getAllRequests()
{
	if (!requests.empty())
	{
		requests.clear();
	}
	string temp;
	ifstream file(filepathRequests);// open file
	if (file.is_open())
	{
		Request temp_request;

		while (!file.eof())
		{
			getline(file, temp_request.user.username);
			getline(file, temp_request.stock.bloodGroup);
			getline(file, temp);
			temp_request.stock.available = stoi(temp);
			getline(file, temp);
			temp_request.status = stoi(temp);
			requests.push_back(temp_request);
			if (temp_request.status == 0)
			{
				toApprove = true;
			}
		}
		file.close();

	}
	else
	{
		cout << "Requests File not found" << endl;
	}
}

// ViewAllStock function by Michaela
// Displays all stock in the system
// Directs hospitals to requestStock
// Directs admin to viewAllRequests
void viewAllStock()
{
	getAllStock();

	system("CLS");
	cout << "\n\t\t\tView All Stock\n" << endl;
	for (size_t i = 0; i < totals.size(); i++)
	{
		cout << totals[i].bloodGroup << "\t\t" << totals[i].available << endl;
	}
	if (loggedinUser->permission == 1 && loggedinUser->permission != 0)
	{
		requestStock();
	}
	else if (loggedinUser->permission == 0)
	{
		viewAllRequests();
	}
}

// ViewAllRequests function by Michaela
// Allows an Admin to View all requests in the system
// Admin can then edit the status of any request
void viewAllRequests()
{
	string input;
	getAllRequests();

	system("CLS");
	cout << "\n\t\t\tView All Requests\n" << endl;
	for (size_t i = 0; i < requests.size(); i++)
	{
		cout << "[" << i << "] " << requests[i].user.username << "\t" << requests[i].stock.bloodGroup << "\t" << requests[i].stock.available << "\t";
		switch (requests[i].status)
		{
		case 0:
			cout << "Status: Pending" << endl;
			break;
		case 1:
			cout << "Status: Accepted" << endl;
			break;
		case 2:
			cout << "Status: Denied" << endl;
			break;
		}
	}
	cout << "\nTo change the status of a request please enter the request number, otherwise [a] to go back to main menu.\n" << endl;
	cin >> input;
	if (input == "a")
	{
		return;
	}
	else
	{
		system("CLS");
		int j = stoi(input);
		cout << requests[j].user.username << "\t" << requests[j].stock.bloodGroup << "\t" << requests[j].stock.available << "\t";
		switch (requests[j].status)
		{
		case 0:
			cout << "Status: Pending" << endl;
			break;
		case 1:
			cout << "Status: Accepted" << endl;
			break;
		case 2:
			cout << "Status: Denied" << endl;
			break;
		}
		cout << "\n[a] To accept this request\n[b] To deny this request\n[c] Go back to main menu.\n" << endl;
		input = "";
		cin >> input;
		if (input == "a")
		{
			requests[j].status = 1;
			cout << "accepted pending request:" << endl;
			for (size_t j = 0; j < requests.size(); j++)
			{
				for (size_t i = 0; i < totals.size(); i++)
				{
					if (totals[i].bloodGroup == requests[j].stock.bloodGroup)
					{
						totals[i].available -= requests[j].stock.available;
					}
				}
			}
		}
		else if (input == "b")
		{
			requests[j].status = 2;
			cout << "denied pending request:" << endl;
		}

		ofstream outfile;
		outfile.open(filepathRequests, ios::trunc);
		for (size_t i = 0; i < requests.size(); i++) {
			if (i == 0)
			{
				outfile << requests[i].user.username << "\n" << requests[i].stock.bloodGroup << "\n" << requests[i].stock.available << "\n" << requests[i].status;
			}
			else
			{
				outfile << "\n" << requests[i].user.username << "\n" << requests[i].stock.bloodGroup << "\n" << requests[i].stock.available << "\n" << requests[i].status;
			}
		}
		outfile.close();
		return;
	}
}

// RequestStock function by Michaela
// Allows a hospital to enter a new request for stock in the system
void requestStock()
{
	string input;
	int amount;

	cout << "\n[a] To go back to the menu\nOtherwise to request stock please enter the Blood Group : ";
	cin >> input;
	if (input == "a")
	{
		return;
	}
	else
	{
		for (size_t i = 0; i < totals.size(); i++)
		{
			if (totals[i].bloodGroup == input)
			{
				cout << "Please enter how much of the stock you want: ";
				cin >> amount;
				if (amount <= totals[i].available)
				{
					ofstream file(filepathRequests, ios::app);// open file, 
					if (file.is_open())
					{
						file << "\n" << loggedinUser->username << "\n" << input << "\n" << amount << "\n" << 0;
						cout << "\n" << loggedinUser->username << "\n" << input << "\n" << amount << "\n" << 0;
					}
					file.close();

					message = "Stock has been successfully requested.";
					hospitalMenu();
				}
			}
		}
	}
}

// ViewAllBookings function by Michaela
// Allows an Admin to View all bookings in the system
// Admin can then add new bookings, or edit bookings
void viewAllBookings()
{
	string input;
	system("CLS");
	cout << "\n\t\t\tView All Bookings\n" << endl;

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].booking.d == 1)
		{
			cout << users[i].username << "\t\t" << users[i].bloodGroup << "\t\t" << users[i].booking.location << "\t\t" << users[i].booking.date << endl;
		}
	}
	cout << "\n[a] To add a new booking\n[b] To go back to the Menu\nOtherwise enter the username of the booking to edit." << endl;
	cin >> input;
	if (input == "a")
	{
		cout << "To add a booking, please login as the donor and go through the process as them.\n[a] To login as the donor\n[b] To go back to the Menu" << endl;
		input = "";
		cin >> input;
		if (input == "a")
		{
			loggedinUser = {};
			getAllLogins();
			loginRedirection();
		}
		else
		{
			return;
		}
	}
	else if (input == "b")
	{
		return;
	}
	else
	{
		cout << "To edit a booking, please login as the donor and go through the process as them.\n[a] To login as the donor\n[b] To go back to the Menu" << endl;
		input = "";
		cin >> input;
		if (input == "a")
		{
			loggedinUser = {};
			getAllLogins();
			loginRedirection();
		}
		else
		{
			return;
		}
	}
}

// ViewAllLogins function by Michaela
// Allows an Admin to View all logins in the system
// Admin can enter a username to edit the login
void viewAllLogins()
{
	getAllLogins();

	string input;
	system("CLS");
	cout << "\n\t\t\tView All Users\n" << endl;
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].username != "default")
		{
			cout << users[i].username << "\t\t" << users[i].password << "\t\t" << users[i].bloodGroup << "\t\t";
			switch (users[i].permission)
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
// Rewrites file with all users
void editUser(string user)
{
	string tempBlood;
	char choice = 'f';
	system("CLS");
	cout << "\n\t\t\tEdit User " << user << "\n" << endl;


	for (auto it = users.begin(); it != users.end(); it++)
	{
		if ((*it).username == user)
		{
			cout << "\t\t" << (*it).username << endl;
			cout << "\t\t" << (*it).password << endl;
			cout << "\t\t" << (*it).bloodGroup << endl;
			cout << "\t\t" << (*it).permission << endl;
			cout << endl;
			cout << "To change password please enter [a]\nTo go back to the MainMenu plese enter [b]\nTo change users blood group please enter [c]\nTo delete user please enter [d]";
			cin >> choice;
			switch (choice)
			{
			case 'a':
				cout << "\nPlease enter new password: " << endl;
				cin >> (*it).password;
				break;
			case 'c':
				do {
					cout << message << endl;
					cout << "Enter Blood Type or if you dont know enter na: ";
					cin >> tempBlood;
				} while (checkIfBloodGroupExists(tempBlood) == false);
				message = "";
				(*it).bloodGroup = tempBlood;
				break;
			case 'd':
				system("CLS");
				cout << "\n\n\n\n\t\t\tAre you sure you want to delete user " << (*it).username << "? [y] yes or [n] no" << endl;
				cin >> choice;
				if (choice != 'y')
				{
					message = "\nUser not deleted\n";
				}
				else
				{
					if (choice == 'y')
					{
						if ((*it).username == user)
						{
							loggedinUser = {};
							it = users.erase(it);
							system("CLS");
							cout << "\n\t\tYour account has been deleted\n" << endl;
							system("PAUSE");
						}
					}
					else if (choice == 'y')
					{
						it = users.erase(it);
					}
				}
				break;
			default:
				break;
			}
			ofstream usersFile(filepathUsersLogin);// open file, 
			if (usersFile.is_open())
			{
				for (size_t i = 0; i < users.size(); i++)
				{
					if (i == 0)
					{
						usersFile << users[i].username << "\n" << users[i].password << "\n" << users[i].bloodGroup << "\n" << users[i].permission;

					}
					else
					{
						usersFile << "\n" << users[i].username << "\n" << users[i].password << "\n" << users[i].bloodGroup << "\n" << users[i].permission;
					}
				}
			}
			usersFile.close();
			getAllLogins();
			if (loggedinUser == nullptr)
			{
				deletefile(user);
				rego();
			}
			else if (loggedinUser->permission == 0)
			{
				viewAllLogins();
			}
			else
			{
				welcome();
			}
		}
	}

}

// HospitalMenu function by Michaela
// Allows hospital user to view and request stock
void hospitalMenu()
{
	if (loggedinUser->permission == 2) // if loggedinUser isnt an admin, logs the user out and boots to main screen
	{
		loggedinUser = {};
		rego();
	}
	char input;

	system("CLS");
	cout << "\n\t\t\tWelcome to the Hospital Homepage.\n\n";
	cout << message << endl;
	message = "";
	cout << "[a] Display Stock\n[b] Logout" << endl;
	cin >> input;
	if (input == 'a')
	{
		viewAllStock();
	}
	else
	{
		loggedinUser = {};
		rego();
	}
}


//Functions by Delear for the donor user
void welcome()
{
	int a;  //local variable 

	system("CLS");
	cout << "\n-----Welcome-----";  //main menu

	cout << message;
	message = "";

	cout << "\nBefore booking an appointment, you must first under-go an eligibility test \nto see if you're eligible to donate!" << endl;
	cout << "\nPress 1 to see if youre eligible to donate";
	cout << "\nPress 2 to book an appointment";
	cout << "\nPress 3 to view, update or cancel an appointment";
	cout << "\nPress 4 to view, update or delete your account";
	cout << "\nPress 5 to logout" << endl;

	cin >> a;
	switch (a)  //switch case for main menu screen
	{
	case 1: eligibilityTest();
		break;
	case 2: bookAppt();
		break;
	case 3: viewAppt();
		break;
	case 4:
		editUser(loggedinUser->username);
		break;
	case 5:
		loggedinUser = {}; //reset the structure as user logs out
		rego();
		break;
	default:
		message = "\nInvalid answer, please choose from the following options: \n";
		welcome();
		break;
	}

}
void eligibilityTest()
{
	char op, a, b;  //local variables
	if (loggedinUser->booking.d == 1) //d = 1 means they already booked an appoint
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
	loggedinUser->booking.e = 0;  //e = 0 means they haven't done the eligibilty test
	string Questions[7] = { {"\nAre you younger than 16 years and older than 71"}, {"\nDo you weigh less than 50kgs"}, {"\nHave you used drugs such as meth and cocaine before"},
		{"\nHave you had any new tattoos or piercings within the last 3months?? "}, {"\nHave you had a cold, sore throat or influenza within the last 28 days"}, {"\nHave you taken any antibiotics within the last 7 days"},
		{"\nHave you or anyone in your household tested positive for covid-19 in the last 14 days"} };  //string array to store the eligibility questions

	cout << "\nAnswer the following questions with y = yes, or n = no" << endl;
	for (int i = 0; i < 7; i++)  //for loop to loop through the questions within the array
	{
		cout << Questions[i] << endl;
		cin >> a;

		while (a != 'n' && a != 'y')
		{
			cout << "Invalid answer, please either type y = yes, or n = no: ";
			cin >> a;
		}




		if (a == 'y')
		{
			loggedinUser->booking.e = 0;
			message = "\nSorry, you are not eligible, plese try again later.\n";
			welcome();

		}
		else if (a == 'n')

		{
			cout << "Success" << endl;
			loggedinUser->booking.e = 1;  //e = 1 means they can donate and book an appointment
		}


	}

	cout << "\nYou are eligible to donate blood \n\nWould you like to book an appointment? y/n: ";
	cin >> b;


	if (b == 'y')
	{
		loggedinUser->booking.e = 1;
		cout << "\nThank you for choosing to book an appointment ";
		bookAppt();
	}
	else
	{
		cout << "\nYou chose not to book an appointment" << endl;
		welcome();
	}


}
void bookAppt()
{

	system("CLS");
	//local variables
	char b, c, op;


	if (loggedinUser->booking.e == 0 && loggedinUser->booking.d == 0)  //d = 0 & e = 0 redirects them to the eligibilty test if they haven't done it already
	{
		cout << "\nYou must be eligible first before you can donate, \nType y to go to the eligibility test \nType n to go back to main page " << endl;
		cin >> c;
		if (c == 'y')
		{
			eligibilityTest();
		}
		else
		{
			welcome();
		}
	}
	if (loggedinUser->booking.d == 1)  //d = 1 means they already booked an appointment
	{
		cout << "\nYou already booked an appointment" << endl;
		cout << "\nif you would like to view or cancel an appointment press y, or press n to go back to the main page: ";
		cin >> op;
		if (op == 'y')
		{
			viewAppt();
		}
		else
		{
			welcome();
		}
	}


	cout << "\nWhat location would you like to donate, please type it in lower case, choose from the following \nAuckland\nWellington\nChristchurch " << endl;
	cin >> loggedinUser->booking.location;
	if (loggedinUser->booking.location != "auckland" && loggedinUser->booking.location != "wellington" && loggedinUser->booking.location != "christchurch")
	{
		cout << "\nInvalid answer, choose from the folliwng options: ";
		cin >> loggedinUser->booking.location;
		while (loggedinUser->booking.location != "auckland" && loggedinUser->booking.location != "wellington" && loggedinUser->booking.location != "christchurch")
		{
			cout << "\nInvalid answer, choose from the folliwng options: ";
			cin >> loggedinUser->booking.location;
		}
	}
	cout << "\nWhat date would you like to donate, please type in this format: yyyymmdd: ";
	cin >> loggedinUser->booking.date;
	if (loggedinUser->booking.date <= 20220620 || loggedinUser->booking.date >= 20240620)
	{
		cout << "\nInvalid date, choose current timeline: ";
		cin >> loggedinUser->booking.date;
		while (loggedinUser->booking.date <= 20220620 || loggedinUser->booking.date >= 20240620) //while loop if they keep answering wrong
		{
			cout << "\nInvalid date, choose current timeline: ";
			cin >> loggedinUser->booking.date;
		}
	}
	ofstream file;
	file.open(filepathBookings, ios::app);
	if (file.is_open())
	{
		file << "\n" << loggedinUser->username << "\n" << loggedinUser->bloodGroup << "\n" << loggedinUser->booking.location << "\n" << loggedinUser->booking.date;
		file.close();
		getAllLogins();
	}
	else
	{
		cout << "Cannot open file";
	}

	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nTo view your details please type y, or type n to go back to the main menu: ";
		cin >> b;
		loggedinUser->booking.d = 1;
		if (b == 'y')
		{
			cout << "\nThank you for choosing to book an appointment" << endl;
			viewAppt();
		}
		else
		{
			cout << "\nYou chose not to view your details" << endl;
			welcome();
		}
	} while (b != 'y');

}
void viewAppt()
{
	char a, b, op;

	if (loggedinUser->booking.e == 0)
	{
		cout << "\nYou must be eligible first before you can donate, \nType y to go to the eligibility test \nType n to go back to main page " << endl;
		cin >> b;
		if (b == 'y')
		{
			eligibilityTest();
		}
		else
		{
			welcome();
		}
	}

	if (loggedinUser->booking.d == 0)
	{
		cout << "\nYou haven't booked an appointment" << endl;
		cout << "\nIf you would like to book an appointment press y, or press n to go back to the main page" << endl;
		cin >> op;
		if (op == 'y')
		{
			bookAppt();
		}
		else
		{
			welcome();
		}
	}

	cout << "\n\n" << loggedinUser->username;
	cout << "\n" << loggedinUser->bloodGroup;
	cout << "\n" << loggedinUser->booking.location;
	cout << "\n" << loggedinUser->booking.date << endl;

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
			deletefile(loggedinUser->username);
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

	cout << "\nWhat location would you like to donate, please type it in lower case, choose from the following \nAuckland\nWellington\nChristchurch " << endl;
	cin >> loggedinUser->booking.location;
	if (loggedinUser->booking.location != "auckland" && loggedinUser->booking.location != "wellington" && loggedinUser->booking.location != "christchurch")
	{
		cout << "\nInvalid answer, choose from the folliwng options: ";
		cin >> loggedinUser->booking.location;
		while (loggedinUser->booking.location != "auckland" && loggedinUser->booking.location != "wellington" && loggedinUser->booking.location != "christchurch")
		{
			cout << "\nInvalid answer, choose from the folliwng options: ";
			cin >> loggedinUser->booking.location;
		}
	}
	cout << "\nWhat date would you like to donate, please type in this format: yyyymmdd: ";
	cin >> loggedinUser->booking.date;
	if (loggedinUser->booking.date <= 20220620 || loggedinUser->booking.date >= 20240620)
	{
		cout << "\nInvalid date, choose current timeline: ";
		cin >> loggedinUser->booking.date;
		while (loggedinUser->booking.date <= 20220620 || loggedinUser->booking.date >= 20240620) //while loop if they keep answering wrong
		{
			cout << "\nInvalid date, choose current timeline: ";
			cin >> loggedinUser->booking.date;
		}
	}
	ofstream updateFile;
	updateFile.open(filepathBookings, ios::trunc);
	if (updateFile.is_open())
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].booking.d == 1)
			{
				int j = 0;
				if (j == 0)
				{
					updateFile << users[i].username << "\n" << users[i].bloodGroup << "\n" << users[i].booking.location << "\n" << users[i].booking.date;
				}
				else
				{
					updateFile << "\n" << users[i].username << "\n" << users[i].bloodGroup << "\n" << users[i].booking.location << "\n" << users[i].booking.date;
				}
				j++;
			}
		}
		updateFile.close();
		getAllLogins();
	}
	else
	{
		cout << "Cannot open file";
	}

	cout << " ** Info updated !!!!\n";
	//users.clear();



	char a;  //local variable
	do  //do-while loop incase they fail to answer any of the following options
	{
		cout << "\nIf you're happy with this and like to donate then type y, \nOr if you want to cancel your appointment then press n: ";
		cin >> a;
		loggedinUser->booking.d = 1;
		if (a == 'y')
		{
			cout << "\nThank you for choosing to book an appointment" << endl;
			welcome();
		}
		else if (a == 'n')
		{
			cout << "\nAre you sure you want to cancel your appointment? y/n: ";
			cin >> a;
			if (a == 'y')
			{
				deletefile(loggedinUser->username);
			}
			else
			{
				welcome();
			}
		}
	} while (a != 'y' && a != 'n');
}
void deletefile(string user)  
{
	char choice = 'f';
	getAllLogins();

	for (auto it = users.begin(); it != users.end(); it++)
	{
		if ((*it).booking.d == 1)
		{
			if ((*it).username == user)
			{
				system("CLS");
				cout << "\n\n\n\n\t\t\tAre you sure you want to cancel the booking for " << (*it).username << "? [y] yes or [n] no" << endl;
				cin >> choice;
				if (choice != 'y')
				{
					message = "\nUser not deleted\n";
					welcome();
				}
				else
				{
					(*it).booking.date = NULL;
					(*it).booking.location = "";
					(*it).booking.e = 0;  //resets the eligibilty criteria to 0 so they must re-do the test again
					(*it).booking.d = 0;  //resets the booking to 0 so they must book again if theyre eligible
				}
			}
		}
	}


	ofstream deleteFile;
	deleteFile.open(filepathBookings, ios::trunc);
	if (deleteFile.is_open())
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].booking.d == 1)
			{
				int j = 0;
				if (j == 0)
				{
					deleteFile << users[i].username << "\n" << users[i].bloodGroup << "\n" << users[i].booking.location << "\n" << users[i].booking.date;

				}
				else
				{
					deleteFile << "\n" << users[i].username << "\n" << users[i].bloodGroup << "\n" << users[i].booking.location << "\n" << users[i].booking.date;
				}
			}
		}
		deleteFile.close();
	}
	else
	{
		cout << "Cannot open file";
	}
	getAllLogins();
	cout << endl << "Canceled successfully" << endl;
	if (loggedinUser->permission == 0)
	{
		return;
	}
	else { welcome(); }
}