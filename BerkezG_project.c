/*
*Name: Gleb Berkez
Class: CS2060
Instructor: Deb Harding

Purpose of Program:
Login to AirUCCS and rent out a home to a renter
Get user input for amount of nights, set the charges,
and come up with an owner's report for all the information gathered.
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Maximum length of a string
#define STRING_LENGTH 80

//Two dimensional array storage amounts for rows and columns of surve data
#define VACATION_RENTERS 5
#define RENTER_SURVEY_CATEGORIES 3

//Rental property login and sentinal values
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1

//rental property constant ranges
#define MIN_RENTAL_NIGHTS 1
#define MAX_RENTAL_NIGHTS 14
#define MIN_RATE 0
#define MAX_RATE 1000
#define DISCOUNT_MULTIPLIER 2



bool validateLogin(const char* correctLogin, int maxStringLength);
bool userLogin(const char* correctId, const char* correctPasscode, int strLength);

bool validInt(const char* inputStr, unsigned int* myInt, int minvalue, int maxValue);
void removeNewLine(int stringLength, char* inputString);

//struct for properties
typedef struct property {
	//Basic Variables
	char name[STRING_LENGTH];
	char location[STRING_LENGTH];

	unsigned int rate;
	unsigned int discount;
	unsigned int interval1;
	unsigned int interval2;

	//Totals
	unsigned int totalRenters;
	unsigned int totalNights;
	unsigned int totalCharges;

	//Ratings
	unsigned int checkInProcess;
	unsigned int cleanliness;
	unsigned int amenities;

	//Rating averages
	double checkInProcessAvg;
	double cleanlinessAvg;
	double amenitiesAvg;

	//2D array for survey
	unsigned int renterSurvey[RENTER_SURVEY_CATEGORIES][VACATION_RENTERS];

}Property;

void setUpProperty(Property* propertyPtr, const int strLength, int minRentalNights, int maxRentalNights, int minRate, int maxRate);
void vacationerRentalMode(int sentinalValue, Property* property, int minRentalNights, int maxRentalNights, int strLength, int maxNumRenters, int numCategories, int discountMultiplier);
void displayRentalPropertyInformation(Property* property, int minRentalNights, int maxRentalNights, int discountMultiplier, int vacationRenters, int renterSurveyCategories);

int main(void) {

	//declare Property structure 
	Property property1 = { {'\0'} };

	//Login owner
	puts("AirUCCS Rental Property Owner Login");
	bool userLoginResult = userLogin(CORRECT_ID, CORRECT_PASSCODE, STRING_LENGTH);
	if (userLoginResult == true) {
		

		//assings values by owner input to appropriate values in struct - name, location, rate, discount, and intervals.
		puts("Set up rental property information");
		setUpProperty(&property1, STRING_LENGTH, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, MIN_RATE, MAX_RATE);


		/*Vacationer rental mode : be able to read property ratings, rent a property and rate a property.
			The following repeats until the rental property owner enters sentinel value followed by correct login(correct id and code).
		*/
		vacationerRentalMode(SENTINAL_NEG1, &property1, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, STRING_LENGTH, VACATION_RENTERS, RENTER_SURVEY_CATEGORIES, DISCOUNT_MULTIPLIER);

		
		//printf("%s\n%s\n%d\n%d\n%d\n%d", property1.location, property1.name, property1.rate, property1.discount, property1.interval1, property1.interval2);
	}
	else {
		puts("Exiting AirUCCS");
	}

	//once rental property mode is finished, owner report will come next only after another valid login.
	userLoginResult = userLogin(CORRECT_ID, CORRECT_PASSCODE, STRING_LENGTH);
	if (userLoginResult == true) {

		//Display Property name, location, totals, and rating averages
		printf("Rental Property Report\nName: %s\nLocation: %s\n\n", property1.name, property1.location);
		printf("%15s%15s%15s\n", "Total Renters", "Total Nights", "Total Charges");
		printf("%15d%15d%15d\n\n", property1.totalRenters, property1.totalNights, property1.totalCharges);
		
		puts("Category Rating Averages\n");
		printf("Check-in Process: %1.1f\n", property1.checkInProcessAvg);
		printf("Cleanliness: %1.1f\n", property1.cleanlinessAvg);
		printf("Amenities: %1.1f\n\n", property1.amenitiesAvg);
	}
	
	puts("Exiting AirUCCS");
	


	return 0;

}

