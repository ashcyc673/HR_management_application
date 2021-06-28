//DBS211 NGG Group Assignment Milestone2
//Name: Daniel Yuan         Student Number: 143997195    Email: xkyuan1@myseneca.ca
//Name: Aakarshan Singh     Student Number: 107642159    Email: asingh430@myseneca.ca
//Name: YoonYeong Jeong     Student Number: 159760180    Email: yjeong17@myseneca.ca
//Name: Reza Poursafa       Student Number: 140640194    Email: rpoursafa@myseneca.ca
//Name: Chiao-Ya Chang      Student Number: 130402191    Email: cchang76@myseneca.ca


#include<iostream>
#include<iomanip>
#include <cstring>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;


struct Employee
{
	int employeeNumber;
	string lastName;
	string firstName;
	string email;
	string phone;
	string extension;
	string reportsTo;
	string jobTitle;
	string city;

	Employee()
	{
		employeeNumber = 0;
	}
};

bool isNumber(char userInput[])
{
	bool state = true;

	for (int i = 0; i < strlen(userInput); i++)
	{
		if (!isdigit(userInput[i]))
		{
			state = false;
			break;
		}
	}

	return state;
} // this function will be used to get digit numbers from users. what it does is checking if user's input is digit number or not. if user's input is digit number this function weill return true. otherwise false.

int getOption()
// this one will be used in a switch case function in main file to verify if user's input is valid or not.
{
	char option[255];
	int optionNumber = 0;
	bool state = false;
	while (!state)
	{
		cin.getline(option, 255, '\n'); //get input from the user
		if (isNumber(option)) //check if user's input is a digit number or not.
		{
			optionNumber = stoi(option); // if it is a digit number assign the number in to temp int type of number to verify if the number is in the range which is 0 to 5( those numbers are executable numbers which will be used in switch case) 
			if (optionNumber >= 0 && optionNumber <= 5)
			{
				state = true; // if the number is in the range return true.
			}
			else
			{
				cout << "Out of range." << endl; // or display warning message.
			}
		}
		else
		{
			cout << "You need to enter a number." << endl; // if user's input is not a digit numer display warning message.
		}
	}
	return optionNumber; //returns safe input which will be used in the switch case(digit number between 0 to 5) 
}


void menu() //header for the menu
{
	cout << "*********************HR Menu*********************" << endl;
	cout << "1)	Find Employee" << endl;
	cout << "2)	Employees Report" << endl;
	cout << "3)	Add Employee" << endl;
	cout << "4)	Update Employee" << endl;
	cout << "5)	Remove Employee" << endl;
	cout << "0)	Exit" << endl;
	cout << endl;
}
// this option will be use to display the menu options given by instruction and let users know what types of menu we have ,let them choose the option.

