#include "PNG_Reader.h"
#include "CRC_code.h"


using namespace std;

int main()
{
	PNG_Reader img;
	img.readFile("demo.png");
	return 0;
}