//Login function
//Take in the valid ID Password, and number of attempts and returns
//a boolean to proceed or exit the program
bool userLogin(const char* correctId, const char* correctPasscode, int strLength) {
	bool loginPassed = false;
	bool correctUserIdInput = false;
	bool correctUserPassInput = false;
	int loginAttempts = 0;

	

	//Receive user input for ID and validate
	puts("Enter your airUCCS Id: ");
	while ((correctUserIdInput == false) && (loginAttempts < LOGIN_MAX_ATTEMPTS)) {
		correctUserIdInput = validateLogin(correctId, strLength);
		if (correctUserIdInput == false) {
			loginAttempts++;
			puts("Incorrect Login ID");
		}
	}//end while loop

	if (correctUserIdInput == true) {
		puts("Enter your airUCCS passcode: ");
		while ((correctUserPassInput == false) && (loginAttempts < LOGIN_MAX_ATTEMPTS)) {
			correctUserPassInput = validateLogin(correctPasscode, strLength);
			if (correctUserPassInput == false) {
				loginAttempts++;
				puts("Incorrect Passcode");
			}

		}//end while loop
	}//if

	//if login success, change boolean to true
	if (correctUserIdInput == true && correctUserPassInput == true) {
		loginPassed = true;
	}//if

	return loginPassed;

}//userLogin


//validate ID and Password for login
bool validateLogin(const char* correctLogin, int strLength) {

	bool correctUserInput = false;
	char userLoginAttempt[STRING_LENGTH] = "";
	
	fgets(userLoginAttempt, strLength, stdin);

	removeNewLine(strLength, userLoginAttempt);
	

	//value 0 means strings are identical. Anything else is a failed login attempt.
	int stringCompareValue = strcmp(userLoginAttempt, correctLogin);

	if (stringCompareValue == 0) {
		correctUserInput = true;
	}
	return correctUserInput;
	

}// end validateLogin 

//function to set up property information by owner of property
void setUpProperty(Property* propertyPtr, const int strLength, int minRentalNights, int maxRentalNights, int minRate, int maxRate) {
	
	//User input for values of strings
	puts("Enter name of property:");
	fgets(propertyPtr->name, strLength, stdin);

	puts("Enter location of property:");
	fgets(propertyPtr->location, strLength, stdin);

	
	
	//user input string to pass into validation function. Gets reused each time.
	char inputStr[STRING_LENGTH] = { '\0' };

	
	/*Uses fgets for user input. Assigns user input to inputStr. Removes new line character. Passes the
	inputStr to validInt function and assigns a validated integer to int in parameter*/

	//validate nightly rate
	bool validBool = false;
	while (validBool == false) {

		puts("Enter nightly rate: ");
		fgets(inputStr, strLength, stdin);
		removeNewLine(strLength, inputStr);
		validBool = validInt(inputStr, &propertyPtr->rate, minRate, maxRate);
		
	}
	
	//validate discount rate
	validBool = false;
	while (validBool == false) {

		puts("Enter discount rate: ");
		fgets(inputStr, strLength, stdin);
		removeNewLine(strLength, inputStr);
		validBool = validInt(inputStr, &propertyPtr->discount, minRate, maxRate);
	}
	
	//validate interval 1
	validBool = false;
	while (!validBool) {

		puts("Enter interval 1: ");
		fgets(inputStr, strLength, stdin);
		removeNewLine(strLength, inputStr);
		validBool = validInt(inputStr, &propertyPtr->interval1, minRentalNights, maxRentalNights);
	}
	
	//validate interval 2
	validBool = false;
	while (validBool == false) {

		puts("Enter interval 2: ");
		fgets(inputStr, strLength, stdin);
		removeNewLine(strLength, inputStr);
		validBool = validInt(inputStr, &propertyPtr->interval2, minRentalNights, maxRentalNights);
	}
}
	
	
//validate integer by string to long function. Uses input string and assigns a validated integer to passed in int.
//Although it assigns the integer in parameter, validInt returns a bool.
bool validInt(const char* inputStr, unsigned int* myInt, int minValue, int maxValue) {
	bool myBool = false;
	char* end;
	long intTest = strtol(inputStr, &end, 10);

	
	//check if string
	if (end == inputStr) {
		fprintf(stderr, "%s: Not a decimal. Enter a decimal\n", inputStr);
	}

	//check for extra characters after an integer token (also checks if input has decimal)
	else if ('\0' != *end) {
		fprintf(stderr, "%s: extra characters at end of input: %s\n", inputStr, end);
	}

	//check for range
	else if (intTest < minValue || intTest > maxValue) {
		printf("Error. Must be greater than or equal to %d and less than or equal to %d\n", minValue, maxValue);
	}

	else {
		*myInt = (int)intTest;
		myBool = true;
	}
	return myBool;
}

	
//Removes new line from string from user input using fgets. USE ONLY AFTER FGETS for STRINGS
//Replaces '\n' with '\0'
void removeNewLine(int stringLength, char* inputString) {

	int countChar = 0;
	
	while ((countChar < stringLength) && (inputString[countChar] != '\n')) {
		countChar++;
	}
	inputString[countChar] = '\0';
}//removeNewLine




