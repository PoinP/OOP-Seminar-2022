#include "Menu.h"
#include "Plane.h"
#include "Utility.h"
#include "PlaneActions.h"

#include <iostream>

using namespace Utility;
using namespace PlaneActions;

void Menu::createPlaneMenu(const char* path)
{
	if (!doesFileExist(path))
	{
		std::cout << "The file " << path << " doesn't exist!\n" << std::endl;
		return;
	}

	Plane newPlane;
	newPlane.InitalizePlaneThroughInput();

	std::ofstream outputStream(path, std::ios::binary | std::ios::app);

	if (!outputStream.is_open())
	{
		std::cout << "Error opening file! No plane has been created!\n" << std::endl;
		return;
	}

	if (newPlane.WritePlaneInfo(outputStream))
		std::cout << "Record saved!\n" << std::endl;
	else
		std::cout << "Error saving the record! Please try again!\n" << std::endl;

	outputStream.close();
}

void Menu::searchPlaneMenu(const char* path)
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

	if (foundPlane.IsEmpty())
	{
		std::cout << "Record not found!\n" << std::endl;
		return;
	}

	foundPlane.PrintPlaneInfo();
}

void Menu::deletePlaneMenu(const char* path)
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

	if (deletedPlane.IsEmpty())
	{
		std::cout << "Record not found! No plane has been deleted!\n" << std::endl;
		return;
	}

	std::cout << "You have deleted: ";
	deletedPlane.PrintPlaneInfo();
	std::cout << std::endl;
}