#include <iostream>
#include <vector>

#define MAX_T 100000
#define MAX_TP 13
#define MAX_TA 13

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
    Task(int comput, int deadline, unsigned int time);
    ~Task();

    void setComput(int c) { comput = c; }
    int getComput() const { return comput; }

    void setDeadline(int d) { deadline = d; }
    int getDeadline() const { return deadline; }

    void setTime(unsigned int t) { time = t; }
    unsigned int getTime() const { return time; }

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

Task::Task(int comput, int deadline, unsigned int time) {
    this->comput = comput;
    this->time = time;
    this->deadline = deadline;
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
        if ((unsigned int)deadline != (unsigned int)-1 && time >= (unsigned int)deadline)  // se o processo perdeu o deadline
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

void executeTasks() {
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
}

int main() {

    std::vector<Task> tasks;
    
    int T, TP, TA; // tempo de simulação, número de tarefas periódicas e número de tarefas aperiódicas

    std::cout << "\n\nDigite T (1 <= T <= 100000), TP (1 <= TP <= 13), e TA (1 <= TA <= 13): " << std::endl;
    std::cin >> T >> TP >> TA;
    std::cout << "Digite a computação, período e deadline das tarefas periódicas (1 <= Ci, Pi, Di <= 100000): " << std::endl;
    
    for(int i = 0; i < TP; i++){ // carrega as tarefas periódicas
        int Ci, Pi, Di;
        std::cin >> Ci >> Pi >> Di;
        Task task;
        task.setComput(Ci);
        task.setTime(Pi);
        task.setDeadline(Di);
        task.load(i, 'T' + ('A' + i), task.getComput(), task.getDeadline());  
        tasks.push_back(task);
    }

    std::cout<<"Digite a computação e deadline das tarefas aperiódicas (1 <= Ci, Di <= 100000): " << std::endl;

    // Como as tarefas aperiódicas precisam ser carregadas após as tarefas periódicas, o índice de carregamento é TP + i
    // e vão até TP + TA - 1
    for(int i = TP; i < TP + TA; i++){ // carrega as tarefas aperiódicas
        int Ci, Di;
        std::cin >> Ci >> Di;
        Task task;
        task.setComput(Ci);
        task.setDeadline(Di);
        task.load(i, 'T' + ('A' + i), task.getComput(), task.getDeadline());
        tasks.push_back(task);
    }

    for (size_t i = 0; i < tasks.size(); ++i) {
        const auto& task = tasks[i];
        char taskId = 'A' + i; // Gera o identificador da tarefa (A, B, C, etc.)
        std::cout << "T" << taskId << ": ";
        std::cout << "(" << task.getComput() << ",";
        if (task.getTime() != 0) { // Verifica se a tarefa é periódica
            std::cout << task.getTime() << ",";
        }
        std::cout << task.getDeadline() << ")\n" << std::endl;
    }

    executeTasks();
    return 0;
}
