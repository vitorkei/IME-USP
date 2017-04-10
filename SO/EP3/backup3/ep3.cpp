/*
Vítor Kei Taira Tamada - 8516250
Luiz Felipe Moumdjian Girotto - 8941189
*/

#include <bits/stdc++.h>
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
  
  int t;
  int virMemPos; // Posição inicial do processo na memória virtual

  vector<int> acc_freq; // Frequencia dos acessos por pagina, para o Optimal
  vector< list<bool> > age; // Vetor para o algoritmo least recently used
} process;

typedef struct t
{
  vector<bool> pa; // Presente / Ausente na memória física
  vector<int> address; // Endereços na memória física || address == -2: entrada vazia; address == -1: entrada ocupada, mas ausente na memória física
  vector<bool> R; // Utilizado pelos algoritmos de substituição de página
} table;

// Protótipos
void executa(int);

// Algoritmos de gerência de espaço livre
void firstFit(int);
void nextFit(int);
void bestFit(int);
void worstFit(int);

// Algoritmos de substituição de página
void optimal(int, int);
void secondChance(int, int);
void clk(int, int);
void LRU4(int, int);

// Funções auxiliares
void writeMem(int, int, int); // Escreve na memória (ep3.mem ou ep3.vir)
void freeProc(int); // Libera um processo dos bitmaps, da tabela de páginas e das memórias quando ele acaba
void fillPageTable(int); // Preenche a tabela de páginas
bool pageFit(int, int); // Aloca um processo na memória física sem ser por page fault
int  binToDec (list<bool>); // Converte um número binário (de 8 bits especificamente) em sua representação em decimal
void clearR(); // Zera os bits R da tabela de páginas
void printPhyVec(); // Imprime o bitmap da memória física
void printVirVec(); // Imprime o bitmap da memória virtual
void printPageTable(); // Imprime a tabela de páginas (endereço, bit R e bit de presente/ausente de cada entrada)
void printMem(int); // Imprime a memória física ou virtual

// Variáveis globais
fstream trace;  // Arquivo de trace de entrada
fstream phyMem; // /tmp/ep3.mem
fstream virMem; // /tmp/ep3.vir
vector<int> pM; // Vetor auxiliar da memória física (para impressão)
vector<int> vM; // Vetor auxiliar da memória virtual (para impressão)

int esp = 0;    // Define qual o algoritmo de gerência de espaço livre será usado; inicializado como zero (não chama nenhum algoritmo)
int pag = 0;    // Define qual o algoritmo de substituição de páginas será usado; iniciaizado como zero (não chama nenhum algoritmo)

int total, virt, s, p, endSim = 0;

vector<bool> phyVec; // bitmap
vector<bool> virVec; // bitmap
vector<process> procVec; // Vetor de processos
int pageTableSize = 0; // Tamanho da tabela de páginas

table pageTable; // Tabela de páginas
queue<int> scq; // Second chance queue
vector<int> clkv; // Clock vector (lista circular)
int iClk = 0; // clock index

int nextPos = 0; // Auxiliar para o Next Fit

int main()
{
  int interval;
  string cmd;
  char arg[100];
  
  while(true)
  {
    cout << "(ep3): ";
    cin >> cmd;
    
    if     (cmd.compare("sai") == 0)     { if(trace.is_open()) trace.close(); break; }
    else if(cmd.compare("carrega") == 0 || cmd.compare("c") == 0) { cin >> arg; trace.open(arg, ios::in);      }
    else if(cmd.compare("espaco") == 0 || cmd.compare("e") == 0)    cin >> esp;
    else if(cmd.compare("substitui") == 0 || cmd.compare("s") == 0) cin >> pag;
    else if(cmd.compare("executa") == 0 || cmd.compare("x") == 0) { cin >> interval; executa(interval); break;  }
    else cout << "Comando inválido\n";
  }
}

