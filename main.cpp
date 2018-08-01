#include "PNG_Reader.h"
#include "CRC_code.h"


using namespace std;

int main()
{
	PNG_Reader img;
	img.readFile("simple.png");
	return 0;
}