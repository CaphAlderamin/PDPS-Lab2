#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
//#include "omp.h"
using namespace std;

const int ThreadsNumber = 6;

//Умножение матриц
__int64** MatrixMultiplication(int dimension, __int64** matrixA, __int64** matrixB)
{
    __int64** matrixC = new __int64* [dimension];
    for (int i = 0; i < dimension; i++)
    {
        matrixC[i] = new __int64[dimension];
    }

    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
        {
            __int64 result = 0;
            for (int k = 0; k < dimension; k++)
            {
                result += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = result;
        }
    return matrixC;
}
__int64** MatrixMultiplicationThreads(int dimension, __int64** matrixA, __int64** matrixB)
{
    __int64** matrixC = new __int64* [dimension];
#pragma omp parallel for num_threads(ThreadsNumber)
    for (int i = 0; i < dimension; i++)
    {
        matrixC[i] = new __int64[dimension];
    }

#pragma omp parallel for num_threads(ThreadsNumber)
    for (int i = 0; i < dimension; i++)
        for (int j = 0; j < dimension; j++)
        {
            __int64 result = 0;
            for (int k = 0; k < dimension; k++)
            {
                result += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = result;
        }
    return matrixC;
}

//Заполнение матриц
__int64** matrixRandCreation(int dimension)
{
    __int64** matrix = new __int64* [dimension];
    for (int i = 0; i < dimension; i++)
    {
        matrix[i] = new __int64[dimension];
    }

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }

    return matrix;
}

//Печать матриц
void matrixPrint(int dimension, __int64** matrixA, __int64** matrixB, __int64** matrixC)
{
    cout << endl;
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            cout << matrixA[i][j] << " ";
        }
        cout << "   ";
        for (int j = 0; j < dimension; j++)
        {
            cout << matrixB[i][j] << " ";
        }
        cout << "   ";
        for (int j = 0; j < dimension; j++)
        {
            cout << matrixC[i][j] << " ";
        }
        cout << endl;
    }
}

int secondTask()
{
    int dimension = 2048;
    int printDimension = 4;
    int repeats = 5;

    cout << "Dimension: " << dimension << "\nThread number: " << ThreadsNumber << "\nRepeats number: " << repeats << endl;

    srand(time(NULL));

    vector<chrono::duration<float>> severalThreadDuration;
    vector<chrono::duration<float>> oneThreadDuration;

    __int64** matrixA = 0;
    __int64** matrixB = 0;
    __int64** matrixC = 0;

    std::cout << endl << "Select the method:" << endl;
    std::cout << "1 - Several threads realization / 2 - One thread realization / Other - Back" << endl;
    int select = 0;
    cin >> select;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Incorrect enter" << endl;
    }
    else
    {
        if (select == 1 || select == 2)
        {
            cout << "||";
            for (int i = 0; i < repeats; i++)
                cout << "=";
            cout << "||" << endl << "||";
        }

        // =============== Многопоточное произведение матриц ===============
        if (select == 1)
        {
            // Произведение матриц
            for (int n = 0; n < repeats; n++)
            {
                cout << "=";

                //Заполнение матриц
                matrixA = matrixRandCreation(dimension);
                matrixB = matrixRandCreation(dimension);

                auto start = chrono::high_resolution_clock::now(); //Точка для начала счета времени

                matrixC = MatrixMultiplicationThreads(dimension, matrixA, matrixB);

                auto end = chrono::high_resolution_clock::now(); //Точка для конца счета времени
                severalThreadDuration.emplace_back(end - start);

                //matrixPrint(printDimension, matrixA, matrixB, matrixC); //Вывод полученной матрицы

                for (int i = 0; i < dimension; i++)
                {
                    delete[] matrixA[i];
                    delete[] matrixB[i];
                    delete[] matrixC[i];
                }
            }
            cout << "||" << endl << endl; 

            //Вывод результата для многопоточного вычисления
            float severalThreadDurationTime = 0; int i = 0;
            cout << "All several threads calculation times: ";
            for (auto& time : severalThreadDuration)
            {
                cout << endl << "Repeat " << i << ": " << time.count(); i++;
                severalThreadDurationTime += time.count();
            }
            cout << "\nGeneral time of several threads calculation    " << severalThreadDurationTime << endl;
            cout << "Average time of several threads calculation    " << severalThreadDurationTime / repeats << endl << endl;

        }
        //==================================================================

        // =============== Однопоточное произведение матриц ===============
        if (select == 2)
        {
            // Произведение матриц
            for (int n = 0; n < repeats; n++)
            {
                cout << "=";

                //Заполнение матриц
                matrixA = matrixRandCreation(dimension);
                matrixB = matrixRandCreation(dimension);

                auto start = chrono::high_resolution_clock::now(); //Точка для начала счета времени

                matrixC = MatrixMultiplication(dimension, matrixA, matrixB);

                auto end = chrono::high_resolution_clock::now(); //Точка для конца счета времени
                oneThreadDuration.emplace_back(end - start);

                //matrixPrint(printDimension, matrixA, matrixB, matrixC); //Вывод полученной матрицы
                
                for (int i = 0; i < dimension; i++)
                {
                    delete[] matrixA[i];
                    delete[] matrixB[i];
                    delete[] matrixC[i];
                }
            }
            cout << "||" << endl << endl;

            //Вывод результата для однопоточного вычисления
            float oneThreadDurationTime = 0;  int i = 0;
            cout << "All one thread calculation times: ";
            for (auto& time : oneThreadDuration)
            {
                cout << endl << "Repeat " << i << ": " << time.count(); i++;
                oneThreadDurationTime += time.count();
            }
            cout << "\nGeneral time of one thread calculation " << oneThreadDurationTime << endl;
            cout << "Average time of one thread calculation " << oneThreadDurationTime / repeats << endl << endl;
        }
        //==================================================================
    }

    // Отчистка памяти
    delete[] matrixA;
    delete[] matrixB;
    delete[] matrixC;

    return 1;
}