#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//using namespace std;

inline int UcharArrToUint(unsigned char* arr)
{
	int res = 0;
	res  = arr[0];	res <<= 8;
	res |= arr[1];	res <<= 8;
	res |= arr[2];	res <<= 8;
	res |= arr[3];
	return res;
}

inline int UcharArrToUshort(unsigned char* arr)
{
	int res = 0;
	res  = arr[0];	res <<= 8;
	res |= arr[1];
	return res;
}

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
		width = UcharArrToUint(arr + 4);
		height = UcharArrToUint(arr + 8);
		
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
		<< (int)day << "." << (int)month << "." << year << "  "
		<< (int)hour << ":" << (int)minute << ":" << (int)second 
		<< std::endl
		<<"----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			t  I  M  E  |year| | other data |
		   116 73 77 69 00  00 00 00 00 00 00
		   0   1  2  3  4   5  6  7  8  9  10
		*/
		length = UcharArrToUint(arr - 4) + 12;
		year = UcharArrToUshort(arr +4);

		month	= arr[6];
		day		= arr[7];
		hour	= arr[8];
		minute	= arr[9];
		second	= arr[10];
	}
	int length = 19;
	const std::string Name = "tIME";
};

struct pngGamma
{
	unsigned int gamma = 45455;
	void list()
	{
		std::cout<< "---------Gamma---------\n"
		<< "gamma = " << gamma << std::endl
		<<"-----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			 g  A  M  A |  gamma   |
		   105 67 67 80 00  00 00 00
		   0   1  2  3  4   5  6  7 
		*/
		length = UcharArrToUint(arr - 4) + 12;
		gamma  = UcharArrToUint(arr + 4);
	}
	int length = 16;
	const std::string Name = "gAMA";
};

struct pngData
{
	//unsigned int gamma = 45455;
	void list()
	{
		std::cout<< "---------IDAT---------\n"
		<< "length = " << length << std::endl
		<<"-----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			 I  D  A  T 
			73 68 65 84 
			 0  1  2  3  
		*/
		length = UcharArrToUint(arr - 4) + 12;
	}
	int length = 0;
	const std::string Name = "IDAT";
};

struct pngEnd
{
	void list()
	{
		std::cout<< "---------IEND---------\n"
		<< "length = " << length << std::endl
		<<"-----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			 I  E  N  D
			73 69 78 68 
			 0  1  2  3  
		*/
		length = UcharArrToUint(arr - 4) + 8;
	}
	int length = 0;
	const std::string Name = "IEND";
};


struct pngText
{
	void list()
	{
		std::cout<< "---------tEXt---------\n";
		for (int i = 0; i < Text.size(); ++i)
		{
			std::cout<<Text[i]<<std::endl;
		}
		std::cout<<"-----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			  t  E  X   t
			116 69 88 116 
			  0  1  2   3  
		*/
		length = UcharArrToUint(arr - 4) + 12;
		std::string t;
		char w;
		for (int i = 4; i < length-12; ++i)
		{
			w = arr[i];
			if(w == 0)
				w = 13;
			t.push_back(w);
		}
		Text.push_back(t);
	}
	int length = 0;
	const std::string Name = "tEXt";
	std::vector<std::string> Text;
};

//Background color
struct pngBKGD
{
	unsigned short greyscale = 0,red = 0,green = 0,blue = 0;
	unsigned char Pindex = 0;

	void list()
	{
		std::cout<< "---------BKGD---------\n"
		<< "length = " << length << std::endl;
		switch(ctype)
		{
			case 0:
			case 4:
				std::cout << "greyscale = " << greyscale << std::endl;
			break;
			case 2:
			case 6:
				std::cout << "red = "	<< red		<< std::endl;
				std::cout << "green = "	<< green	<< std::endl;
				std::cout << "blue = "	<< blue		<< std::endl;
			break;
			case 3:
				std::cout << "Pindex = " << Pindex << std::endl;
			break;
		}
		std::cout<<"-----------------------\n\n";
	}
	void read(unsigned char * arr ,unsigned char colourType = 6)
	{
	/*   ______________________
		|Colour types 0 and 4  |		The array must beginning
		|----------------------|		 b  K  G  D 
		|Greyscale     |2 bytes|		98 75 71 68
		|--------------|-------|		 0  1  2  3  
		|Colour types 2 and 6  |
		|--------------|-------|
		|Red           |2 bytes|
		|--------------|-------|
		|Green	       |2 bytes|
		|--------------|-------|
		|Blue          |2 bytes|
		|--------------|-------|
		|Colour type 3         |
		|--------------|-------|
		|Palette index |1 byte |
		|______________|_______|*/

		length = UcharArrToUint(arr - 4) + 12;
		ctype = colourType; 
		switch(colourType)
		{
			case 0:
			case 4:
				greyscale = UcharArrToUshort(arr + 4);
			break;
			case 2:
			case 6:
				red 	= UcharArrToUshort(arr + 4);
				green 	= UcharArrToUshort(arr + 6);
				blue 	= UcharArrToUshort(arr + 8);
			break;
			case 3:
				Pindex = UcharArrToUshort(arr + 12);
			break;
		}

	}
	int length = 0;
	const std::string Name = "bKGD";
private:
	unsigned char ctype;
};

