#define PORT 1234

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
void ReceiveList(SOCKET& Connection);
void CreateDetail(SOCKET& Connection);
void EditDetail(SOCKET& Connection); void DeleteDetail(SOCKET& Connection);

#pragma warning(disable:4996)

void main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return;
	}
	std::cout << "Connected!\n";
	char ClientInput = '0';
	char directives[255];
	recv(Connection, directives, sizeof(directives), 0);

	while (ClientInput != '6')
	{
		cout << directives << "\n\t" << "Input: ";
		cin >> ClientInput;
		send(Connection, &ClientInput, 1, 0);
		switch (ClientInput)
		{
		case '1': ReceiveList(Connection); break;
		case '2': CreateDetail(Connection); break;
		case '3': EditDetail(Connection); break;
		case '4': DeleteDetail(Connection);  break;
		case '5': return;
		default:
			cout << "Wrong input" << endl;
		}
		cin.clear();
		cin.ignore(1000, '\n');
	}
	system("pause");
	return;
}

template<typename VALUE>
void CatchWrongValue(VALUE value)
{
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		throw exception("Wrong input.");
	}
}

void ReceiveList(SOCKET &Connection)
{
	int sizeoflist;
	char sizeoflist_ch[3];
	recv(Connection, sizeoflist_ch, 3, 0);
	sizeoflist = atoi(sizeoflist_ch);
	char buffer[70];
	while (sizeoflist-- > 0)
	{
		recv(Connection, buffer, sizeof(buffer), 0);
		cout << buffer << endl;
	}
}

void CreateDetail(SOCKET& Connection)
{
	char NewDetail[70];
	cout << "Input detail:";
	cin.getline(new char[1], 1);
	cin.getline(NewDetail, 70);
	send(Connection, NewDetail, 70, 0);
	char message[255];
	recv(Connection, message, 255, 0);
	cout << message << endl;
}

void EditDetail(SOCKET& Connection)
{
	char DetailName[50];
	int quantity;
	char quantity_ch[5];
	cout << "Input detail name:";
	cin >> DetailName;
	send(Connection, DetailName, 50, 0);
	char GetMessagegg[20];
	char FinalMessage[16];
	recv(Connection, GetMessagegg, 20, 0);
	if (GetMessagegg[0] == '0')
	{
		cout << "input new quantity: ";
		cin >> quantity;
		itoa(quantity, quantity_ch, 10);
		send(Connection, quantity_ch, 5, 0);
		recv(Connection, FinalMessage, 16, 0);
		cout << "\t" << FinalMessage << endl;
	}
	else
	{
		cout << GetMessagegg << endl;
		return;
	}
}

void DeleteDetail(SOCKET& Connection)
{
	char DetailName[50];
	char GMessage[20];
	cout << "Input detail name:";
	cin >> DetailName;
	send(Connection, DetailName, 50, 0);
	recv(Connection, GMessage, 20, 0);
	cout << GMessage << endl;
}