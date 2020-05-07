#include "..\..\blakelib\blakelib.h"

using namespace std;

struct Thing {
	bool Alive;
	Thing() { Alive = true; }
};

void print_vec(vector<Thing> &v) 
{
	for (auto &i : v)
		cout << i.Alive << " ";
	cout << endl;
}

int main() {
	vector<Thing> v(15);
	print_vec(v);
	v[3].Alive = false;
	v[8].Alive = false; 
	v[11].Alive = false;
	v[0].Alive = false;
	print_vec(v);
	auto _beg_alive_it = partition(v.begin(), v.end(), [](const Thing& t) { return t.Alive == false; });
	int dead_count = _beg_alive_it - v.begin();
	print_vec(v);
	cout << "_beg_alive_it =" << (*_beg_alive_it).Alive << endl;
	cout << "Count: " << dead_count << endl;
	return 0;
}