#include <iostream>

class Task {
private:
	int pid; // identificador do processo
	char symbol; // idle = '.'
	int numPreemp;
	int numContSwitch;
	int comput;
	int deadline;
	unsigned int time; 
	std::string grid;
public:
	Task();
	~Task();
	void load(int p, char s, int c, int d);
	void run();
	std::string getGrid();
	int getNumPreemp();
	int getNumContSwitch();
};

Task::Task() {
	grid = "";  
	numPreemp = numContSwitch = 0;
	pid = -1;
	symbol = ' ';
	comput = 0;
	time = 0;
	deadline = -1;
}

Task::~Task() {}


// Carrega um processo na CPU
void Task::load(int p, char s, int c, int d) {
	symbol = s; // simbolo do processo
	deadline = d; // deadline do processo
	if (pid == -1) // se a CPU estiver ociosa
		pid = p; // carrega o processo
	else { // se a CPU estiver ocupada
		if (pid != p) { // se o processo carregado for diferente do processo que estava sendo executado
			pid = p; // carrega o processo
			++numContSwitch; // incrementa o numero de trocas de contexto
			if (comput > 0) // se o processo que estava sendo executado nao terminou
				++numPreemp; // incrementa o numero de preempcoes
		}
	}
	comput = c; // tempo de computacao do processo
}

// Executa um ciclo de clock
void Task::run() {
	if (pid != -1) { // se a CPU estiver ocupada
		if (deadline!=-1 && time>=deadline)  // se o processo perdeu o deadline
			grid.append(1, tolower(symbol)); // insere o simbolo do processo em minusculo
		else // se o processo nao perdeu o deadline
			grid.append(1, symbol); // insere o simbolo do processo
		++time; // incrementa o tempo
		comput--; // decrementa o tempo de computacao do processo
	}
	else { // se a CPU estiver ociosa
		++time; // incrementa o tempo
		grid.append(1, '_'); // insere o simbolo de idle
	}
}

// Retorna a grade de execucao
std::string Task::getGrid() {
	return grid;
}

// Retorna o numero de preempcoes
int Task::getNumPreemp() {
	return numPreemp;
}

// Retorna o numero de trocas de contexto
int Task::getNumContSwitch() {
	return numContSwitch;
}


int main() {
	Task Task; 
	
	std::cout << "----- (BS)" << std::endl; 
	Task.load((0<<5)|0,'A',4,10); // carrega o processo A
	Task.run(); // executa um ciclo de clock
	Task.run(); 
	Task.run();
	Task.run();
	Task.load((0<<5)|1,'B',8,20); // carrega o processo B
	Task.run();
	Task.run();
	Task.run();
	Task.run();
	Task.run();
	Task.run();
	Task.load((1<<5)|0,'A',4,20); // carrega o processo A
	Task.run();
	Task.run();
	Task.run();
	Task.run();
	Task.load((0<<5)|1,'B',2,20); // carrega o processo B
	Task.run();
	Task.run();
	Task.load((0<<5)|2,'C',1,-1); // carrega o processo C
	Task.run();
	Task.load((0<<5)|3,'D',1,-1); // carrega o processo D
	Task.run();
	Task.load(26,'.',100000,-1); // carrega o processo idle
	Task.run();
	Task.run();
	Task.load((2<<5)|0,'A',4,30);  // carrega o processo A

	std::cout << Task.getGrid() << std::endl;
	std::cout << Task.getNumContSwitch() << " " << Task.getNumPreemp() << std::endl; 

	return 0;
}
