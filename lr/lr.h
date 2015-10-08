#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;

class LR
{
public:
	LR(vector<vector<double> > x, vector<int> y, double alpha, double beta, int times);
	void logstic();//vector<vector<double>> x, vector<int> y, vector<double> theta, int times);
	void log_normal();
	void log_l2();
	void log_stomatic();
	vector<vector<double> > x;
	vector<int> y;
	vector<double> theta;
	int times;
	double alpha;
	double beta;
	void predict(vector<vector<double> > x, vector<int> y);
private:
	double sigm(double a);
	vector<double> array_multiply(vector<vector<double> > x, vector<int> y, vector<double> theta);
	double array_m(vector<double> x, vector<double> theta);
};
