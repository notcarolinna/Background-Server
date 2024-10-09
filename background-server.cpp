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
    unsigned t_arrival, t_comp; // tempo de chegada e de computação
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
	void load(int f_pid, char f_symbol, int f_comp, int f_deadline); 
	void run();
	std::string getGrid();
	int getNumPreemp();
	int getNumContSwitch();
};

Cpu::Cpu() {
	grid = "";
	numPreemp = 0;
    numContSwitch = -1;
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
        if(f_pid != -1){
            ++numContSwitch; // incrementa o número de trocas de contexto
        }
    }
	else { // se o processador estiver ocupado
		if (pid != f_pid) { // se a tarefa carregada for diferente da que está rodando
			pid = f_pid; // carrega a nova tarefa
			++numContSwitch; // incrementa o número de trocas de contexto
			if (comput > 0) { // se a tarefa que estava rodando não terminou
				++numPreemp; // incrementa o número de preempções
                std::cout << "Preempção na tarefa " << symbol << " no tempo " << time << std::endl;
            }
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
    int prev_number = -1;

    while(tempo < T){ 
        bool continuar_procurando = true;

        if(!periodicTasks.empty()){ // se houver tarefas periódicas
            for(int i = 0; i < periodicTasks.size(); i++){ // para cada tarefa periódica
                continuar_procurando = true;
                PeriodicTask task;
                    for(int j = 0; j < periodicTasks.size(); j++){
                        task = periodicTasks[j];
                        if(task.t_falta_comp == 0) { //acabou de processar
                            if(task.t_prox_period == tempo){
                                task.t_falta_comp = task.t_comp;
                                task.t_deadline = tempo + task.t_deadline_original;
                                task.t_prox_period += task.t_period;
                            }
                            else if(task.t_prox_period < tempo) {
                                task.t_falta_comp += task.t_comp;
                                task.t_deadline = tempo + task.t_deadline_original;
                                task.t_prox_period = tempo + task.t_period;
                            }
                        }
                        else if(continuar_procurando){
                            i = j;
                            continuar_procurando = false;
                        }
                        periodicTasks[j] = task;
                    }
                task = periodicTasks[i];
                if(tempo  == 4) {
                    std::cout << "task: " << task.s_symbol << " (faltam " << task.t_falta_comp << " computacoes)" << std::endl;
                }
                if(continuar_procurando)
                    break;

                if (prev_number != i)
                    cpu.load(i, task.s_symbol, task.t_falta_comp, task.t_deadline); // carrega a tarefa
                cpu.run(); 

                task.t_falta_comp = task.t_falta_comp - 1;
                periodicTasks[i] = task;
                prev_number = i; 
                tempo++; 
                
                if (task.t_falta_comp > 0 && tempo > task.t_prox_period) 
                    i--; // se a tarefa não terminou e não passou o período, roda a mesma tarefa novamente
                if(tempo == T) // se o tempo de simulação acabou
                    break;
            }
        }

        int pTasks = periodicTasks.size();
        if(!aperiodicTasks.empty() && continuar_procurando){ // se houver tarefas aperiódicas
            for(int i = 0; i < aperiodicTasks.size(); i++){ // para cada tarefa aperiódica
                if(aperiodicTasks[i].t_arrival <= tempo && aperiodicTasks[i].t_comp != 0){
                    continuar_procurando = false; // para de procurar
                    if(prev_number != i+pTasks)
                        cpu.load(i+pTasks, aperiodicTasks[i].s_symbol, aperiodicTasks[i].t_comp, T);
                    cpu.run(); 
                    tempo++;
                    aperiodicTasks[i].t_comp -= 1;
                    prev_number = i+pTasks;
                    break;
                }
            }
        }

        if(continuar_procurando) { // se o tempo de simulação não acabou
            cpu.load(-1, '.', 0, T); 
            cpu.run(); 
            tempo++;
            prev_number = -1;
        }
    }

    if (cpu.symbol == '.'){
        PeriodicTask task = periodicTasks[0];
        cpu.load(0, task.s_symbol, task.t_comp, task.t_deadline);
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