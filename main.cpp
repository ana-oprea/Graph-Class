#include <bits/stdc++.h>

using namespace std;

// ifstream f("dfs.in");
// ofstream g("dfs.out");

// ifstream f("bfs.in");
// ofstream g("bfs.out");

// ifstream f("ctc.in");
// ofstream g("ctc.out");

// ifstream f("sortaret.in");
// ofstream g("sortaret.out");

ifstream f("hh.in");
ofstream g("hh.out");

class Graf{
private:
    int nrNoduri;
    vector<vector<int>> listaVecini;
public:
    Graf();
    Graf(int);
    int get_nrNoduri();
    void set_nrNoduri(int);
    void set_listaVecini();
    void get_listaVecini();
    void adaugaMuchie(int, int);

    //citire graf orientat
    void citireOrientat();
    void citereNeorientat();

    // DFS
    void DFS(int, vector<int>&);
    void nrCompConexe();

    // BFS
    void BFS(int, vector<int>&);
    void Distante();

    // CTC
    void Tarjan();
    void DFS_CTC(int, vector<int>&, vector<int>&, stack<int>&, vector<bool>&, int&);

    // Sortare topologica
    void Kahn();

};

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

int Graf::get_nrNoduri(){
    return this->nrNoduri;
}

void Graf::set_nrNoduri(int n){
    this->nrNoduri = n;
}

void Graf::set_listaVecini(){
    for (int i = 0; i < this->get_nrNoduri(); i++){
        vector<int> aux;
        listaVecini.push_back(aux);
    }
}

void Graf::get_listaVecini(){
    for (int i = 0; i < this->get_nrNoduri(); i++){
        cout << i + 1 << ": ";
        for (int j = 0; j < listaVecini[i].size(); j++){
            cout << listaVecini[i][j] + 1 << " ";
        }
        cout << "\n";
    }
}

void Graf::citireOrientat(){
    int n,
        m,
        nod1,
        nod2;

    f >> n >> m;

    this->set_nrNoduri(n);
    this->set_listaVecini();
    for (int i = 1; i <= m; i++){
        f >> nod1 >> nod2;
        this->adaugaMuchie(nod1 - 1, nod2 - 1);
    }
}

void Graf::citereNeorientat(){
    int n, // nr noduri 
        m, // nr muchii  
        nod1, // primul nod al muchiei
        nod2; // al doilea nod al muchiei

    f >> n >> m;

    this->set_nrNoduri(n);
    this->set_listaVecini();

    for (int i = 0; i < m; i++){
        f >> nod1;
        f >> nod2;
        this->adaugaMuchie(nod1 - 1, nod2 - 1);
        this->adaugaMuchie(nod2 - 1, nod1 - 1);
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

void Graf::nrCompConexe(){
    this->citereNeorientat();
    //get_listaVecini();

    vector<int> vizitat(this->get_nrNoduri(), 0); // vector care retine daca un nod a fost vizitat sau nu
    int nrComponenteConexe = 0;

    for(int i = 0; i < this->get_nrNoduri(); i++){
        if (vizitat[i] == 0){
            nrComponenteConexe ++;
            DFS(i, vizitat);
        }
    }

    g << nrComponenteConexe;
}

void Graf::BFS(int s, vector<int> &distanta){
    vector<int> vizitat (this->get_nrNoduri(), 0); // vector care retine care retin nodurile vizitate
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
                vizitat[listaVecini[nod - 1][i] - 1] = 1;   // il marchez ca fiind viiztat
                coada.push(listaVecini[nod - 1][i]);    // il adaug in coada
                distanta[listaVecini[nod - 1][i] - 1] = distanta[nod - 1] + 1;  // ii adaug distanta in vectorul de distante
            }
        }
    }

}

void Graf::Distante(){
    int n,
        m,
        s,
        nod1,
        nod2;

    f >> n >> m >> s;

    this->set_nrNoduri(n);
    this->set_listaVecini();
    for (int i = 1; i <= m; i++){
        f >> nod1 >> nod2;
        this->adaugaMuchie(nod1 - 1, nod2);
    }

    vector<int> distanta (this->get_nrNoduri() , 0);

    BFS(s, distanta);

    for (int i = 0 ; i < this->get_nrNoduri(); i++){
        // daca distanta este 0 si nodul respectiv nu este nodul de start atunci inseamna ca nu putem sa ajungem la nodul respectiv deci distanta de la s la el este - 1
        if (distanta[i] == 0 && i + 1 != s){
            distanta[i] = -1;
        }
        g << distanta[i] << " ";
   }
}


void Graf::DFS_CTC(int nod_curent, vector<int> &descoperit, vector<int> &low, stack<int> &stiva, vector<bool> &membruStiva, int &nr){
    static int timp = 0;
    descoperit[nod_curent] = low[nod_curent] = timp++;
    stiva.push(nod_curent);
    membruStiva[nod_curent] = true;

    for (auto elem: listaVecini[nod_curent]){
        if (descoperit[elem] == -1){
            DFS_CTC(elem, descoperit, low, stiva, membruStiva, nr);
        }
        if(membruStiva[elem] == true){
            low[nod_curent] = min(low[nod_curent], low[elem]);
        }
    }
    if (descoperit[nod_curent] == low[nod_curent]){
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

void Graf::Tarjan(){
    this->citireOrientat();
    vector<int> descoperit(this->get_nrNoduri(), -1);
    vector<int> low(this->get_nrNoduri(), -1);
    stack<int> stiva;
    vector<bool> membruStiva(this->get_nrNoduri(), false);
    int nr = 0; // numarul componenetelor tare conexe

    for (int i = 0; i < this->get_nrNoduri(); i++){
        if (descoperit[i] == -1)
            DFS_CTC(i, descoperit, low, stiva, membruStiva, nr);
    }
    g << nr << "\n";
    set<int> multime;
    for (int i = 0; i < this->get_nrNoduri(); i++){
        multime.insert(low[i]);
    }
    for (auto elem : multime){
        for (int i = 0; i < this->get_nrNoduri(); i++){
            if (low[i] == elem)
               g << i + 1<< " ";
        }
        g << "\n";
    }
}

void Graf::Kahn(){
    this->citireOrientat();
    vector<int> in_degree(this->get_nrNoduri(),0);
    queue<int> q;
    vector<int> order; 
    int nod;

    for (int i = 0; i < this->get_nrNoduri(); i++){
        for (auto elem: listaVecini[i]){
            in_degree[elem]++;
        }
    }

    for (int i = 0; i < this->get_nrNoduri(); i++){
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

    for (int i = 0; i < this->get_nrNoduri(); i++){
        g << order[i] + 1<< " ";
    }
}

bool HavelHakimi(){
    // citere date de intrare
    int n, x;
    f >> n;
    vector<int> grad;
    
    for (int i = 0; i < n; i++){
        f >> x;
        grad.push_back(x);
    }

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

int main(){
    Graf g1;
    // g1.nrCompConexe();
    // g1.Distante();
    // g1.Tarjan();
    // g1.Kahn();
     g << HavelHakimi();
    return 0;
}