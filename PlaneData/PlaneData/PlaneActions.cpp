#include "PlaneActions.h"
#include "Utility.h"
#include <iostream>

using namespace Utility;

void PlaneActions::printAllPlanes(const char* path)
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
		plane.PrintPlaneInfo();
	}

	std::cout << std::endl;

	inputStream.close();
}

Plane PlaneActions::deletePlaneInfo(const char* path, const long long& id)
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
		if (tempPlane.GetId() == id)
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

Plane PlaneActions::findPlaneInfo(std::ifstream& inputStream, const long long& id)
{
	if (!inputStream.good())
		inputStream.clear();

	if (inputStream.is_open())
	{
		Plane plane;

		inputStream.seekg(0, inputStream.beg);

		while (inputStream.read(reinterpret_cast<char*>(&plane), sizeof(plane)))
		{
			if (plane.GetId() == id)
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
