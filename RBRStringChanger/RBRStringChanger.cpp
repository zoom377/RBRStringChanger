#include "pch.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//NOTES:
//I need to find the car name string offsets
//I don't think we can change the length of the car name strings without potentially breaking the application
//For each string that I want to replace, I need to know the size of the original string, the offset and the replacement string

//Car names are updated correctly now. Next I can do the car specs.
//The specs for installed cars are in "%RBR dir%\RBRCIT\carlist\carList.ini". This ini file contains details of all the cars downloaded by RBRCIT (not currently installed cars)
//Each car is under a header [Car_NN] but I don't know the numbers of the currently installed cars.
//I must search for the name of a car and assume the following keys correspond.
//If I search one line at a time and compare the whole line I'm more likely to find the correct car.
//Specs display and availability in carList.ini:
//Model
//Horsepower
//Torque UNAVAILABLE
//Engine UNAVAILABLE
//Tyres UNAVAILABLE
//Weight
//Transmission



//Original car strings and offsets:
//The strings are here for dev purposes and to know the length of the originals
//I found these offsets by searching the exe with string.find()

const std::string car00String1 = "Citroen Xsara";//Car00
const std::string car00String2 = "Citroen";
const size_t car00String1Offset = 3346000;
const size_t car00String2Offset = 3346388;

const std::string car01String1 = "Hyundai Accent";//car01
const std::string car01String2 = "Hyundai";
const size_t car01String1Offset = 3345984;
const size_t car01String2Offset = 0;

const std::string car02String1 = "MG ZR Super 1600";//car02
const std::string car02String2 = "MG";
const size_t car02String1Offset = 3345964;
const size_t car02String2Offset = 0;

const std::string car03String1 = "Mitsubishi Lancer Evo VII";//car03
const std::string car03String2 = "Mitsubishi";
const size_t car03String1Offset = 3345936;
const size_t car03String2Offset = 0;

const std::string car04String1 = "Peugeot 206";//car04
const std::string car04String2 = "Peugeot";
const size_t car04String1Offset = 3345924;
const size_t car04String2Offset = 0;

const std::string car05String1 = "Subaru Impreza 2003";//car05
const std::string car05String2 = "Subaru '03";
const size_t car05String1Offset = 3345904;
const size_t car05String2Offset = 0;

const std::string car06String1 = "Toyota Corolla";//car06
const std::string car06String2 = "Toyota";
const size_t car06String1Offset = 3345888;
const size_t car06String2Offset = 0;

const std::string car07String1 = "Subaru Impreza 2000";//car07
const std::string car07String2 = "Subaru '00";
const size_t car07String1Offset = 3345868;
const size_t car07String2Offset = 0;


const std::string carsIniSubPath = "\\Cars\\Cars.ini";
const std::string carListIniSubPath = "\\RBRCIT\\carlist\\carList.ini";
const std::string rbrExeSubPath = "\\RichardBurnsRally_SSE.exe";

std::string car00String1Replacement = std::string(256, '\0');
std::string car01String1Replacement = std::string(256, '\0');
std::string car02String1Replacement = std::string(256, '\0');
std::string car03String1Replacement = std::string(256, '\0');
std::string car04String1Replacement = std::string(256, '\0');
std::string car05String1Replacement = std::string(256, '\0');
std::string car06String1Replacement = std::string(256, '\0');
std::string car07String1Replacement = std::string(256, '\0');


//Dev tool: return all the occurences of stringToFind in string.
std::vector<size_t> findAllOccurrences(std::string& string, const char* stringToFind)
{
	std::vector<size_t> foundPositions;

	size_t lastFound = -1;
	do
	{
		lastFound = string.find(stringToFind, lastFound + 1);
		if (lastFound != std::string::npos)
		{
			foundPositions.push_back(lastFound);//Add to container if an occurence was found
			std::cout << stringToFind << " found at: " << lastFound << std::endl;
		}
	} while (lastFound != std::string::npos);

	return foundPositions;
}

void replaceCarStrings(std::string& string)
{
	//string.replace(car00String1Offset, strlen(car00String1), car00String1);
}

