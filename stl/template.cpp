#include<iostream>
using namespace std;

template <class T>
class mycontainer{
	T element;
	public:
		mycontainer (T arg) {element = arg;}
		T increase () {return ++element;}
};

template <>
class mycontainer <char> {
	char element;
	public:
		mycontainer (char arg) {element = arg;}
		char uppercase ()
		{
			if ((element >= 'a') && (element <= 'z'))
				element+= 'A'-'a';
			return element;
		}
};

int main(){
	mycontainer<double> myint(7.89);
	mycontainer<char> mychar ('j');
	cout << myint.increase()<<endl;
	cout << mychar.uppercase() << endl;
	return 0;
}

