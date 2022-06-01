#include <iostream>

using namespace std;

class base
{
	public:
		int i;
		int j;
		base(int i, int j)
		{
			this->i = i;
			this->j = j;
		}
		virtual void print()
		{
			cout << "base" << endl;
		}

};

class sub : public base
{
	public:
		sub(int i, int j) : base(i, j)
		{
			cout << "sub" << endl;
		}
		void print()
		{
			cout << "sub" << endl;
		}
};

class sub1 : public sub
{
	public:
		sub1(int i, int j) : sub(i, j)
		{
			cout << "sub111" << endl;
		}
		void print()
		{
			cout << "sub1" << endl;
		}
};

int main()
{
	sub1 s(1, 2);
	s.print();
	return 0;
}