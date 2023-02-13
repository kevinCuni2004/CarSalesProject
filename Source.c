#define _CRT_SECURE_NO_WARNINGS
//No warnings on scanf

//Required libraries
#include <stdio.h>
//standard input ouput library enabling printf, scanf, files and more
#include <Windows.h>
//allows me to use system("cls") in order to clear the console log
#include <string.h>
//allows me to use strcmp to compare strings, strcat to join strings, strcpy to copy strings and much more
#include <errno.h>
//allows me to use error checking functions
#include <ctype.h>
//allows me to check for spaces and digits though a char array and also make chars lower or upper case and much more
#include <stdlib.h>
//this allows me to use atoi that converts chars into int
//this has a lot more functions to it but I am mostly including it for atoi
#include <stdbool.h>
//this allows me to create boolean variables where true = 1, and false = 0

//Constants
#define LEGAL_AGE 18
#define MAX_AGE 120
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 21
#define DISCOUNT_PERCENTAGE 0.15f
#define ARR_LEN 10
#define STRING_LEN 100
#define CSV_FILE "data.txt"
#define BUFF_SIZE 4096  //Indicating buffer size for integer validation

//Menu Options
#define MENU_OPTION_VIEW_STOCK 'a'
#define MENU_OPTION_BUY_CAR 'b'
#define MENU_OPTION_VIEW_SALES 'c'
#define MENU_OPTION_EXIT 'x'

//Variables
short carIndex = 0, stockNeeded = 0;
char userChoice, response;
bool intChecked = true, def = false, timer = true;
char line[201];
//External ints
extern int errno;
//Declaring file pointer
FILE* fp;

struct Customers {
	char name[50];
	short age;
	short firstCar;
	int price;
	int ogPrice;
	char model[STRING_LEN];
	short discountGiven;
	short discount;
	short amount;
};  //This struct is here to temporarily store the customer info

//Arrays
int carPrices[ARR_LEN];
char carModels[ARR_LEN][STRING_LEN];
int carStocks[ARR_LEN];
int carStocksSold[ARR_LEN];
int totalSales[ARR_LEN];

//Bool Function to check if user input is an number
bool checkINT(short* num, char* c) {//taking the short and a character as parameters
	int i = 0;//increment
	while (isspace(c[i])) i++;  //checking for white spaces and moving the increment past them

	int len = strlen(c);
	if (len == i) return false;  //if the whole string was just white spaces just return false

	char intBuff[BUFF_SIZE];//I am storing the short into this array of chars in order to navigate through every digit
	int intChars = 0;//this works as a pointer for intChars

	if (c[i] == '-') return false;  //check for a negative sign and return false if found since no negativ input is required

	while (i < len && !isspace(c[i])) {//while it hasn't reached the end of the string and the character at i is not a space we check for numerical digits
		if (!isdigit(c[i])) return false;//return false if no digits

		intBuff[intChars] = c[i]; //if the while conditions are fulfilled and c[i] is not a numerical digit it stores c[i] into intBuff
		intChars++;//increment the intBuff pointer
		i++;//increment the string pointer
	}

	intBuff[intChars] = '\0';//putting a null terminator to make it a proper int

	while (isspace(c[i])) i++;  //checking for white spaces one more time as there may be some after digits or chars

	if (c[i] != '\0') return false;  //if the end of c has not been reached return false and terminate
	//if the function has reached this point and it hasn't reached the end of c this is not a short

	*num = atoi(intBuff);//convert intBuff into an short if no characters were found

	return true;//return true if no characters were found
}

//function to do get the user input and send it to checkINT for validation
//i had to put this in a function because menuOptionBuyCars() had a stack overflow
int checkINTiter() {
	short data;
	char buff[BUFF_SIZE];
	fgets(buff, BUFF_SIZE, stdin); //get user input
	intChecked = checkINT(&data, buff); //call checkINT to validate

	if (!intChecked) {
		printf(" Please enter a valid choice: ");
		return checkINTiter();  //if intchecked is false return the function again(recursive function)
	}
	else return data; //else return the data to menuOptionBuyCars
}

