#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

struct StudentRecord
{
    char name[32];
    unsigned int facultyNumber;
    bool hasFailedExams;
    double averageGrade;
};

void writeStudentRecord(std::ostream& stream, const StudentRecord& student, const bool& binary);
void writeStudentsRecords(std::ostream& stream, const StudentRecord* students, const int& studentsAmount, const bool& binary = false);

StudentRecord* readStudentsRecords(std::ifstream& stream, int& studentsAmount, const bool& binary = false);
StudentRecord readStudentRecord(std::istream& stream, const bool& binary = false, const int& studentPos = 0);

int getStudentsAmount(std::ifstream& stream, const bool& binary);
bool doesFileExist(const char* path);
void createFile(const char* path);

void printMenu();
void setFile(char* path, bool& binary);
void addStudent(const char* path, const bool& binary);
void printAllStudents(const char* path, const bool& binary);
void saveToFile(const StudentRecord* students, const int& studentsAmount, const char* extension, const bool& binary);

char* createPath(const char* extension);
bool shouldOverride(const char* path);

StudentRecord* loadStudentData(int& studentsAmount, const char* path, const bool& binary);

int main()
{
    bool binary;
    char pathToFile[32];
    setFile(pathToFile, binary);

    int userValue;
    int studentsAmount;
    StudentRecord* myStudents;

    do
    {
        printMenu();

        std::cin >> userValue;

        switch (userValue)
        {
        case 1:
            addStudent(pathToFile, binary);
            break;
        case 2:
            printAllStudents(pathToFile, binary);
            break;
        case 3:
            // I can save the data from one file to another without a separate function
            // that loads it into memory but I wanted to try and make it like that :)))
            myStudents = loadStudentData(studentsAmount, pathToFile, binary);
            saveToFile(myStudents, studentsAmount, ".csv", false);
            break;
        case 4:
            myStudents = loadStudentData(studentsAmount, pathToFile, binary);
            saveToFile(myStudents, studentsAmount, ".bin", true);
            break;
        case 5:
            setFile(pathToFile, binary);
            break;
        default:
            std::cout << "Invalid value! Please try again!";
            break;
        }
    } while (userValue != 9);
}

#pragma region Menu

void setFile(char* path, bool& binary)
{
    char userInput;

    do
    {
        std::cout << "Which file would you want to open: ";
        std::cin >> path;

        if (!doesFileExist(path))
        {
            do
            {
                std::cout << "File does not exist! Do you want to create it [y/n]: ";
                std::cin >> userInput;

                if (userInput == 'y')
                    createFile(path);

            } while (userInput != 'y' && userInput != 'n');
        }
    } while (!doesFileExist(path));

    do
    {
        std::cout << "Is this a binary file [y/n]: ";
        std::cin >> userInput;
    } while (userInput != 'y' && userInput != 'n');

    binary = userInput == 'y' ? true : false;
}

StudentRecord* loadStudentData(int& studentsAmount, const char* path, const bool& binary)
{
    std::ifstream stream;

    if (binary)
        stream.open(path, std::ios::binary);
    else
        stream.open(path);

    if (!stream.is_open())
    {
        std::cout << "Unexpected error has occured!" << std::endl;
        return nullptr;
    }

    StudentRecord* students = readStudentsRecords(stream, studentsAmount, binary);

    stream.close();

    return students;
}

void printMenu()
{
    std::cout << "\nWhat would you like to do?\n"
        << "1. Add a student\n"
        << "2. Print all students\n"
        << "3. Save information to .csv file\n"
        << "4. Save information to .bin file\n"
        << "5. Open new file\n"
        << "9. Exit"
        << std::endl;
}

void addStudent(const char* path, const bool& binary)
{
    std::cin.ignore();

    std::cout << "Please enter a student's name, faculty number, average grade and if he has passed his exams or not [1/0]: " << std::endl;

    StudentRecord myStudent = readStudentRecord(std::cin);

    std::ofstream stream;

    if (binary)
        stream.open(path, std::ios::app | std::ios::binary);
    else
        stream.open(path, std::ios::app);

    writeStudentRecord(stream, myStudent, binary);

    stream.close();
}

void printAllStudents(const char* path, const bool& binary)
{
    std::ifstream stream;

    if (binary)
        stream.open(path, std::ios::binary);
    else
        stream.open(path);

    int studentsAmount;
    StudentRecord* students = readStudentsRecords(stream, studentsAmount, binary);

    writeStudentsRecords(std::cout, students, studentsAmount);

    stream.close();
}

