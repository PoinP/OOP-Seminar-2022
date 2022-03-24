#ifndef PLANEACTIONS

#define PLANEACTIONS
#include "Plane.h"

namespace PlaneActions
{
	Plane findPlaneInfo(std::ifstream& inputStream, const long long& id);
	Plane deletePlaneInfo(const char* path, const long long& id);
	void printAllPlanes(const char* path);
};

#endif