int main()
{
	
	char* exePath = nullptr;//This ptr will be set to a c string managed elsewhere
	_get_pgmptr(&exePath);//Get the path to the exe file of this application. I'm guessing this is a pointer to memory already allocated and managed by the application
	std::string exePathStr(exePath);//to std::string

	//Set exe path manually for debug purposes:
	//exePathStr = "D:\\Games\\RBR Modded\\RichardBurnsRally_SSE.exe";

	//Trim this exe file from path to get directory
	size_t lastFoundBackslash = -1;
	size_t foundBackslash = -1;
	do
	{
		foundBackslash = exePathStr.find('\\', lastFoundBackslash + 1);
		if (foundBackslash != std::string::npos) lastFoundBackslash = foundBackslash;
	} while (foundBackslash != std::string::npos);

	//Calculate directories
	std::string rbrDirectory = exePathStr.substr(0, lastFoundBackslash);
	std::string carsIniPath = rbrDirectory + carsIniSubPath;
	std::string rbrExePath = rbrDirectory + rbrExeSubPath;

	std::cout << "RBR directory: " << rbrDirectory << std::endl;
	//std::cout << "Ini path: " << carsIniPath << std::endl;
	//std::cout << "Rbr exe path: " << rbrExePath << std::endl;

	//Open out file stream to rbr exe
	//If it isn't opened with std::ios::ate then the file get truncated/deleted!
	std::ofstream rbrofstream = std::ofstream(rbrExePath, std::ios::binary || std::ios::ate);
	if (rbrofstream.is_open())
	{
		//Get replacement strings from Cars.ini and resize them. If car names not found then put originals
		//GetPrivateProfileString returns length of string that was put into buffer
		//&car00String1Replacement[0] expresses the address of the first character of the string
		//GetPrivateProfileString GetLastError() returns error code 2(ERROR_FILE_NOT_FOUND) if the key doesn't exist (car doesn't have a name)
		car00String1Replacement.resize(GetPrivateProfileStringA("Car00", "CarName", car00String1.c_str(), &car00String1Replacement[0], 256, carsIniPath.c_str()));
		car01String1Replacement.resize(GetPrivateProfileStringA("Car01", "CarName", car01String1.c_str(), &car01String1Replacement[0], 256, carsIniPath.c_str()));
		car02String1Replacement.resize(GetPrivateProfileStringA("Car02", "CarName", car02String1.c_str(), &car02String1Replacement[0], 256, carsIniPath.c_str()));
		car03String1Replacement.resize(GetPrivateProfileStringA("Car03", "CarName", car03String1.c_str(), &car03String1Replacement[0], 256, carsIniPath.c_str()));
		car04String1Replacement.resize(GetPrivateProfileStringA("Car04", "CarName", car04String1.c_str(), &car04String1Replacement[0], 256, carsIniPath.c_str()));
		car05String1Replacement.resize(GetPrivateProfileStringA("Car05", "CarName", car05String1.c_str(), &car05String1Replacement[0], 256, carsIniPath.c_str()));
		car06String1Replacement.resize(GetPrivateProfileStringA("Car06", "CarName", car06String1.c_str(), &car06String1Replacement[0], 256, carsIniPath.c_str()));
		car07String1Replacement.resize(GetPrivateProfileStringA("Car07", "CarName", car07String1.c_str(), &car07String1Replacement[0], 256, carsIniPath.c_str()));

		std::cout << "Car 00: " << car00String1Replacement << std::endl;
		std::cout << "Car 01: " << car01String1Replacement << std::endl;
		std::cout << "Car 02: " << car02String1Replacement << std::endl;
		std::cout << "Car 03: " << car03String1Replacement << std::endl;
		std::cout << "Car 04: " << car04String1Replacement << std::endl;
		std::cout << "Car 05: " << car05String1Replacement << std::endl;
		std::cout << "Car 06: " << car06String1Replacement << std::endl;
		std::cout << "Car 07: " << car07String1Replacement << std::endl;
		//std::cout << "LastError: " << GetLastError() << std::endl;


		std::cout << "Writing new strings...\n";
		rbrofstream.seekp(0, std::ios::end);
		std::cout << "RichardBurnsRally.exe size before: " << rbrofstream.tellp() << std::endl;

		rbrofstream.seekp(car00String1Offset);
		rbrofstream.write(car00String1Replacement.c_str(), car00String1.length());

		rbrofstream.seekp(car01String1Offset);
		rbrofstream.write(car01String1Replacement.c_str(), car01String1.length());

		rbrofstream.seekp(car02String1Offset);
		rbrofstream.write(car02String1Replacement.c_str(), car02String1.length());

		rbrofstream.seekp(car03String1Offset);
		rbrofstream.write(car03String1Replacement.c_str(), car03String1.length());

		rbrofstream.seekp(car04String1Offset);
		rbrofstream.write(car04String1Replacement.c_str(), car04String1.length());

		rbrofstream.seekp(car05String1Offset);
		rbrofstream.write(car05String1Replacement.c_str(), car05String1.length());

		rbrofstream.seekp(car06String1Offset);
		rbrofstream.write(car06String1Replacement.c_str(), car06String1.length());

		rbrofstream.seekp(car07String1Offset);
		rbrofstream.write(car07String1Replacement.c_str(), car07String1.length());

		rbrofstream.seekp(0, std::ios::end);
		std::cout << "RichardBurnsRally.exe size after: " << rbrofstream.tellp() << std::endl;
		rbrofstream.close();
	}
	else std::cout << "Couldn't write to " << rbrExePath << ". The game might be running or the .exe file might be read-only.\n";

	//std::cout << rbrString.substr(car00String1Offset, 250) << std::endl;


	//else std::cout << "Could't open RichardBurnsRally_SSE.exe, it might be read-only." << std::endl;

	std::cout << "Press return to exit...";
	std::string input;
	std::getline(std::cin, input);
}