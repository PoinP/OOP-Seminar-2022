#ifndef PLANE

#define PLANE
#include<fstream>

class Plane
{
public:
	Plane();
	Plane(const int& id, const char* plane, const char* type, const int& flights);
	~Plane();

	long long GetId() const;
	Plane setId(const int& id);
	Plane setPlane(const char* plane);
	Plane setType(const char* type);
	Plane setFlights(const int& flights);
	Plane InitalizePlaneThroughInput();

	bool WritePlaneInfo(std::ofstream& outputStream) const;
	void PrintPlaneInfo() const;
	bool IsEmpty() const;

private:
	long long _id;
	char _plane[256];
	char _type[256];
	long long _flights;
};

#endif

