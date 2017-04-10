#include <bits/stdc++.h>
using namespace std;

int main()
{
  int pf;
  double t, t_count = 0, t_mean = 0, var = 0, dp, max_v = 0, min_v = 100;
  vector<double> ts;
  fstream input;
  string line;

  while(cin >> t >> pf)
  {
    t_mean += t;
    t_count++;
    ts.push_back(t);
    if(t > max_v) max_v = t;
    if(t < min_v) min_v = t;
  }
  t_mean /= t_count; // média
  
  // Desvio padrão
  for(int i = 0; i < ts.size(); i++)
    var += pow((ts[i] - t_mean), 2);
  var /= (ts.size() - 1);
  dp = sqrt(var);
  
  cout << endl << (t_mean - 1.96 * dp) << " " << (t_mean + 1.96 * dp) << " " << max_v << " " << min_v << endl << endl;
}

