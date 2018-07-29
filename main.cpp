#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

enum PNG_ImageType{
	Greyscale = 0,Truecolour = 2,IndexedColour = 3,GreyscaleWithAlpha = 4,TrueColourWithAlpha = 6
};

struct pngHead{
	unsigned int width = 0,height = 0;
	unsigned char deep = 0,colorType = 0,compressionMethod = 0,filterMethod = 0,interlaceMethod = 0;
	void list()
	{
		std::cout<< "---------Head---------\n"
		<< "width = "				<< width					<< " px"	<< std::endl
		<< "height = "				<< height					<< " px"	<< std::endl
		<< "deep = "				<< (int)deep				<< " byte"	<< std::endl
		<< "colorType = "			<< (int)colorType			<< std::endl
		<< "compressionMethod = "	<< (int)compressionMethod	<< std::endl
		<< "filterMethod = "		<< (int)filterMethod		<< std::endl
		<< "interlaceMethod = "		<< (int)interlaceMethod		<< std::endl
		<<"----------------------\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			I  H  D  R |  width  | |  height | | other data |
		  	73 72 68 82 00 00 00 00 00 00 00 00 00 00 00 00 00
			0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
		*/

		width  = arr[4];	width <<= 8;
		width |= arr[5]; 	width <<= 8;
		width |= arr[6]; 	width <<= 8;
		width |= arr[7];

		height  = arr[8];	height <<= 8;
		height |= arr[9];	height <<= 8;
		height |= arr[10];	height <<= 8;
		height |= arr[11];

		deep				= arr[12];
		colorType			= arr[13];
		compressionMethod	= arr[14];
		filterMethod		= arr[15];
		interlaceMethod		= arr[16];
	}
	const int length = 25;
};

struct pngTime
{
	unsigned short year = 0;
	unsigned char month = 0,day = 0,hour = 0,minute = 0,second = 0;
	void list()
	{
		std::cout<< "---------Time---------\n"
		<< day << "." << month << "." << year << "  "
		<< hour << ":" << minute << ":" << second 
		<< std::endl
		<<"----------------------\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			t  I  M  E  |year| | other data |
		   116 73 77 69 00  00 00 00 00 00 00
		   0   1  2  3  4   5  6  7  8  9  10
		*/
		year  = arr[4];	year <<= 8;
		year |= arr[5];

		month	= arr[6];
		day		= arr[7];
		hour	= arr[8];
		minute	= arr[9];
		second	= arr[10];
	}
	const int length = 10;
};


class PNG_Reader
{
	pngHead Head;
	std::string fileName;
public:
	void readFile(std::string filename)
	{
		fileName = filename;
		std::ifstream fin(fileName);
		if(!fin.is_open())
			throw std::runtime_error("Could not open file");
		fin.seekg (0, fin.end);
    	unsigned int fileSize = fin.tellg();
		fin.seekg (0, fin.beg);
		unsigned char *arr = new unsigned char[fileSize];
		fin.read(reinterpret_cast<char*>(arr),fileSize);
		/*	The array must beginning
			  P  N  G							I  H  D  R |  width  | |  height | | other data |
		  137 80 78 71 13 10 26 10 00 00 00 13 73 72 68 82 00 00 00 00 00 00 00 00 00 00 00 00 00
			0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
		*/
		fin.close();
		if(arr[1] != 'P' || arr[2] != 'N' || arr[3] != 'G')
			throw std::runtime_error("File isn't PNG");

		int position = 12;

		if(arr[12] == 'I' && arr[13] == 'H' && arr[14] == 'D' && arr[15] == 'R')
		{
			Head.read(arr + position);
			Head.list();		
			position += Head.length;
		}
		


		delete[] arr;
	}
};

int main()
{
	PNG_Reader img;
	img.readFile("demo.png");
	return 0;
}