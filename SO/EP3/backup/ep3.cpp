/*
Vítor Kei Taira Tamada - 8516250
Luiz Felipe Moumdjian Girotto - 8941189
*/

#include <bits/stdc++.h>
#include <readline/readline.h>
using namespace std;

// Structs
typedef struct p
{
  int id;      // PID
  int t0;      // Momento de chegada do processo
  string nome; // Nome do processo
  int tf;      // Momento de finalização do processo
  int b;       // Quantidade de memória utilizada pelo processo
  
  vector<int> page; // Entradas na tabela de páginas
  vector<int> pi; // Equivale a pi, 1 <= i <= n
  vector<int> ti; // Equivale a ti, 1 <= i <= n
  
  int virMemPos; // Posição inicial do processo na memória virtual
} process;

typedef struct t
{
  vector<bool> pa; // Presente / Ausente na memória real/física
  vector<int> address; // Endereços na memória real/física
  vector<bool> R; // Utilizado pelos algoritmos de substituição de página
} table;

// Protótipos
void executa(int);
void firstFit(int);
void nextFit(int);
void bestFit(int);
void worstFit(int);

void writeMem(int, int, int);
void freeProc(int);

void printVirVec();

// Variáveis globais
fstream tracer; // Arquivo de tracer de entrada
fstream phyMem; // /tmp/ep3.mem
fstream virMem; // /tmp/ep3.vir

int esp = 0;    // Define qual o algoritmo de gerência de espaço livre será usado; inicializado como zero (não chama nenhum algoritmo)
int pag = 0;    // Define qual o algoritmo de substituição de páginas será usado; iniciaizado como zero (não chama nenhum algoritmo)

int total, virt, s, p;

vector<bool> phyVec; // bitmap
vector<bool> virVec; // bitmap
vector<process> procVec; // Vetor de processos

int nextPos = 0; // Auxiliar para o Next Fit

typedef map<string, int> dict;
typedef dict::const_iterator it;

int main()
{
  int interval;
  string cmd;
  char arg[100];
  
  while(true)
  {
    cout << "(ep3): ";
    cin >> cmd;
    
    if     (cmd.compare("sai") == 0)     { if(tracer.is_open()) tracer.close(); break; }
    else if(cmd.compare("carrega") == 0) { cin >> arg; tracer.open(arg, ios::in);      }
    else if(cmd.compare("espaco") == 0)    cin >> esp;
    else if(cmd.compare("substitui") == 0) cin >> pag;
    else if(cmd.compare("executa") == 0) { cin >> interval; executa(interval); break;  }
    else cout << "Comando inválido\n";
  }
}

// Comando executa <intervalo>
void executa(int interval) // if(t % interval == 0) imprime infos
{
  dict d;
  string line, nome;
  int t0, tf, b, pn, tn, aux = -1, i, j, t;
  
  // Recebe total, total, virtual, s e p que estão na primeira linha do arquivo de tracer
  getline(tracer, line);
  istringstream iss0(line);
  iss0 >> total >> virt >> s >> p;
  //vector<bool> bitmap(total / s);

  // Inicializa os vetores que representam a memória real e a memória virtual
  for(i = 0; i < total/s; i++) phyVec.push_back(false);
  for(i = 0; i < virt/s; i++)  virVec.push_back(false);

  // Abre/Cria os arquivos ep3.mem e ep3.vir como especificado no enunciado
  phyMem.open("/tmp/ep3.mem", ios::in | ios::out | ios::trunc);
  virMem.open("/tmp/ep3.vir", ios::in | ios::out | ios::trunc);
  for(int i = 0; i < total/s; i++)
    phyMem.write((char*) &aux, sizeof(int));
  for(int i = 0; i < virt/s; i++)
    virMem.write((char*) &aux, sizeof(int));
  
  // Lê as linhas seguintes do arquivo de tracer
  for(i = 0; getline(tracer, line); i++)
  {
    istringstream iss(line);
    iss >> t0 >> nome >> tf >> b;
    process proc;
    proc.id = i;
    proc.t0 = t0;
    proc.nome = nome;
    proc.tf = tf;
    proc.b = b;
    
    for(j = 0; j < b / p; j++) proc.page.push_back(-1);
    
    while(iss >> pn >> tn)
    {
      proc.pi.push_back(pn);
      proc.ti.push_back(tn);
    }
    
    procVec.push_back(proc);
    /*
    d[nome] = i;
    while(iss >> pn >> tn)
      cout << "pn = " << pn << " || tn = " << tn << endl;
    cout << t0 << " " << nome << " " << tf << " " << b << " " << d[nome] << endl;
    ++i;
    */
  }
  
  for(t = 0, i = 0; t <= 31; t++)
  {
    if(procVec[i].t0 == t)
    {
      // processos podem chegar ao mesmo tempo e não lidamos com isso
      if     (esp == 1) firstFit(i);
      else if(esp == 2) nextFit(i);
      else if(esp == 3) bestFit(i);
      else if(esp == 4) worstFit(i);
      i++;
    }
    for(j = 0; j < i; j++)
    {
      printVirVec();
      if(procVec[j].tf == t)
        freeProc(j);
    }
  }
}

