#include <bits/stdc++.h>
using namespace std;

class Graf{
private:
    int nrNoduri;
    vector<vector<int>> listaVecini;

    //setteri
    void setNrNoduri(int);
    void setListaVecini();
    void setListaVeciniCosturi();

    void adaugaMuchie(int, int);
    void adaugaMuchieCost(int, int, int);

    //metode
    void DFS(int, vector<int>&);
    void BFS(int, vector<int>&);
    void DFS_CTC(int, vector<int>&, vector<int>&, stack<int>&, vector<bool>&, int&);
    vector<int> APM();
    vector<int> EulerCircuit();

public:
    Graf();
    Graf(int);
    Graf(int, vector<vector<int>>);
    
    int getNrNoduri() const;
    void printListaVecini() const;

    // DFS
    int nrCompConexe();

    // BFS
    vector<int> BFSInfoarena(int);

    // CTC
    void CompTareConexe(ofstream&);

    // Sortare topologica
    vector<int> SortareTop();

    // Havel Hakimi
    bool HavelHakimi(vector<int>);

    // APM
    void printAPM(ofstream&);

    // Dijkstra
    void Dijkstra();

   // ciclu eulerian
    void hasEulerCircuit(ofstream& g);

    // hamilton
    int hamilton(vector<vector<pair<int,int>>>);

    // Royfloyd
    vector<vector<int>> Royfloyd();

};

vector<vector<int>> Graf::Royfloyd(){
    vector<vector<int>> m = this->listaVecini;
    for (int i = 0; i < getNrNoduri(); i++){
        for (int j = 0; j < getNrNoduri(); j++){
            for (int k = 0; k < getNrNoduri(); k++){
                if (m[j][i] !=0 && m[i][k] != 0 && m[j][i] + m[i][k] < m[j][k]){
                    m[j][k] = m[j][i] + m[i][k];
                }
            }
        }
    }
    return m;
}


Graf::Graf(int n, vector<vector<int>> lv){
    this->nrNoduri = n;
    this->listaVecini = lv;
}

int Graf::hamilton(vector<vector<pair<int,int>>> l){
    vector<vector<int>> minCost((1 << getNrNoduri()), vector<int>(getNrNoduri(), 1e9));
    minCost.resize((1 << getNrNoduri()));

    minCost[1][0] = 0;

    for (int i = 0; i < (1 << getNrNoduri()); i++){
        for (int j = 0; j < getNrNoduri(); j++){
            if ((i & (1 << j))){
                for (auto elem: l[j]){
                    if ((i & (1 << elem.first))){
                        minCost[i][j] = min(minCost[i][j], minCost[i ^ (1 << j)][elem.first] + elem.second);
                    }
                }
            }
        }
    }

    int sol = 1e9;
    for (auto elem : l[0]){
        sol = min(sol, minCost[(1 << getNrNoduri()) - 1][elem.first] + elem.second);
    }

    return sol;
}

void Graf::adaugaMuchieCost(int nod1, int nod2, int cost){
    listaVecini[nod1][nod2] = cost;
}

void Graf::hasEulerCircuit(ofstream& g){
    int odd = 0;
    for (int i = 0; i < getNrNoduri(); i++){
        if (listaVecini[i].size() % 2 != 0)
            odd ++;
    }
    if (odd == 0){
        vector<int> rez;
        rez = EulerCircuit();
        for(int i = 0; i < rez.size(); i++)
            g << rez[i] + 1<< " ";
    }
    else
        g << -1;
}

vector<int> Graf::EulerCircuit(){
    stack<int> st;
    vector<int> path;

    int nod, edge;
    st.push(0);

    while (st.empty() == false){
        nod = st.top();
        if (listaVecini[nod].size() > 0){
            edge = listaVecini[nod][0];
            listaVecini[nod].erase(listaVecini[nod].begin());

            for (int i = 0; i < listaVecini[edge].size(); i++){
                if (listaVecini[edge][i] == nod){
                    listaVecini[edge].erase(listaVecini[edge].begin() + i);
                    break;
                }
            }
            st.push(edge);
        }
        else{
            st.pop();
            path.push_back(nod);
        }
    }

    return path;
}

