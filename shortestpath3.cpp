#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <cfloat>
#include <vector>
#include <stack>
using namespace std;

#define V 259
vector<string> stations;
unordered_map<string, string> statclr;
double graph[V][V];
int minDistance(double dist[], bool vist[]) {
    double min = DBL_MAX;
    int min_index,v;
    for (v = 0; v < V; v++)
        if (vist[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}
bool isIntersection(int si){
    int c=0;
    vector<string> st;
    vector<string> clr;
    for(int i=0;i<V;i++)
    {
        if(graph[si][i] != 0)
        {
            c++;
            st.push_back(stations[i]);
            clr.push_back(statclr[stations[i]]);
        }
    }
    if(c>2 || stations[si]=="Dwarka Sector 21")
        return true;
    return false;
}
void printSolution(vector<double>& dist, vector<int>& parent, int src, int dest) {
    cout << "Shortest route from " << stations[src] << " to " << stations[dest] << ":\n\n";
    stack<int> path;
    int current = dest;
    while (current != -1) {
        path.push(current);
        current = parent[current];
    }
    vector<int> bcn;
    int cc=0;
    while (!path.empty()) {
        string topstat = stations[path.top()];
        bcn.push_back(path.top());
        if(bcn.size() == 3)
        {
            bool isint = isIntersection(bcn[1]);
            if(isint==1)
            {
                if(statclr[stations[bcn[0]]].compare(statclr[stations[bcn[2]]]) != 0)
                    cout << endl << "Change to " << statclr[stations[bcn[2]]] << endl << endl;
            }
            cc++;
            cout << cc <<  ". " << topstat << endl;
            bcn.erase(bcn.begin());
        }
        else
        {
            cc++;
            cout << cc <<  ". " << topstat << endl;
        }
        path.pop();
    }
    cout << endl;
    cout << "Total Distance = " << dist[dest] << " km" << endl;
}

void dijkstra(double graph[V][V], int src, int dest) {
    vector<double> dist(V, DBL_MAX);
    vector<int> parent(V, -1);
    bool vist[V] = {false};

    dist[src] = 0;

    for (int count = 0; count <V-1; count++) {
        int u = minDistance(&dist[0], vist);
        vist[u] = true;
        int v;
        for (v = 0; v < V; v++){
            if (!vist[v] && graph[u][v] && dist[u] != DBL_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }
    printSolution(dist, parent, src, dest);
}
string inputStation(vector<string> statslow, string x){
    char stlt;
    while(true){
        cout << "Enter starting letter of " << x << " station: ";
        cin >> stlt;
        if(!isalpha(stlt)){
            cout << "Wrong input! Try again..." << endl;
            continue;
        }
        else
            break;
    }
    stlt = tolower(stlt);
    int ct = 0;
    vector<string> stwl;
    for(int i=0;i<V;i++){
        char ch = tolower(statslow[i][0]);
        if(ch == stlt){
            ct++;
            cout << ct << ". " << statslow[i] << endl;
            stwl.push_back(statslow[i]);
        }
    }
    int chc;
    cout << endl << "Choose one: ";
    cin >> chc;
    cout << endl << stwl[chc-1] << " chosen as " << x << endl;
    return stwl[chc-1];
}

int main() {

    cout << "=================================" << endl;
    cout << "|| Delhi Metro Journey Planner ||" << endl;
    cout << "=================================" << endl;
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    ifstream inputFile("D:/MTECH CSE/Adv Algo Lab Project/DMRC_Adj_Matrix.csv");
    string line;

    for (int i = 0; i < V; ++i) {
        getline(inputFile, line);
        stringstream ss(line);
        string value;
        for (int j = 0; j < V; ++j) {
            getline(ss, value, ',');
            graph[i][j] = stod(value);
        }
    }
    ifstream inputFile2("D:/MTECH CSE/Adv Algo Lab Project/stationcolors.txt");
    string ln;
    unordered_map<string,int> stid;
    int ctr = 0;
    while (getline(inputFile2, ln)) {
        stringstream ss(ln);
        string station, color;
        getline(ss, station, ',');
        getline(ss, color, ',');
        stations.push_back(station);
        statclr[station] = color;
        stid[station] = ctr;
        ctr++;
    }
    vector<string> statslow = stations;
    sort(statslow.begin(),statslow.end());
    string srcst = inputStation(statslow,"Source");
    cout << endl;
    string destst = inputStation(statslow,"Destination");
    int src = stid[srcst];
    int dest = stid[destst];
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    dijkstra(graph, src, dest);
    return 0;
}