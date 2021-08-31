#include <iostream>
#include "SerialPort.h"
#include <fstream>

using namespace std;

char port[12] = "\\\\.\\COM4";
byte pod[16384];
int kraj;

int main()
{
	SerialPort port(port);
	ponovo:
	if (port.isConnected())
	{
		int a;
		std::cout << "1) Citanje EEPROM-a\n";
		std::cout << "2) Brisanje EEPROM-a\n";
		std::cout << "3) Programiranje EEPROM-a\n";

		std::cout << "Unesite operaciju koju zelite da izvrsite : ";
		std::cin >> a;
		
		if (a == 3)
		{
			ifstream rom("rom.bin", ios::binary | ios::in);
			for (int i = 0; i < MAX_DATA_LENGTH; i++)
			{
				if (rom && rom.peek() == EOF)
				{
					kraj = i;
					goto kraj_unosa;
				}
				else
				{
					rom >> pod[i];
				}
			}
			kraj = MAX_DATA_LENGTH;
		kraj_unosa:
			for (int i = 0; i < kraj; i++)
			{
				if (i % 16 == 0)
				{
					printf("\n%04X    %02X", i, pod[i]);
				}
				else if (i % 8 == 0)
				{
					printf("  %02X", pod[i]);
				}
				else
				{
					printf(" %02X", pod[i]);
				}
			}
			char t[2] = "3";
			port.writeSerialPort(t, 1);
			pod[kraj] = 0;
			for (int i = 0; i < kraj; i++)
			{
				t[0] = char(pod[i]);
				char a[2] = "2";
				while (a[0] != '1')
				{
					port.readSerialPort(a, 1);
				}
				ponovo1:
				if (!port.writeSerialPort(t, 1))
				{
					goto ponovo1;
				}
				cout << i << "\t" << pod[i] <<endl;
			}
		}
		else if (a == 2)
		{
			char t[2] = "2";
			port.writeSerialPort(t, 1);
			char p[5];
			port.readSerialPort(p, 5);
		}
		else if (a == 1)
		{
			char t[2] = "1";
			port.writeSerialPort(t, 1);
			int suma = 0;
			for (int i = 0; i < 128; i ++)
			{
				char p[3];
				char a[2] = "0";
				while (a[0] != '1')
					port.readSerialPort(a, 1);
				
				port.readSerialPort(p, 2);
				
				if (i % 16 == 0)
				{
					printf("\n%04X    %c%c", i, p[0],p[1]);
				}
				else if (i % 8 == 0)
				{
					printf("  %c%c", p[0],p[1]);
				}
				else
				{
					printf(" %c%c", p[0],p[1]);
				}
			}
		}
	}
	else
	{
		goto ponovo;
	}
	return 0;
}