Graf::Graf(int n){
    this->nrNoduri = n;
    for (int i = 1; i <= nrNoduri; i++){
        vector<int> aux;
        listaVecini.push_back(aux);
    }
}

Graf::Graf(){

}

void Graf::adaugaMuchie(int m1, int m2){
    listaVecini[m1].push_back(m2);
}

int Graf::getNrNoduri() const{
    return this->nrNoduri;
}

void Graf::setNrNoduri(int n){
    this->nrNoduri = n;
}

void Graf::setListaVecini(){
    for (int i = 0; i < this->getNrNoduri(); i++){
        vector<int> aux;
        listaVecini.push_back(aux);
    }
}

void Graf::setListaVeciniCosturi(){
    for (int i = 0; i < this->getNrNoduri(); i++){
        vector<int> aux(getNrNoduri(), 0);
        listaVecini.push_back(aux);
    }
}

void Graf::printListaVecini() const{
    for (int i = 0; i < this->getNrNoduri(); i++){
        cout << i + 1 << ": ";
        for (int j = 0; j < listaVecini[i].size(); j++){
            cout << listaVecini[i][j] + 1 << " ";
        }
        cout << "\n";
    }
}

void Graf::DFS(int nod, vector<int> &vizitat){
    vizitat[nod] = 1; // vizitez nodul curent

    // pentru fiecare vecin al nodului curent
    for (auto i : listaVecini[nod]){
        if (vizitat[i] == 0){ // daca nu e vizitat atunci facem dfs pe el (avem i-1 pentru ca indicii pornesc de la 0 dar nodul meu e numerotat de la 1)
            DFS(i, vizitat);
        }
    }
}

int Graf::nrCompConexe(){
    vector<int> vizitat(this->getNrNoduri(), 0); // vector care retine daca un nod a fost vizitat sau nu
    int nrComponenteConexe = 0;

    for(int i = 0; i < this->getNrNoduri(); i++){
        if (vizitat[i] == 0){
            nrComponenteConexe ++;
            DFS(i, vizitat);
        }
    }

    return nrComponenteConexe;
}

void Graf::BFS(int s, vector<int> &distanta){
    vector<int> vizitat (this->getNrNoduri(), 0); // vector care retine care retin nodurile vizitate
    queue<int> coada;   // coada pentru bfs in care retin nodurile care urmeaza sa le parcurg

    vizitat[s - 1] = 1; // pornesc de la nodul de start dat in cerinta
    coada.push(s);  // adaug in coada nodul meu de start

    // cat timp coada nu e goala
    while(coada.empty() == false){
        int nod = coada.front(); // in nod retin nodul curent pe care il parcurg (il scot din coada)
        coada.pop(); // il scot din coada pentru ca l-am parcurs deja

        // adaug in coada fiecare vecin al nodului meu care nu a fost inca vizitat
        for (int i = 0 ; i < listaVecini[nod - 1].size(); i++){
            if (vizitat[listaVecini[nod - 1][i] - 1] == 0){ // daca nu a fost vizitat
                vizitat[listaVecini[nod - 1][i] - 1] = 1;   // il marchez ca fiind vizitat
                coada.push(listaVecini[nod - 1][i]);    // il adaug in coada
                distanta[listaVecini[nod - 1][i] - 1] = distanta[nod - 1] + 1;  // ii adaug distanta in vectorul de BFSInfoarena
            }
        }
    }

}

vector<int> Graf::BFSInfoarena(int s){
    vector<int> distanta (this->getNrNoduri() , 0);

    BFS(s, distanta);

    return distanta;
}