struct pngCHRM
{
	unsigned int /*Defoult data*/
	whiteX	= 31270, whiteY	= 32900,
	redX	= 64000, redY	= 33000,
	greenX	= 30000, greenY	= 30000,
	blueX	= 15000, blueY	= 6000;
	void list()
	{
		std::cout<< "---------cHRM---------\n"
		<< "whiteX = "	<< whiteX	<< std::endl
		<< "whiteY = "	<< whiteY	<< std::endl
		<< "redX = "	<< redX		<< std::endl
		<< "redY = "	<< redY		<< std::endl
		<< "greenX  = "	<< greenX	<< std::endl
		<< "greenY  = "	<< greenY	<< std::endl
		<< "blueX  = "	<< blueX	<< std::endl
		<< "blueY  = "	<< blueY	<< std::endl
		<< "length = "	<< length	<< std::endl
		<<"-----------------------\n\n";
	}
	void read(unsigned char * arr )
	{
		/*	The array must beginning
			 c  H  R  M | whiteX  | |  whiteY | |   redX  | |   redY  | | greenX  |
			99 72 82 77 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
			0   1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 
			-----------------------------------------------------------------------
			|  greenY | |  blueX  | |  blueY  |
			00 00 00 00 00 00 00 00 00 00 00 00
			24 25 26 27 28 29 30 31 32 33 34 35
		*/
		length = UcharArrToUint(arr - 4) + 12;

		whiteX = UcharArrToUint(arr + 4);
		whiteY = UcharArrToUint(arr + 8);
	
		redX = UcharArrToUint(arr + 12);
		redY = UcharArrToUint(arr + 16);

		greenX = UcharArrToUint(arr + 20);
		greenY = UcharArrToUint(arr + 24);

		blueX = UcharArrToUint(arr + 28);
		blueY = UcharArrToUint(arr + 32);
	}
	int length = 36;
	const std::string Name = "cHRM";
};

class PNG_Reader
{
	pngHead Head;
	pngTime Time;
	pngGamma Gamma;
	pngCHRM Chrm;//cHRM
	pngData Data;
	pngBKGD Bkgd;
	pngEnd end;
	pngText Text;
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
		int Errors = 0;
		while(position <= fileSize)
		{
			int i = 0;
			/*----IDAT----*/
				for (i = 0; i < 4 && arr[position + i] == Data.Name[i]; ++i);
				if(i == 4)
				{
					Data.read(arr+position);
					Data.list();
					position += Data.length;
					continue;
				}
			/*   -IDAT-   */

			/*----tEXt----*/
				for (i = 0; i < 4 && arr[position + i] == Text.Name[i]; ++i);
				if(i == 4)
				{
					Text.read(arr+position);
					position += Text.length;
					continue;
				}
			/*   -tEXt-   */
			/*----tIME----*/
				for (i = 0; i < 4 && arr[position + i] == Time.Name[i]; ++i);
				if(i == 4)
				{
					Time.read(arr+position);
					Time.list();
					position += Time.length;
					continue;
				}
			/*   -tIME-   */
			/*----gAMA----*/
				for (i = 0; i < 4 && arr[position + i] == Gamma.Name[i]; ++i);
				if(i == 4)
				{
					Gamma.read(arr+position);
					Gamma.list();
					position += Gamma.length;
					continue;
				}
			/*   -gAMA-   */
			/*----cHRM----*/
				for (i = 0; i < 4 && arr[position + i] == Chrm.Name[i]; ++i);
				if(i == 4)
				{
					Chrm.read(arr+position);
					Chrm.list();
					position += Chrm.length;
					continue;
				}
			/*   -cHRM-   */
			/*----bKGD----*/
				for (i = 0; i < 4 && arr[position + i] == Bkgd.Name[i]; ++i);
				if(i == 4)
				{
					Bkgd.read(arr+position, Head.colorType);
					Bkgd.list();
					position += Bkgd.length;
					continue;
				}
			/*   -bKGD-   */
			/*----IEND----*/
				for (i = 0; i < 4 && arr[position + i] == end.Name[i]; ++i);
				if(i == 4)
				{
					end.read(arr+position);	
					end.list();
					position += end.length;
					break;
				}
			/*   -IEND-   */	

			position++;
			Errors++;
		}

		Text.list();
		std::cout<<"-------Errors-------\n"<<Errors<<"\n--------------------\n";
		delete[] arr;
	}
};

int main()
{
	PNG_Reader img;
	img.readFile("demo.png");
	return 0;
}