// Comando executa <intervalo>
void executa(int interval) // if(t % interval == 0) imprime infos
{
  string line, nome;
  int t0, tf, b, pn, tn, aux = -1, i, j, t, RCounter = 0;
  
  // Recebe total, total, virtual, s e p que estão na primeira linha do arquivo de trace
  getline(trace, line);
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
  {
    phyMem.write((char*) &aux, sizeof(int));
    pM.push_back(-1);
  }
  for(int i = 0; i < virt/s; i++)
  {
    virMem.write((char*) &aux, sizeof(int));
    vM.push_back(-1);
  }
  
  // Lê as linhas seguintes do arquivo de trace
  for(i = 0; getline(trace, line); i++)
  {
    istringstream iss(line);
    iss >> t0 >> nome >> tf >> b;
    process proc;
    proc.id = i;
    proc.t0 = t0;
    proc.nome = nome;
    proc.tf = tf;
    proc.b = b;
    proc.t = 0;
    
    // Colocando o numero de paginas em que o processo é dividido,
    // e preparando o vetor do numero de acessos por pagina.
    // Também preparando o vetor de listas que representa os
    // contadores do LR4U.
    
    proc.age.resize(ceil((double) b / ((double) p)));
    
    for(j = 0; j < ceil((double) b / ((double) p)); j++)
    {
      proc.page.push_back(-1);
      proc.acc_freq.push_back(0);
      for (int k = 0; k < 8; k++)
        proc.age[j].push_back(false);
    }
    
    pageTableSize += ceil((double) b / ((double) p));
    
    // Colocando os acessos realizados pelo processo, e os contando
    // (caso o tipo de algoritmo de alocação seja o optimal)
    while(iss >> pn >> tn)
    {
      proc.pi.push_back(pn);
      proc.acc_freq[pn / p] += 1; //Para pegar a casa de cada pagina
      proc.ti.push_back(tn);
    }
    
    procVec.push_back(proc);
    
    if(tf > endSim) endSim = tf;
  }
  
  // Preenche a tabela de páginas com "entradas vazias"
  for(i = 0; i < pageTableSize; i++)
  {
    pageTable.pa.push_back(false);
    pageTable.address.push_back(-2);
    pageTable.R.push_back(false);
  }
  
  // Ciclo principal
  for(t = 0, i = 0; t <= endSim; t++, RCounter = (RCounter + 1) % 10)
  {
    if(RCounter == 0) clearR(); // Reinicia o bit R a cada 10 pulsos de clocks
    if(t % interval == 0) // Imprime o estado das memórias e o conteúdo dos bitmaps que mantém o status da memória
    {
      cout << "Memória física:\n";
      printMem(0);
      cout << "\nBitmap referente à memória física:\n";
      printPhyVec();
      
      cout << "\nMemória virtual:\n";
      printMem(1);
      cout << "\nBitmap referente à memória virtual:\n";
      printVirVec();
      
      cout << "\n------\n\n";
    }
  
    // Executa algoritmo de gerência de espaço livre quando um processo chega
    while(procVec[i].t0 == t && i < procVec.size())
    {
      if     (esp == 1) firstFit(i);
      else if(esp == 2) nextFit(i);
      else if(esp == 3) bestFit(i);
      else if(esp == 4) worstFit(i);
      fillPageTable(i);
      i++;
    }
    
    // Executa algoritmo de gerência de página (se necessário) quando uma página de um processo é acessada
    for (j = 0; j < i; j++)
    {
      if (procVec[j].ti[procVec[j].t] == t)
      {
        int wantedPage = procVec[j].pi[procVec[j].t] / p; 
        if(!pageFit(j, wantedPage))
        {
          if     (pag == 1) optimal(j, wantedPage); 
          else if(pag == 2) secondChance(j, wantedPage);
          else if(pag == 3) clk(j, wantedPage);
          else if(pag == 4) LRU4(j, wantedPage);
        }
        // checar qual pagina o processo esta se referindo, usando o pn
        procVec[j].t += 1;
      }
    }
    
    // Adicionando R ao bit mais significativo de cada pagina, de cada processo
    if (pag == 4)
    {
      for (j = 0; j < procVec.size(); j++)
        for (int k = 0; k < procVec[j].age.size(); k++)
        {
          if (procVec[j].page[k] >= 0)
          {
            procVec[j].age[k].push_front(pageTable.R[procVec[j].page[k]]);
            procVec[j].age[k].pop_back();
          }
        }
    }
    
    // Libera os processos que acabaram
    for(j = 0; j < i; j++)
    {
      if(procVec[j].tf == t)
        freeProc(j);
    }
  }
}

