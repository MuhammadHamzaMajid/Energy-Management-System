// Problem 1
/*Pseudocode
Function informative_statement(location, category, cost_per_unit):
	Print locality based on location (Rural or Urban), usage category, and cost per unit.

Main function:
	Declare variables: location, monthly_consumption, cost, bill, name, house_no, email, phone_no, meter_no, and strings l, m, h.
	Print introduction message.

	Prompt the user to select their location (Rural or Urban).
		Validate the input to ensure it's either Rural (0) or Urban (1).

	Gather user information: name, house_no, email, phone_no, meter_no, and monthly_consumption.

	Determine the cost per unit based on location and monthly_consumption.
		If location is Rural:
			Calculate cost based on predefined ranges for Rural.
		If location is Urban:
			Calculate cost based on predefined ranges for Urban.

	Calculate the total bill by multiplying monthly_consumption and cost.

	Print the total electricity bill.

	Open a file "Monthly Bill.txt" for appending.

	Write the user information and bill details to the file.

	Close the file.

	Print a success message.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

//Function for printing bill information
void informative_statement(int l, char s[], float b) {
	if (l == 0)
		printf("\n\nYour locality: Rural\nYour usage category: %s\nApplied Cost per unit: %.2f pounds\n\n", s, b);
	if (l == 1)
		printf("\n\nYour locality: Urban\nYour usage category: %s\nApplied Cost per unit: %.2f pounds\n\n", s, b);
}
int main() {
	//Declaing variables
	int location = 0;
	int monthly_consumption = 0; float cost = 0, bill = 0;
	char name[50], house_no[10], email[50], phone_no[15], meter_no[15],
		l[] = "LOW", m[] = "MEDIUM", h[] = "HIGH";

	printf("This is a program designed to calculate your electricity bill\n\n");

	//Location selection menu
	printf("Please select your location from below.\n");
	printf("(Enter 0 for rural area, 1 for urban area): ");
	scanf(" %d", &location);
	//Loop to encounter input errors
	while (location != 0 && location != 1) {
		printf("Please enter correct location i.e 0 or 1: ");
		scanf(" %d", &location);
	}

	//Information gathering
	printf("\n\nEnter your name(without spaces): ");
	scanf(" %s", &name);
	printf("\nEnter your house number: ");
	scanf(" %s", &house_no);
	printf("\nEnter your email address: ");
	scanf(" %s", &email);
	printf("\nEnter your phone number(without spaces): ");
	scanf(" %s", &phone_no);
	printf("\nEnter your meter number: ");
	scanf(" %s", &meter_no);
	printf("\nNow enter the energy consumed per month(in KW, in whole numbers): ");
	scanf(" %d", &monthly_consumption);

	//Determining the amount of bill using given parameters
	if (location == 0) {
		if (monthly_consumption >= 0 && monthly_consumption < 51) {
			cost = 2;
			informative_statement(location, l, cost);
		}
		if (monthly_consumption >= 51 && monthly_consumption < 101) {
			cost = 2.75;
			informative_statement(location, m, cost);
		}
		if (monthly_consumption >= 101) {
			cost = 4.15;
			informative_statement(location, h, cost);
		}
	}
	if (location == 1) {
		if (monthly_consumption >= 0 && monthly_consumption < 51) {
			cost = 3;
			informative_statement(location, l, cost);
		}
		if (monthly_consumption >= 51 && monthly_consumption < 101) {
			cost = 3.75;
			informative_statement(location, m, cost);
		}
		if (monthly_consumption >= 101) {
			cost = 4.50;
			informative_statement(location, h, cost);
		}
	}
	bill = monthly_consumption * cost;
	printf("Your total electricty bill for the month is : %.2f pounds\n\n", bill);


	//Creating a file for storing information
	FILE* fp = fopen("Monthly Bill.txt", "w");

	fprintf(fp, "\n    MONTHLY BILL   \n");
	fprintf(fp, "Username:\t%s\n", name);
	fprintf(fp, "House Number:\t%s\n", house_no);
	fprintf(fp, "Email address:\t%s\n", email);
	fprintf(fp, "Phone number:\t%s\n", phone_no);
	fprintf(fp, "Meter number:\t%s\n", meter_no);
	fprintf(fp, "Units consumed:\t%d\n", monthly_consumption);
	fprintf(fp, "Cost applied per unit:\t%.2f\n", cost);
	fprintf(fp, "BILL FOR THE MONTH = %.2f Pounds\n\n", bill);
	fclose(fp);
	printf("The data is succesfully recorded in 'Monthly Bill.txt'\n.");
}
