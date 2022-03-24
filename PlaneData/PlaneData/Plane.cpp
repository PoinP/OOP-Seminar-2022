#include "Plane.h"
#include "Utility.h"

#include <iostream>

Plane::Plane()
	: _id(0), _flights(0)
{
	_type[0] = '\0';
	_plane[0] = '\0';
}

Plane::Plane(const int& id, const char* plane, const char* type, const int& flights)
	: _id(id), _flights(flights)
{
	Utility::strcpy_safe(_plane, plane, sizeof(_plane));
	Utility::strcpy_safe(_type, type, sizeof(_type));
}

Plane::~Plane()
{}

long long Plane::GetId() const
{
	return _id;
}

Plane Plane::setId(const int& id)
{
	_id = id;

	return *this;
}

Plane Plane::setPlane(const char* plane)
{
	Utility::strcpy_safe(_plane, plane, sizeof(_plane));

	return *this;
}

Plane Plane::setType(const char* type)
{
	Utility::strcpy_safe(_type, type, sizeof(_type));

	return *this;
}

Plane Plane::setFlights(const int& flights)
{
	_flights = flights;

	return *this;
}

bool Plane::WritePlaneInfo(std::ofstream& outputStream) const
{
	if (!outputStream.good())
		outputStream.clear();

	if (outputStream.is_open())
	{
		outputStream.write(reinterpret_cast<const char*>(this), sizeof(*this));
		return outputStream.good();
	}

	std::cout << "File stream isn't open!" << std::endl;
	return false;
}

void Plane::PrintPlaneInfo() const
{
	std::cout << _id << " "
		<< _plane << " "
		<< _type << " "
		<< _flights
		<< std::endl;
}

Plane Plane::InitalizePlaneThroughInput()
{
	std::cin >> _id >> _plane >>_type >> _flights;

	return *this;
}

bool Plane::IsEmpty() const
{
	return _plane[0] == '\0' || _type[0] == '\0';
}