void saveToFile(const StudentRecord* students, const int& studentsAmount, const char* extension, const bool& binary)
{
    char* path = createPath(extension);

    if (!shouldOverride(path))
        return;


    std::ofstream stream;

    if (binary)
        stream.open(path, std::ios::binary);
    else
        stream.open(path);

    if (!stream.is_open())
    {
        std::cout << "Failed to open file! No information has been saved!" << std::endl;
        return;
    }

    writeStudentsRecords(stream, students, studentsAmount, binary);

    std::cout << "Infromation saved to " << path << "!" << std::endl;

    stream.close();

    delete[] path;
}

char* createPath(const char* extension)
{
    char fileName[32];
    std::cout << "How do you want to call the file: ";
    std::cin >> fileName;

    char* path = new char[strlen(fileName) + strlen(extension) + 1];
    strcpy(path, fileName);
    strcat(path, extension);

    return path;
}

bool shouldOverride(const char* path)
{
    if (doesFileExist(path))
    {
        char userInput;
        std::cout << "This file already exists! If you continue it's going to be overridden! Do you want to continue [y/n]: ";
        std::cin >> userInput;

        if (userInput != 'y')
        {
            std::cout << "Cancelling..." << std::endl;
            return false;
        }
    }

    return true;
}

#pragma endregion


StudentRecord readStudentRecord(std::istream& stream, const bool& binary, const int& studentPos)
{
    StudentRecord student;

    if (!stream.good())
        stream.clear();

    if (binary)
    {
        stream.seekg(((long long)studentPos * sizeof(student)), stream.beg);
        stream.read(reinterpret_cast<char*>(&student), sizeof(student));
    }
    else
    {
        for (int i = 0; i < studentPos; i++)
        {
            char buff[512];
            stream.getline(buff, 512);
        }

        if (&stream == &std::cin)
        {
            stream.getline(student.name, sizeof(student.name));
            stream >> student.facultyNumber
                >> student.averageGrade
                >> student.hasFailedExams;
        }
        else
        {
            stream.getline(student.name, sizeof(student.name), ',');

            stream >> student.facultyNumber;
            stream.get();

            stream >> student.averageGrade;
            stream.get();

            stream >> student.hasFailedExams;
        }
    }

    stream.clear();
    stream.seekg(0, stream.beg);

    return student;
}

StudentRecord* readStudentsRecords(std::ifstream& stream, int& studentsAmount, const bool& binary)
{
    if (!stream.good())
        stream.clear();

    studentsAmount = getStudentsAmount(stream, binary);

    if (studentsAmount == 0)
        return nullptr;

    StudentRecord* students = new StudentRecord[studentsAmount];

    for (int i = 0; i < studentsAmount; i++)
    {
        students[i] = readStudentRecord(stream, binary, i);
    }

    return students;
}

void writeStudentRecord(std::ostream& stream, const StudentRecord& student, const bool& binary = false)
{
    if (!stream.good())
        stream.clear();

    if (binary)
    {
        stream.write(reinterpret_cast<const char*>(&student), sizeof student);
        return;
    }

    stream << student.name << ","
        << student.facultyNumber << ","
        << student.averageGrade << ","
        << student.hasFailedExams
        << std::endl;
}

void writeStudentsRecords(std::ostream& stream, const StudentRecord* students, const int& studentsAmount, const bool& binary)
{
    if (!stream.good())
        stream.clear();

    for (int i = 0; i < studentsAmount; i++)
    {
        writeStudentRecord(stream, students[i], binary);
    }
}

int getStudentsAmount(std::ifstream& stream, const bool& binary)
{
    int studentsAmount = 0;

    if (binary)
    {
        stream.seekg(0, stream.end);
        studentsAmount = (int)(stream.tellg() / sizeof(StudentRecord));
    }
    else
    {
        char buff[512];
        stream.seekg(0, stream.beg);

        while (stream.getline(buff, 512))
        {
            studentsAmount++;
        }
    }

    stream.clear();
    stream.seekg(0, stream.beg);

    return studentsAmount;
}

bool doesFileExist(const char* path)
{
    std::ifstream stream(path);
    bool status = stream.good();
    stream.close();

    return status;
}

void createFile(const char* path)
{
    std::ofstream stream(path);
    stream.close();
}