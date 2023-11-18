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

struct statistics localSearch(bool random, string neighborhood, bool steepest){

    struct statistics localStat;
    localStat.allSolutionsValue = 0;
    localStat.bestSolutionValue = 100000000;
    localStat.worstSolutionValue = 0;
    for(int k = 0; k<200; k++){
        //cout<<k<<endl;
        int solution[100];
        int notChosen[100];
        int neighbours[200][10];
        for(int i=0; i<200; i++)
        {
            int tmpNearest[10];
            for(int j=0; j<10; j++)
                tmpNearest[j] = 1000000000;
            for(int j=0; j<200; j++)
            {
                for(int l=0; l<10; l++)
                {
                    if(distances[i][j] < tmpNearest[l])
                    {
                        for(int m=9; m>l; m--)
                        {
                            tmpNearest[m] = tmpNearest[m-1];
                        }
                        tmpNearest[l] = distances[i][j];
                        break;
                    }
                }
            }
            for(int j=0; j<10;j++)
                neighbours[i][j] = tmpNearest[j];
        }

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

                /*
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
                */
                for(int i=0; i<100; i++)
                {
                    int moveValue;
                    for(int j=0; j<100; j++)
                    {
                        bool skip = true;
                        for(int l=0; l<10; l++)
                            if((neighbours[solution[(100+i-1)%100]][l] == notChosen[j]) || (neighbours[solution[(i+1)%100]][l] == notChosen[j]))
                            {
                                skip = false;
                                break;
                            }
                        if(skip)
                            continue;
                        moveValue = distances[solution[(100+i-1)%100]][notChosen[j]] + distances[notChosen[j]][solution[(i+1)%100]] - distances[solution[(100+i-1)%100]][solution[i]] - distances[solution[i]][solution[(i+1)%100]];
                        if(moveValue < bestInterMove[0])
                        {
                            bestInterMove[0] = moveValue;
                            bestInterMove[1] = i;
                            bestInterMove[2] = j;
                        }
                    }
                }





                if(neighborhood == "edges")
                {
                    int bestIntraMove[3] = {0, 0, 0};
                    for(int i=1; i<100; i++)
                    {
                        for(int j=0; j<i; j++)
                        {
                            bool skip = true;
                            for(int l=0; l<10; l++)
                                if((neighbours[solution[(100+j-1)%100]][l] == solution[i]) || (neighbours[solution[(i+1)%100]][l] == solution[j]))
                                {
                                    skip = false;
                                    break;
                                }
                            if(skip)
                                continue;
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
                /*
                test++;
                int distanceValue = distances[solution[99]][solution[0]];
                for(int s=0; s<100; s++)
                {
                    cout<<solution[s]<<" ";
                    if(s<99)
                        distanceValue += distances[solution[s]][solution[s+1]];
                }
                cout<<endl<<distanceValue<<endl<<endl;
                */
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

}

int main(){
    clock_t timer;
    vector<vector<int>> dataA;

    dataA = readCSV(pathD);
    //readDataset(dataA);

    dataToDistance(dataA);

    float p = 0.5;
    //struct statistics regStat = regreat2(p, 1, 1);

    timer = clock();
    //random
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

    ofstream outputFile("solutionCandidateD.txt");

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

    return 0;
}