int findEmployee(Connection* conn, int employeeNumber, Employee* emp)
{// this function will display an individual employee's information on displayEmployee function when user type case 1 on main file.

	try { //check any errors for the result of a connection failure

		Statement* stmt = conn->createStatement(); //create an statement object
		ResultSet* rs = stmt->executeQuery("SELECT employeenumber, firstname, lastname , email, phone, extension, reportsto, jobtitle, city FROM offices o JOIN employees e ON o.officecode = e.officecode WHERE employeenumber = " + to_string(employeeNumber));

		if (rs->next()) { //fetch the data from the execureQuery() function

			emp->employeeNumber = rs->getInt(1);
			emp->lastName = rs->getString(2);
			emp->firstName = rs->getString(3);
			emp->email = rs->getString(4);
			emp->phone = rs->getString(5);
			emp->extension = rs->getString(6);
			emp->reportsTo = rs->getString(7);
			emp->jobTitle = rs->getString(8);
			emp->city = rs->getString(9);

			delete[] rs;
			rs = nullptr;
			conn->terminateStatement(stmt); //close the connection and terminate statement
			return 1; // it returns 1 if user is exist
		}
		else
		{
			return 0; //returns 0 if user is not exist
		}

	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}

void addEmployee(Connection* conn, struct Employee* emp) { //Insert new employee info into the table

	if (conn != nullptr) {

		try {
			if (!findEmployee(conn, emp->employeeNumber, emp)) {

				Statement* statement = conn->createStatement();
				string msg = "";
				msg += "INSERT INTO employees(employeenumber , lastname , firstname , extension , email ,officecode , reportsto, jobtitle) VALUES ( ";
				msg += to_string(emp->employeeNumber) + ", '" + emp->lastName + "', '" + emp->firstName + "', '" + emp->extension + "', '" + emp->email + "', '3'" + ", 1002" + ", '" + emp->jobTitle + "' )";
				statement->executeUpdate(msg);
				conn->commit();
				conn->terminateStatement(statement);
				cout << "Employee added successfully!" << endl;
			}
			else {
				cout << "Employee already exists!";
			}

		}

		catch (SQLException& error) {
			cout << error.getErrorCode() << ": " << error.getMessage();
		}
	}
	else {
		cout << "connection not successful!" << endl;
	}
}


void displayEmployee(Connection* conn, Employee* emp)
{
	cout << "Employee Number >>";
	int employeeNum = 0;
	char userInput[256];
	bool state = false;

	while (!state)
	{
		cin.getline(userInput, 256, '\n');// get input about employee number from the user to get employee's information. 
		if (isNumber(userInput)) //check if user's input is a digit number or not.
		{
			employeeNum = stoi(userInput); //convert string to interger
			state = true;  // return true if input is valid 
		}
		else
		{
			cout << "Please enter the employee number: "; // otherwise print warning message
		}
	}

	if (findEmployee(conn, employeeNum, emp))// check if valid input is matched with valid employee's information. and if it is true
	{
		cout << endl;
		cout << "employeeNumber = " << emp->employeeNumber << endl;
		cout << "lastName = " << emp->lastName << endl;
		cout << "firstName = " << emp->firstName << endl;
		cout << "email = " << emp->email << endl;
		cout << "phone = " << emp->phone << endl;
		cout << "extension = " << emp->extension << endl;
		cout << "reportsTo = " << emp->reportsTo << endl;
		cout << "jobTitle = " << emp->jobTitle << endl;
		cout << "city = " << emp->city << endl;
		cout << endl; // display individual employee's information
	}
	else
	{
		cout << "Employee " << userInput << " does not exist.." << endl; //else display wanring message
	}
}


void displayAllEmployees(Connection* conn) // when user's input from switch case is 2 this program will be executed to display all employee's information
{
	cout << endl;
	cout << left << setw(8) << "E";
	cout << left << setw(20) << "Employee Name";
	cout << left << setw(35) << "Email";
	cout << left << setw(18) << "Phone";
	cout << left << setw(8) << "Ext";
	cout << "Manager" << endl;
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	// display header for the employee report
	try //check any errors for the result of a connection failure. If connection fails, the catch statement will run and show the error message.
	{
		Statement* stmt = conn->createStatement(); //create an statement object
		ResultSet* rs = stmt->executeQuery("SELECT e.employeenumber, e.firstname || ' ' || e.lastname , e.email, phone, e.extension,  m.firstname || ' ' || m.lastname FROM offices o JOIN employees e ON o.officecode = e.officecode LEFT JOIN employees m ON e.reportsto = m.employeenumber ");

		while (rs->next()) //fetch the data from the execureQuery() function.
		{				 // set the data to columns with indexes from 1 to 5
			cout << left << setw(8) << rs->getInt(1);
			cout << left << setw(20) << rs->getString(2);
			cout << left << setw(35) << rs->getString(3);
			cout << left << setw(18) << rs->getString(4);
			cout << left << setw(8) << rs->getString(5);
			cout << rs->getString(6) << endl;
		}
		delete[] rs;
		rs = nullptr;
		conn->terminateStatement(stmt);//close the connection and terminate statement
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
}


void updateEmployee(Connection* conn, int employeeNumber, Employee* emp) {
	if (conn != nullptr) {
		try {
			if (findEmployee(conn, employeeNumber, emp)) {
				string extension;
				cout << "Update employee's extension: ";
				cin >> extension;
				cin.ignore(1000, '\n');
				cin.clear();
				Statement* statement = conn->createStatement();
				string msg = "";
				msg += "UPDATE employees SET extension = '" + extension + "' WHERE employeenumber = " + to_string(employeeNumber) ;
				
				statement->executeUpdate(msg);
				conn->commit();
				conn->terminateStatement(statement);
				cout << "Update successfully!" << endl;
			}
			else {
				cout << "Employee does not exists!";
			}
		}

		catch (SQLException& error) {
			cout << error.getErrorCode() << ": " << error.getMessage();
		}
	}
	else {
		cout << "connection not successful!" << endl;
	}
}

void deleteEmployee(Connection* conn, int employeeNumber, Employee* emp)
{
	if (conn != nullptr) {

		try {

			Employee* emp = new  Employee();
			if (findEmployee(conn, employeeNumber, emp)) {

				Statement* statement = conn->createStatement();
				string msg = "";
				msg += "DELETE FROM employees WHERE employeenumber = " + to_string(employeeNumber);
				statement->executeUpdate(msg);
				conn->commit();
				conn->terminateStatement(statement);
				cout << "Employee Successfully deleted!" << endl;

				delete emp;
				emp = nullptr;
			}
			else {
				cout << "Employee doesn't exist!" << endl;
			}
		}
		catch (SQLException& error) {
			cout << error.getErrorCode() << ": " << error.getMessage();
		}
	}
	else {
		cout << "connection not successful!" << endl;
	}

}


int main(void)
{

	try {//check any errors for the result of a connection failure

		Environment* env = nullptr; // reference variable to Oracle database
		Connection* conn = nullptr;

		string str;
		string usr = "dbs211_202g04";
		string pass = "28506104";
		string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

		env = Environment::createEnvironment(Environment::DEFAULT); //create environment
		conn = env->createConnection(usr, pass, srv); //create connection

		Employee* emp = new Employee();

		menu();// call the menu function to display options so that users know what options we have for the menu.
		int input = 0;
		do {
			cout << endl;
			cout << "Please enter a number > ";
			input = getOption(); // get input from the user by useing getOption function. input will be the menu option that users want to check.
			switch (input) { // by using switch case we are going to display option depends on inputs.
			case 0:
				cout << "BYE BYE:)" << endl; // when a user's input is 0 excute the programming by showing the message.
				break;
			case 1:
				displayEmployee(conn, emp); // when user's input is 1, it will show individual employee's information who is registered on this program. which user want to know about, by using displayEMployee function.
				break;
			case 2:
				displayAllEmployees(conn); //when user's input is 2 it will display all the employee's information who are registered on this program.
				break;
			case 3:
				cout << "Employee Number: ";
				cin >> emp->employeeNumber;
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Last Name: ";
				getline(cin, emp->lastName, '\n');
				cout << "First Name: ";
				getline(cin, emp->firstName, '\n');
				cout << "Email: ";
				getline(cin, emp->email, '\n');
				cout << "extension: ";
				getline(cin, emp->extension, '\n');
				cout << "Job Title: ";
				getline(cin, emp->jobTitle, '\n');
				cout << "City: ";
				getline(cin, emp->city, '\n');
				addEmployee(conn, emp);
				break;
			case 4:
				cout << "Employee Number: ";
				cin >> emp->employeeNumber;
				cin.clear();
				cin.ignore(1000, '\n');
				updateEmployee(conn, emp->employeeNumber, emp);
				break;
			case 5:
				cout << "Employee Number: ";
				int empNum;
				cin >> empNum;
				cin.clear();
				cin.ignore(1000, '\n');
				deleteEmployee(conn, empNum, emp);
				break;
				// menu options case 3,4,5 will be uploaded by following milestones.
			}

		} while (input != 0);//this program will get input over and over untill user type integer number 0. when input is 0 , the program will be executed.

		env->terminateConnection(conn);
		Environment::terminateEnvironment(env); //terminate connection
	}
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}

	return 0;
}
