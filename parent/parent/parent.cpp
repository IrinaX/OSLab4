#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    STARTUPINFO start_inf;//Структура STARTUPINFO используется с функцией CreateProcess
    PROCESS_INFORMATION proc_inf;// Структура PROCESS_INFORMATION заполняется функцией CreateProcess с информацией о недавно созданном процессе и его первичном потоке

    HANDLE hIn, //дескриптор потока ввода анонимного канала
        hOut, // дескриптор потока вывода анонимного канала
        hTemp;// дескриптор для сохранения стандартного дескриптора ввода

    ZeroMemory(//заполняет блок памяти нулями
        &start_inf,// указатель на блок памяти
        sizeof(start_inf)// размер блока памяти
    );
    start_inf.cb = sizeof(start_inf);// устанавливаем размер структуры, в байтах.

    //Чтобы передать дочернему процессу дескриптор потока, необходимо определить необходимые поля структуры SECURITY_ATTRIBUTES.
    SECURITY_ATTRIBUTES attrib;
    attrib.nLength = sizeof(attrib); //размер структуры в байтах. Устанавливается равным размеру структуры типа SECURITY_ATTRIBUTES.
    attrib.lpSecurityDescriptor = 0; //Указатель на дескриптор безопасности для объекта контролирующего совместное его использование.
    // Если NULL, то с объектом будет связан дескриптор безопасности по умолчанию вызывающего процесса.

    attrib.bInheritHandle = true;//Определяет, будет ли возвращаемый дескриптор наследоваться при создании нового процесса. Если TRUE, то новый процесс наследует дескриптор.

    //создаем анонимный канал
    CreatePipe(&hOut,   // адрес переменной, в которую будет записан идентификатор канала для чтения данных
        &hIn,    // адрес переменной, в которую будет записан идентификатор канала для записи данных
        &attrib, // адрес переменной для атрибутов защиты
        0);// количество байт памяти, зарезервированной для канала. Есль 0 то размер буфера задается по умолчанию

    hTemp = GetStdHandle(STD_INPUT_HANDLE);//сохраняем дескриптор стандартного потока вывода
    SetStdHandle(STD_INPUT_HANDLE,
        hOut);//переназначаем дескриптор т.е. перенаправляем стандартный поток ввода на поток ввода канала

    char strData[50] = "Data to child";

    DWORD n;

    CreateProcess(L"D:\\notWIN\\universityDolgi\\OC\\4lab\\child\\Debug\\child.exe", 0, 0, 0, true, 0, 0, 0, &start_inf, &proc_inf);
    //true - дескрипторы берутся из текущего процесса

    SetStdHandle(STD_INPUT_HANDLE, hTemp);

    WriteFile(//пишет данные в файл с места, обозначенного указателем позиции в файле.
        hIn,// дескриптор файла
        strData, // буфер данных
        sizeof(char) * 50,// число байтов для записи
        &n,// число записанных байтов
        0);// асинхронный буфер


    Sleep(300);

    //пункт 3.2 лабораторной работы:  
    SetStdHandle(STD_INPUT_HANDLE, hOut);

    char newStrData[10][5] = {"bbb\n","zzz\n","aaa\n","yyy\n",{26}};
    // код 26 /x1A-это управляющий символ CTRL+Z. Это также EOF маркер конца файла.

    if (!CreateProcess(L"C:\\Windows\\System32\\sort.exe", 0, 0, 0, true, 0, 0, 0, &start_inf, &proc_inf)) {
        cout << "\nerror!\n" << endl;
        return 0;
    }
    SetStdHandle(STD_INPUT_HANDLE, hTemp);

    WriteFile(hIn, newStrData, sizeof(char) * 50, &n, 0);
  
    return 0;
}