// First fit
void firstFit(int index)
{
  int i, j, len = ceil((double) procVec[index].b / ((double) s));
  
  for(i = 0; i < virVec.size(); i++)
    if(!virVec[i])
    {
      for(j = i; j < virVec.size() && j < i + len; j++)
        if(virVec[j]) { i = j; break; }
      if(j - i == len)
      {
        for(j = i; j < i + len; j++)
        {
          virVec[j] = true;
          writeMem(1, j * 4, procVec[index].id);
        }
        procVec[index].virMemPos = i;
        break;
      }
    }
}

// Next fit
void nextFit(int index)
{
  int j, len = ceil((double) procVec[index].b / ((double) s));

  while(nextPos < virVec.size())
  {
    if(!virVec[nextPos])
    {
      for(j = nextPos; j < virVec.size() && j < nextPos + len; j++)
        if(virVec[j]) { nextPos = j; break; }
      if(j - nextPos == len)
      {
        for(j = nextPos; j < nextPos + len; j++)
        {
          virVec[j] = true;
          writeMem(1, j * 4, procVec[index].id);
        }
        procVec[index].virMemPos = nextPos;
        break;
      }
    }
    nextPos = (nextPos + 1) % virVec.size();
  }
}

// Best fit
void bestFit(int index)
{
  int i, j, k, len = ceil((double) procVec[index].b / ((double) s)), min;
  vector<int> gapSize;
  vector<int> gapIni;
  
  for(i = 0; i < virVec.size(); i++)
    if(!virVec[i])
    {
      for(j = i; j < virVec.size() && j < i + len; j++)
        if(virVec[j]) { i = j; break; }
      if(j - i == len)
      {
        for(k = j; k < virVec.size() && !virVec[k]; k++);
        gapSize.push_back(k - i);
        gapIni.push_back(i);
        i = k;
      }
    }

  k = 0;  
  min = gapSize[0];  
  for(i = 1; i < gapSize.size(); i++)
  {
    if(min > gapSize[i])
    {
      min = gapSize[i]; 
      k = i;
    }
  }
  
  for(j = gapIni[k]; j < gapIni[k] + len; j++)
  {
    virVec[j] = true;
    writeMem(1, j * 4, procVec[index].id);
  }
  cout << "gapinik = " << gapIni[k] << endl;
  procVec[index].virMemPos = gapIni[k];
  
  printVirVec();
}

// Worst fit
void worstFit(int index)
{
  int i, j, k, len = ceil((double) procVec[index].b / ((double) s)), max;
  vector<int> gapSize;
  vector<int> gapIni;
  
  for(i = 0; i < virVec.size(); i++)
    if(!virVec[i])
    {
      for(j = i; j < virVec.size() && j < i + len; j++)
        if(virVec[j]) { i = j; break; }
      if(j - i == len)
      {
        for(k = j; k < virVec.size() && !virVec[k]; k++);
        gapSize.push_back(k - i);
        gapIni.push_back(i);
        i = k;
      }
    }

  k = 0;  
  max = gapSize[0];  
  for(i = 1; i < gapSize.size(); i++)
  {
    if(max < gapSize[i])
    {
      max = gapSize[i]; 
      k = i;
    }
  }
  
  for(j = gapIni[k]; j < gapIni[k] + len; j++)
  {
    virVec[j] = true;
    writeMem(1, j * 4, procVec[index].id);
  }
  procVec[index].virMemPos = gapIni[k];
  
  for(i = 0; i < virVec.size(); i++)
    cout << virVec[i] << " ";
  cout << endl;
}

// Escreve no arquivo ep3.mem (mem == 0) ou ep3.vir (mem == 1)
void writeMem(int mem, int pos, int pid)
{
  if(mem == 0)
  {
    phyMem.seekp(pos);
    phyMem.write((char*) &pid, sizeof(int));
  }
  else if (mem == 1)
  {
    virMem.seekp(pos);
    virMem.write((char*) &pid, sizeof(int));
  }
  else // Este trecho é mais para ajudar na hora de debugar o código
  {
    tracer.close();
    cout << "Chamada da função writeMem() com parâmetro 1 incorreto\nFinalizando programa...\n";
    exit(-1);
  }
}

// Libera o espaço de um processo que já terminou da memória virtual
void freeProc(int index)
{
  int i, len = ceil((double) procVec[index].b / ((double) s));
  cout << procVec[index].nome << endl;
  for(i = procVec[index].virMemPos; i < len + procVec[index].virMemPos; i++)
  {
    virVec[i] = false;
    writeMem(1, i * 4, -1);
  }
  printVirVec();
}


// FUNÇÃO DE TESTES/DEBUGGAR
void printVirVec()
{
  for(int i = 0; i < virVec.size(); i++)
    cout << virVec[i] << " ";
  cout << endl;
}