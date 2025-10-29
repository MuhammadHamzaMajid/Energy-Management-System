/*
1. Start
2. Define a structure named Room with the following attributes:
	 - TimeDat: a 2D array to store time data
	 - TempArr: a 2D array to store temperature data
	 - HumiArr: an array to store humidity data
	 - MotionArr: a 2D array to store motion sensor data
	 - TempDeviceState: an array to store temperature device state
	 - HumiDeviceState: an array to store humidity device state
	 - MotionDeviceState: a 2D array to store motion device state
	 - TempDeviceHours, HumiDeviceHours, MotionDeviceHours: variables to store device usage hours
	 - TempConsumption, HumiConsumption, MotionConsumption, TotalConsumption: variables to store energy consumption
3. Define a function named ReadDataFile that reads data from files into the Room struct:
	 - Open the file specified by the location parameter
	 - Read data from the file and store it in the Room struct attributes
	 - Close the file
4. Define a function named setDeviceStates to set device states based on thresholds:
	 - Iterate over each hour in the day:
		 - Set the temperature device state based on the temperature thresholds
		 - Set the humidity device state based on the humidity thresholds
		 - Set the motion device state based on the motion sensor threshold
5. Define a function named getRoomEnergy to calculate energy consumption per day per room:
	 - Iterate over each hour in the day:
		 - Calculate device usage hours and energy consumption for temperature and humidity sensors
		 - Calculate device usage hours and energy consumption for motion sensors
		 - Calculate total energy consumption
6. In the main function:
	 - Prompt the user to set thresholds for temperature, humidity, and motion sensors
	 - Read data from files for three rooms (LivingRoom, Kitchen, Bedroom) using the ReadDataFile function
	 - Set device states for each room using the setDeviceStates function
	 - Calculate energy consumption for each room using the getRoomEnergy function
	 - Print the results for each room (device hours, energy consumption)
	 - Write the results to a file named "Energy.txt"
7. End
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//Create a structure Room
struct Room {
	char TimeDat[24][25];
	float TempArr[24][2];
	float HumiArr[24];
	float MotionArr[24][6];
	int TempDeviceState[24] = { 0 };
	int HumiDeviceState[24] = { 0 };
	int MotionDeviceState[24][6] = { 0 };
	int TempDeviceHours = 0, HumiDeviceHours = 0; float MotionDeviceHours = 0;
	int TempConsumption = 0, HumiConsumption = 0; float MotionConsumption = 0, TotalConsumption = 0;

};

//Function to Read data from files into the Room struct
struct Room ReadDataFile(char location[]) {
	struct Room r;
	char TimeDat[24][25] = {};
	float TempArr[24][2] = {};
	float HumiArr[24] = {};
	float MotionArr[24][6] = {};
	int t;
	FILE* fp; // assign to the file location
	fp = fopen(location, "r");
	if (fp == NULL)
	{
		printf("Error!\n");
		exit(1);
	}
	for (t = 0; (t < 24 && !feof(fp)); t++)
	{
		fscanf(fp, "%s", TimeDat[t]); // 00:42:52 TimeDat[0] == 00:42 : 52
		//read temperatures
		fscanf(fp, "%f%f", &TempArr[t][0], &TempArr[t][1]);
		r.TempArr[t][0] = TempArr[t][0];
		r.TempArr[t][1] = TempArr[t][1];
		//read humidity
		fscanf(fp, "%f", &HumiArr[t]);
		r.HumiArr[t] = HumiArr[t];
		//read motion data
		for (int k = 0; k < 6; k++) {
			fscanf(fp, "%f", &MotionArr[t][k]);
			r.MotionArr[t][k] = MotionArr[t][k];
		}
	}
	fclose(fp);

	//Loop for timedata loading, as time here is a 2D array, so we load it in a separate loop
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 20; j++) {
			r.TimeDat[i][j] = TimeDat[i][j];
			if (TimeDat[i][j] == '\0')
				break;
		}
	}
	return r;
}
struct Room setDeviceStates(struct Room r, int lt, int ht, int lh, int hh, int lm) {
	//Loop for Setting the Temperature Device State

	for (int i = 0; i < 24; i++) {
		if (r.TempArr[i][1] < lt || r.TempArr[i][1] > ht)
			r.TempDeviceState[i] = 1;
	}

	//Loop for setting the Humidity Device State
	for (int i = 0; i < 24; i++) {
		if (r.HumiArr[i] < lh || r.HumiArr[i] > hh)
			r.HumiDeviceState[i] = 1;
	}

	//Loop for setting the Motion Device State, if distance > 10 m, device = on
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 6; j++) {
			if (r.MotionArr[i][j] < lm)
				r.MotionDeviceState[i][j] = 1;
		}
	}
	return r;

}
//Function for calculatng the energy consumed er day per room
struct Room getRoomEnergy(struct Room r) {
	int mins_for_motion = 0;
	for (int i = 0; i < 24; i++) {
		if (r.TempDeviceState[i] == 1)
			r.TempDeviceHours++;
		if (r.HumiDeviceState[i] == 1) {
			r.HumiDeviceHours++;
		}
		for (int j = 0; j < 6; j++) {
			if(r.MotionDeviceState[i][j] == 1)
				mins_for_motion++;
		}
	}
	r.MotionDeviceHours = mins_for_motion / 6;

	r.TempConsumption = r.TempDeviceHours * 150;
	r.HumiConsumption = r.HumiDeviceHours * 125;
	r.MotionConsumption = r.MotionDeviceHours * 100;
	r.TotalConsumption = r.TempConsumption + r.HumiConsumption + r.MotionConsumption;
	return r;
}

int main() {
	printf("This program is designed to calculate your energy consumption.\n\n\n\n\n");
	//Getting thresholds for turning the device ON
	int lt, ht, lh, hh, lm;
	printf("Set a lower threshold(whole number) of internal temperature for the regulator to turn ON: ");
	scanf(" %d", &lt);
	printf("Set a upper threshold(whole number) of internal temperature for the regulator to turn ON: ");
	scanf(" %d", &ht);

	printf("Set a lower threshold(whole number) of humidity for the regulator to turn ON: ");
	scanf(" %d", &lh);
	printf("Set a upper threshold(whole number) of humidity for the regulator to turn ON: ");
	scanf(" %d", &hh);

	printf("Set the limit of the motion sensor for turning ON lights: ");
	scanf(" %d", &lm);
	time_t t;
	time(&t);
	char living_room[] = "LivingRoom.txt", kitchen[] = "Kitchen.txt", bedroom[] = "Bedroom.txt";
	struct Room LivingRoom = ReadDataFile(living_room);
	struct Room Kitchen = ReadDataFile(kitchen);
	struct Room Bedroom = ReadDataFile(bedroom);
	LivingRoom = setDeviceStates(LivingRoom, lt, ht, lh, hh, lm); LivingRoom = getRoomEnergy(LivingRoom);
	Kitchen = setDeviceStates(Kitchen, lt, ht, lh, hh, lm); Kitchen = getRoomEnergy(Kitchen);
	Bedroom = setDeviceStates(Bedroom, lt, ht, lh, hh, lm); Bedroom = getRoomEnergy(Bedroom);
	float total_energy = LivingRoom.TotalConsumption + Kitchen.TotalConsumption + Bedroom.TotalConsumption;
	printf("On today's date:\n%s\n\n\n", ctime(&t));
	printf("In the Living Room:\n\n");
	printf("The Temperature Sensor was turned on for:%d hours.\n", LivingRoom.TempDeviceHours);
	printf("Energy consumed by Temperature Sensor:\t %d KJ\n\n", LivingRoom.TempConsumption);
	printf("The Humidity Sensor was turned on for:\t %d hours.\n", LivingRoom.HumiDeviceHours);
	printf("Energy consumed by Humidity Sensor:\t %d KJ\n\n", LivingRoom.HumiConsumption);
	printf("The Motion Sensor was turned on for:\t %.1f hours.\n", LivingRoom.MotionDeviceHours);
	printf("Energy consumed by Motion Sensor:\t %.2f KJ\n\n", LivingRoom.MotionConsumption);
	printf("Total Energy Consumption in the Living Room:\t %.2f KJ\n\n\n", LivingRoom.TotalConsumption);

	printf("In the BedRoom:\n\n");
	printf("The Temperature Sensor was turned on for:%d hours.\n", Bedroom.TempDeviceHours);
	printf("Energy consumed by Temperature Sensor:\t %d KJ\n\n", Bedroom.TempConsumption);
	printf("The Humidity Sensor was turned on for:\t %d hours.\n", Bedroom.HumiDeviceHours);
	printf("Energy consumed by Humidity Sensor:\t %d KJ\n\n", Bedroom.HumiConsumption);
	printf("The Motion Sensor was turned on for:\t %.1f hours.\n", Bedroom.MotionDeviceHours);
	printf("Energy consumed by Motion Sensor:\t %.2f KJ\n\n", Bedroom.MotionConsumption);
	printf("Total Energy Consumption in the bedroom:\t %.2f KJ\n\n\n", Bedroom.TotalConsumption);

	printf("In the Kitchen:\n\n");
	printf("The Temperature Sensor was turned on for:%d hours.\n", Kitchen.TempDeviceHours);
	printf("Energy consumed by Temperature Sensor:\t %d KJ\n\n", Kitchen.TempConsumption);
	printf("The Humidity Sensor was turned on for:\t %d hours.\n", Kitchen.HumiDeviceHours);
	printf("Energy consumed by Humidity Sensor:\t %d KJ\n\n", Kitchen.HumiConsumption);
	printf("The Motion Sensor was turned on for:\t %.1f hours.\n", Kitchen.MotionDeviceHours);
	printf("Energy consumed by Motion Sensor:\t %.2f KJ\n\n", Kitchen.MotionConsumption);
	printf("Total Energy Consumption in the kitchen:\t %.2f KJ\n\n\n", Kitchen.TotalConsumption);
	printf("Total energy consumption:\t\t\t %.2f KJ\n\n\n\n", total_energy);

	FILE* fp = fopen("Energy.txt", "a");

	fprintf(fp, "On today's date:\n%s\n\n\n", ctime(&t));
	fprintf(fp, "In the Living Room:\n\n");
	fprintf(fp, "The Temperature Sensor was turned on for:%d hours.\n", LivingRoom.TempDeviceHours);
	fprintf(fp, "Energy consumed by Temperature Sensor:\t %d KJ\n\n", LivingRoom.TempConsumption);
	fprintf(fp, "The Humidity Sensor was turned on for:\t %d hours.\n", LivingRoom.HumiDeviceHours);
	fprintf(fp, "Energy consumed by Humidity Sensor:\t %d KJ\n\n", LivingRoom.HumiConsumption);
	fprintf(fp, "The Motion Sensor was turned on for:\t %.1f hours.\n", LivingRoom.MotionDeviceHours);
	fprintf(fp, "Energy consumed by Motion Sensor:\t %.2f KJ\n\n", LivingRoom.MotionConsumption);
	fprintf(fp, "Total Energy Consumption in the Living Room:\t %.2f KJ\n\n\n", LivingRoom.TotalConsumption);

	fprintf(fp, "In the BedRoom:\n\n");
	fprintf(fp, "The Temperature Sensor was turned on for:%d hours.\n", Bedroom.TempDeviceHours);
	fprintf(fp, "Energy consumed by Temperature Sensor:\t %d KJ\n\n", Bedroom.TempConsumption);
	fprintf(fp, "The Humidity Sensor was turned on for:\t %d hours.\n", Bedroom.HumiDeviceHours);
	fprintf(fp, "Energy consumed by Humidity Sensor:\t %d KJ\n\n", Bedroom.HumiConsumption);
	fprintf(fp, "The Motion Sensor was turned on for:\t %.1f hours.\n", Bedroom.MotionDeviceHours);
	fprintf(fp, "Energy consumed by Motion Sensor:\t %.2f KJ\n\n", Bedroom.MotionConsumption);
	fprintf(fp, "Total Energy Consumption in the bedroom:\t %.2f KJ\n\n\n", Bedroom.TotalConsumption);

	fprintf(fp, "In the Kitchen:\n\n");
	fprintf(fp, "The Temperature Sensor was turned on for:%d hours.\n", Kitchen.TempDeviceHours);
	fprintf(fp, "Energy consumed by Temperature Sensor:\t %d KJ\n\n", Kitchen.TempConsumption);
	fprintf(fp, "The Humidity Sensor was turned on for:\t %d hours.\n", Kitchen.HumiDeviceHours);
	fprintf(fp, "Energy consumed by Humidity Sensor:\t %d KJ\n\n", Kitchen.HumiConsumption);
	fprintf(fp, "The Motion Sensor was turned on for:\t %.1f hours.\n", Kitchen.MotionDeviceHours);
	fprintf(fp, "Energy consumed by Motion Sensor:\t %.2f KJ\n\n", Kitchen.MotionConsumption);
	fprintf(fp, "Total Energy Consumption in the kitchen:\t %.2f KJ\n\n\n", Kitchen.TotalConsumption);
	fprintf(fp, "Total energy consumption:\t\t\t %.2f KJ\n\n\n\n", total_energy);
	fclose(fp);
	printf("Data has been successfully loaded into Energ log, 'Energ.txt\n");
}
