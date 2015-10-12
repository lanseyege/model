#include "hmm.h"

HMM::HMM()
{
	N = 0;
	M = 0;
	state_all = 100;
	vocab_max_size = 1000;
	A = new double*[state_all];
	B = new double*[state_all];//[vocab_hash_size];
	pi = new double[state_all];
	state = new string[state_all];
	for(int i = 0; i < state_all; i++)
	{
		state[i] = "";
		pi[i] = 0;
		A[i] = new double[state_all];
		B[i] = new double[vocab_hash_size];
		
	}
	for(int i = 0; i < state_all; i++)
	{
		for(int j = 0; j < vocab_hash_size; j++)
		{
			B[i][j] = 0;
		}
		for(int j = 0; j < state_all; j++)
		{
			A[i][j] = 0;
		}
	}

	vocab_hash = new int[vocab_hash_size];
	for(int i = 0; i < vocab_hash_size; i++)
		vocab_hash[i] = -1;
}
int HMM::GetWordHash(string word)
{
	unsigned long long a, hash = 0;
	for(a = 0; a < word.length(); a++)
		hash = hash * 257 + word[a];
	hash = hash % vocab_hash_size;
	return hash;
}
int HMM::AddWordToVocab(string word)
{
	int hash, n = word.length();
	vocab.push_back(word);

	M++;

	hash = GetWordHash(word);
	while (vocab_hash[hash] != -1) 
		hash = (hash + 1) % vocab_hash_size;
	vocab_hash[hash] = M - 1;
	return M - 1;
}
int HMM::SearchVocab(string word)
{
	unsigned int hash = GetWordHash(word);
	while (true)
	{
		if (vocab_hash[hash] == -1) return -1;
		if (word == vocab[vocab_hash[hash]])//.word)
			return vocab_hash[hash];
		hash = (hash + 1) % vocab_hash_size;
	}
	return -1;
}
int HMM::s_state(string s)
{
	for(int i =0 ; i < N; i++)
	{
		if(state[i] == s){
			return i;
		}
	}
	state[N] = s;
	N ++ ;
	return N - 1;
}

