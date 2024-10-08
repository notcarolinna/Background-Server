// Cpu.cpp (Roland Teodorowitsch; 30 ago. 2022)

#include <iostream>

using namespace std;

class Cpu {
private:
	int pid;
	char symbol;
	int numPreemp;
	int numContSwitch;
	string grid;
	int comput;
	int deadline;
	unsigned int time;
public:
	Cpu();
	~Cpu();
	void load(int p, char s, int c, int d);
	void run();
	string getGrid();
	int getNumPreemp();
	int getNumContSwitch();
};

Cpu::Cpu() {
	grid = "";
	numPreemp = numContSwitch = 0;
	pid = -1;
	symbol = ' ';
	comput = 0;
	time = 0;
	deadline = -1;
}

Cpu::~Cpu() {}

void Cpu::load(int p, char s, int c, int d) {
	symbol = s;
	deadline = d;
	if (pid == -1)
		pid = p;
	else {
		if (pid != p) {
			pid = p;
			++numContSwitch;
			if (comput > 0)
				++numPreemp;
		}
	}
	comput = c;
}

void Cpu::run() {
	if (pid != -1) {
		if (deadline!=-1 && time>=deadline)
			grid.append(1, tolower(symbol));
		else
			grid.append(1, symbol);
		++time;
		comput--;
	}
	else {
		++time;
		grid.append(1, '_');
	}
}

string Cpu::getGrid() {
	return grid;
}

int Cpu::getNumPreemp() {
	return numPreemp;
}

int Cpu::getNumContSwitch() {
	return numContSwitch;
}

int main() {
	Cpu cpu;
	
	cout << "----- (BS)" << endl;
	cpu.load((0<<5)|0,'A',4,10);
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.load((0<<5)|1,'B',8,20);
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.load((1<<5)|0,'A',4,20);
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.run();
	cpu.load((0<<5)|1,'B',2,20);
	cpu.run();
	cpu.run();
	cpu.load((0<<5)|2,'C',1,-1);
	cpu.run();
	cpu.load((0<<5)|3,'D',1,-1);
	cpu.run();
	cpu.load(26,'.',100000,-1);
	cpu.run();
	cpu.run();
	cpu.load((2<<5)|0,'A',4,30);

	cout << cpu.getGrid() << endl;
	cout << cpu.getNumContSwitch() << " " << cpu.getNumPreemp() << endl;

	return 0;
}
