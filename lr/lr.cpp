#include "lr.h"

LR::LR(vector<vector<double> > x, vector<int> y, double alpha, double beta, int times)
{
	LR::x=x;
	LR::y=y;
	LR::alpha=alpha;
	LR::beta=beta;
	LR::times=times;
	for(unsigned i=0;i<x[0].size();i++)
	{
		LR::theta.push_back(2.0 * rand() / RAND_MAX - 1.0);
	}
	
}
void LR::log_normal()//vector<vector<double>> x, vector<int> y
{
	unsigned m=x.size();
	unsigned n=x[0].size();
	vector<double> temp(m);
	double res=0;
	for(int iter=0; iter < times; iter++)
	{
		temp = array_multiply(x, y, theta);
		for(unsigned i=0; i<n; i++)
		{
			for(unsigned j=0; j<m;j++)
			{
				res += temp[j] * x[j][i];
			}
			theta[i] -= alpha * res;
			res = 0;
		}

	}
}
void LR::log_l2()
{
	unsigned m=x.size();
	unsigned n=x[0].size();
	vector<double> temp(m);
	double res=0;
	for(int iter=0; iter < times; iter++)
	{
		temp = array_multiply(x, y, theta);
		for(unsigned i=0; i<n; i++)
		{
			for(unsigned j=0; j<m;j++)
			{
				res += temp[j] * x[j][i];
			}
			theta[i] = theta[i] - alpha * res - beta * theta[i];
			res = 0;
		}
	}
}

vector<double> LR::array_multiply(vector<vector<double> > x, vector<int> y, vector<double> theta)
{
	unsigned n = theta.size();
	unsigned m = x.size();
	vector<double> temp(m);
	for(unsigned i=0; i<m;i++)
	{
		temp[i] = sigm(array_m(x[i], theta)) - y[i];
	}
	return temp;
}
double LR::array_m(vector<double> x, vector<double> theta)
{
	unsigned n = theta.size();
	double res = 0;
	for(unsigned i=0; i<n; i++)
	{
		res += x[i] * theta[i];
	}
	return res;
}
double LR::sigm(double a)
{
	return 1.0 / (1 + exp(-1.0 * a));
}
void LR::predict(vector<vector<double> > x, vector<int> y)
{

	unsigned m=x.size();
	unsigned n=x[0].size();
	double a;int r=0;
	for(unsigned i=0;i<m;i++) 
	{
		a=sigm(array_m(x[i],LR::theta));
		if(a<0.5)
		{
			if(y[i]==0)
				r++;
		}
		else {
			if(y[i]==1)
				r++;
		}
	}
	double rs=1.0*r/m;
	cout<<"correct rate is "<<r<<" / "<<m<<": "<<rs<<endl;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
int ArgPos(char *str, int argc, char **argv)
{
    int a;
    
    for (a=1; a<argc; a++) if (!strcmp(str, argv[a])) return a;
    
    return -1;
}
int main(int argc, char* argv[])
{
	string train_path ;
	string test_path ;
	double alpha = 0.0001;
	double beta = 0.1;
	int times = 30;
	int normal = 0;
	if(argc == 1)
	{
		cout<<"logistic toolkit v 1.0"<<endl;
		cout<<"Options:"<<endl;
		cout<<"\t-train <file>"<<endl;
		cout<<"\t\t train file path, file format is libsvm's format"<<endl;
		cout<<"\t-test <file>"<<endl;
		cout<<"\t\t test file path"<<endl;
		cout<<"\t-alpha <double>"<<endl;
		cout<<"\t\tdefault is 0.0001"<<endl;
		cout<<"\t-beta <double>"<<endl;
		cout<<"\t\tdefault is 0.1"<<endl;
		cout<<"\t-times <int>"<<endl;
		cout<<"\t\tdefault is 30"<<endl;
		cout<<"\t-normal <int>"<<endl;
		cout<<"\t\tdefault is no punish, the number is 0, others implement that has punish"<<endl;
		
		return 0;
	}
	int b;
	if((b = ArgPos((char *)"-train", argc, argv)) > 0) 
		train_path = argv[b + 1];
	if((b = ArgPos((char *)"-test", argc, argv)) > 0)
		test_path = argv[b + 1];
	if((b = ArgPos((char *)"-alpha", argc, argv)) > 0)
		alpha = ::atof(argv[b + 1]);
	if((b = ArgPos((char *)"-beta", argc, argv)) > 0)
		beta = ::atof(argv[b + 1]);
	if((b = ArgPos((char *)"-times", argc, argv)) > 0)
		times = ::atoi(argv[b + 1]);
	if((b = ArgPos((char *)"-normal", argc, argv)) > 0)
		normal = ::atoi(argv[b + 1]);

	vector<vector<double> > x;
	vector<int> y;
	vector<double> x_temp;
	ifstream file;
	file.open(train_path.c_str());//"D:\\Program Files\\data\\ijcnn1\\ijcnn1.t");
	string s;
	if(! file.is_open())
	{
		exit(-1);	
	}
	
	unsigned i=0,k=0,a=0;
	vector<string> temp;
	while(! file.eof()){	
		getline(file,s);
		if(s.length()<2) break;		
		temp = split(s , ' ');
		if(temp[0]=="+1"||temp[0]=="1"||temp[0]=="+1.0"||temp[0]=="1.0")
			y.push_back(1);
		else
			y.push_back(0);
		x_temp.push_back(1);
		for(i=1;i<temp.size();i++)
		{
			a=temp[i].find(":",0);
			x_temp.push_back(atof(temp[i].substr(a+1,temp[i].length()).c_str()));
		}
		x.push_back(x_temp);
		x_temp.clear();
		k++;
		temp.clear();
	}
	cout<<"train file line: "<<x.size()<<" features: "<<x[0].size()<<endl;
	file.close();		
	
	file.open(test_path.c_str());//"D:\\Program Files\\data\\ijcnn1\\ijcnn1.tr");
	vector<vector<double> > xx;
	vector<int> yy;
	if(! file.is_open())
	{
		exit(-1);	
	}
	i=0,k=0,a=0;	
	while(! file.eof()){
		getline(file,s);
		if(s.length()<2) break;
		temp = split(s , ' ');
		if(temp[0]=="+1"||temp[0]=="1"||temp[0]=="+1.0"||temp[0]=="1.0")
			yy.push_back(1);
		else
			yy.push_back(0);
		x_temp.push_back(1);
		for(i=1;i<temp.size();i++)
		{
			a=temp[i].find(":",0);
			x_temp.push_back(atof(temp[i].substr(a+1,temp[i].length()).c_str()));
		}
		xx.push_back(x_temp);
		x_temp.clear();
		k++;
		temp.clear();
	}
	file.close();
	cout<<"test file lines: "<<xx.size()<<" features: "<<xx[0].size()<<endl;
	
	cout<<"parameters: times "<<times<<", alpha "<<alpha<<", beta "<<beta<<endl;
	cout<<"the formal is : "<<normal<<endl;
	LR *lr = new LR(x, y, alpha, beta, times);
	if(normal == 0)
		lr->log_normal();
	else
		lr->log_l2();
	lr->predict(xx,yy);
	
}
