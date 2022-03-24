#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>

#include "Plane.h"
#include "Utility.h"
#include "Menu.h"
#include "PlaneActions.h"

using namespace Utility;
using namespace Menu;

void generateData(const char* path, const int& amount);

int main()
{
	const char* path = "Planes.db";

	// Used for generating data! 
	// srand(time(NULL));
	// generateData(path, 200);

	if (!doesFileExist(path))
		createFile(path);

	char userInput[32];
	do
	{
		std::cout << "What would you like to do?\n"
			<< "Create, Search, Delete, PrintAll or Exit:"
			<< std::endl;

		std::cin >> userInput;

		toLowercase(userInput);

		if (compare(userInput, "create"))
		{
			createPlaneMenu(path);
		}
		else if (compare(userInput, "search"))
		{
			searchPlaneMenu(path);
		}
		else if (compare(userInput, "delete"))
		{
			deletePlaneMenu(path);
		}
		else if (compare(userInput, "printall"))
		{
			PlaneActions::printAllPlanes(path);
		}
		else if (!compare(userInput, "exit"))
		{
			std::cout << "Invalid input! Please try again!" << std::endl;
		}
	} while (!compare(userInput, "exit"));

	std::cout << "\nGoodbye!" << std::endl;
}

void generateData(const char* path, const int& amount)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	Plane newPlane;

	std::ofstream outputStream(path, std::ios::binary | std::ios::app);

	if (!outputStream.is_open())
	{
		std::cout << "Error opening file! No plane has been saved!\n" << std::endl;
		return;
	}

	const char types[6][256] =
	{
		"Fighter",
		"Striker",
		"Bomber",
		"Jet Aircraft",
		"Airliner",
		"Heavy Fighter"
	};

	const char planes[23][256] =
	{
		"Aero A.304",
		"Aero L-29",
		"Albatros B.I",
		"Albatros D.III",
		"Albatros F.2",
		"Albatros L 73",
		"Antonov An-2",
		"Antonov An-14",
		"Antonov An-24",
		"Arado Ar 65",
		"Arado Ar 96",
		"Focke-Wulf Fw 43",
		"MiG-15",
		"MiG-17",
		"Lim-6",
		"MiG-19",
		"MiG-21",
		"MiG-23",
		"Mil Mi-1",
		"Mil Mi-2",
		"Mil Mi-4",
		"Mil Mi-6",
		"Mil Mi-8"
	};

	for (int i = 0; i < amount; i++)
	{
		newPlane.setId(getRandom(100000, 999999))
			.setPlane(planes[getRandom(0, 22)])
			.setType(types[getRandom(0, 5)])
			.setFlights(getRandom(50, 10000));

		if (newPlane.WritePlaneInfo(outputStream))
			std::cout << "Error saving the record! Please try again!\n" << std::endl;
	}

	outputStream.close();

	std::cout << "Generated " << amount << " entries!\n" << std::endl;
}