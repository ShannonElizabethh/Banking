#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

struct AccountInfo
{
	char userName[50];
	char password[50];
	int userID;
	char fname[50];
	char lname[50];
	double accountBalance;

};

void NewAccount() {
	FILE* userFile;
	struct AccountInfo newUser;

	//open file to get last users ID#
	userFile = fopen("userAccounts.txt", "rb");

	int lastID = 0;
	while (fread(&newUser, sizeof(newUser), 1, userFile)) {
		if (userFile == NULL) {
			lastID = 0;
		}
		else {
			lastID = newUser.userID;
		}
	}

	//increase ID by 1
	lastID++;

	//close file
	fclose(userFile);

	//open file to add new user
	userFile = fopen("userAccounts.txt", "ab");

	//get user info
	printf("\nEnter User Name.\n");
	(void)scanf("%s", &newUser.userName);

	printf("\nEnter Password.\n");
	(void)scanf("%s", &newUser.password);

	printf("\nEnter First Name.\n");
	(void)scanf("%s", &newUser.fname);

	printf("\nEnter Last Name.\n");
	(void)scanf("%s", &newUser.lname);

	printf("\nEnter Amount to Deposit.\n");
	(void)scanf("%lf", &newUser.accountBalance);

	newUser.userID = lastID;

	//write to file
	fwrite(&newUser, sizeof(newUser), 1, userFile);

	//close file
	fclose(userFile);

}

int accountAccess(char inputName[], char inputPass[]) {
	FILE* userFile;
	struct AccountInfo newUser;

	//open file to read
	userFile = fopen("userAccounts.txt", "rb");


	//find correct user and print info
	char tempUsername[50];
	char tempPassword[50];
	int accountFound = 0;

	while (fread(&newUser, sizeof(newUser), 1, userFile)) {
		
		strcpy(tempUsername, newUser.userName);
		strcpy(tempPassword, newUser.password);

		if (strcmp(tempUsername, inputName) == 0 && strcmp(tempPassword, inputPass) == 0) {
			accountFound++;
		}

	}

	//if username & password not found or correct 
	printf("\n");
	if (accountFound == 0) {
		printf("Invalid Entry\n");
	 }

	return accountFound;

}

void accountInfo(char inputName[]) {
	FILE* userFile;
	struct AccountInfo user;
	char tempUsername[50];

	//open file to read
	userFile = fopen("userAccounts.txt", "rb");

	//printout account info
	while (fread(&user, sizeof(user), 1, userFile)) {

		//get username
		strcpy(tempUsername, user.userName);

		//if correct username print info
		if (strcmp(tempUsername, inputName) == 0) {
			printf("\nAccount Info:\n");
			printf("	UserName: %s\n	First Name: %s\n	Last Name: %s\n", user.userName, user.fname, user.lname);
			printf("	ID: %d\n	Balance: %lf\n", user.userID, user.accountBalance);
		}
	}

}

void updateInfo(char inputName[], double newBal) {
	FILE* userFile;
	FILE* updateFile;
	struct AccountInfo user;
	struct AccountInfo updatedUser;
	char tempUsername[50];

	//open file for writing
	updateFile = fopen("userUpdate.txt", "wb+");

	//open file to read
	userFile = fopen("userAccounts.txt", "rb");

	//put user info into updated file 
	while (fread(&user, sizeof(user), 1, userFile)) {

		//get user info
		strcpy(updatedUser.userName, user.userName);
		strcpy(updatedUser.password, user.password);
		strcpy(updatedUser.fname, user.fname);
		strcpy(updatedUser.lname, user.lname);
		updatedUser.accountBalance = user.accountBalance;
		updatedUser.userID = user.userID;

		if (strcmp(user.userName, inputName) == 0) {
			updatedUser.accountBalance = newBal;
		}

		fwrite(&updatedUser, sizeof(updatedUser), 1, updateFile);
	}

	//close files
	fclose(userFile);
	fclose(updateFile);

	//open file for writing
	userFile = fopen("userAccounts.txt", "wb+");
	
	//open file to read
	updateFile = fopen("userUpdate.txt", "rb");

	//copy updated file into user account file
	char filename[100], c;

	c = fgetc(updateFile);
	while (c != EOF) {
		fputc(c, userFile);
		c = fgetc(updateFile);
	}

	//close files
	fclose(userFile);
	fclose(updateFile);

}