void arrDeclar() {
	carPrices[0] = 32250;
	carPrices[1] = 33775;
	carPrices[2] = 42860;
	carPrices[3] = 44260;
	carPrices[4] = 49400;
	carPrices[5] = 53770;
	carPrices[6] = 57225;
	carPrices[7] = 65115;
	carPrices[8] = 86165;
	carPrices[9] = 96565;
	//car prices based on real values

	strcpy(carModels[0], "Mercedes Benz A180");
	strcpy(carModels[1], "Mercedes Benz B200");
	strcpy(carModels[2], "Mercedes Benz C200");
	strcpy(carModels[3], "Mercedes Benz C220");
	strcpy(carModels[4], "Mercedes Benz C300");
	strcpy(carModels[5], "Mercedes Benz E220");
	strcpy(carModels[6], "Mercedes Benz E300");
	strcpy(carModels[7], "Mercedes Benz E450");
	strcpy(carModels[8], "Mercedes Benz S350");
	strcpy(carModels[9], "Mercedes Benz S500");
	//model types

	carStocks[0] = 12;
	carStocks[1] = 11;
	carStocks[2] = 6;
	carStocks[3] = 9;
	carStocks[4] = 7;
	carStocks[5] = 6;
	carStocks[6] = 4;
	carStocks[7] = 3;
	carStocks[8] = 4;
	carStocks[9] = 2;
	//initial stocks

	for (int j = 0; j < ARR_LEN; j++) carStocksSold[j] = 0;  //array to store sold stocks

	for (int i = 0; i < ARR_LEN; i++) totalSales[i] = 0;  //array to store total sales
}//The arrays are declared here so as to have more organization

void clrsys() {
	system("cls");  //Clear System
}//clearing the system can do more due to being in a function

char* conjoinData(char name[50], unsigned short age, short firstCar, int price, int ogPrice,
	char model[STRING_LEN], short discountGiven, short discount, short amount) {

	char data[50];//temporary string to convert ints and shorts into stirngs

	strcpy(line, name);//name is copied into line as line is empty at this point
	strcat(line, ",");//adding , to separate

	_itoa(age, data, 10);//convert short to string
	strcat(line, data);//adding age in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	_itoa(firstCar, data, 2);//convert short to string
	strcat(line, data);//adding firstCar in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	_itoa(price, data, 10);//convert int to string
	strcat(line, data);//adding price in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	_itoa(ogPrice, data, 10);//convert int to string
	strcat(line, data);//adding ogPrice in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	_itoa(discountGiven, data, 2);//convert short to string
	strcat(line, data);//adding discountGiven in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	_itoa(discount, data, 10);//convert short to string
	strcat(line, data);//adding discountValue in the form of data(string) into line
	strcat(line, ",");//adding , to separate

	strcat(line, model);//adding model into line
	strcat(line, ",");//adding , to separate

	_itoa(amount, data, 10);//convert short to string
	strcat(line, data);//adding age in the form of data(string) into line

	return line;//returning line into writeData function
}

void writeData(char data[201]) {
	int errorNumber;

	fp = fopen(CSV_FILE, "a+");  //open or create file

	if (fp == NULL) {
		errorNumber = errno;
		printf(" Error!\n ErrNo = %d", errorNumber);
		perror("\n Error Message:\n");
		printf(" Error opening %s, %s.", CSV_FILE, strerror(errorNumber));
	}  //checking and displaying any errors on opening the file
	else {
		fprintf(fp, "%s", data);
		fprintf(fp, "\n");
		fclose(fp);
	}
}  //write data into file

void reader() {  //Checking for previous sales
	int errorNumber;

	fp = fopen(CSV_FILE, "r");  //open or create file

	if (fp == NULL) {
		errorNumber = errno;
		printf(" Error!\n ErrNo = %d", errorNumber);
		perror("\n Error Message:\n");
		printf(" Error opening %s, %s.", CSV_FILE, strerror(errorNumber));
	}//checking and displaying any errors on opening the file
	else {
		do {
			struct Customers newCustomer;
			int scanResult = fscanf(fp, "%49[^,],%hd,%hd,%d,%d,%hd,%hd,%99[^,],%hd\n",
				newCustomer.name, &newCustomer.age, &newCustomer.firstCar,
				&newCustomer.price, &newCustomer.ogPrice, &newCustomer.discountGiven,
				&newCustomer.discount, newCustomer.model, &newCustomer.amount);  //read data

			for (int i = 0; i < ARR_LEN; i++) {
				if (strcmp(newCustomer.model, carModels[i]) == 0) {
					carStocks[i] -= newCustomer.amount;
					carStocksSold[i] += newCustomer.amount;
					totalSales[i] += newCustomer.price;
				} //update the arrays accordingly by comparing the model names
			}
		} while (!feof(fp));
		fclose(fp);
	}
}