/* ____ ____ ____ ____ ____ ____
   |    |    |  | |  | |    |  |
   |___ |___ |__| |__| |    |  |
   |       | |    |  | |    |  |
   |___ ___| |    |  | |___ |__|
*/

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
      for(j = nextPos; j < nextPos + len; j++)
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
  procVec[index].virMemPos = gapIni[k];
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
}

/* ____ ____ ____         ____
   |  | |  | |    | |\  | |  |
   |__| |__| | _  | | \ | |__|
   |    |  | |  | | |  \| |  |
   |    |  | |__| | |   | |  |
*/

// Optimal
void optimal (int index, int newPage)
{
  int i, j, k, auxEnd, address, max_acc = 0, max_i = 0, max_j = 0;

  for (i = 0; i < procVec.size(); i++)
    for (j = 0; j < procVec[i].acc_freq.size(); j++)
    {
      // Não só tem que ser maior que os outros acessos, como tem que estar presente também.
      if (procVec[i].acc_freq[j] >= max_acc && pageTable.pa[procVec[i].page[j]])
      {
        max_acc = procVec[i].acc_freq[j];
        max_i = i;
        max_j = j;
      }
    }

  // Remove a página do processo do bitmap da memória física e do arquivo ep3.mem
  address = pageTable.address[procVec[max_i].page[max_j]];
  for (k = 0; k < ceil((double) p / ((double) s)); k++)
  {
    phyVec[address + k] = false;
    writeMem(0, (address + k) * 4, -1);
  }
  pageTable.address[procVec[max_i].page[max_j]] = -1;
  pageTable.pa[procVec[max_i].page[max_j]] = false;

  // Insere a nova página no bitmap da memória física e do arquivo ep3.mem  
  auxEnd = p / s;
  for(k = address; k < address + auxEnd; k++)
  {
    phyVec[k] = true;
    writeMem(0, k * 4, procVec[index].id);
  }
  pageTable.address[procVec[index].page[newPage]] = address;
  pageTable.pa[procVec[index].page[newPage]] = true;
  pageTable.R[procVec[index].page[newPage]] = true;

  procVec[index].acc_freq[newPage] -= 1;
}

// Second chance
void secondChance(int index, int newPage)
{
  int i, auxEnd, address;
  
  // Busca o próximo elemento da fila com bit R == 0
  while(pageTable.R[scq.front()])
  {
    i = scq.front();
    pageTable.R[i] = false;
    scq.pop();
    scq.push(i);
  }
  
  i = scq.front();
  
  // Remove a página do processo do bitmap da memória física e do arquivo ep3.mem
  address = pageTable.address[i];
  for (int k = 0; k < ceil((double) p / ((double) s)); k++)
  {
    phyVec[address + k] = false;
    writeMem(0, (address + k) * 4, -1);
  }
  pageTable.address[i] = -1;
  pageTable.pa[i] = false;

  // Insere a nova página no bitmap da memória física e do arquivo ep3.mem  
  auxEnd = p / s;
  for(i = address; i < address + auxEnd; i++)
  {
    phyVec[i] = true;
    writeMem(0, i * 4, procVec[index].id);
  }
  pageTable.address[procVec[index].page[newPage]] = address;
  pageTable.pa[procVec[index].page[newPage]] = true;
  pageTable.R[procVec[index].page[newPage]] = true;

  // Remove a página que foi substituida da fila e insere a nova
  scq.pop();
  scq.push(procVec[index].page[newPage]);
}

// Relógio
void clk(int index, int newPage)
{
  int i, auxEnd, address;
  
  // Busca o próximo elemento do lista circular com bit R == 0
  while(pageTable.R[clkv[iClk]])
  {
    pageTable.R[clkv[iClk]] = false;
    iClk = (iClk + 1) % clkv.size();
  }
  
  // Remove a página do processo do bitmap da memória física e do arquivo ep3.mem
  address = pageTable.address[iClk];
  for (int k = 0; k < ceil((double) p / ((double) s)); k++)
  {
    phyVec[address + k] = false;
    writeMem(0, (address + k) * 4, -1);
  }
  pageTable.address[iClk] = -1;
  pageTable.pa[iClk] = false;
  
  // Insere a nova página no bitmap da memória física e do arquivo ep3.mem  
  auxEnd = p / s;
  for(i = address; i < address + auxEnd; i++)
  {
    phyVec[i] = true;
    writeMem(0, i * 4, procVec[index].id);
  }
  pageTable.address[procVec[index].page[newPage]] = address;
  pageTable.pa[procVec[index].page[newPage]] = true;
  pageTable.R[procVec[index].page[newPage]] = true;
  
  clkv[iClk] = procVec[index].page[newPage];
  iClk = (iClk + 1) % clkv.size();
}

