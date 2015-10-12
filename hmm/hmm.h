#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define vocab_hash_size 300000

class HMM
{
public:
	HMM();
	void learn_param();
	void learn_bw();
	vector<int> viterbi(vector<string> vec);
	void predict(string path, string rest);

	void read_train(string path);

private:
	int GetWordHash(string word);
	int AddWordToVocab(string word);
	int SearchVocab(string word);
	int s_state(string s);
	double  **A;// A[1...N][1...N], aij is the transformation 
		//prob from i at time t to j at time t+1
	double  **B;// B[1...N][1...M], bik is the prob 
		//observe k at state i
	double  *pi; // pi is the initial state
	int T;// number of sequences
	int N;// number of states;
	int M;// number of obervations

	int state_all ;
	string *state;
	int *vocab_hash;
	int vocab_max_size ;
	vector<string> vocab;
};
