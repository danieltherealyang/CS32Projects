#include "Mesa.h"
	#include "Player.h"
	#include "History.h"
	#include "globals.h"
	#include <iostream>
	using namespace std;

	int main()
	{
	    Mesa m(3, 4);
	    m.addPlayer(2, 4);
	    m.addGark(2, 3);
	    m.addGark(3, 4);
	    m.player()->moveOrAttack(LEFT);
	    m.player()->moveOrAttack(DOWN);
	    m.player()->moveOrAttack(LEFT);
	    m.player()->moveOrAttack(UP);
	    m.player()->moveOrAttack(LEFT);
	    m.player()->moveOrAttack(DOWN);
	    m.history().display();
	    cout << "====" << endl;
	}