void Graf::DFS_CTC(int nod_curent, vector<int> &descoperit, vector<int> &low, stack<int> &stiva, vector<bool> &membruStiva, int &nr){
    static int timp = 0;
    descoperit[nod_curent] = low[nod_curent] = timp++;
    stiva.push(nod_curent);
    membruStiva[nod_curent] = true;

    // pentru fiecare vecin al nodului curent verificam daca a fost vezitat, daca nu a fost vizitat facem dfs pe el
    for (auto elem: listaVecini[nod_curent]){
        if (descoperit[elem] == -1){
            DFS_CTC(elem, descoperit, low, stiva, membruStiva, nr);
        }
        // daca elementul nostru se afla te stiva atunci modificam valoarea low a nodului in minimul dintre low nodului curent si low vecinului curent
        if(membruStiva[elem] == true){
            low[nod_curent] = min(low[nod_curent], low[elem]);
        }
    }
    // daca avem ca timpul lui de descoperire este identic cu low atunci am gasit o componeneta tare conexa
    if (descoperit[nod_curent] == low[nod_curent]){
        // parcurgem stiva pana dam de nodul nostru curent
        // adica fiecare nod pe care l-am descoperit dupa nodul curent face parte din componenta noastra tare conexa
        for (int nod = stiva.top();;nod = stiva.top()){
            stiva.pop();
            membruStiva[nod] = false;
            low[nod] = descoperit[nod_curent];
            if (nod == nod_curent)
                break;
        }
        nr++;
    }
}

void Graf::CompTareConexe(ofstream& g){
    vector<int> descoperit(this->getNrNoduri(), -1);
    vector<int> low(this->getNrNoduri(), -1);
    stack<int> stiva;
    vector<bool> membruStiva(this->getNrNoduri(), false);
    int nr = 0; // numarul componenetelor tare conexe

    for (int i = 0; i < this->getNrNoduri(); i++){
        if (descoperit[i] == -1)
            DFS_CTC(i, descoperit, low, stiva, membruStiva, nr);
    }
    g << nr << "\n";
    set<int> multime;
    for (int i = 0; i < this->getNrNoduri(); i++){
        multime.insert(low[i]);
    }

    for (auto elem : multime){
        for (int i = 0; i < this->getNrNoduri(); i++){
            if (low[i] == elem)
                g << i + 1<< " ";
        }
    g << "\n";
    }
}

vector<int> Graf::SortareTop(){
    vector<int> in_degree(this->getNrNoduri(),0);
    queue<int> q;
    vector<int> order;
    int nod;

    for (int i = 0; i < this->getNrNoduri(); i++){
        for (auto elem: listaVecini[i]){
            in_degree[elem]++;
        }
    }

    for (int i = 0; i < this->getNrNoduri(); i++){
        if (in_degree[i] == 0)
            q.push(i);
    }

    while(q.empty() == 0){
        nod = q.front();
        q.pop();
        order.push_back(nod);
        for (auto elem: listaVecini[nod]){
            in_degree[elem]--;
            if (in_degree[elem] == 0)
                q.push(elem);
        }
    }

    return order;
}

bool Graf::HavelHakimi(vector<int> grad){
    while(true){
        // sortam elementele crescator
        sort(grad.begin(), grad.end(), greater<int>());

        int count = 0;

        // verificam daca toate elementele sunt egale cu 0
        for (auto elem: grad)
            if (elem != 0)
                count++;


        // daca toate sunt 0 atunci insemana ca se paote construi un graf
        if (count == 0)
            return true;


        int poz = grad.size();

        // caut pozitia primului 0 si il retin in poz
        for (int i = 0; i < grad.size(); i++){
            if (grad[i] == 0){
                poz = i;
                break;
                }
        }

        // daca gradul curent e mai mare decat numarul de noduri disponibile atunci nu putem sa construim graful
        if (grad[0] > poz - 1)
            return false;

        for (int i = 1; i < poz; i++){
            grad[i]--; // scad gradul nodului la care pot sa duc o muchie
            grad[0]--; // scad si gradul nodului din care duc muchii
            if (grad[i] < 0) // daca dau de o valoare negativa inseamna ca nu pot sa fac un graf
                return false;
        }

} }

