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

//ifstream f("apm.in");
//ofstream g("apm.out");

ifstream f("dijkstra.in");
ofstream g("dijkstra.out");


class Graf{
private:
    int nrNoduri;
    vector<vector<int>> listaVecini;
    vector<vector<int>> listaVeciniW;
public:
    Graf();
    Graf(int);
    int get_nrNoduri();
    void set_nrNoduri(int);
    void set_listaVecini();
    void adaugaMuchie(int, int);
    void print_listaVecini();

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

    void get_listaVeciniW();
    void set_listaVeciniWOr();
    void set_listaVeciniWNeo();

    // APM
    vector<int> APM();

    // Dijkstra
    vector<int> Dijkstra();
};

vector<int> Graf::Dijkstra(){
    this->set_listaVeciniWOr();

    vector<int> distante(get_nrNoduri(), INFINITY);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector <bool> vizitat(get_nrNoduri(), false);
    distante[0] = 0;
    pq.push(make_pair(0,0));

    while (pq.empty() == false){
        int nod_curent = pq.top().second;
        pq.pop();

        if (vizitat[nod_curent] == false){
            vizitat[nod_curent] = true;
                for (int j = 0; j < listaVeciniW[nod_curent].size(); j++){
                    if (listaVeciniW[nod_curent][j] != 0){
                        if (distante[j] > distante[nod_curent] + listaVeciniW[nod_curent][j]){
                            distante[j] = distante[nod_curent] + listaVeciniW[nod_curent][j];
                            pq.push(make_pair(distante[j], j));

                    }

            }

        }
    }
    }
    for (int i = 1; i < get_nrNoduri(); i++){
        if (distante[i] != INFINITY){
            g << distante[i] << " ";
        }
        else
            g << 0 << " ";
    }
}

void Graf::set_listaVeciniWNeo(){
    int n,
        m,
        nod1,
        nod2,
        greutate;

    f >> n >> m;
    this->set_nrNoduri(n);

    for (int i = 0; i < this->get_nrNoduri(); i++){
        vector<int> aux(get_nrNoduri(), 0);
        listaVeciniW.push_back(aux);
    }

    for (int i = 1; i <= m; i++){
        f >> nod1 >> nod2 >> greutate;
        listaVeciniW[nod1 - 1][nod2 - 1] = greutate;
        listaVeciniW[nod2 - 1][nod1 - 1] = greutate;
    }
}

void Graf::set_listaVeciniWOr(){
    int n,
        m,
        nod1,
        nod2,
        greutate;

    f >> n >> m;
    this->set_nrNoduri(n);

    for (int i = 0; i < this->get_nrNoduri(); i++){
        vector<int> aux(get_nrNoduri(), 0);
        listaVeciniW.push_back(aux);
    }

    for (int i = 1; i <= m; i++){
        f >> nod1 >> nod2 >> greutate;
        listaVeciniW[nod1 - 1][nod2 - 1] = greutate;
    }
}

vector<int> Graf::APM(){
    this->set_listaVeciniWNeo();
    vector<bool> vizitat(get_nrNoduri(), false);
    vector<int> apm;
    vizitat[0] = true;
    int row, col;
    int nr_muchii = 0;
    int suma = 0;

    while (nr_muchii < get_nrNoduri() - 1){
        int min = INFINITY;
        row = 0;
        col = 0;

        for (int i = 0; i< get_nrNoduri(); i++){
            if(vizitat[i] == true){
                for (int j = 0; j < get_nrNoduri(); j++){
                    if (vizitat[j] == false && listaVeciniW[i][j] != 0){
                        if (min > listaVeciniW[i][j]){
                            min = listaVeciniW[i][j];
                            row = i;
                            col = j;
                        }
                    }

                }
            }
        }
        suma += listaVeciniW[row][col];
        apm.push_back(row);
        apm.push_back(col);
        vizitat[col] = true;
        nr_muchii ++;
    }
    g << suma << "\n";
    g << apm.size() / 2 << "\n";
    return apm;
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

void Graf::print_listaVecini(){
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
    Graf g2;
    // g1.nrCompConexe();
    // g1.Distante();
    // g1.Tarjan();
    // g1.Kahn();
    // g << HavelHakimi();
    // vector<int> apm = g2.APM();
    // for (int i = 0; i < apm.size() - 1; i+=2){
    //     g << apm[i] + 1 << " " << apm[i+1] + 1 << "\n";
    // }
    g1.Dijkstra();
    return 0;
}
