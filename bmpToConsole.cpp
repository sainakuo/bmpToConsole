#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <istream>

using namespace std;

//biBitCount = 24, на каждый цвет по 8 бит
//Длина каждой строки выровнена до кратного четырех. То есть для изображения шириной 3 точки длина строки равна 12 байт (3 точки по 3 байта, всего 9 байт, затем увеличиваем длину строки до кратного 4
//Строчки в bmp-файле хранятся в обратном порядке, то есть в начале идет самая нижняя строка изображения. Внутри строки байты имеют порядок BGR BGR ...

int main()
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	string fileName = "test.bmp";
	ifstream fin(fileName, ios_base::binary);

	int mx, my;

	if (fin.is_open())
	{
		fin.read((char*)&bfh, sizeof(bfh));
		fin.read((char*)&bih, sizeof(bih));

		if (bih.biBitCount != 24)
		{
			cout << "file is not 24-bit!" << endl;
			return 1;
		}

		mx = bih.biWidth;
		my = bih.biHeight;

		int mx3 = mx * 3;
		int extrabytes = 0; 
		RGBTRIPLE tempRGB;
		unsigned char extraByte; //считываем сюда лишний байт

		//лишние байты до кратного 4
		while (mx3 % 4 != 0)
		{
			mx3++;
			extrabytes++;
		}

		char** v = new char*[my]; //выводимый на экран массив

		for (int i = 0; i < my; i++)
		{
			v[i] = new char[mx];
		}

		for (int i = 0; i < my; i++)
		{
			for (int j = 0; j < mx; j++)
			{				
				fin.read((char*)&tempRGB, sizeof(tempRGB));

				//белый
				if (tempRGB.rgbtRed == 255 && tempRGB.rgbtGreen == 255 && tempRGB.rgbtBlue == 255)
				{
					v[i][j] = '-';
				}

				//черный
				else if (tempRGB.rgbtRed == 0 && tempRGB.rgbtGreen == 0 && tempRGB.rgbtBlue == 0)
				{
					v[i][j] = '*';
				}
				else
				{
					cout << "file is not black and white! " << endl;
					return 2;
				}			
			}
		
			for (int k = 0; k < extrabytes; k++)
			{
				fin.read((char*)&extraByte, sizeof(extraByte));
			}
		}

		//печатаем рисунок. Переворачиваем, т.к. строчки в bmp - файле хранятся в обратном порядке
		for (int i = my - 1; i >= 0; i--)
		{
			for (int j = 0; j < mx; j++)
			{
				cout << v[i][j] << " ";
			}
			cout << endl;
		}

		for (int i = 0; i < my; i++)
		{
			delete[] v[i];
		}

		delete[] v;
	}
	else
	{
		cout << "can't open bmp file!" << endl;
	}

	return 0;
}


/*
struct BITMAPFILEHEADERg
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
};

struct BITMAPINFOHEADERg
{
	DWORD  biSize;
	LONG   biWidth;
	LONG   biHeight;
	WORD   biPlanes;
	WORD   biBitCount;
	DWORD  biCompression;
	DWORD  biSizeImage;
	LONG   biXPelsPerMeter;
	LONG   biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
};

struct RGBTRIPLE
{
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
};
*/