/*
	Vacationer rental mode : be able to read property ratings, rent a property and rate a property.
    The following repeats until the rental property owner enters sentinel value followed by correct login(correct id and code).
*/

void vacationerRentalMode(int sentinalValue, Property* property, int minRentalNights, int maxRentalNights, int strLength, int maxNumRenters, int numCategories, int discountMultiplier) {

	//initialize min and max rating scale, current number of renters in vacationer mode, and userInput amount of nights to rent from renters
	unsigned int minRating = 1;
	unsigned int maxRating = 5;
	unsigned int numRenter = 0;
	int userInputNumNights = 0;


	/*while number of renters is not over 5 and sentinalValue is not inputted,
	  display Rental Property Information, ask each new renter how many nights for the stay,
	  and get renter survey data
	*/
	bool boolFlag = false;
	while(boolFlag == false && numRenter < maxNumRenters){


		displayRentalPropertyInformation(property, minRentalNights, maxRentalNights, discountMultiplier, maxNumRenters, numCategories);

		//validate number of nights to be spent or sentinal value from user input
		char inputStr[STRING_LENGTH] = {'\0'};
		bool validBool = false;
		while (validBool == false) {

			puts("Enter the number of nights you want to stay: \n");
			fgets(inputStr, strLength, stdin);
			removeNewLine(strLength, inputStr);
			
			//use string to long function
			char* end;
			long intTest = strtol(inputStr, &end, 10);


			//check if string
			if (end == inputStr) {
				fprintf(stderr, "%s: Not a decimal. Enter a decimal\n", inputStr);
			}

			//check for extra characters after an integer token (also checks if input has decimal)
			else if ('\0' != *end) {
				fprintf(stderr, "%s: extra characters at end of input: %s\n", inputStr, end);
			}

			//check for sentinal value. Return true if true
			else if ((int)intTest == sentinalValue) {
				userInputNumNights = (int)intTest;
				validBool = true;
			}
			//check for range
			else if (intTest < minRentalNights || intTest > maxRentalNights) {
				puts("Error. You did not enter the number of nights correctly");
			}
			else {
				userInputNumNights = (int)intTest;
				validBool = true;

				//calculate charge based on discount rates and intervals
				int charge = 0;
				if (userInputNumNights <= property->interval1) {
					charge = userInputNumNights * property->rate;
				}
				else if (userInputNumNights > property->interval1 && userInputNumNights <= property->interval2) {
					charge = ((property->interval1) * property->rate) + ((userInputNumNights - property->interval1) * (property->rate - property->discount));
				}
				else {
					charge = ((property->interval1) * property->rate) + ((property->interval2 - property->interval1) * (property->rate - property->discount)) + ((userInputNumNights-1 - property->interval2) * (property->rate - (property->discount * discountMultiplier)));
				}

				//show user how many nights was picked and charge
				puts("Nights\tCharge");
				printf("%d\t%d\n\n", userInputNumNights, charge);
				
				//update total charges
				property->totalCharges += charge;
			}
			
		}//while checking for valid input
	

		//if sentinal value was placed, move onto owner report. Else, resume onto rating surveys for renter
		if (userInputNumNights == sentinalValue) {
			boolFlag = true;
		}
		else {

			numRenter++;
			//Ask the user to take our survey
			puts("We want to know how your experience was renting our property.");
			printf("Using the rating system %d to %d enter your rating for each category:\n", minRating, maxRating);
			puts("1: Check-in Process\n2: Cleanliness\n3: Amenities\n\n");


			//validate each user input for category ratings (1-5) using fgets
			for (unsigned int currentCategory = 0; currentCategory < numCategories; currentCategory++) {

				//validate rating and store it into survey inside property structure
				printf("Enter your rating for\nCategory%d: ", currentCategory + 1);
				fgets(inputStr, strLength, stdin);
				removeNewLine(strLength, &inputStr);
				validInt(inputStr, &property->renterSurvey[currentCategory][numRenter-1], minRating, maxRating);
			}


			//update rating averages and totals
			int sumTotalCheckin = 0;
			int sumTotalCleanliness = 0;
			int sumTotalAmenities = 0;

			for (unsigned int currentCategory = 0; currentCategory < numCategories; currentCategory++) {
				for (unsigned int currentRenter = 0; currentRenter < numRenter; currentRenter++) {
					//average for check-in process
					if (currentCategory == 0) {
						sumTotalCheckin += property->renterSurvey[currentCategory][currentRenter];
					}
					else if (currentCategory == 1) {
						sumTotalCleanliness += property->renterSurvey[currentCategory][currentRenter];
					}
					else {
						sumTotalAmenities += property->renterSurvey[currentCategory][currentRenter];
					}//if -else	
				}//for renters
			}//for categories

			//update averages in property structure
			property->checkInProcessAvg = (double)sumTotalCheckin / numRenter;
			property->cleanlinessAvg = (double)sumTotalCleanliness / numRenter;
			property->amenitiesAvg = (double)sumTotalAmenities / numRenter;

			//update totals
			property->totalNights += userInputNumNights;
			property->totalRenters += numRenter;
			

			//if number of renters is 5, escape to owner summary
			if (numRenter == 5) {
				boolFlag = true;
			}

		}//if boolflag else get user ratings
	}
}