// Least Recently Used (quarta versão)
void LRU4 (int index, int newPage)
{
  int i, j, k, auxEnd, address, min_count = 256, min_i = 0, min_j = 0;

  for (i = 0; i < procVec.size(); i++)
    for (j = 0; j < procVec[i].age.size(); j++)
    {
      // Não só tem que ser maior que os outros acessos, como tem que estar presente também.
      if (procVec[i].page[j] >= 0)
      {
        if (binToDec(procVec[i].age[j]) <= min_count && pageTable.pa[procVec[i].page[j]])
        {
          min_count = binToDec(procVec[i].age[j]);
          min_i = i;
          min_j = j;
        }
      }
    }

  // Remove a página do processo do bitmap da memória física e do arquivo ep3.mem
  address = pageTable.address[procVec[min_i].page[min_j]];
  for (k = 0; k < ceil((double) p / ((double) s)); k++)
  {
    phyVec[address + k] = false;
    writeMem(0, (address + k) * 4, -1);
  }
  pageTable.address[procVec[min_i].page[min_j]] = -1;
  pageTable.pa[procVec[min_i].page[min_j]] = false;

  // Insere a nova página no bitmap da memória física e do arquivo ep3.mem  
  auxEnd = p / s;
  for(k = address; k < address + auxEnd; k++)
  {
    phyVec[k] = true;
    writeMem(0, k * 4, procVec[index].id);
  }
  pageTable.address[procVec[index].page[newPage]] = address;
  pageTable.pa[procVec[index].page[newPage]] = true;
  pageTable.R[procVec[index].page[newPage]] = true;
}

/* ____                    ____ ____
   |  | |  | \  / | |    | |  | |  |
   |__| |  |  \/  | |    | |__| |__|
   |  | |  |  /\  | |    | |  | |\
   |  | |__| /  \ | |___ | |  | | \
*/

// Escreve no arquivo ep3.mem (mem == 0) ou ep3.vir (mem == 1)
void writeMem(int mem, int pos, int pid)
{
  if(mem == 0)
  {
    phyMem.seekp(pos);
    phyMem.write((char*) &pid, sizeof(int));
    pM[pos / 4] = pid;
  }
  else if (mem == 1)
  {
    virMem.seekp(pos);
    virMem.write((char*) &pid, sizeof(int));
    vM[pos / 4] = pid;
  }
  else // Este trecho é mais para ajudar na hora de debugar o código
  {
    trace.close();
    cout << "Chamada da função writeMem() com parâmetro 1 incorreto\nFinalizando programa...\n";
    exit(-1);
  }
}

// Libera o espaço de um processo que já terminou da memória virtual
void freeProc(int index)
{
  int i, j, k, len = ceil((double) procVec[index].b / ((double) s));
  for(i = procVec[index].virMemPos; i < len + procVec[index].virMemPos; i++)
  {
    virVec[i] = false;
    writeMem(1, i * 4, -1);
  }
  
  // Libera a entrada na tabela de páginas
  for(j = 0; j < procVec[index].page.size(); j++)
  {
    if(pageTable.address[procVec[index].page[j]] >= 0)
      for (k = 0; k < p / s; k++)
      {
        phyVec[pageTable.address[procVec[index].page[j]] + k] = false;
        writeMem(0, (pageTable.address[procVec[index].page[j]] + k) * 4, -1);
      }
  
    pageTable.pa[procVec[index].page[j]] = false;
    pageTable.address[procVec[index].page[j]] = -2;
    pageTable.R[procVec[index].page[j]] = false;
    
    procVec[index].page[j] = -1;
  }
}

