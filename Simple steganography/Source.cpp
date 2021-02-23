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
	byte* fileBytes = nullptr;
	byte* outputBytes;

	cin >> choice;
	switch (choice)
	{
	case 1:
		cout << "\nNhap duong dan toi file ma ban muon giau vao hinh: ";
		cin.ignore();
		getline(cin, hideFile);

		cout << "Nhap duong dan toi file hinh bmp: ";
		getline(cin, bmpFile);

		ReadBMP((char*)bmpFile.c_str(), bmp);

		byteNum = GetFileBytes(hideFile, fileBytes);

		//kích thước file + 4 byte để chứa kích thước file > số pixel * (6/8)
		//vì mỗi pixel chứa được 6 bit (2 bit mỗi color channel)
		if ((double)byteNum + 4.0 > (double)bmp.dib.width * (double)bmp.dib.height * 0.75) {
			cout << "Kich thuoc file qua lon de duoc giau" << endl;
			break;
		}

		hide(fileBytes, byteNum, bmp);

		WriteBMP((char*)bmpFile.c_str(), bmp);
		cout << "\nHide thanh cong " << byteNum << " bytes!";

		break;

	case 2:
		cout << "\nNhap duong dan toi file hinh bmp: ";
		cin.ignore();
		getline(cin, bmpFile);

		ReadBMP((char*)bmpFile.c_str(), bmp);

		int byteCount = extract(bmp, outputBytes);
		CreateFileFromBytes("Hidden file", outputBytes, byteCount);
		delete[] outputBytes;
		cout << "Extract thanh cong!" << endl;

		break;
	}

	cout << endl;
	system("pause");
}