#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <time.h>
#include <cmath>
#include <random>
#include <queue>
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
                distances[i][j] = sqrt((dataset[i][0]-dataset[j][0])*(dataset[i][0]-dataset[j][0])+(dataset[i][1]-dataset[j][1])*(dataset[i][1]-dataset[j][1])) + dataset[j][2] + dataset[i][2];
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

struct qElement {

    int value;

    bool nodeExchange;

    int node0; //node to exchange, node to be exchanged with and two neighborhood nodes
    int node1;
    int node2;
    int node3;

    qElement(int value, bool nodeExchange, int node0, int node1, int node2, int node3)
        : value(value)
        , nodeExchange(nodeExchange)
        , node0(node0)
        , node1(node1)
        , node2(node2)
        , node3(node3)
    {
    }
};

struct CompareValue {
    bool operator()(qElement const& e1, qElement const& e2)
    {
        return e1.value > e2.value;
    }
};

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
        solution[0] = rand() % 200;
        int newVertex;
        while(true)
        {
            newVertex = rand()%200;
            if(newVertex != solution[0])
                solution[1] = newVertex;
                break;
        }

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

            /*
            for(int s=0; s<int(p*(200-takenVertex)); s++)
            {
                cout<<potencialVertex[s]<<" ";
            }
            cout<<endl;
            */

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
        for(int i=0; i<100; i++)
        {
            solutionDistance += distances[solution[i]][solution[i+1]];
        }

/*
        for(int s=0; s<100; s++)
        {
            cout<<solution[s]<<"  ";
        }
        cout<<endl;
*/

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

