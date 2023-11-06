#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <cmath>
#include <random>
using namespace std;

string pathA = "Data\\TSPA.csv";
string pathB = "Data\\TSPB.csv";
string pathC = "Data\\TSPC.csv";
string pathD = "Data\\TSPD.csv";

int distances[200][200];


vector<vector<int>> readCSV(string path){

    fstream csv_file;
    csv_file.open(path, ios::in);

    string line;
    vector<vector<int>> dataset;

    while(getline(csv_file, line)){
        stringstream lineStream(line);
        string cell;
        vector<int> parsedRow;

        while(getline(lineStream, cell, ';')){
            parsedRow.push_back(stoi(cell));
        }
        dataset.push_back(parsedRow);
    }

    return dataset;
}


void readDataset(vector<vector<int>> dataset){

    for(int i=0; i<dataset.size(); i++){
        for(int j=0; j<dataset[i].size(); j++){
            cout<< dataset[i][j]<< ", ";
        }
        cout<< endl;
    }
}



void dataToDistance(vector<vector<int>> dataset)
{
    for(int i=0; i<dataset.size(); i++)
    {
        for(int j=0; j<dataset.size();j++)
        {
            if(i==j)
            {
                distances[i][j] = 0;
            }
            else
            {
                distances[i][j] = sqrt((dataset[i][0]-dataset[j][0])*(dataset[i][0]-dataset[j][0])+(dataset[i][1]-dataset[j][1])*(dataset[i][1]-dataset[j][1])) + dataset[j][2];
            }
        }
    }
/*
    for(int i=0;i<dataset.size();i++)
    {
        for(int j=0;j<dataset.size();j++)
        {
            cout<<distances[i][j]<<' ';
        }
        cout<<endl;
    }
*/
}

struct statistics
{
    int bestSolution[100];
    int bestSolutionValue;
    int worstSolutionValue;
    int allSolutionsValue;
};

struct statistics regreat(float p)
{
    struct statistics regStat;
    regStat.allSolutionsValue = 0;
    regStat.bestSolutionValue = 100000000;
    regStat.worstSolutionValue = 0;
    for(int l=0; l<200; l++)
    {
        int takenVertex = 2;
        int solution[100];
        solution[0] = l;
        int newVertex;
        int nearestVertex[2] = {1000000000, 0};
        for(int i=0; i<200; i++)
        {
            if(i==l)
            {
                continue;
            }
            if(distances[solution[0]][i] < nearestVertex[0])
            {
                nearestVertex[0] = distances[solution[0]][i];
                nearestVertex[1] = i;
            }
        }
        solution[1] = nearestVertex[1];

        while(takenVertex < 100)
        {
            int potencialVertex[int(p*(200-takenVertex))];
            for(int j=0; j<int(p*(200-takenVertex)); j++)
            {
                while(true)
                {
                    bool same = false;
                    newVertex = rand()%200;
                    for(int k=0; k<j; k++)
                    {
                        if(newVertex == potencialVertex[k])
                        {
                            same = true;
                            break;
                        }
                    }
                    for(int k=0; k<takenVertex; k++)
                    {
                        if(newVertex == solution[k])
                        {
                            same = true;
                            break;
                        }
                    }
                    if(!same)
                    {
                        break;
                    }
                }

                potencialVertex[j] = newVertex;
            }

            int bestVertex[3] = {0, 0, 0};
            for(int i=0; i<int(p*(200-takenVertex)); i++)
            {
                int bestInsert = 1000000000;
                int secoundInsert = 100000000;
                int bestInsertLocation;
                for(int j=0; j<takenVertex-1; j++)
                {
                    int tmpInsert = distances[solution[j]][potencialVertex[i]] + distances[potencialVertex[i]][solution[j+1]] - distances[solution[j]][solution[j+1]];
                    if(tmpInsert < bestInsert)
                    {
                        bestInsert = tmpInsert;
                        bestInsertLocation = j+1;
                    }
                    else if(tmpInsert < secoundInsert)
                    {
                        secoundInsert = tmpInsert;
                    }
                }
                if(secoundInsert-bestInsert > bestVertex[1])
                {
                    bestVertex[0] = potencialVertex[i];
                    //bestVertex[1] = a*(secoundInsert-bestInsert) - b*bestInsert;
                    bestVertex[1] = secoundInsert-bestInsert;
                    bestVertex[2] = bestInsertLocation;
                }
            }

            for(int i=99; i>bestVertex[2]; i--)
            {
                solution[i] = solution[i-1];
            }
            solution[bestVertex[2]] = bestVertex[0];
            takenVertex++;
        }
        int solutionDistance = distances[solution[-1]][solution[0]];
        for(int i=0; i<99; i++)
        {
            solutionDistance += distances[solution[i]][solution[i+1]];
        }

        if(solutionDistance > regStat.worstSolutionValue)
        {
            regStat.worstSolutionValue = solutionDistance;
        }
        if(solutionDistance < regStat.bestSolutionValue)
        {
            regStat.bestSolutionValue = solutionDistance;
            for(int i=0; i<100; i++)
            {
                regStat.bestSolution[i] = solution[i];
            }
        }
        regStat.allSolutionsValue += solutionDistance;
    }

