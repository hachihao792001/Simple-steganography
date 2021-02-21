#include "Steganography.h"
#include "File.h"

int main()
{
	cout << "Steganography don gian: Giau file vao anh BMP\nMade by Chi Hao, khong xu li input sai\n\n";
	cout << "Nhap lua chon: " << endl;
	cout << "1: hide (giau 1 file vao hinh)" << endl;
	cout << "2: extract (lay file bi giau ra khoi hinh)" << endl;

	int choice;
	string hideFile;
	string bmpFile;
	BMPFile bmp;
	int byteNum;

	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "Nhap duong dan toi file ma ban muon giau vao hinh: ";
		cin.ignore();
		getline(cin, hideFile);

		cout << "Nhap duong dan toi file hinh bmp: ";
		getline(cin, bmpFile);

		ReadBMP((char*)bmpFile.c_str(), bmp);

		byte* fileBytes;
		byteNum = GetFileBytes(hideFile, fileBytes);

		//kích thước file + 4 byte để chứa kích thước file > số pixel * (6/8)
		//vì mỗi pixel chứa được 6 bit (2 bit mỗi color channel)
		if ((double)byteNum + 4.0 > (double)bmp.dib.width * (double)bmp.dib.height * 0.75) {
			cout << "Kich thuoc file qua lon de duoc giau" << endl;
			return 0;
		}

		hide(fileBytes, byteNum, bmp);

		WriteBMP((char*)bmpFile.c_str(), bmp);
		cout << "Hide thanh cong!";
		break;

	case 2:
		cout << "Nhap duong dan toi file hinh bmp: ";
		cin.ignore();
		getline(cin, bmpFile);

		ReadBMP((char*)bmpFile.c_str(), bmp);

		byte* outputBytes;
		int byteCount = extract(bmp, outputBytes);
		CreateFileFromBytes("result", outputBytes, byteCount);
		delete[] outputBytes;

		cout << "Da tao file result!" << endl;
		break;
	}
}