struct statistics regreat2(float p, float a, float b, int start)
{
    struct statistics regStat;
    regStat.allSolutionsValue = 0;
    regStat.bestSolutionValue = 100000000;
    regStat.worstSolutionValue = 0;
    for(int l=0; l<1; l++)
    {
        int takenVertex = 2;
        int solution[100];
        solution[0] = start;
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

        /*
        while(true)
        {
            newVertex = rand()%200;
            if(newVertex != solution[0])
                solution[1] = newVertex;
                break;
        }
        */

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

            /*
            for(int s=0; s<int(p*(200-takenVertex)); s++)
            {
                cout<<potencialVertex[s]<<" ";
            }
            cout<<endl;
            */

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

/*
        for(int s=0; s<100; s++)
        {
            cout<<solution[s]<<"  ";
        }
        cout<<endl;
*/

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

struct statistics localSearch(bool random, string neighborhood, bool steepest){

    struct statistics localStat;
    localStat.allSolutionsValue = 0;
    localStat.bestSolutionValue = 100000000;
    localStat.worstSolutionValue = 0;
    for(int k = 0; k<200; k++){
        int solution[100];
        int notChosen[100];
        if(random){
            int newVertex;
            for(int i=0; i<100; i++){
                while(true){
                    bool same = false;
                    newVertex = rand()%200;
                    for(int j=0; j<i; j++){
                        if(newVertex == solution[j]){
                            same = true;
                            break;
                        }
                    }
                    if(!same){
                        solution[i] = newVertex;
                        break;
                    }
                }
            }
        }
        else{
            struct statistics regStat = regreat2(0.5, 1, 1, k);
            for(int i=0; i<100; i++)
            {
                solution[i] = regStat.bestSolution[i];
            }
        }
        int tmp = 0;
        for(int i=0; i<200; i++)
        {
            bool same = false;
            for(int j=0; j<100; j++)
            {
                if(i==solution[j])
                {
                    same = true;
                    break;
                }
            }
            if(!same)
            {
                notChosen[tmp] = i;
                tmp++;
            }
        }


        if(steepest)
        {
            int test = 0;
            while(true)
            {
                int bestInterMove[3] = {0, 0, 0};
                for(int i=0; i<100; i++)
                {
                    int moveValue;
                    for(int j=0; j<100; j++)
                    {
                        moveValue = distances[solution[(100+i-1)%100]][notChosen[j]] + distances[notChosen[j]][solution[(i+1)%100]] - distances[solution[(100+i-1)%100]][solution[i]] - distances[solution[i]][solution[(i+1)%100]];
                        if(moveValue < bestInterMove[0])
                        {
                            bestInterMove[0] = moveValue;
                            bestInterMove[1] = i;
                            bestInterMove[2] = j;
                        }
                    }
                }

                if(neighborhood == "nodes")
                {
                    int bestIntraMove[3] = {0, 0, 0};
                    for(int i=0; i<100; i++)
                    {
                        for(int j=0; j<100; j++)
                        {
                            if(i==j)
                            {
                                continue;
                            }
                            int moveDecrease = distances[solution[(100+i-1)%100]][solution[i]]+distances[solution[i]][solution[(i+1)%100]]+distances[solution[(100+j-1)%100]][solution[j]]+distances[solution[j]][solution[(j+1)%100]];
                            int moveInsrase = distances[solution[(100+i-1)%100]][solution[j]]+distances[solution[i]][solution[(j+1)%100]]+distances[solution[(100+j-1)%100]][solution[i]]+distances[solution[j]][solution[(i+1)%100]];


                            if((i-j == 1) || (i-j == -99))
                            {
                                moveDecrease -= distances[solution[j]][solution[i]];
                                moveInsrase += distances[solution[i]][solution[j]];
                            }
                            else if((j-i == 1) || (j-i == -99))
                            {
                                moveDecrease -= distances[solution[i]][solution[j]];
                                moveInsrase += distances[solution[j]][solution[i]];
                            }
                            if(moveInsrase-moveDecrease < bestIntraMove[0])
                            {
                                bestIntraMove[0] = moveInsrase-moveDecrease;
                                bestIntraMove[1] = i;
                                bestIntraMove[2] = j;
                            }
                        }
                    }

                    if(bestInterMove[0] < bestIntraMove[0])
                    {
                        tmp = solution[bestInterMove[1]];
                        solution[bestInterMove[1]] = notChosen[bestInterMove[2]];
                        notChosen[bestInterMove[2]] = tmp;
                    }
                    else if((bestInterMove[0] > bestIntraMove[0]) || ((bestInterMove[0] == bestIntraMove[0]) && (bestIntraMove[0] < 0)))
                    {
                        tmp = solution[bestIntraMove[1]];
                        solution[bestIntraMove[1]] = solution[bestIntraMove[2]];
                        solution[bestIntraMove[2]] = tmp;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    int bestIntraMove[3] = {0, 0, 0};
                    for(int i=1; i<100; i++)
                    {
                        for(int j=0; j<i; j++)
                        {
                            int moveDecrease = 0;
                            int moveIncrease = 0;
                            for(int l=j; l<=i+1; l++)
                            {
                                moveDecrease += distances[solution[(100+l-1)%100]][solution[l%100]];

                            }
                            if((j==0) && (i==99))
                            {
                                moveDecrease -= distances[solution[99]][solution[0]];
                                moveIncrease += distances[solution[0]][solution[99]];
                            }
                            for(int l=i; l>j; l--)
                            {
                                moveIncrease += distances[solution[l]][solution[l-1]];
                            }
                            moveIncrease += distances[solution[(100+j-1)%100]][solution[i]] + distances[solution[j]][solution[(i+1)%100]];

                            if(moveIncrease-moveDecrease < bestIntraMove[0])
                            {
                                bestIntraMove[0] = moveIncrease-moveDecrease;
                                bestIntraMove[1] = j;
                                bestIntraMove[2] = i;
                            }
                        }
                    }
                    if(bestInterMove[0] < bestIntraMove[0])
                    {
                        tmp = solution[bestInterMove[1]];
                        solution[bestInterMove[1]] = notChosen[bestInterMove[2]];
                        notChosen[bestInterMove[2]] = tmp;
                    }
                    else if((bestInterMove[0] > bestIntraMove[0]) || ((bestInterMove[0] == bestIntraMove[0]) && (bestIntraMove[0] < 0)))
                    {
                        for(int i=0; i<(bestIntraMove[2]-bestIntraMove[1]+1)/2; i++)
                        {
                            tmp = solution[bestIntraMove[1]+i];
                            solution[bestIntraMove[1]+i] = solution[bestIntraMove[2]-i];
                            solution[bestIntraMove[2]-i] = tmp;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            int distanceValue = distances[solution[99]][solution[0]];
            for(int s=0; s<100; s++)
            {
                if(s<99)
                    distanceValue += distances[solution[s]][solution[s+1]];
            }
            localStat.allSolutionsValue += distanceValue;
            if(localStat.bestSolutionValue > distanceValue)
            {
                localStat.bestSolutionValue = distanceValue;
                for(int i=0; i<100; i++)
                {
                    localStat.bestSolution[i] = solution[i];
                }
            }
            if(localStat.worstSolutionValue < distanceValue)
            {
                localStat.worstSolutionValue = distanceValue;
            }
        }
        else
        {
            int test = 0;
            while(true)
            {
                int solution2[100];
                int shafle = rand()%100;
                for(int i=0; i<100; i++)
                {
                    solution2[i] = solution[(i+shafle)%100];
                }
                for(int i=0; i<100; i++)
                {
                    solution[i] = solution2[i];
                }
                bool found = false;
                if(rand()%2==1)
                {
                    for(int i=0; i<100; i++)
                    {
                        int moveValue;
                        for(int j=0; j<100; j++)
                        {
                            moveValue = distances[solution[(100+i-1)%100]][notChosen[j]] + distances[notChosen[j]][solution[(i+1)%100]] - distances[solution[(100+i-1)%100]][solution[i]] - distances[solution[i]][solution[(i+1)%100]];
                            if(moveValue < 0)
                            {
                                tmp = solution[i];
                                solution[i] = notChosen[j];
                                notChosen[j] = tmp;
                                found = true;
                            }
                        }
                        if(found)
                        {
                            break;
                        }
                    }
                    if(found)
                    {
                        continue;
                    }

                    if(neighborhood == "nodes")
                    {
                        for(int i=0; i<100; i++)
                        {
                            for(int j=0; j<100; j++)
                            {
                                if(i==j)
                                {
                                    continue;
                                }
                                int moveDecrease = distances[solution[(100+i-1)%100]][solution[i]]+distances[solution[i]][solution[(i+1)%100]]+distances[solution[(100+j-1)%100]][solution[j]]+distances[solution[j]][solution[(j+1)%100]];
                                int moveInsrase = distances[solution[(100+i-1)%100]][solution[j]]+distances[solution[i]][solution[(j+1)%100]]+distances[solution[(100+j-1)%100]][solution[i]]+distances[solution[j]][solution[(i+1)%100]];


                                if((i-j == 1) || (i-j == -99))
                                {
                                    moveDecrease -= distances[solution[j]][solution[i]];
                                    moveInsrase += distances[solution[i]][solution[j]];
                                }
                                else if((j-i == 1) || (j-i == -99))
                                {
                                    moveDecrease -= distances[solution[i]][solution[j]];
                                    moveInsrase += distances[solution[j]][solution[i]];
                                }
                                if(moveInsrase-moveDecrease < 0)
                                {
                                    tmp = solution[i];
                                    solution[i] = solution[j];
                                    solution[j] = tmp;
                                    found = true;
                                    break;
                                }
                            }
                            if(found)
                            {
                                break;
                            }
                        }
                        if(found)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        for(int i=1; i<100; i++)
                        {
                            for(int j=0; j<i; j++)
                            {
                                int moveDecrease = 0;
                                int moveIncrease = 0;
                                for(int l=j; l<=i+1; l++)
                                {
                                    moveDecrease += distances[solution[(100+l-1)%100]][solution[l%100]];

                                }
                                if((j==0) && (i==99))
                                {
                                    moveDecrease -= distances[solution[99]][solution[0]];
                                    moveIncrease += distances[solution[0]][solution[99]];
                                }
                                for(int l=i; l>j; l--)
                                {
                                    moveIncrease += distances[solution[l]][solution[l-1]];
                                }
                                moveIncrease += distances[solution[(100+j-1)%100]][solution[i]] + distances[solution[j]][solution[(i+1)%100]];

                                if(moveIncrease-moveDecrease < 0)
                                {
                                    for(int m=0; m<(i-j+1)/2; m++)
                                    {
                                        tmp = solution[j+m];
                                        solution[j+m] = solution[i-m];
                                        solution[i-m] = tmp;
                                    }
                                    found = true;
                                    break;
                                }
                            }
                            if(found)
                            {
                                break;
                            }
                        }
                        if(found)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if(neighborhood == "nodes")
                    {
                        for(int i=0; i<100; i++)
                        {
                            for(int j=0; j<100; j++)
                            {
                                if(i==j)
                                {
                                    continue;
                                }
                                int moveDecrease = distances[solution[(100+i-1)%100]][solution[i]]+distances[solution[i]][solution[(i+1)%100]]+distances[solution[(100+j-1)%100]][solution[j]]+distances[solution[j]][solution[(j+1)%100]];
                                int moveInsrase = distances[solution[(100+i-1)%100]][solution[j]]+distances[solution[i]][solution[(j+1)%100]]+distances[solution[(100+j-1)%100]][solution[i]]+distances[solution[j]][solution[(i+1)%100]];


                                if((i-j == 1) || (i-j == -99))
                                {
                                    moveDecrease -= distances[solution[j]][solution[i]];
                                    moveInsrase += distances[solution[i]][solution[j]];
                                }
                                else if((j-i == 1) || (j-i == -99))
                                {
                                    moveDecrease -= distances[solution[i]][solution[j]];
                                    moveInsrase += distances[solution[j]][solution[i]];
                                }
                                if(moveInsrase-moveDecrease < 0)
                                {
                                    tmp = solution[i];
                                    solution[i] = solution[j];
                                    solution[j] = tmp;
                                    found = true;
                                    break;
                                }
                            }
                            if(found)
                            {
                                break;
                            }
                        }
                        if(found)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        for(int i=1; i<100; i++)
                        {
                            for(int j=0; j<i; j++)
                            {
                                int moveDecrease = 0;
                                int moveIncrease = 0;
                                for(int l=j; l<=i+1; l++)
                                {
                                    moveDecrease += distances[solution[(100+l-1)%100]][solution[l%100]];

                                }
                                if((j==0) && (i==99))
                                {
                                    moveDecrease -= distances[solution[99]][solution[0]];
                                    moveIncrease += distances[solution[0]][solution[99]];
                                }
                                for(int l=i; l>j; l--)
                                {
                                    moveIncrease += distances[solution[l]][solution[l-1]];
                                }
                                moveIncrease += distances[solution[(100+j-1)%100]][solution[i]] + distances[solution[j]][solution[(i+1)%100]];

                                if(moveIncrease-moveDecrease < 0)
                                {
                                    for(int m=0; m<(i-j+1)/2; m++)
                                    {
                                        tmp = solution[j+m];
                                        solution[j+m] = solution[i-m];
                                        solution[i-m] = tmp;
                                    }
                                    found = true;
                                    break;
                                }
                            }
                            if(found)
                            {
                                break;
                            }
                        }
                        if(found)
                        {
                            continue;
                        }
                    }
                    for(int i=0; i<100; i++)
                    {
                        int moveValue;
                        for(int j=0; j<100; j++)
                        {
                            moveValue = distances[solution[(100+i-1)%100]][notChosen[j]] + distances[notChosen[j]][solution[(i+1)%100]] - distances[solution[(100+i-1)%100]][solution[i]] - distances[solution[i]][solution[(i+1)%100]];
                            if(moveValue < 0)
                            {
                                tmp = solution[i];
                                solution[i] = notChosen[j];
                                notChosen[j] = tmp;
                                found = true;
                            }
                        }
                        if(found)
                        {
                            break;
                        }
                    }
                    if(found)
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            int distanceValue = distances[solution[99]][solution[0]];
            for(int s=0; s<100; s++)
            {
                if(s<99)
                    distanceValue += distances[solution[s]][solution[s+1]];
            }
            localStat.allSolutionsValue += distanceValue;
            if(localStat.bestSolutionValue > distanceValue)
            {
                localStat.bestSolutionValue = distanceValue;
                for(int i=0; i<100; i++)
                {
                    localStat.bestSolution[i] = solution[i];
                }
            }
            if(localStat.worstSolutionValue < distanceValue)
            {
                localStat.worstSolutionValue = distanceValue;
            }
        }
    }
    localStat.allSolutionsValue = localStat.allSolutionsValue/200;
    return localStat;

};


struct statistics steepestQueue () {
    struct statistics localStat;
    localStat.allSolutionsValue = 0;
    localStat.bestSolutionValue = 100000000;
    localStat.worstSolutionValue = 0;
    for(int k = 0; k<200; k++){
        int solution[100];
        int notChosen[100];
        int newVertex;
        for(int i=0; i<100; i++){
            while(true){
                bool same = false;
                newVertex = rand()%200;
                for(int j=0; j<i; j++){
                    if(newVertex == solution[j]){
                        same = true;
                        break;
                    }
                }
                if(!same){
                    solution[i] = newVertex;
                    break;
                }
            }
        }
        int tmp = 0;
        for(int i=0; i<200; i++)
        {
            bool same = false;
            for(int j=0; j<100; j++)
            {
                if(i==solution[j])
                {
                    same = true;
                    break;
                }
            }
            if(!same)
            {
                notChosen[tmp] = i;
                tmp++;
            }
        }
        ///*
        int solutionValue = 0;
        for(int i=0; i<100; i++) {
            solutionValue += distances[solution[1]][solution[(i+1)%100]];
        }
        //cout << endl << solutionValue<<" random";
        //*/
        priority_queue<qElement, vector<qElement>, CompareValue> Q;
        priority_queue<qElement, vector<qElement>, CompareValue> Qinverted;
        priority_queue<qElement, vector<qElement>, CompareValue> Qtmp;
        int moveValue = 0;
        int nodes[4];
        for(int i=0; i<100; i++) {
            for(int j=0; j<100; j++) {
                nodes[0] = solution[i];
                nodes[1] = notChosen[j];
                nodes[2] = solution[(100+i-1)%100];
                nodes[3] = solution[(i+1)%100];
                moveValue = distances[nodes[2]][nodes[1]] + distances[nodes[3]][nodes[1]] - distances[nodes[2]][nodes[0]] - distances[nodes[3]][nodes[0]];
                if(moveValue<0) {
                    Q.push(qElement(moveValue, true, nodes[0], nodes[1], nodes[2], nodes[3]));
                }
            }
        }
        for(int i=0; i<100; i++) {
            for(int j=i+1; j<100; j++) {
                nodes[0] = solution[i];
                nodes[1] = solution[(i+1)%100];
                nodes[2] = solution[j];
                nodes[3] = solution[(j+1)%100];
                moveValue = distances[nodes[0]][nodes[2]] + distances[nodes[1]][nodes[3]] - distances[nodes[0]][nodes[1]] - distances[nodes[2]][nodes[3]];
                if(moveValue<0) {
                    Q.push(qElement(moveValue, false, nodes[0], nodes[1], nodes[2], nodes[3]));
                }
            }
        }
        while(!Q.empty()) {
            //cout << Q.size() << endl;
            /*
            int solutionValue = 0;
            for(int i=0; i<100; i++) {
                solutionValue += distances[solution[1]][solution[(i+1)%100]];
                cout<<solution[i]<<" ";
            }
            cout << endl << solutionValue;

            */
            qElement e = Q.top();
            Q.pop();
            //cout<<"  "<<e.value<<"  "<<e.nodeExchange<<"  "<<e.node0<<"  "<<e.node1<<"  "<<e.node2<<"  "<<e.node3<<endl;
            if(e.nodeExchange) {
                int found = -1;
                for(int i=0; i<100; i++) {
                    if(notChosen[i]==e.node1) {
                        found = i;
                        break;
                    }
                }
                int found2 = -1;
                if(found!=-1) {
                    for(int i=0; i<100; i++) {
                        if((solution[(100+i-1)%100] == e.node2 || solution[(100+i-1)%100] == e.node3) && (solution[(i+1)%100] == e.node2 || solution[(i+1)%100] == e.node3) && solution[i] == e.node0) {
                            found2 = i;
                            break;
                        }
                    }
                }
                if(found2!=-1) {
                    int tmp = notChosen[found];
                    notChosen[found] = solution[found2];
                    solution[found2] = tmp;
                    for(int j = (100+found2-1)%100; j<((100+found2-1)%100+3); j++) {
                        for(int i=0; i<100; i++) {
                            nodes[0] = solution[j%100];
                            nodes[1] = notChosen[i];
                            nodes[2] = solution[(100+j-1)%100];
                            nodes[3] = solution[(j+1)%100];
                            moveValue = distances[nodes[2]][nodes[1]] + distances[nodes[3]][nodes[1]] - distances[nodes[2]][nodes[0]] - distances[nodes[3]][nodes[0]];
                            if(moveValue<0) {
                                Q.push(qElement(moveValue, true, nodes[0], nodes[1], nodes[2], nodes[3]));
                            }
                        }
                    }
                    for(int i=(100+found2-1)%100; i<((100+found2-1)%100+2); i++) {
                        for(int j=0; j<100; j++) {
                            if(i==j) {
                                continue;
                            }
                            nodes[0] = solution[i%100];
                            nodes[1] = solution[(i+1)%100];
                            nodes[2] = solution[j];
                            nodes[3] = solution[(j+1)%100];
                            moveValue = distances[nodes[0]][nodes[2]] + distances[nodes[1]][nodes[3]] - distances[nodes[0]][nodes[1]] - distances[nodes[2]][nodes[3]];
                            if(moveValue<0) {
                                Q.push(qElement(moveValue, false, nodes[0], nodes[1], nodes[2], nodes[3]));
                            }
                        }
                    }
                }
            }
            else {
                int found1 = -1;
                bool inverted1;
                for(int i=0; i<100; i++) {
                    if(solution[i]==e.node0 && solution[(i+1)%100]==e.node1) {
                        found1 = i;
                        inverted1 = false;
                        break;
                    }
                    if(solution[i]==e.node1 && solution[(i+1)%100]==e.node0) {
                        found1 = i;
                        inverted1 = true;
                        break;
                    }
                }
                int found2 = -1;
                bool inverted2;
                if(found1!=-1) {
                    for(int i=0; i<100; i++) {
                        if(solution[i]==e.node2 && solution[(i+1)%100]==e.node3) {
                            found2 = i;
                            inverted2 = false;
                            break;
                        }
                        if(solution[i]==e.node3 && solution[(i+1)%100]==e.node2) {
                            found2 = i;
                            inverted2 = true;
                            break;
                        }
                    }
                }
                if(found2!=-1) {
                    if(inverted1!=inverted2) {
                        Qinverted.push(e);
                    }
                    else {
                        while (!Qinverted.empty()) {
                            qElement eInv = Qinverted.top();
                            Qinverted.pop();
                            Q.push(eInv);
                        }
                        if(found1>found2) {
                            for(int i=0; i<(found1-found2+1)/2; i++) {
                                tmp = solution[(found2+1+i)%100];
                                solution[(found2+1+i)%100] = solution[(100+found1-i)%100];
                                solution[(100+found1-i)%100] = tmp;
                            }
                        }
                        else {
                            if(found1<found2)
                            {
                                for(int i=0; i<(found2-found1+1)/2; i++) {
                                    tmp = solution[(found1+1+i)%100];
                                    solution[(found1+1+i)%100] = solution[(100+found2-i)%100];
                                    solution[(100+found2-i)%100] = tmp;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        for(int i=found1; i<found1+2; i++) {
                            for(int j=0; j<100; j++) {
                                nodes[0] = solution[i%100];
                                nodes[1] = notChosen[j];
                                nodes[2] = solution[(100+i-1)%100];
                                nodes[3] = solution[(i+1)%100];
                                moveValue = distances[nodes[2]][nodes[1]] + distances[nodes[3]][nodes[1]] - distances[nodes[2]][nodes[0]] - distances[nodes[3]][nodes[0]];
                                if(moveValue<0) {
                                    Q.push(qElement(moveValue, true, nodes[0], nodes[1], nodes[2], nodes[3]));
                                }
                            }
                        }
                        for(int i=found2; i<found2+2; i++) {
                            for(int j=0; j<100; j++) {
                                nodes[0] = solution[i%100];
                                nodes[1] = notChosen[j];
                                nodes[2] = solution[(100+i-1)%100];
                                nodes[3] = solution[(i+1)%100];
                                moveValue = distances[nodes[2]][nodes[1]] + distances[nodes[3]][nodes[1]] - distances[nodes[2]][nodes[0]] - distances[nodes[3]][nodes[0]];
                                if(moveValue<0) {
                                    Q.push(qElement(moveValue, true, nodes[0], nodes[1], nodes[2], nodes[3]));
                                }
                            }
                        }
                        for(int i=0; i<100; i++) {
                            if(i!=found2) {
                                nodes[0] = solution[found2];
                                nodes[1] = solution[(found2+1)%100];
                                nodes[2] = solution[i];
                                nodes[3] = solution[(i+1)%100];
                                moveValue = distances[nodes[0]][nodes[2]] + distances[nodes[1]][nodes[3]] - distances[nodes[0]][nodes[1]] - distances[nodes[2]][nodes[3]];
                                if(moveValue<0) {
                                    Q.push(qElement(moveValue, false, nodes[0], nodes[1], nodes[2], nodes[3]));
                                }
                            }
                            if(i!=found1) {
                                nodes[0] = solution[found1];
                                nodes[1] = solution[(found1+1)%100];
                                nodes[2] = solution[i];
                                nodes[3] = solution[(i+1)%100];
                                moveValue = distances[nodes[0]][nodes[2]] + distances[nodes[1]][nodes[3]] - distances[nodes[0]][nodes[1]] - distances[nodes[2]][nodes[3]];
                                if(moveValue<0) {
                                    Q.push(qElement(moveValue, false, nodes[0], nodes[1], nodes[2], nodes[3]));
                                }
                            }
                        }
                    }
                }
            }

        }
        int distanceValue = 0;
        for(int i=0; i<100; i++) {
            distanceValue += distances[solution[i]][solution[(i+1)%100]];
        }
        localStat.allSolutionsValue += distanceValue;
        if(localStat.bestSolutionValue > distanceValue)
        {
            localStat.bestSolutionValue = distanceValue;
            for(int i=0; i<100; i++)
            {
                localStat.bestSolution[i] = solution[i];
            }
        }
        if(localStat.worstSolutionValue < distanceValue)
        {
            localStat.worstSolutionValue = distanceValue;
        }
    }
    localStat.allSolutionsValue = localStat.allSolutionsValue/200;
    return localStat;
};

int main(){
    clock_t timer;
    vector<vector<int>> data;

    data = readCSV(pathD);
    //readDataset(dataA);

    dataToDistance(data);

    timer = clock();
    //struct statistics localStat1 = steepestQueue();
    struct statistics localStat1 = localSearch(true, "edges", true);
    timer = clock() - timer;
    cout << "All Solutions Value: " << localStat1.allSolutionsValue << endl;
    cout << "Best Solution Value: " << localStat1.bestSolutionValue << endl;
    cout << "Worst Solution Value: " << localStat1.worstSolutionValue << endl;
    cout << localStat1.allSolutionsValue << " (" << localStat1.bestSolutionValue << " - " << localStat1.worstSolutionValue << ")" << endl;
    cout << "Best Solution: ";
    for (int i = 0; i < 100; i++) {
        cout << localStat1.bestSolution[i];
        if (i < 99) {
            cout << ", ";
        }
    }
    cout << endl;

    ofstream outputFile("solutionSteepestD.txt");

    if (outputFile.is_open()) {
        for (int i = 0; i < 100; i++) {
            outputFile << localStat1.bestSolution[i] << "\n";
        }
        outputFile.close();
        cout << "Best Solution values written to best_solution.txt" << endl;
    } else {
        cerr << "Failed to open the file for writing." << endl;
    }

    cout<< endl<< "Time: "<< ((float)timer)/CLOCKS_PER_SEC<< endl;

    //struct statistics localStat2 = localSearch(true, "edges", true);
    //cout<<endl<<localStat2.allSolutionsValue/200<<"  "<<localStat2.bestSolutionValue<<" "<<localStat2.worstSolutionValue;
    //cout<<endl<<distances[156][166]<<" "<<distances[166][176]<<" "<<distances[156][66]<<" "<<distances[66][176];
    return 0;
}
