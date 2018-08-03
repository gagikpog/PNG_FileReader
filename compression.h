#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char BYTE;

struct array
{
    unsigned int size;
    BYTE* data;
    BYTE& operator[](unsigned int n)
    {
        if(n < size)
            return  data[n];
        else 
            throw std::out_of_range("my array error");
    }
};

struct zlib
{
    BYTE CINFO = 0, CM = 0,FLEVEL = 0,FCHECK = 0;
    bool FDICT = false;
    void read(array arr)
    {
        if (arr.size < 4)
            throw std::runtime_error("The wrong title for Deflate.");
        //if(arr[0] != 120 || arr[1] != 218)
            //throw std::runtime_error("The wrong title for Deflate");
        /*  CINFO (Compression info)
            |  res  |   | const |   | arr[2]|
            0111 0000 = 1111 0000 & 0111 1000
            | CINFO |   |  res  |
            0000 0111 = 0111 0000 >> 4
        */
        CINFO = (arr[0] & (BYTE)240) >> 4;
        /*  СМ (Compression method)
            |   CM  |   | const |   | arr[0]|
            0000 1000 = 0000 1111 & 0111 1000
        */
        CM = (arr[0] & (BYTE)15);
        /*  FLEVEL (Compression level)
            |  res  |   | const |   | arr[1] |
            1100 0000 = 1100 0000 & 11 0 11010
            | FLEVEL|   |  res  |
            0000 0011 = 1100 0000 >> 6
        */
       FLEVEL = (arr[1] & (BYTE)192) >> 6;
        /*  FDICT (Preset dictionary)
            | FDICT |   | const |   | arr[1] |
            0000 0000 = 0010 0000 & 11 0 11010
        */
       FDICT = arr[1] & (BYTE)32;
       /*  FCHECK (check bits for CMF and FLG)
            | FCHECK|   | const |   | arr[1] |
            0001 1010 = 0001 1111 & 11 0 11010
        */
       FCHECK = (arr[1] & (BYTE)31);
    }

    void list()
    {
        std::cout<<"-----------Zilb head-------------"<<std::endl
        << "CINFO = "   << (int)CINFO    << std::endl
        << "СМ = "      << (int)CM       << std::endl
        << "FLEVEL = "  << (int)FLEVEL   << std::endl
        << "FDICT = "   << FDICT         << std::endl
        << "FCHECK = "  << (int)FCHECK   << std::endl
        <<"--------------------------------------"<< std::endl;
    }
};

array decompression(array arr)
{
    zlib zHead;
    zHead.read(arr);
    zHead.list();
    array res;
    return res;
}

#endif //COMPRESSION_H