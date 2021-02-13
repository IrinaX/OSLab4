#include <iostream>
using namespace std;
#define SIZE 50
int main()
{
	char data[SIZE];

	cout << endl << "Child: " << endl;

	for (int i = 0; i < SIZE; i++)
		scanf_s("%c", &data[i], 1);

	cout << data << endl;

	system("pause");
	return 0;
}