// Insere as partes de cada processo na tabela de páginas
void fillPageTable(int index)
{
  int i, j, finish = 0;
  
  for(i = 0; i < pageTableSize && finish != procVec[index].page.size(); i++)
    if(pageTable.address[i] == -2)
    {
      pageTable.address[i] = -1;
      for(j = 0; j < procVec[index].page.size(); j++)
        if(procVec[index].page[j] == -1)
        {
          procVec[index].page[j] = i;
          finish++;
          break;
        }
    }
}

// Tenta colocar as páginas de um processo na memória física
// return true; == Não precisa executar algoritmo de substituição de página
// return false; == Precisa executar algoritmo de substituição de página
bool pageFit(int index, int wantedPage)
{
  int i, j, len = ceil((double) p / ((double) s));

  // Verifica se a página do processo está presente na memória física. Em caso positivo, não precisa fazer nada
  if(pageTable.pa[procVec[index].page[wantedPage]])
    return true;
  
  // A página do processo não está presente na memória física, então verifica se há espaço livre para inseri-la utilizando a ideia do firstFit
  for(i = 0; i < phyVec.size(); i++)
    if(!phyVec[i])
    {
      for(j = i; j < phyVec.size() && j < i + len; j++)
        if(phyVec[j]) { i = j; break; }
      if(j - i == len)
      {
        for(j = i; j < i + len; j++)
        {
          phyVec[j] = true;
          writeMem(0, j * 4, procVec[index].id);
        }
        
        pageTable.address[procVec[index].page[wantedPage]] = i;
        pageTable.pa[procVec[index].page[wantedPage]] = true;
        pageTable.R[procVec[index].page[wantedPage]] = true; // checar para ver se é isso mesmo
        
        // Optimal
        if          (pag == 1) procVec[index].acc_freq[wantedPage] -= 1;
        // Second chance
        else if     (pag == 2) scq.push(procVec[index].page[wantedPage]);
        // Relógio
        else if     (pag == 3) clkv.push_back(procVec[index].page[wantedPage]);
        // Least Recently Used Ver. 4
        // Não faz nada especial nos acessos, neste caso, apenas a cada pulso do clock.
        //else if     (pag == 4) procVec[index].age[WantedPage].push_front(true);
        return true;
      }
    }
  
  return false;
}

// Converte um número em binário (especificamente de 8 bits) para sua representação em decimal
// min = 0 || max = 255
int binToDec(list<bool> bin)
{
  int dec = 0, i = 7;
  list<bool>::iterator it;
  
  for(it = bin.begin(); it != bin.end(); it++)
  {
    if(*it) dec += pow(2, i);
    i--;
  }
  
  return dec;
}

// Zera os bits R da tabela de páginas
void clearR()
{
  for(int i = 0; i < pageTable.R.size(); i++)
    pageTable.R[i] = false;
}

// Imprime o bitmap da memória física
void printPhyVec()
{
  for(int i = 0; i < phyVec.size(); i++)
    cout << phyVec[i] << " ";
  cout << endl;
}

// Imprime o bitmap da memória virtual
void printVirVec()
{
  for(int i = 0; i < virVec.size(); i++)
    cout << virVec[i] << " ";
  cout << endl;
}

// Imprime a tabela de páginas
void printPageTable()
{
  cout << "Tabela de páginas (Tamanho: " << pageTableSize << ")" << endl;
  for(int i = 0; i < pageTableSize; i++)
    cout << i << ") Address = " << pageTable.address[i] << " || P/A = " << pageTable.pa[i] << " || R = " << pageTable.R[i] << endl;
}

// Imprime o conteúdo do arquivo ep3.mem ou ep3.vir de forma legível ao usuário
void printMem(int mem)
{
  if(mem == 0)
    for(int i = 0; i < pM.size(); i++)
      cout << pM[i] << " ";
  else if(mem == 1)
    for(int i = 0; i < vM.size(); i++)
      cout << vM[i] << " ";
  else // Este trecho é mais para ajudar na hora de debugar o código
  {
    trace.close();
    cout << "Chamada da função printMem() com parãmetro 1 incorreto.\nFinalizando programa....\n";
    exit(-1);
  }
  cout << endl;
}