void sorter() {
	int a;
	int b;
	char c[100];
	int d;
	int e;
	//These vars are temprary placeholders for bubble sorting the arrays.
	for (int i = 0; i < ARR_LEN - 1; i++) {
		for (int j = i + 1; j < ARR_LEN; j++) {
			if (carStocks[i] < carStocks[j]) {
				a = carStocks[i];
				carStocks[i] = carStocks[j];
				carStocks[j] = a;
				//Sorting of car stocks

				b = carPrices[i];
				carPrices[i] = carPrices[j];
				carPrices[j] = b;
				//Sorting of car prices

				strcpy(c, carModels[i]);
				strcpy(carModels[i], carModels[j]);
				strcpy(carModels[j], c);
				//Sorting of car models

				e = carStocksSold[i];
				carStocksSold[i] = carStocksSold[j];
				carStocksSold[j] = e;
				//Sorting of car stocks sold

				d = totalSales[i];
				totalSales[i] = totalSales[j];
				totalSales[j] = d;
				//Sorting of car stocks sold
			}
		}
	}
}

void salesSorter() {
	int a;
	int b;
	char c[100];
	int d;
	//These vars are temprary placeholders for bubble sorting the arrays.
	for (int i = 0; i < ARR_LEN - 1; i++) {
		for (int j = i + 1; j < ARR_LEN; j++) {
			if (totalSales[i] < totalSales[j]) {
				a = carStocks[i];
				carStocks[i] = carStocks[j];
				carStocks[j] = a;
				//Sorting of car stocks

				strcpy(c, carModels[i]);
				strcpy(carModels[i], carModels[j]);
				strcpy(carModels[j], c);
				//Sorting of car models

				b = carStocksSold[i];
				carStocksSold[i] = carStocksSold[j];
				carStocksSold[j] = b;
				//Sorting of car stocks sold

				d = totalSales[i];
				totalSales[i] = totalSales[j];
				totalSales[j] = d;
				//Sorting of car stocks sold
			}
		}
	}
}


void menu(bool def) {//bool def is the boolean that tells the menu if the switch defaulted
	clrsys();//clear the system
	sorter();//sort and update the arrays if a purchase was made
	printf("  Hello and Welcome to Car Sales Inc.\n");
	printf("  We hope you are well today!\n\n");
	printf("  Menu:\n");
	printf("  a. View Car Stocks\n");
	printf("  b. Buy Cars\n");
	printf("  c. View Sales Data\n");
	printf("  x. Exit\n");
	if (def) {
		printf("\n  Please enter a valid option: ");
	}//ask the user for a valid option if the switch has defaulted
	else {
		printf("\n  Please choose an option: ");
	}//else just display the normal text
	//Menu
}  //organized look

void menuOptionViewStocks() {
	clrsys();
	printf("\n");
	printf("  ID        Model             Price     Stock Remaining\n");
	for (int k = 0; k < ARR_LEN; k++) {
		printf(" --------------------------------------------------------\n");
		printf("   %d  ", k);
		printf(" %s ", carModels[k]);
		printf("   %d GBP ", carPrices[k]);
		printf("     %d Cars  \n", carStocks[k]);
		printf(" --------------------------------------------------------\n");
	}
	//outputs the car stocks
}

