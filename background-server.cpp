#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_T 100000
#define MAX_TP 13
#define MAX_TA 13

struct PeriodicTask {
    unsigned t_comp, t_falta_comp, t_period, t_prox_period, t_deadline, t_deadline_original; // computação, período e deadline
    unsigned ex, wt; // tempo de execução e de espera
    char s_symbol; 

    PeriodicTask(unsigned pc=0, unsigned pp=0, unsigned pd=0) {
    t_comp  = t_falta_comp = pc; t_period = pp; t_prox_period = pp; t_deadline = pd, t_deadline_original = pd;
    ex = wt = 0;
    }
};

struct AperiodicTask {
    unsigned t_arrival, t_comp, t_comp_original; // tempo de chegada e de computação
    unsigned ex, wt; // tempo de execução e de espera
    char s_symbol;

    AperiodicTask(unsigned pa=0, unsigned pc=0) {
    t_arrival = pa; t_comp = t_comp_original = pc;
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
    char symbol; // deixei esse atributo público
	Cpu();
	~Cpu();
	void load(int f_pid, char f_symbol, int f_comp, int f_deadline); 
	void run();
	std::string getGrid();
    void addSwitch();   
	int getNumPreemp();
	int getNumContSwitch();
    int getDeadline(); // adicionei este método para retornar o deadline 
    char getLastSymbol(); // adicionei este método para retornar o último símbolo
};

Cpu::Cpu() {
	grid = "";
	numPreemp = 0;
    numContSwitch = 0;
	pid = -1;
	symbol = ' ';
	comput = 0;
	time = 0;
	deadline = -1;
}

Cpu::~Cpu() {}


void Cpu::load(int f_pid, char f_symbol, int f_comp, int f_deadline) {

	if (pid == -1) {// se o processador estiver ocioso
		pid = f_pid; // carrega a tarefa

        // Verificação adicionada para o caso de ir para o IDLE no caso do processador estar ocioso
        if(f_pid != -1){
            ++numContSwitch; // incrementa o número de trocas de contexto quando sai do IDLE
        }
    }

	else { // se o processador estiver ocupado
		if (pid != f_pid) { // se a tarefa carregada for diferente da que está rodando
			pid = f_pid; // carrega a nova tarefa
			++numContSwitch; // incrementa o número de trocas de contexto
			if (comput > 0) { // se a tarefa que estava rodando não terminou
				++numPreemp; // incrementa o número de preempções
            }

            // Verificação adicionada para o caso de ir para o IDLE
            if(f_pid == -1){ // se a tarefa carregada for IDLE
                ++numPreemp; // incrementa o número de preempções a cada ida para o IDLE    
            }
		}
	}
    symbol = f_symbol; // "nome" da tarefa, tipo TA, TB, TC...
	deadline = f_deadline; // deadline da tarefa
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
		grid.append(1, '.'); // coloca um IDLE
	}
}

void Cpu::addSwitch() {
    ++numContSwitch;
}

std::string Cpu::getGrid() {
	return grid;
}

char Cpu::getLastSymbol() {
    return grid[grid.size()-1];
}

int Cpu::getNumPreemp() {
	return numPreemp;
}

int Cpu::getNumContSwitch() {
	return numContSwitch;
}

int Cpu::getDeadline() {
    return deadline;
}

void taskInput(size_t& T, size_t& TP, size_t& TA, std::vector<PeriodicTask>& periodicTasks, std::vector<AperiodicTask>& aperiodicTasks) {
    std::cout << "\n\nDigite o tempo de simulação, número de tarefas periódicas e número de tarefas aperiódicas:" << std::endl;
    std::cin >> T >> TP >> TA;

    periodicTasks.clear();
    aperiodicTasks.clear();

    char aux = 'A';
    int TS;

    //std::cout << "Digite o tempo de simulação, período e deadline das tarefas servidoras:" << std::endl;
    //std::cin >> TS >> TS >> TS;

    std::cout << "Digite a computação, período e deadline das tarefas periódicas:" << std::endl;
    for(size_t i = 0; i < TP; i++) {
        unsigned t_comp, t_period, t_deadline;
        std::cin >> t_comp >> t_period >> t_deadline;
        PeriodicTask tp(t_comp, t_period, t_deadline);
        tp.s_symbol = aux++;
        periodicTasks.push_back(tp);
    }

    std::sort(periodicTasks.begin(), periodicTasks.end(), [](const PeriodicTask& a, const PeriodicTask& b) {
        return a.t_period < b.t_period;
    });

    std::cout << "Digite o tempo de chegada e de computação das tarefas aperiódicas:" << std::endl;
    for(size_t i = 0; i < TA; i++) {
        unsigned t_arrival, t_comp;
        std::cin >> t_arrival >> t_comp;
        AperiodicTask ta(t_arrival, t_comp);
        ta.s_symbol = aux++;
        aperiodicTasks.push_back(ta);
    }
}