    regStat.allSolutionsValue = regStat.allSolutionsValue/200;

    return regStat;
};

struct statistics regreat2(float p, float a, float b)
{
    struct statistics regStat;
    regStat.allSolutionsValue = 0;
    regStat.bestSolutionValue = 100000000;
    regStat.worstSolutionValue = 0;
    for(int l=0; l<200; l++)
    {
        int takenVertex = 2;
        int solution[100];
        solution[0] = l;
        int newVertex;
        int nearestVertex[2] = {1000000000, 0};
        for(int i=0; i<200; i++)
        {
            if(i==l)
            {
                continue;
            }
            if(distances[solution[0]][i] < nearestVertex[0])
            {
                nearestVertex[0] = distances[solution[0]][i];
                nearestVertex[1] = i;
            }
        }
        solution[1] = nearestVertex[1];

        while(takenVertex < 100)
        {
            int potencialVertex[int(p*(200-takenVertex))];
            for(int j=0; j<int(p*(200-takenVertex)); j++)
            {
                while(true)
                {
                    bool same = false;
                    newVertex = rand()%200;
                    for(int k=0; k<j; k++)
                    {
                        if(newVertex == potencialVertex[k])
                        {
                            same = true;
                            break;
                        }
                    }
                    for(int k=0; k<takenVertex; k++)
                    {
                        if(newVertex == solution[k])
                        {
                            same = true;
                            break;
                        }
                    }
                    if(!same)
                    {
                        break;
                    }
                }

                potencialVertex[j] = newVertex;
            }

            int bestVertex[3] = {0, -1000000000, 0};
            for(int i=0; i<int(p*(200-takenVertex)); i++)
            {
                int bestInsert = 1000000000;
                int secoundInsert = 100000000;
                int bestInsertLocation;
                for(int j=0; j<takenVertex-1; j++)
                {
                    int tmpInsert = distances[solution[j]][potencialVertex[i]] + distances[potencialVertex[i]][solution[j+1]] - distances[solution[j]][solution[j+1]];
                    if(tmpInsert < bestInsert)
                    {
                        bestInsert = tmpInsert;
                        bestInsertLocation = j+1;
                    }
                    else if(tmpInsert < secoundInsert)
                    {
                        secoundInsert = tmpInsert;
                    }
                }
                if(bestVertex[1] = a*(secoundInsert-bestInsert) - b*bestInsert> bestVertex[1])
                {
                    bestVertex[0] = potencialVertex[i];
                    bestVertex[1] = a*(secoundInsert-bestInsert) - b*bestInsert;
                    //bestVertex[1] = secoundInsert-bestInsert;
                    bestVertex[2] = bestInsertLocation;
                }
            }

            for(int i=99; i>bestVertex[2]; i--)
            {
                solution[i] = solution[i-1];
            }
            solution[bestVertex[2]] = bestVertex[0];
            takenVertex++;
        }
        int solutionDistance = distances[solution[-1]][solution[0]];
        for(int i=0; i<99; i++)
        {
            solutionDistance += distances[solution[i]][solution[i+1]];
        }

        if(solutionDistance > regStat.worstSolutionValue)
        {
            regStat.worstSolutionValue = solutionDistance;
        }
        if(solutionDistance < regStat.bestSolutionValue)
        {
            regStat.bestSolutionValue = solutionDistance;
            for(int i=0; i<100; i++)
            {
                regStat.bestSolution[i] = solution[i];
            }
        }
        regStat.allSolutionsValue += solutionDistance;
    }

    regStat.allSolutionsValue = regStat.allSolutionsValue/200;

    return regStat;
};


int main(){
    clock_t timer;
    vector<vector<int>> data;

    timer = clock();

    data = readCSV(pathD);
    //readDataset(dataA);

    dataToDistance(data);

    float p = 0.5;
    int bestSolution[100];
    int bestSolutionValue = 0;
    int worstSolution[100];
    int worstSolutionValue = 10000000;
    int allSolutionsValue = 0;

    struct statistics regStat = regreat2(p, 0.75, 0.25);

    cout << "Best Solution Value: " << regStat.bestSolutionValue << endl;
    cout << "Worst Solution Value: " << regStat.worstSolutionValue << endl;
    cout << "All Solutions Value: " << regStat.allSolutionsValue << endl;

    cout << "Best Solution: ";
    for (int i = 0; i < 100; i++) {
        cout << regStat.bestSolution[i];
        if (i < 99) {
            cout << ", ";
        }
    }
    cout << endl;

    ofstream outputFile("solutionws075025D.txt");

    if (outputFile.is_open()) {
        for (int i = 0; i < 100; i++) {
            outputFile << regStat.bestSolution[i] << "\n";
        }
        outputFile.close();
        cout << "Best Solution values written to best_solution.txt" << endl;
    } else {
        cerr << "Failed to open the file for writing." << endl;
    }
    timer = clock() - timer;
    cout<< endl<< "Time: "<< ((float)timer)/CLOCKS_PER_SEC<< endl;

    return 0;
}
