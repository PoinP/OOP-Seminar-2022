#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>

struct Plane
{
	long long id;
	char plane[256];
	char type[256];
	long long flights;
};

// Menu functions
void createPlaneMenu(const char* path);
void searchPlaneMenu(const char* path);
void deletePlaneMenu(const char* path);
void printAllPlanes(const char* path);

// Plane CRD (Create-Read-Delete)
bool writePlaneInfo(std::ofstream& outputStream, Plane plane);
Plane findPlaneInfo(std::ifstream& inputStream, const long long& id);
Plane deletePlaneInfo(const char* path, const long long& id);
void printPlaneInfo(Plane plane);

// Utilities
void strcpy_safe(char* destination, const char* source, const int& size);
bool doesFileExist(const char* path);
void createFile(const char* path);
void toLowercase(char* string);
bool compare(const char* str1, const char* str2);

int getRandom(int start, int end);
void generateData(const char* path, const int& amount);

int main()
{
	const char* path = "Planes.db";

	// Used for generating data! 
	// srand(time(NULL));
	// generateData(path, 50);

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
			printAllPlanes(path);
		}
		else if (!compare(userInput, "exit"))
		{
			std::cout << "Invalid input! Please try again!" << std::endl;
		}
	} while (!compare(userInput, "exit"));

	std::cout << "\nGoodbye!" << std::endl;
}

#pragma region Menu

void createPlaneMenu(const char* path)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	Plane newPlane;

	std::cin >> newPlane.id >> newPlane.plane
		>> newPlane.type >> newPlane.flights;

	std::ofstream outputStream(path, std::ios::binary | std::ios::app);

	if (!outputStream.is_open())
	{
		std::cout << "Error opening file! No plane has been created!\n" << std::endl;
		return;
	}

	if (writePlaneInfo(outputStream, newPlane))
		std::cout << "Record saved!\n" << std::endl;
	else
		std::cout << "Error saving the record! Please try again!\n" << std::endl;

	outputStream.close();
}

void searchPlaneMenu(const char* path)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	std::ifstream inputStream(path, std::ios::binary);

	if (!inputStream.is_open())
	{
		std::cout << "Error opening file! Couldn't search for a plane!\n" << std::endl;
		return;
	}

	long long planeId;

	std::cout << "Please enter an id to search for: ";
	std::cin >> planeId;

	Plane foundPlane = findPlaneInfo(inputStream, planeId);

	inputStream.close();

	if (foundPlane.plane[0] == '\0' || foundPlane.type[0] == '\0')
	{
		std::cout << "Record not found!\n" << std::endl;
		return;
	}

	printPlaneInfo(foundPlane);
}

void deletePlaneMenu(const char* path)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	long long planeId;

	std::cout << "Please enter an id of a plane: ";
	std::cin >> planeId;

	Plane deletedPlane = deletePlaneInfo(path, planeId);

	if (deletedPlane.plane[0] == '\0' || deletedPlane.type[0] == '\0')
	{
		std::cout << "Record not found! No plane has been deleted!\n" << std::endl;
		return;
	}

	std::cout << "You have deleted: ";
	printPlaneInfo(deletedPlane);
	std::cout << std::endl;
}

#pragma endregion


// Prints all of the information in a file
void printAllPlanes(const char* path)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	std::ifstream inputStream(path, std::ios::binary);

	if (!inputStream.is_open())
	{
		std::cout << "Error opening file! No data available to print!\n" << std::endl;
		return;
	}

	Plane plane;

	while (inputStream.read(reinterpret_cast<char*>(&plane), sizeof(plane)))
	{
		printPlaneInfo(plane);
	}

	std::cout << std::endl;

	inputStream.close();
}

// Deletes a plane record based on a given ID
Plane deletePlaneInfo(const char* path, const long long& id)
{
	if (!doesFileExist(path))
	{
		std::cout << "The path " << path << " does not exist!" << std::endl;
		return {};
	}

	std::ifstream inputStream(path, std::ios::binary);
	std::ofstream outputStream("temp", std::ios::binary);

	Plane deletedPlane = {};
	Plane tempPlane = {};

	if (!inputStream.is_open() || !outputStream.is_open())
	{
		std::cout << "An error has occured! No plane has been deleted!" << std::endl;
		return deletedPlane;
	}

	while (inputStream.read(reinterpret_cast<char*>(&tempPlane), sizeof(tempPlane)))
	{
		if (tempPlane.id == id)
		{
			deletedPlane = tempPlane;
			continue;
		}

		outputStream.write(reinterpret_cast<const char*>(&tempPlane), sizeof(tempPlane));
	}

	inputStream.close();
	outputStream.close();

	remove(path);
	if (rename("temp", path))
		std::cout << "An error has occured! Renaming of file failed unexpectedly!\n" << std::endl;

	return deletedPlane;
}

// Writes a plane record into an open file output stream
bool writePlaneInfo(std::ofstream& outputStream, Plane plane)
{
	if (!outputStream.good())
		outputStream.clear();

	if (outputStream.is_open())
	{
		outputStream.write(reinterpret_cast<const char*>(&plane), sizeof(plane));
		return outputStream.good();
	}

	std::cout << "File stream isn't open!" << std::endl;
	return false;
}

// Finds a plane record into an open file input stream
Plane findPlaneInfo(std::ifstream& inputStream, const long long& id)
{
	if (!inputStream.good())
		inputStream.clear();

	if (inputStream.is_open())
	{
		Plane plane;

		inputStream.seekg(0, inputStream.beg);

		while (inputStream.read(reinterpret_cast<char*>(&plane), sizeof(plane)))
		{
			if (plane.id == id)
				return plane;
		}

		inputStream.seekg(0, inputStream.beg);
	}
	else
	{
		std::cout << "File stream isn't open!" << std::endl;
	}

	return {};
}

// Prints a specific plane record
void printPlaneInfo(Plane plane)
{
	std::cout << plane.id << " "
		<< plane.plane << " "
		<< plane.type << " "
		<< plane.flights
		<< std::endl;
}

#pragma region Utilities

void strcpy_safe(char* destination, const char* source, const int& size)
{
	int i;
	for (i = 0; i < size - 1 && source[i] != '\0'; i++)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';
}

bool doesFileExist(const char* path)
{
	std::ifstream input(path);
	bool status = input.good();
	input.close();

	return status;
}

void createFile(const char* path)
{
	std::ofstream output(path);
	output.close();
}

bool compare(const char* str1, const char* str2)
{
	for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++)
	{
		if (str1[i] != str2[i])
			return false;
	}

	return true;
}

void toLowercase(char* string)
{
	while (*string != '\0')
	{
		if (*string >= 'A' && *string <= 'Z')
			*string += 'a' - 'A';

		string++;
	}
}

int getRandom(int start, int end)
{
	return rand() % end + start;
}


void generateData(const char* path, const int& amount)
{
	const char types[6][256] =
	{
		"Fighter",
		"Striker",
		"Bomber",
		"Jet Aircraft",
		"Airliner",
		"Heavy FIghter"
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

	for (int i = 0; i < amount; i++)
	{
		newPlane.id = getRandom(100000, 999999);
		strcpy_safe(newPlane.plane, planes[getRandom(0, 22)], 256);
		strcpy_safe(newPlane.type, types[getRandom(0, 5)], 256);
		newPlane.flights = getRandom(50, 10000);

		if (writePlaneInfo(outputStream, newPlane))
			std::cout << "Record saved!\n" << std::endl;
		else
			std::cout << "Error saving the record! Please try again!\n" << std::endl;
	}

	outputStream.close();
}

#pragma endregion