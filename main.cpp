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

// ifstream f("hh.in");
// ofstream g("hh.out");

// ifstream f("apm.in");
// ofstream g("apm.out");

// ifstream f("dijkstra.in");
// ofstream g("dijkstra.out");

ifstream f("ciclueuler.in");
ofstream g("ciclueuler.out");

//ifstream f("hamilton.in");
//ofstream g("hamilton.out");

class Graf{
private:
    int nrNoduri;
    vector<vector<int>> listaVecini;
    vector<vector<int>> listaVeciniCost;

    //setteri
    void set_nrNoduri(int);
    void set_listaVecini();
    void set_listaVeciniCosturi();
    void adaugaMuchie(int, int);

    //metode
    void print_listaVecini() const;
    //citire graf orientat
    void citireOrientat(int, int);
    //citire graf neorientat
    void citereNeorientat(int, int);
    //citire graf neorientat cu costuri
    void citireNeorientatCosturi(int, int);
    //citire graf orientat cu costuri
    void citireOrientatCosturi(int, int);
    void DFS(int, vector<int>&);
    void BFS(int, vector<int>&);
    void DFS_CTC(int, vector<int>&, vector<int>&, stack<int>&, vector<bool>&, int&);
    vector<int> APM();

public:
    Graf();
    Graf(int);
    // mai adauga un constructor
    void adaugaMuchieCost(int, int, int);
    void citireEul();
    //getteri
    int get_nrNoduri() const;

    // DFS
    int nrCompConexe();

    // BFS
    void Distante();

    // CTC
    set<int> Tarjan();

    // Sortare topologica
    vector<int> Kahn();

    // APM
    void printAPM();

    // Dijkstra
    void Dijkstra();

   // ciclu eulerian
    void hasEulerCircuit();
    vector<int> EulerCircuit();

    // hamilton
    int hamilton(vector<vector<pair<int,int>>>);
};

