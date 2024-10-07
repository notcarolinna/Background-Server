#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_T 100000
#define MAX_TP 13
#define MAX_TA 13

struct PeriodicTask {
    unsigned t_comp, t_period, t_deadline, t_deadline_original; // computação, período e deadline
    unsigned ex, wt; // tempo de execução e de espera
    char s_symbol; 

    PeriodicTask(unsigned pc=0, unsigned pp=0, unsigned pd=0) {
    t_comp = pc; t_period = pp; t_deadline = pd, t_deadline_original = pd;
    ex = wt = 0;
    }

};

struct AperiodicTask {
    unsigned t_arrival, t_comp, t_deadline_original; // tempo de chegada e de computação
    unsigned ex, wt; // tempo de execução e de espera
    char s_symbol;

    AperiodicTask(unsigned pa=0, unsigned pc=0) {
    t_arrival = pa; t_comp = pc;
    ex = wt = 0;
    }
};

class Cpu {
private:
	int pid;
	int numPreemp;
	int numContSwitch;
	std::string grid;
	int comput;
	int deadline;
	unsigned int time;
public:
    char symbol;
	Cpu();
	~Cpu();
	void load(int f_pid, char f_symbol, int f_comp, int f_period, int f_deadline); 
	void run();
	std::string getGrid();
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

void Cpu::load(int f_pid, char f_symbol, int f_comp, int f_period, int f_deadline) {
	symbol = f_symbol; // "nome" da tarefa, tipo TA, TB, TC...
	deadline = f_deadline; // deadline da tarefa
	if (pid == -1) // se o processador estiver ocioso
		pid = f_pid; // carrega a tarefa
	else {
		if (pid != f_pid) { // se a tarefa carregada for diferente da que está rodando
			pid = f_pid; // carrega a nova tarefa
			++numContSwitch; // incrementa o número de trocas de contexto
			if (comput > 0) // se a tarefa que estava rodando não terminou
				++numPreemp; // incrementa o número de preempções
		}
	}
	comput = f_comp; 
}

void Cpu::run() {
	if (pid != -1) {
		if (deadline != -1 && time >= static_cast<unsigned int>(deadline)) // se a tarefa perdeu o deadline
			grid.append(1, tolower(symbol)); // coloca em minúsculo
		else // se a tarefa não perdeu o deadline
			grid.append(1, symbol); // coloca em maiúsculo
		++time; 
		comput--; // uma unidade de computação foi feita
	}
	else {
		++time; // se o processador estiver ocioso incrementa o tempo
		grid.append(1, '_'); // coloca um IDLE
	}
}

std::string Cpu::getGrid() {
	return grid;
}

int Cpu::getNumPreemp() {
	return numPreemp;
}

int Cpu::getNumContSwitch() {
	return numContSwitch;
}


int main() {

    PeriodicTask tp;
    AperiodicTask ta;
    Cpu cpu;

    std::cout << "\n\nDigite o tempo de simulação, número de tarefas periódicas e número de tarefas aperiódicas:" << std::endl;
    size_t T, TP, TA;
    std::cin >> T >> TP >> TA;

    std::vector<PeriodicTask> periodicTasks;
    std::vector<AperiodicTask> aperiodicTasks;
    std::vector<int> v_comput;
    
    char aux = 'A';

    std::cout << "Digite a computação, período e deadline das tarefas periódicas:" << std::endl;
    for(size_t i = 0; i < TP; i++){ // carrega as tarefas periódicas
        std::cin >> tp.t_comp >> tp.t_period >> tp.t_deadline;
        tp = PeriodicTask(tp.t_comp, tp.t_period, tp.t_deadline);
        tp.s_symbol = aux;
        periodicTasks.push_back(tp);
        v_comput.push_back(tp.t_period);
        aux++;
       //cpu.load(i, tp.s_symbol, tp.t_comp, tp.t_period, tp.t_deadline);
    }

    // Ordena as tarefas periódicas em ordem crescente de período
    std::sort(periodicTasks.begin(), periodicTasks.end(), [](const PeriodicTask& a, const PeriodicTask& b) {
        return a.t_period < b.t_period;
    });

    std::cout << "Digite o tempo de chegada e de computação das tarefas aperiódicas:" << std::endl;
    for(size_t i = 0; i < TA; i++){ // carrega as tarefas aperiódicas
        std::cin >> ta.t_arrival >> ta.t_comp;
        ta = AperiodicTask(ta.t_arrival, ta.t_comp);
        ta.s_symbol = aux;
        aperiodicTasks.push_back(ta);
        aux++;
    }
    
    int tempo = 0; 
    int* vezes_computadas = new int[TP];
    for(int i = 0; i < TP; i++){
        vezes_computadas[i] = 0;
    }

    int prev_number = -1;
    if(!periodicTasks.empty()){ // se houver tarefas periódicas
        for(int i = 0; i < periodicTasks.size(); i++){ // para cada tarefa periódica

            if(i != 0) 
                for(int j = 0; j < i; j++){ // para cada tarefa periódica anterior

                    if(tempo % periodicTasks[j].t_period == 0){
                        vezes_computadas[j] = 0;
                        periodicTasks[j].t_deadline = tempo + periodicTasks[j].t_deadline_original;
                    }
                if(vezes_computadas[j] != -1)
                    i = j;
                }

            if(prev_number != i)
                cpu.load(i, periodicTasks[i].s_symbol, periodicTasks[i].t_comp, periodicTasks[i].t_period, periodicTasks[i].t_deadline); // carrega a tarefa
            cpu.run(); // roda o processador

            vezes_computadas[i]++;
            prev_number = i;
            tempo++;
            if(vezes_computadas[i] == periodicTasks[i].t_comp)
                vezes_computadas[i] = -1;
            else if (vezes_computadas[i] < periodicTasks[i].t_period && vezes_computadas[i] != -1)
                i--;
        }
    }

    std::cout << "\n\n";
    std::cout << "\n\nTarefas: " << std::endl;

    for (size_t i = 0; i < periodicTasks.size(); ++i) {
        const auto& task = periodicTasks[i];
        std::cout << task.s_symbol << ":";
        std::cout << "(" << task.t_comp << ",";
        std::cout << task.t_period << ",";
        std::cout << task.t_deadline << ")" << std::endl;
    }

    for(size_t i = 0; i < aperiodicTasks.size(); ++i) {
        const auto& task = aperiodicTasks[i];
        std::cout << task.s_symbol << ":";
        std::cout << "(" << task.t_arrival << "," << task.t_comp << ")" << std::endl;
    }

    std::cout << "Grid:" << cpu.getGrid() << std::endl;
    std::cout << "Número de preempções: " << cpu.getNumPreemp() << std::endl;
    std::cout << "Número de trocas de contexto: " << cpu.getNumContSwitch() << std::endl;
    
   return 0;
}