void HMM::read_train(string path)
{
	ifstream file;
	file.open(path.c_str());
	if( ! file.is_open())
	{
		exit(-1);
	}
	cout<<"read train file..."<<endl;
	string s,temp;int a,b,prev=0;int c = 0;
	while (! file.eof())
	{
		c++;
		if(c % 10000 == 0)
			cout<<"has read line "<<c<<endl;
		getline(file, s);		 
		if(s.length() < 1)
			 continue;
		a = s.find(" ", 0);		
		temp = s.substr(0, a);
		b = SearchVocab(temp);
		if(b == -1) 
		{
			b = AddWordToVocab(temp);
		}
		temp = s.substr(a+1, s.length());
		a = s_state(temp);//get state's index
		A[prev][a] ++;
		B[a][b] ++;
		pi[a] ++;
		prev = a;		
	}
	cout<<"all number of line :"<<c<<endl;
	file.close();
	cout<<"train file read over"<<endl;
}
void HMM::learn_param()
{
	double sum = 0, pis = 0;
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			sum += A[i][j];
		}
		for(int j = 0; j < N; j++)
		{
			A[i][j] /= sum;
		}
		sum = 0;
		for(int k = 0; k < M; k++)
		{
			sum += B[i][k];
		}
		for(int k = 0; k < M; k++)
		{
			B[i][k] /= sum;
		}
		pis += pi[i];
	}
	for(int i = 0; i < N; i++)
	{
		pi[i] /= pis;
	}
}
void HMM::predict(string path, string rest)
{
	ifstream file;
	ofstream ofile;
	ofile.open(rest.c_str());
	file.open(path.c_str());
	if( ! file.is_open())
	{
		exit(-1);
	}
	cout<<"read test file "<<endl;
	string s, temp;
	int a, b = 0, r = 0, sum = 0;
	vector<string> vec;
	vector<int> stt_vec;
	vector<int> optimal_path;
	while (! file.eof())
	{
		getline(file, s);
		if(s.length() < 1 )
		{
			if(b == 0) continue;
			optimal_path = viterbi(vec);
			for(int i = 0; i < b; i++)
			{
				if(optimal_path[b-i-1] == stt_vec[i])
					r++;
				ofile<<vec[i]<<" "<<state[stt_vec[i]]<<" "<<
					state[optimal_path[b-i-1]]<<"\n";
			}
			ofile<<"\n";
			b = 0;
			optimal_path.clear();
			stt_vec.clear();
			vec.clear();
			continue;
		}
		a = s.find(" ", 0);
		vec.push_back(s.substr(0,a));
		stt_vec.push_back(s_state(s.substr(a+1,s.length())));
		b++;
		sum++;
	}
	file.close();ofile.close();
	cout<<"correct predict: "<<r<<" all num: "<<sum<<endl;
	cout<<"correct rate: "<<1.0 * r / sum<<endl; 
}
vector<int> HMM::viterbi(vector<string> vec)
{
	unsigned n = vec.size();
	int index;
	int *seten;//[n] 
	double **sigma;//[n][NN] 
	int **lamda;//[n][NN] 
	seten = new int[n];
	sigma = new double*[n];
	lamda = new int*[n];
	for(unsigned i = 0; i < n; i++)
	{
		seten[i] = 0;
		sigma[i] = new double[N];
		lamda[i] = new int[N];
		for(int j = 0; j < N; j++)
		{
			sigma[i][j] = 0;
			lamda[i][j] = 0;
		}
	}
	for(unsigned i = 0; i < n; i++)
	{
		index = SearchVocab(vec[i]);
		if(index > -1)
			seten[i] = index;
		else
			seten[i] = N;
	}
	// Initialization
//	cout<<"Initialization"<<endl;
	for(int i = 0; i < N; i++)
	{
		sigma[0][i] = pi[i] * B[i][seten[0]];
		lamda[0][i] = 0;
	}
	//Recursive
//	cout<<"Recursive"<<endl;
	double max = 0, temp;
	int max_j;
	for(unsigned t = 1; t < n; t++)
	{
		for(int i = 0; i < N; i++)
		{			
			for(int j = 0; j < N; j++)
			{
				temp = sigma[t-1][j] * A[j][i];
				if(temp > max)
				{
					max = temp;
					max_j = j;
				}
			}
			sigma[t][i] = max * B[i][seten[t]];
			lamda[t][i] = max_j;
			max = 0;
		}
	}
	//Finish
//	cout<<"Finish"<<endl;
	max = 0.0;
	for(int i = 0; i < N; i++)
	{
		temp = sigma[n-1][i];
		if(temp > max)
		{
			max = temp;
			max_j = i;
		}
	}
	//BackTracking
//	cout<<"BackTracking"<<endl;
	vector<int> optimal_path;
	optimal_path.push_back(max_j);

	for(int t = n - 2; t >= 0; t--)
	{
		max_j = lamda[t+1][max_j];
		optimal_path.push_back(max_j);
	}

	return optimal_path;
}
int ArgPos(char *str, int argc, char **argv)
{
    int a;
    
    for (a=1; a<argc; a++) if (!strcmp(str, argv[a])) return a;
    
    return -1;
}
int main(int argc, char *argv[])
{
	string train ;
	string test ;
	string rest;
	if(argc == 1)
	{
		cout<<"hmm toolkit v 1.0 "<<endl;		
		cout<<"Options:"<<endl;
		cout<<"\t-train <file>"<<endl;
		cout<<"\t\ttrain file path ,file format is:"<<endl;
		cout<<"\t\t上海 B-NP\n\t\t浦东 I-NP\n\t\t开发 B-NP\n"<<
			"\t\t与 I-NP\n\t\t法制 I-NP\n\t\t建设 I-NP\n\t\t同步 B-VP"<<endl;
		cout<<"\t-test <file>"<<endl;
		cout<<"\t\ttest file path"<<endl;
		cout<<"\t-rest <file>"<<endl;
		cout<<"\t\tstore rest file path"<<endl;
		return 0;
	}
	int b;
	if((b = ArgPos((char *)"-train", argc, argv)) > 0) 
		train = argv[b + 1];
	if((b = ArgPos((char *)"-test", argc, argv)) > 0)
		test = argv[b + 1];
	if((b = ArgPos((char *)"-rest", argc, argv)) > 0)
		rest = argv[b + 1];
	cout<<"start ..."<<endl;
	HMM * hmm = new HMM();
	hmm->read_train(train);
	hmm->learn_param();
	hmm->predict(test, rest);
}