int menuOptionBuyCars() {
	struct Customers newCustomer;  //struct to write values into file
	clrsys();
	menuOptionViewStocks();  //allows the user to view stocks
	getchar();
	printf("  Choose the desired car by pressing one of the numbers: ");
	//carIndex is an index used to navigate through arrays
	carIndex = checkINTiter();//check input
	while (carIndex < 0 || carIndex > 9) {
		printf(" Please enter a valid choice: ");//check requirements
		carIndex = checkINTiter();
	}
	intChecked = true;

	if (carStocks[carIndex] == 0) {
		printf(" We are sorry this car is out of stock.\n");
		return 0;
	}//checking if there is no stock

	clrsys();
	fflush(stdin);
	printf(" This car costs %d GBP. How many do you need? ", carPrices[carIndex]);
	newCustomer.amount = checkINTiter();;//check input
	while (newCustomer.amount < 0) {
		printf(" Please enter a valid amount: ");
		carIndex = checkINTiter();//check requirements
	}
	intChecked = true;
	clrsys();
	bool test1 = false;
	while (newCustomer.amount > carStocks[carIndex]) {
		if (!test1) {
			printf(" We are sorry, we do not possess the required amount of %s.\n", carModels[carIndex]);  //checking if there is enough stock
			printf(" Do you wish to purchase less? Answer 'y' or 'n': ");
		}
		scanf(" %c", &response);
		if (response == 'y') {
			clrsys();
			printf(" How many do you need? ");
			intChecked = true;
			getchar();
			newCustomer.amount = checkINTiter();;//check input
			while (newCustomer.amount < 0) {
				printf(" Please enter a valid amount: ");
				carIndex = checkINTiter();//check requirements
			}
			intChecked = true;
		}
		else if (response == 'n') {
			return 0;
		} //terminate function if user decides to not buy a different stock
		else {
			printf(" Please enter 'y' or 'n': ");
			test1 = true;
		}
	}
	fflush(stdin);//clear buffer
	clrsys();
	printf(" Please enter your full name: ");
	while (1) {
		bool test = true; //boolean to break while
		fflush(stdin);
		fgets(newCustomer.name, 50, stdin);//get user input
		if (newCustomer.name[0] == '\n') test = false;
		for (unsigned int i = 0; i < strlen(newCustomer.name); i++) {
			if (!isalpha(newCustomer.name[i]) && newCustomer.name[i] != '\n' && newCustomer.name[i] != ' ') { //if the char is not a letter and not a newline character and not a space, change test to false to indicate it is not a string, break for loop
				test = false;
				break;
			}
			if (newCustomer.name[i] == '\n' && i != 0) { //if newline char is reached terminate the string with a NULL terminator in order to remove the white spaces that get added by fgets
				newCustomer.name[i] = '\0';
			}
		}
		if (test) {
			break; //break if test was never changed to false
		}
		else {
			printf(" Please enter a valid name: ");
		}
	}
	/* NOTE:
		This while loop that validates the name input is not entirely my work
		I knew how to check for letters using isalpha() function from <ctype.h> library
		However I had an issue where anything inputed after a space would be ignored
		I managed to solve that using fgets, however the name value in the data file would always be 49 chars long
		with white spaces after the name
		A friend of mine studying in the same course at the same uni as me was using a similar approach to mine and was running into the same issue
		They told me how they removed the white spaces after the name and the second if clause in the while loop was created by taking
		inspiration from their code
	*/

	clrsys();
	printf(" How old are you?  ");
	newCustomer.age = checkINTiter();;//check input
	while (newCustomer.age <= 0 || newCustomer.age >= MAX_AGE) {
		printf(" Please enter a valid age. How old are you? ");
		newCustomer.age = checkINTiter();//check requirements
	}
	intChecked = true;

	if (newCustomer.age < LEGAL_AGE) {
		printf(" You are too young to legally purchase a car. We are sorry.");
		return 0;
	}//checking if the user is of legal age

	newCustomer.price = carPrices[carIndex] * newCustomer.amount;
	newCustomer.discountGiven = false;
	newCustomer.firstCar = false;

	if (newCustomer.amount == 1) {
		if (newCustomer.age >= DISCOUNT_MIN_AGE && newCustomer.age <= DISCOUNT_MAX_AGE) {
			clrsys();
			printf(" Is this your first car? Answer 'y' or 'n':  ");
			while (timer) {
				scanf(" %c", &response);
				if (response == 'y') {
					newCustomer.discountGiven = true;
					newCustomer.firstCar = true;
					printf("\n You are eligible for a discount.\n");
					timer = false;
				}
				else if (response == 'n') {
					printf(" Sorry you are not eliglibe for a discount.");
					timer = false;
				}
				else {
					clrsys();
					printf(" Please enter a valid answer. Is this your first car? Answer 'y'or 'n': ");
				}//user input filtering
			}
		}
		else {
			printf(" Sorry you are not eliglibe for a discount.");
		}//checking for discount eligibility
	}

	if (newCustomer.discountGiven) {
		newCustomer.price *= (1 - DISCOUNT_PERCENTAGE);//calculate new price
		newCustomer.ogPrice = carPrices[carIndex];//register original price
		newCustomer.discount = newCustomer.ogPrice - newCustomer.price;//calculate discount value
	}
	else {
		newCustomer.ogPrice = carPrices[carIndex];
		newCustomer.discount = 0;
	}

	printf("\n\n Thank You!\n");
	printf(" You have bought %hd %s cars.\n", newCustomer.amount, carModels[carIndex]);
	printf(" Total cost is %d GBP.", newCustomer.price);

	strcpy(newCustomer.model, carModels[carIndex]);
	carStocksSold[carIndex] += newCustomer.amount;
	totalSales[carIndex] += newCustomer.price;
	carStocks[carIndex] -= newCustomer.amount;

	char data[201];

	strcpy(data, conjoinData(newCustomer.name, newCustomer.age, newCustomer.firstCar, newCustomer.price,
		newCustomer.ogPrice, newCustomer.model, newCustomer.discountGiven, newCustomer.discount, newCustomer.amount));

	writeData(data);

	return 0;
}