int main() {
    size_t T, TP, TA;
    std::vector<PeriodicTask> periodicTasks;
    std::vector<AperiodicTask> aperiodicTasks;
    PeriodicTask tp;
    AperiodicTask ta;
    Cpu cpu;

    taskInput(T, TP, TA, periodicTasks, aperiodicTasks);

    int tempo = 0; 

    while(tempo < T){ 
        bool continuar_procurando = true;
        std::cout << "tempo: " << tempo << std::endl;

        if(!periodicTasks.empty()) { // se houver tarefas periódicas
            PeriodicTask task;
            int achou = 0;
                for(int i = 0; i < periodicTasks.size(); i++) {
                    task = periodicTasks[i];
                    if(task.t_prox_period == tempo) {
                        if(task.t_falta_comp == 0) {
                            task.t_falta_comp += task.t_comp;
                            task.t_deadline = tempo + task.t_deadline_original;
                            task.t_prox_period += task.t_period;
                        }
                        else if(task.t_falta_comp > 0 && task.t_deadline <= tempo) {
                            task.t_falta_comp += task.t_comp;
                            task.t_prox_period += task.t_period;
                        }
                    }
                    if (task.t_falta_comp == task.t_comp && task.t_deadline != task.t_prox_period - task.t_period + task.t_deadline_original) {
                        task.t_deadline = task.t_prox_period - task.t_period + task.t_deadline_original;
                    }

                    if(task.t_falta_comp > 0) {
                        if(continuar_procurando) {
                            achou = i;
                            if(cpu.getLastSymbol() == tolower(task.s_symbol) &&
                            cpu.getDeadline() != task.t_deadline) {
                                cpu.addSwitch();
                            }
                            continuar_procurando = false;
                        }
                        else {
                            task.wt++;
                            task.ex++;
                        }
                    }
                    periodicTasks[i] = task;
                }
            if(!continuar_procurando){
                task = periodicTasks[achou];
                cpu.load(achou, task.s_symbol, task.t_falta_comp, task.t_deadline); // carrega a tarefa
                cpu.run();
                task.ex++;

                task.t_falta_comp = task.t_falta_comp - 1;
                periodicTasks[achou] = task;
                //tempo++; 
            }
        }

        int pTasks = periodicTasks.size();
        if(!aperiodicTasks.empty()) { // se houver tarefas aperiódicas
            for(int i = 0; i < aperiodicTasks.size(); i++) { // para cada tarefa aperiódica
                if(aperiodicTasks[i].t_arrival <= tempo && aperiodicTasks[i].t_comp != 0) {
                    if(!continuar_procurando) {
                        aperiodicTasks[i].ex++;
                        aperiodicTasks[i].wt++;
                    }
                    else {
                        continuar_procurando = false; // para de procurar
                        cpu.load(i+pTasks, aperiodicTasks[i].s_symbol, aperiodicTasks[i].t_comp, T);
                        cpu.run(); 
                        aperiodicTasks[i].ex++;
                        //tempo++;
                        aperiodicTasks[i].t_comp -= 1;
                    }
                }
            }
        }

        if(continuar_procurando && tempo != T) { // se o tempo de simulação não acabou
            cpu.load(-1, '.', 0, T); 
            cpu.run(); 
            //tempo++;
        }
        tempo++;
    }

    std::cout << "\n\n";
    std::cout << "\n\nTarefas: " << std::endl;

    for (size_t i = 0; i < periodicTasks.size(); ++i) {
        const auto& task = periodicTasks[i];

        std::cout << task.s_symbol << ":";
        std::cout << "(" << task.t_comp << ",";
        std::cout << task.t_period << ",";
        std::cout << task.t_deadline << ") -> ";
        //std::cout << task.t_deadline_original << ") -> ";
        std::cout << " ex: " << task.ex << " wt: " << task.wt << std::endl;
    }

    for(size_t i = 0; i < aperiodicTasks.size(); ++i) {
        const auto& task = aperiodicTasks[i];
        std::cout << task.s_symbol << ":";
        std::cout << "(" << task.t_arrival << ",";
        std::cout << task.t_comp << ") -> ";
        //std::cout << task.t_comp_original << ") -> ";
        std::cout << " ex: " << task.ex << " wt: " << task.wt << std::endl;

    }

    std::cout << "Grid:" << cpu.getGrid() << std::endl;
    std::cout << "Número de preempções: " << cpu.getNumPreemp() << std::endl;
    std::cout << "Número de trocas de contexto: " << cpu.getNumContSwitch() << std::endl;
    
   return 0;
}
