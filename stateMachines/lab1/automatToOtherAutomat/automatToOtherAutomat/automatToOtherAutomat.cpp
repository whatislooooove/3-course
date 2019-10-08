﻿//Visual Studio 17, Садовин Владислав
//В автомате Мура, если у какой либо вершины отсутствует у, то писать у-
#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

const string MOORE_VERTEX_CHAR = "q";
const string MEALY_VERTEX_CHAR = "Z";

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Enter automatToOtherAutomat.exe <inputfile> <outputfile>" << endl;
		return 0;
	}
	int inputCharsCount = 0, outputCharsCount = 0, vertexCount = 0;
	string inputChar = "", outputChar = "", vertexValue = "", automat = "";

	ifstream inputFile(argv[1]);
	ofstream outputAutomat(argv[2]);
	if (!inputFile || !outputAutomat)
	{
		cout << "input or output file dont find" << endl;
		return 0;
	}

	inputFile >> inputCharsCount >> outputCharsCount >> vertexCount >> automat;

	string** outputTable = new string*[inputCharsCount]; //создаем таблицу вывода
	for (int i = 0; i < inputCharsCount; i++)
	{
		outputTable[i] = new string[vertexCount];
	}

	map<string, string> states;

	if (automat == "moore")
	{
		cout << "automat: moore" << endl; //добавляем вершины без выходных символов
		for (int i = 0; i < vertexCount; i++)
		{
			vertexValue = MOORE_VERTEX_CHAR;
			vertexValue += to_string(i);
			states.insert(make_pair(vertexValue, ""));
		}

		map<string, string>::iterator it = states.begin();
		for (int i = 0; i < vertexCount; i++, it++) // добавляем выходные символы к вершинам
		{
			inputFile >> outputChar;
			it->second = outputChar;
		}

		for (int i = 0; i < inputCharsCount; i++) //заполняем выход
		{
			for (int j = 0; j < vertexCount; j++)
			{
				inputFile >> inputChar;
				it = states.find(inputChar);
				outputTable[i][j] = it->first + it->second;
				outputAutomat << outputTable[i][j] << ' ';
			}
			outputAutomat << endl;
		}
	}
	else if ("mealy")
	{
		vector<string> vertexes;
		//vector<string> vertexes_x2;
		cout << "automat: mealy" << endl;
		for (int i = 0; i < inputCharsCount; i++) //заполняем таблицу c исходными состояниями (Мили)
		{
			for (int j = 0; j < vertexCount; j++)
			{
				inputFile >> vertexValue;
				outputTable[i][j] = vertexValue;
				vertexes.push_back(vertexValue);
			}
		}

		map<string, string>::iterator it = states.begin();
		int newValue = 0;
		for (int i = 0; i < inputCharsCount; i++)
		{
			for (int j = 0; j < vertexCount; j++)
			{
				it = states.find(outputTable[i][j]);
				if (it == states.end())
				{
					states.insert(make_pair(outputTable[i][j], MEALY_VERTEX_CHAR + to_string(newValue)));
					newValue++;
				}
			}
		}
		//
		it = states.begin();
		for (int i = 0; it != states.end(); it++, i++) { 
			it->second = MEALY_VERTEX_CHAR + to_string(i);
		}
		it = states.begin();
		for (int i = 0; it != states.end(); it++, i++) {
			cout << i << ") Key " << it->first << ", value " << it->second << endl;
		}
		//
		int index = 0;

		for (int i = 0; i < inputCharsCount + 1; i++)
		{
			it = states.begin();
			for (int j = 0; j < states.size(); j++)
			{
				if (i == 0)
				{
					outputAutomat << it->first.substr(2) << ' ';
					it++;
				}
				else if (i == 1)
				{
					index = stoi(it->first.substr(1, 1));
					outputAutomat << states[vertexes.at(index)] << ' ';
					it++;
				}
				else 
				{
					index = stoi(it->first.substr(1, 1));
					outputAutomat << states[vertexes.at(index + vertexCount * (i - 1))] << ' ';
					it++;
				}
	
			}
			outputAutomat << endl;
		}
	}
	else
	{
		outputAutomat << "Unknown automat" << endl;
	}
}