#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

struct pngHead{
	unsigned int width,height;
	unsigned char deep,colorType,compressionMethod,filterMethod,interlaceMethod;
	void list()
	{
		std::cout
		<< "width = "				<< width					<< std::endl
		<< "height = "				<< height					<< std::endl
		<< "deep = "				<< (int)deep				<< std::endl
		<< "colorType = "			<< (int)colorType			<< std::endl
		<< "compressionMethod = "	<< (int)compressionMethod	<< std::endl
		<< "filterMethod = "		<< (int)filterMethod		<< std::endl
		<< "interlaceMethod = "		<< (int)interlaceMethod		<< std::endl;
	}
	void read(std::ifstream& fin)
	{
		const int length = 29;
		unsigned char arr[length];
		for (int i = 0; i < length; i++)
		{
			fin.read(reinterpret_cast<char *>(&arr[i]),1);
		}
		/*	The array must beginning
			  P  N  G							I  H  D  R |  width  | |  height | | other data |
		  137 80 78 71 13 10 26 10 00 00 00 13 73 72 68 82 00 00 00 00 00 00 00 00 00 00 00 00 00
			0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
		*/
		if(arr[1] != 80 || arr[2] != 78 || arr[3] != 71 || arr[12] != 73 || arr[13] != 72 || arr[14] != 68 || arr[15] != 82)
			throw "Error: file isn't PNG";

		width = arr[16]; 			width <<= 8;
		width = width | arr[17]; 	width <<= 8;
		width = width | arr[18]; 	width <<= 8;
		width = width | arr[19];

		height = arr[20];			height <<= 8;
		height = height | arr[21];	height <<= 8;
		height = height | arr[22];	height <<= 8;
		height = height | arr[23];

		deep = arr[24];
		colorType = arr[25];
		compressionMethod = arr[26];
		filterMethod = arr[27];
		interlaceMethod = arr[28];
	}
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
			throw "File not find";
		Head.read(fin);
		Head.list();
	}
};


int main()
{
	PNG_Reader img;
	img.readFile("demo.png");
	return 0;
}