void deposit(double depositAmount, char inputName[]) {
	FILE* userFile;
	struct AccountInfo user;
	char tempUsername[50];

	double newBalance = 0;

	//open file to read
	userFile = fopen("userAccounts.txt", "rb+");

	//printout account info
	while (fread(&user, sizeof(user), 1, userFile)) {

		//get username
		strcpy(tempUsername, user.userName);

		//if correct username print info
		if (strcmp(tempUsername, inputName) == 0) {
			printf("Balance: %lf\n", user.accountBalance);

			newBalance = user.accountBalance + depositAmount;
			printf("newBalance: %lf\n", newBalance);

			user.accountBalance = newBalance;

			updateInfo(inputName, newBalance);

		}
	}

	//updateInfo(inputName, newBalance);
	printf("\nAmount Deposited: %lf\n", depositAmount);

	//close file
	fclose(userFile);

}


void withdrawal(double withdrawalAmount, char inputName[]) {
	FILE* userFile;
	struct AccountInfo user;
	char tempUsername[50];

	double newBalance = 0;

	//open file to read
	userFile = fopen("userAccounts.txt", "rb+");

	//printout account info
	while (fread(&user, sizeof(user), 1, userFile)) {

		//get username
		strcpy(tempUsername, user.userName);

		//if correct username print info
		if (strcmp(tempUsername, inputName) == 0) {
			printf("Balance: %lf\n", user.accountBalance);

			newBalance = user.accountBalance - withdrawalAmount;
			printf("newBalance: %lf\n", newBalance);

			user.accountBalance = newBalance;

			updateInfo(inputName, newBalance);

		}
	}

	//updateInfo(inputName, newBalance);
	printf("\nAmount Withdrawn: %lf\n", withdrawalAmount);

	//close file
	fclose(userFile);
}


void main() {

	int userNum;
	char userName[50];
	char userPass[50];
	char newAccount;
	int accessGranted = 0;
	double depositAmount = 0;
	double withdrawalAmount = 0;

	//ask if user needs new account
	printf("\n**************************************************************\n\n");
	printf("Create New Acount?\n");
	printf("Enter Y for yes or N for no: ");
	scanf_s("%c", &newAccount);

	//if yes create new account
	if (newAccount == 'Y' || newAccount == 'y') {
		NewAccount();

	//else get username & password to login
	} else {
		
		//get username
		printf("\nEnter Username: ");
		(void)scanf("%s", &userName);
		printf("username: %s\n", userName);

		//get password
		printf("\nEnter Password: ");
		(void)scanf("%s", &userPass);
		printf("userpass: %s\n", userPass);

		//check username and password
		accessGranted = accountAccess(userName, userPass);

		//if username and password is not match exit
		if (accessGranted == 0) {
			exit(0);
		}

		while (1) {
			printf("\n**************************************************************\n\n");
		
			//ask user what they need to do
			printf("Get Account Info: 1\nMake Deposit: 2\nMake Withdrawal: 3\nExit: 4\n");
			printf("\nEnter Choice: ");
			scanf_s("%d", &userNum);
			printf("\n");

			switch (userNum) {
				case 1:
					//get account info
					accountInfo(userName);
					break;

				case 2:
					//make a deposit
					printf("\Enter Amount to Deposit: ");
					scanf_s("%lf", &depositAmount);
					deposit(depositAmount, userName);
					break;

				case 3:
					// Make a withdrawal
					printf("Enter Amount to Withdraw: ");
					scanf_s("%lf", &withdrawalAmount);
					withdrawal(withdrawalAmount, userName);
					break;

				case 4:
					//exit program
					printf("Goodbye\n");
					printf("\n**************************************************************\n\n");
					exit(0);
					break;

				default:
					printf("\nInvalid Entry");
					break;
			}
		}
	}

}