int Graf::hamilton(vector<vector<pair<int,int>>> l){
    vector<vector<int>> minCost((1 << get_nrNoduri()), vector<int>(get_nrNoduri(), 1e9));
    minCost.resize((1 << get_nrNoduri()));

    minCost[1][0] = 0;

    for (int i = 0; i < (1 << get_nrNoduri()); i++){
        for (int j = 0; j < get_nrNoduri(); j++){
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
        sol = min(sol, minCost[(1 << get_nrNoduri()) - 1][elem.first] + elem.second);
    }

    return sol;
}

void Graf::adaugaMuchieCost(int nod1, int nod2, int cost){
    listaVeciniCost[nod1][nod2] = cost;
}

void Graf::hasEulerCircuit(){
    int n, m;
    f >> n >> m;
    citereNeorientat(n ,m);
	
    int odd = 0;
    for (int i = 0; i < get_nrNoduri(); i++){
        if (listaVecini[i].size() % 2 != 0)
            odd ++;
    }
    if (odd == 0)
        EulerCircuit();
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

int Graf::get_nrNoduri() const{
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

void Graf::set_listaVeciniCosturi(){
    for (int i = 0; i < this->get_nrNoduri(); i++){
        vector<int> aux(get_nrNoduri(), 0);
        listaVeciniCost.push_back(aux);
    }
}

void Graf::print_listaVecini() const{
    for (int i = 0; i < this->get_nrNoduri(); i++){
        cout << i + 1 << ": ";
        for (int j = 0; j < listaVecini[i].size(); j++){
            cout << listaVecini[i][j] + 1 << " ";
        }
        cout << "\n";
    }
}

void Graf::citireOrientat(int n, int m){
    int nod1,
        nod2;

    this->set_nrNoduri(n);
    this->set_listaVecini();
    for (int i = 1; i <= m; i++){
        f >> nod1 >> nod2;
        this->adaugaMuchie(nod1 - 1, nod2 - 1);
    }
}

void Graf::citereNeorientat(int n, int m){
    int nod1, // primul nod al muchiei
        nod2; // al doilea nod al muchiei

    this->set_nrNoduri(n);
    this->set_listaVecini();

    for (int i = 0; i < m; i++){
        f >> nod1;
        f >> nod2;
        this->adaugaMuchie(nod1 - 1, nod2 - 1);
        this->adaugaMuchie(nod2 - 1, nod1 - 1);
    }
}

void Graf::citireNeorientatCosturi(int n, int m){
    int nod1, // primul nod al muchiei
        nod2,
        cost; // al doilea nod al muchiei

    this->set_nrNoduri(n);
    this->set_listaVeciniCosturi();

    for (int i = 0; i < m; i++){
        f >> nod1;
        f >> nod2;
        f >> cost;
        this->listaVeciniCost[nod1 - 1][nod2 - 1] = cost;
        this->listaVeciniCost[nod2 - 1][nod1 - 1] = cost;
    }
}

void Graf::citireOrientatCosturi(int n, int m){
    int nod1, // primul nod al muchiei
        nod2,
        cost; // al doilea nod al muchiei

    this->set_nrNoduri(n);

    this->set_listaVeciniCosturi();


    for (int i = 0; i < m; i++){
        f >> nod1;
        f >> nod2;
        f >> cost;
        this->listaVeciniCost[nod1 - 1][nod2 - 1] = cost;
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
    int n,
        m;
    f >> n >> m;
    this->citereNeorientat(n, m);

    vector<int> vizitat(this->get_nrNoduri(), 0); // vector care retine daca un nod a fost vizitat sau nu
    int nrComponenteConexe = 0;

    for(int i = 0; i < this->get_nrNoduri(); i++){
        if (vizitat[i] == 0){
            nrComponenteConexe ++;
            DFS(i, vizitat);
        }
    }

    return nrComponenteConexe;
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
                vizitat[listaVecini[nod - 1][i] - 1] = 1;   // il marchez ca fiind vizitat
                coada.push(listaVecini[nod - 1][i]);    // il adaug in coada
                distanta[listaVecini[nod - 1][i] - 1] = distanta[nod - 1] + 1;  // ii adaug distanta in vectorul de distante
            }
        }
    }

}

void Graf::Distante(){
    int n,
        m,
        s;

    f >> n >> m >> s;

    this->citireOrientat(n,m);

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

set<int> Graf::Tarjan(){
    int n,
        m;
    f >> n >> m;
    this->citireOrientat(n, m);
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

    return multime;
    // for (auto elem : multime){
    //     for (int i = 0; i < this->get_nrNoduri(); i++){
    //         if (low[i] == elem)
    //            g << i + 1<< " ";
    //     }
    //     g << "\n";
    // }
}

vector<int> Graf::Kahn(){
    int n,
        m;
    f >> n >> m;
    this->citireOrientat(n, m);
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

    return order;
    // for (int i = 0; i < this->get_nrNoduri(); i++){
    //     g << order[i] + 1<< " ";
    // }
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

vector<int> Graf::APM(){
    int n,
        m;
    f >> n >> m;
    this->citireNeorientatCosturi(n ,m);
    vector<int> parinte(get_nrNoduri(), -1);
    vector<int> id(get_nrNoduri(),  INT_MAX);
    vector<bool> vizitat(get_nrNoduri(), false);

    id[0] = 0;

    for (int i = 0; i < get_nrNoduri(); i++){
        // alegem muchia de cost minim ale carei noduri nu au fost adaugate in APM
        int min = INT_MAX;
        int idMin;
        for (int j = 0; j < get_nrNoduri(); j++){
            if (vizitat[j] == false && id[j] < min){
                min = id[j];
                idMin = j;
            }
        }
        vizitat[idMin] = true;

        for (int j = 0; j < get_nrNoduri(); j++){
            if (listaVeciniCost[idMin][j] && vizitat[j] == false && listaVeciniCost[idMin][j] < id[j]){
                parinte[j] = idMin;
                id[j] = listaVeciniCost[idMin][j];
            }
        }
    }

    return parinte;
}

void Graf::printAPM(){
    vector<int> parinte = this->APM();
    int suma = 0;

    for (int i = 1; i < listaVeciniCost.size(); i++){
        suma += listaVeciniCost[i][parinte[i]];
    }
    g << suma << "\n";

    g << parinte.size() - 1;

    for (int i = 1; i < listaVeciniCost.size(); i++){
        g << "\n" << parinte[i] + 1 << " " << i + 1;
    }
}

void Graf::Dijkstra(){
    
}


int main(){
    // Graf g1;
    // g1.nrCompConexe();
    // g1.Distante();
    // g1.Tarjan();
    // g1.Kahn();
    //g << HavelHakimi();

    // vector<int> rezAPM =
    // g1.printAPM();
    // int suma = 0;
    // for (int i = 0; i < rezAPM.size(); i++){
    //     suma +=
    // }
    
    // g1.citireEul();
    // g1.hasEulerCircuit();

    int n, m;
    f >> n >> m;

    Graf g1(n);

    int nod1,
        nod2,
        cost;

    vector<pair<int, int>> aux;
    vector<vector<pair<int,int>>> l(n , aux);

    for (int i = 0; i < m; i++){
        f >> nod1;
        f >> nod2;
        f >> cost;
        l[nod1].push_back(make_pair(nod2, cost));
    }

    int rez = g1.hamilton(l);
    if (rez == 1e9)
        g << "Nu exista solutie";
    else
        g << rez;
    return 0;
}