void menuOptionViewSales() {
	clrsys();//clear system
	int errorNumber;

	fp = fopen(CSV_FILE, "r");
	//open or create file

	if (fp == NULL) {
		errorNumber = errno;
		printf(" Error!\n ErrNo = %d", errorNumber);
		perror("\n Error Message:\n");
		printf(" Error opening %s, %s.", CSV_FILE, strerror(errorNumber));
	}//checking and displaying any errors on opening the file
	else {
		printf("\n                      Individual Client Sales\n");
		printf("---------------------------------------------------------------------------\n");
		do {
			struct Customers newCustomer;
			int scanResult = fscanf(fp, "%49[^,],%hd,%hd,%d,%d,%hd,%hd,%99[^,],%hd\n",
				newCustomer.name, &newCustomer.age, &newCustomer.firstCar,
				&newCustomer.price, &newCustomer.ogPrice, &newCustomer.discountGiven,
				&newCustomer.discount, newCustomer.model, &newCustomer.amount); //read and display everytime the do while loops in order to avoid struct arrays

			printf("---------------------------------------------------------------------------\n");
			printf("      Name: %s\n", newCustomer.name);
			printf("      Age: %hd\n", newCustomer.age);
			if (newCustomer.firstCar == 0) {
				printf("      Not First Car\n");
			}
			else if (newCustomer.firstCar == 1) {
				printf("      First Car\n");
			}
			else {
				printf("      N/A\n.");
			}

			if (newCustomer.discountGiven == 1) {
				printf("      Discount was given\n");
				printf("      Original Price: %d\n", newCustomer.ogPrice);
				printf("      Discount: %hd\n", newCustomer.discount);
				printf("      Discounted Price: %d\n", newCustomer.price);
			}
			else if (newCustomer.discountGiven == 0) {
				printf("      No discount was given\n");
				printf("      Price: %d\n", newCustomer.ogPrice);
				printf("      Total Price: %d\n", newCustomer.price);
			}
			else {
				printf("      N/A\n");
				printf("      Price: %d\n", newCustomer.ogPrice);
				printf("      Total Price: %d\n", newCustomer.price);
			}
			printf("      Model: %s\n", newCustomer.model);
			printf("      Amount Purchased: %hd\n", newCustomer.amount);
			printf("---------------------------------------------------------------------------\n");
		} while (!feof(fp));
		fclose(fp);
		salesSorter();
		printf("\n\n------------------------------------------------------------------------------------------\n");
		printf("                           Car Sales\n");
		printf("------------------------------------------------------------------------------------------\n");
		printf("  ID        Model             Stock Sold    Stock Remaining       Total in Revenue\n");
		for (int i = 0; i < ARR_LEN; i++) {
			printf(" ------------------------------------------------------------------------------------------\n");
			printf("   %d  ", i);
			printf(" %s ", carModels[i]);
			printf("   %d cars sold ", carStocksSold[i]);
			printf("     %d cars left  ", carStocks[i]);
			printf("     %d total in revenue \n", totalSales[i]);
			printf(" ------------------------------------------------------------------------------------------\n");
		}
	}
}


int main(void) {
	arrDeclar();//declare arrays
	reader();//read the data file for sales
	sorter();//sort the arrays
	do {
		menu(def);//show menu
		def = false;//def is false so as not to trigger the menu validation without needing to
		userChoice = getchar();
		//Get user input
		switch (userChoice) {//switch through menu options
		case MENU_OPTION_VIEW_STOCK:
			menuOptionViewStocks();
			break;
		case MENU_OPTION_BUY_CAR:
			menuOptionBuyCars();
			break;
		case MENU_OPTION_VIEW_SALES:
			menuOptionViewSales();
			break;
		case MENU_OPTION_EXIT:
			printf(" Thank you for choosing Car Sales Inc..\n We welome you again. Good Bye!");//Thanking the user
			break;
		default: def = true;//if the switch has defaulted make def true
		}

		if (userChoice == MENU_OPTION_EXIT) {
			printf("\n\n Press Enter to exit...");//if the user wants to exit print this message to tell the user how to
		}
		else {
			if (!def) {//if the switch hasn't defaulted print this, if the switch has defaulted this makes no sense
				printf("\n\n Press Enter to return to the Menu...");
			}
		}

		if (!def) {//since this is used to get the enter input by the user only use them when the switch hasn't defaulted
			getchar();
			getchar();
		}

	} while (userChoice != MENU_OPTION_EXIT);

	clrsys();
	printf("\n\n Have a good day!\n\n");//greeting the user
	return 0;
}