//displays rental property information
void displayRentalPropertyInformation(Property* property, int minRentalNights, int maxRentalNights, int discountMultiplier, int vacationRenters, int renterSurveyCategories) {


	printf("\n\nRental Property: %sLocation: %sRental Property can be rented for %d to %d nights.\n", property->name, property->location, MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS);
	printf("%d rate a night for the first %d nights\n", property->rate, property->interval1);
	printf("%d discount rate a night for nights %d to %d\n", property->discount, property->interval1 + 1, property->interval2);
	printf("%d discount rate a night for each remaining night over %d\n", property->discount * DISCOUNT_MULTIPLIER, property->interval2);

	//if there are no ratings, say there are no ratings. Otherwise, show the 2d array of rantings
	if (property->renterSurvey[0][0] == 0) {
		puts("\nNo Ratings Currently");
	}
	//display ratings
	else {
		printf("Rating Categories: %30s%15s%15s", "Check - in Process", "Amenities", "CLeanliness\n");
		for (int renterNumber = 0; renterNumber < vacationRenters; renterNumber++) {
			printf("Survey %d\t", renterNumber+1);
			for (int categoryNumber = 0; categoryNumber < renterSurveyCategories; categoryNumber++) {
				printf("%d\t", property->renterSurvey[categoryNumber][renterNumber]);
			}
			puts("\n");
			
		}
	}
}