vector<int> Graf::APM(){
    vector<int> parinte(getNrNoduri(), -1);
    vector<int> id(getNrNoduri(),  INT_MAX);
    vector<bool> vizitat(getNrNoduri(), false);

    id[0] = 0;

    for (int i = 0; i < getNrNoduri(); i++){
        // alegem muchia de cost minim ale carei noduri nu au fost adaugate in APM
        int min = INT_MAX;
        int idMin;
        for (int j = 0; j < getNrNoduri(); j++){
            if (vizitat[j] == false && id[j] < min){
                min = id[j];
                idMin = j;
            }
        }
        vizitat[idMin] = true;

        for (int j = 0; j < getNrNoduri(); j++){
            if (listaVecini[idMin][j] && vizitat[j] == false && listaVecini[idMin][j] < id[j]){
                parinte[j] = idMin;
                id[j] = listaVecini[idMin][j];
            }
        }
    }

    return parinte;
}

void Graf::printAPM(ofstream& g){
    vector<int> parinte = this->APM();
    int suma = 0;

    for (int i = 1; i < listaVecini.size(); i++){
        suma += listaVecini[i][parinte[i]];
    }
    g << suma << "\n";

    g << parinte.size() - 1;

    for (int i = 1; i < listaVecini.size(); i++){
       g << "\n" << parinte[i] + 1 << " " << i + 1;
    }
}

void Graf::Dijkstra(){
    
}


