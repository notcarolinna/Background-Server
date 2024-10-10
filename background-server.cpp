#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_T 100000
#define MAX_TPA 13

struct PeriodicTask {
    unsigned int t_comp, t_falta_comp, t_period, t_prox_period, t_deadline, t_deadline_original; // computação, período e deadline
    unsigned int ex, wt; // tempo de execução e de espera
    char s_symbol; 

    PeriodicTask(unsigned pc=0, unsigned pp=0, unsigned pd=0) {
    t_comp  = t_falta_comp = pc; t_period = pp; t_prox_period = pp; t_deadline = pd, t_deadline_original = pd;
    ex = wt = 0;
    }
};

struct AperiodicTask {
    unsigned int t_arrival, t_comp, t_comp_original; // tempo de chegada e de computação
    unsigned int ex, wt; // tempo de execução e de espera
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
	void load(int f_pid, char f_symbol, int f_comp, unsigned int f_deadline); 
	void run();
	std::string getGrid();
    void addSwitch();   
	int getNumPreemp();
	int getNumContSwitch();
    unsigned int getDeadline(); // adicionei este método para retornar o deadline 
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


void Cpu::load(int f_pid, char f_symbol, int f_comp, unsigned int f_deadline) {

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

unsigned int Cpu::getDeadline() {
    return deadline;
}

std::vector<std::pair<int,std::pair<std::vector<PeriodicTask>, std::vector<AperiodicTask>>>> taskInput() {
    std::vector<std::pair<int,std::pair<std::vector<PeriodicTask>, std::vector<AperiodicTask>>>> inputs;

    std::vector<PeriodicTask> periodicTasks;
    std::vector<AperiodicTask> aperiodicTasks;
    size_t T, TP, TA;

    for(size_t i = 0; i < MAX_TPA; i++) {
        //std::cout << "\n\nDigite o tempo de simulação, número de tarefas periódicas e número de tarefas aperiódicas:" << std::endl;
        std::cin >> T >> TP >> TA;

        if(T == 0 && TP == 0 && TA == 0) {
            break;
        }

        periodicTasks.clear();
        aperiodicTasks.clear();

        char aux = 'A';
        int TS;

        //std::cout << "Digite o tempo de simulação, período e deadline das tarefas servidoras:" << std::endl;
        std::cin >> TS >> TS >> TS;

        //std::cout << "Digite a computação, período e deadline das tarefas periódicas:" << std::endl;
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

        //std::cout << "Digite o tempo de chegada e de computação das tarefas aperiódicas:" << std::endl;
        for(size_t i = 0; i < TA; i++) {
            unsigned t_arrival, t_comp;
            std::cin >> t_arrival >> t_comp;
            AperiodicTask ta(t_arrival, t_comp);
            ta.s_symbol = aux++;
            aperiodicTasks.push_back(ta);
        }
        inputs.push_back(std::make_pair(T, std::make_pair(periodicTasks, aperiodicTasks)));
    }
    return inputs;
}

int main() {
    Cpu cpu;
    size_t T;
    size_t tempo; 
    std::vector<PeriodicTask> periodicTasks;
    std::vector<AperiodicTask> aperiodicTasks;
    std::vector<std::pair<int,std::pair<std::vector<PeriodicTask>, std::vector<AperiodicTask>>>> inputs;

    std::cout << "ENTRADAS" << std::endl;
    inputs = taskInput();
    std::cout << "SAIDAS" << std::endl;

    for(size_t j = 0; j < inputs.size(); j++) {
        cpu = Cpu();
        tempo = 0;
        T = inputs[j].first;
        periodicTasks = inputs[j].second.first;
        aperiodicTasks = inputs[j].second.second;

        while(tempo < T){ 
            bool continuar_procurando = true;

            if(!periodicTasks.empty()) { 
                PeriodicTask task;
                int achou = 0;
                    for(size_t i = 0; i < periodicTasks.size(); i++) {
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
                }
            }

            size_t pTasks = periodicTasks.size();
            if(!aperiodicTasks.empty()) { 
                for(size_t i = 0; i < aperiodicTasks.size(); i++) {
                    if(aperiodicTasks[i].t_arrival <= tempo && aperiodicTasks[i].t_comp != 0) {
                        if(!continuar_procurando) {
                            aperiodicTasks[i].ex++;
                            aperiodicTasks[i].wt++;
                        }
                        else {
                            continuar_procurando = false; 
                            cpu.load(i+pTasks, aperiodicTasks[i].s_symbol, aperiodicTasks[i].t_comp, T);
                            cpu.run(); 
                            aperiodicTasks[i].ex++;
                            aperiodicTasks[i].t_comp -= 1;
                        }
                    }
                }
            }

            if(continuar_procurando && tempo != T) { // se o tempo de simulação não acabou
                cpu.load(-1, '.', 0, T); 
                cpu.run(); 
            }
            tempo++;
        }

        std::cout << "\n\n";

        std::cout << cpu.getGrid() << std::endl;
        std::cout << cpu.getNumPreemp();
        std::cout << " " << cpu.getNumContSwitch() << std::endl;

        for (size_t i = 0; i < periodicTasks.size(); ++i) {
            const auto& task = periodicTasks[i];
            std::cout << task.ex << " " << task.wt << std::endl;
        }

        for(size_t i = 0; i < aperiodicTasks.size(); ++i) {
            const auto& task = aperiodicTasks[i];
            std::cout << task.ex << " " << task.wt << std::endl;

        }
    }
   return 0;
}