int main(){
    // ifstream f("dfs.in");
    // ofstream g("dfs.out");

    // ifstream f("bfs.in");
    // ofstream g("bfs.out");

    // ifstream f("ctc.in");
    // ofstream g("ctc.out");

    // ifstream f("sortaret.in");
    // ofstream g("sortaret.out");

    // ifstream f("hh.in");
    // ofstream g("hh.out");

    // ifstream f("apm.in");
    // ofstream g("apm.out");

    // ifstream f("dijkstra.in");
    // ofstream g("dijkstra.out");

    // ifstream f("ciclueuler.in");
    // ofstream g("ciclueuler.out");

    //ifstream f("hamilton.in");
    //ofstream g("hamilton.out");

    // ifstream f("royfloyd.in");
    // ofstream g("royfloyd.out");

    // --------------number of conex components---------------- 
    // int n,
    //     m, 
    //     nod1, // primul nod al muchiei
    //     nod2; // al doilea nod al muchiei

    // f >> n >> m;

    // vector<vector<int>> lv;

    // for (int i = 0; i < n; i++){
    //     vector<int> aux;
    //     lv.push_back(aux);
    // }

    // for (int i = 0; i < m; i++){
    //     f >> nod1;
    //     f >> nod2;
    //     lv[nod1 - 1].push_back(nod2 -1);
    //     lv[nod2 - 1].push_back(nod1 - 1);
    // }
    // Graf g1(n, lv);
    // g << g1.nrCompConexe();

    // --------------minimum distance from start node to the other nodes---------------- 
    //     int n,
    //         m,
    //         s;

    //     f >> n >> m >> s;

    //     int nod1,
    //         nod2;

    //     vector<vector<int>> lv;
    //     for (int i = 0; i < n; i++){
    //         vector<int> aux;
    //         lv.push_back(aux);
    //     }

    //     for (int i = 1; i <= m; i++){
    //         f >> nod1 >> nod2;
    //         lv[nod1 - 1].push_back(nod2);
    //     }

    //     Graf g1(n, lv);
    
    //     vector<int> distanta;
    //     distanta = g1.BFSInfoarena(s);

    //     for (int i = 0 ; i < n; i++){
    //         // daca distanta este 0 si nodul respectiv nu este nodul de start atunci inseamna ca nu putem sa ajungem la nodul respectiv deci distanta de la s la el este - 1
    //         if (distanta[i] == 0 && i + 1 != s){
    //             distanta[i] = -1;
    //         }
    //         g << distanta[i] << " ";
    //    }

    // --------------strong connected components---------------- 
    // int n,
    //     m;
    // f >> n >> m;
    // int nod1,
    //     nod2;

    // vector<vector<int>> lv;
    // for (int i = 0; i < n; i++){
    //     vector<int> aux;
    //     lv.push_back(aux);
    // }

    // for (int i = 1; i <= m; i++){
    //     f >> nod1 >> nod2;
    //     lv[nod1- 1].push_back(nod2 - 1);
    // }
    // Graf g1(n, lv);
    // g1.CompTareConexe(g);

    // --------------topological sort---------------- 
    // int n,
    //     m;
    // f >> n >> m;
    // int nod1,
    //     nod2;

    // vector<vector<int>> lv;
    // for (int i = 0; i < n; i++){
    //     vector<int> aux;
    //     lv.push_back(aux);
    // }

    // for (int i = 1; i <= m; i++){
    //     f >> nod1 >> nod2;
    //     lv[nod1 - 1].push_back(nod2 - 1);
    // }
    // Graf g1(n, lv);
    // vector<int> rez;
    // rez = g1.SortareTop();
    // for (int i = 0; i < n; i++){
    //     g << rez[i] + 1<< " ";
    // }

    // --------------can sequence form a graph---------------- 
    // int n, x;
    // f >> n;
    // Graf g1(n);
    // vector<int> grad;

    // for (int i = 0; i < n; i++){
    //     f >> x;
    //     grad.push_back(x);
    // }
    // g << g1.HavelHakimi(grad);
    
    // --------------eulerian cycle---------------- 
    // int n,
    //     m, 
    //     nod1, // primul nod al muchiei
    //     nod2; // al doilea nod al muchiei

    // f >> n >> m;

    // vector<vector<int>> lv;

    // for (int i = 0; i < n; i++){
    //     vector<int> aux;
    //     lv.push_back(aux);
    // }

    // for (int i = 0; i < m; i++){
    //     f >> nod1;
    //     f >> nod2;
    //     lv[nod1 - 1].push_back(nod2 -1);
    //     lv[nod2 - 1].push_back(nod1 - 1);
    // }
    // Graf g1(n, lv);
    // g1.hasEulerCircuit(g);

    // --------------minimum cost hamiltonian cycle---------------- 
    // int n, m;
    // f >> n >> m;

    // Graf g1(n);

    // int nod1,
    //     nod2,
    //     cost;

    // vector<pair<int, int>> aux;
    // vector<vector<pair<int,int>>> l(n , aux);

    // for (int i = 0; i < m; i++){
    //     f >> nod1;
    //     f >> nod2;
    //     f >> cost;
    //     l[nod1].push_back(make_pair(nod2, cost));
    // }

    // int rez = g1.hamilton(l);
    // if (rez == 1e9)
    //     g << "Nu exista solutie";
    // else
    //     g << rez;

    // --------------royfloyd---------------- 
    // int n;

    // f >> n;

    // int cost;

    // vector<vector<int>> lv;
    // for (int i = 0; i < n; i++){
    //     vector<int> aux(n ,0);
    //     lv.push_back(aux);
    // }

    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         f >> cost;
    //         if (cost == 0 && i != j)
    //             lv[i][j] = 1e9;
    //         else
    //             lv[i][j] = cost;
    //     }
    // }

    // Graf g1(n, lv);
    // lv = g1.Royfloyd();
    
    // for (int i = 0; i < n; i++){
    //     for (int j = 0; j < n; j++){
    //         g << lv[i][j] << " ";
    //     }
    //     g << "\n";
    // }



    return 0;
}
