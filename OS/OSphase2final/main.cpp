#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

char M[300][4]; // Physical Memory
char buffer[40];
char IR[4]; // Instruction Register (4 bytes)
char R[5];  // General Purpose Register (4 bytes)
int IC;     // Instruction Counter Register (2 bytes)
int C;      // Toggle (1 byte)
int SI;     // System Interrupt
int PI;     // Program Interrupt
int TI;     // Timing Interrupt
int PTR;    // Page Table Register
bool breakFlag;

struct PCB
{ // Process Control Block
    int job_id;
    int TTL; // Total TIme Limit
    int TLL; // Total Line Limit
    int TTC; // Total Time Count
    int LLC; // Total Line Count

    void setPCB(int id, int ttl, int tll)
    {
        // Function to set the PCB with the following
        job_id = id;
        TTL = ttl;
        TLL = tll;
        TTC = 0;
        LLC = 0;
    }
};

PCB pcb;

string error[7] = {"No Error", "Out of Data", "Line Limit Exceeded", "Time Limit Exceeded",
                   "Operation Code Error", "Operand Error", "Invalid Page Fault"};

void init();
void read(int RA);
void write(int RA);
int addressMap(int VA);
void execute_user_program();
void start_execution();
int allocate();
void load();

void init()
{
    memset(M, '\0', 1200);
    memset(IR, '\0', 4);
    memset(R, '\0', 5);
    C = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    breakFlag = false;
}

void Terminate(int EM, int EM2 = -1)
{
    fout << endl
         << endl;
    // If the program terminated normally, print the appropriate message
    if (EM == 0)
    {
        fout << " terminated normally. " << error[EM] << endl;
    }
    // If the program terminated abnormally, print the appropriate message
    else
    {
        fout << "terminated abnormally due to " << error[EM] << (EM2 != -1 ? (". " + error[EM2]) : "") << endl;
        fout << "Job Id = " << pcb.job_id << ", IC = " << IC << ", IR = " << IR << ", C = " << C << ", R = " << R << ", TTL = " << pcb.TTL << ", TTC = " << pcb.TTC << ", TLL = " << pcb.TLL << ", LLC = " << pcb.LLC;
    }
}
void read(int RA)
{
    // Read a line from the input file into a buffer
    fin.getline(buffer, 41);
    // Check if the buffer contains the end-of-job marker
    char temp[5];
    memset(temp, '\0', 5);
    memcpy(temp, buffer, 4);
    // If the end-of-job marker is found, terminate the program
    if (!strcmp(temp, "$END"))
    {
        Terminate(1);
        breakFlag = true;
    }
    // Otherwise, store the line in memory at the specified location
    else
    {
        strcpy(M[RA], buffer);
    }
}

void write(int RA)
{
    // Check if the program has exceeded its line limit
    if (pcb.LLC + 1 > pcb.TLL)
    {
        Terminate(2);
        breakFlag = true;
    }
    // If not, read the specified number of lines from memory and write to output file
    else
    {
        char str[40];
        int k = 0;
        for (int i = RA; i < (RA + 10); i++)
        {
            for (int j = 0; j < 4; j++)
                str[k++] = M[i][j];
        }
        fout << str << endl;
        pcb.LLC++;
    }
}

int mos(int RA = 0)
{
    // Check if the timer interrupt (TI) is set to 0
    if (TI == 0)
    {
        // If the system interrupt (SI) is set to a non-zero value, handle the system call
        if (SI != 0)
        {
            switch (SI)
            {
            case 1:
                // Read from input device
                read(RA);
                break;
            case 2:
                // Write to output device
                write(RA);
                break;
            case 3:
                // Terminate the program
                Terminate(0);
                breakFlag = true;
                break;
            default:
                cout << "Error with SI." << endl;
            }
            // Reset the system interrupt (SI) to 0 after handling the system call
            SI = 0;
        }
        // If the program interrupt (PI) is set to a non-zero value, handle the interrupt
        else if (PI != 0)
        {
            switch (PI)
            {
            case 1:
                // Read/write to an illegal memory location
                Terminate(4);
                breakFlag = true;
                break;
            case 2:
                // Handle page fault
                Terminate(5);
                breakFlag = true;
                break;
            case 3:
                // Reset the program interrupt (PI) to 0
                PI = 0;
                // Page Fault checking
                char temp[3];
                memset(temp, '\0', 3);
                memcpy(temp, IR, 2);

                // If the instruction is a GD or SR instruction, allocate a new page frame and update the page table register (PTR)
                if (!strcmp(temp, "GD") || !strcmp(temp, "SR"))
                {
                    int m;
                    do
                    {
                        m = allocate();
                    } while (M[m * 10][0] != '\0');

                    int currPTR = PTR;
                    while (M[currPTR][0] != '*')
                        currPTR++;

                    itoa(m, M[currPTR], 10);

                    cout << "Valid Page Fault, page frame = " << m << endl;
                    cout << "PTR = " << PTR << endl;
                    for (int i = 0; i < 300; i++)
                    {
                        cout << "M[" << i << "] :";
                        for (int j = 0; j < 4; j++)
                        {
                            cout << M[i][j];
                        }
                        cout << endl;
                    }
                    cout << endl;

                    // If the time limit (TTL) is exceeded, set the timer interrupt (TI) to 2 and handle the interrupt
                    if (pcb.TTC + 1 > pcb.TTL)
                    {
                        TI = 2;
                        PI = 3;
                        mos();
                        break;
                    }
                    pcb.TTC++;

                    // Return 1 to indicate that a page fault occurred
                    return 1;
                }

                // If the instruction is a PD, LR, H, CR, or BT instruction, terminate the program and handle the interrupt
                else if (!strcmp(temp, "PD") || !strcmp(temp, "LR") || !strcmp(temp, "H") || !strcmp(temp, "CR") || !strcmp(temp, "BT"))
                {
                    Terminate(6);
                    breakFlag = true;

                    if (pcb.TTC + 1 > pcb.TTL)
                    {
                        TI = 2;
                        PI = 3;
                        mos();
                        break;
                    }
                    // pcb.TTC++;
                }
                else
                {
                    PI = 1;
                    mos();
                }
                return 0;
            default:
                cout << "Error with PI." << endl;
            }
            PI = 0;
        }
    }
    else
    {
        if (SI != 0)
        {
            switch (SI)
            {
            case 1:
                Terminate(3);
                breakFlag = true;
                break;
            case 2:
                write(RA);
                if (!breakFlag)
                    Terminate(3);
                break;
            case 3:
                Terminate(0);
                breakFlag = true;
                break;
            default:
                cout << "Error with SI." << endl;
            }
            SI = 0;
        }
        else if (PI != 0)
        {
            switch (PI)
            {
            case 1:
                Terminate(3, 4);
                breakFlag = true;
                break;
            case 2:
                Terminate(3, 5);
                breakFlag = true;
                break;
            case 3:
                Terminate(3);
                breakFlag = true;
                break;
            default:
                cout << "Error with PI." << endl;
            }
            PI = 0;
        }
    }

    return 0;
}

int addressMap(int VA)
{
    // Funtion to accept Virtual Address and return Real Address
    //   0 < VA < 100
    if (0 <= VA && VA < 100)
    {
        int pte = PTR + VA / 10; // pte = main memory location
        if (M[pte][0] == '*')
        {
            PI = 3;
            return 0;
        }
        cout << "In addressMap(), VA = " << VA << ", pte = " << pte << ", M[pte] = " << M[pte] << endl;
        return atoi(M[pte]) * 10 + VA % 10; // Real Address
    }
    PI = 2;
    return 0;
}

void execute_user_program()
{
    char temp[3], loca[2];
    int locIR, RA;

    while (true)
    {
        if (breakFlag)
            break;

        RA = addressMap(IC); // GD10 here 10 is also a virtual address so convert to real address
        if (PI != 0)
        {
            if (mos())
            {
                continue;
            }
            break;
        }
        cout << "IC = " << IC << ", RA = " << RA << endl;
        memcpy(IR, M[RA], 4); // Memory to IR, instruction fetched
        IC += 1;

        memset(temp, '\0', 3);
        memcpy(temp, IR, 2);
        for (int i = 0; i < 2; i++)
        {
            if (!((47 < IR[i + 2] && IR[i + 2] < 58) || IR[i + 2] == 0))
            {
                PI = 2;
                break;
            }
            loca[i] = IR[i + 2];
        }

        if (PI != 0)
        {
            mos();
            break;
        }

        // loca[0] = IR[2];
        // loca[1] = IR[3];
        locIR = atoi(loca);

        RA = addressMap(locIR);
        if (PI != 0)
        {
            if (mos())
            {
                IC--;
                continue;
            }
            break;
        }

        cout << "IC = " << IC << ", RA = " << RA << ", IR = " << IR << endl;
        if (pcb.TTC + 1 > pcb.TTL)
        {
            TI = 2;
            PI = 3;
            mos();
            break;
        }

        if (!strcmp(temp, "LR"))
        {
            memcpy(R, M[RA], 4);
            pcb.TTC++;
        }
        else if (!strcmp(temp, "SR"))
        {
            memcpy(M[RA], R, 4);
            pcb.TTC++;
        }
        else if (!strcmp(temp, "CR"))
        {
            if (!strcmp(R, M[RA]))
                C = 1;
            else
                C = 0;
            pcb.TTC++;
        }
        else if (!strcmp(temp, "BT"))
        {
            if (C == 1)
                IC = RA;
            pcb.TTC++;
        }
        else if (!strcmp(temp, "GD"))
        {
            SI = 1;
            mos(RA);
            pcb.TTC++;
        }
        else if (!strcmp(temp, "PD"))
        {
            SI = 2;
            mos(RA);
            pcb.TTC++;
        }
        else if (!strcmp(temp, "H"))
        {
            SI = 3;
            mos();
            pcb.TTC++;
            break;
        }
        else
        {
            PI = 1;
            mos();
            break;
        }
        memset(IR, '\0', 4);
    }
}

void start_execution()
{
    IC = 0;
    execute_user_program();
}

int allocate()
{
    // Function to ramdomly geneterate a number. This number will be reserved for page table
    srand(time(0));
    return (rand() % 30); // %30 bcz we want the random number between 0 to 29 as we have 30 frames in main memory
}

void load()
{
    int m;                    // Variable to hold memory location
    int currPTR;              // Points to the last empty location in Page Table Register
    char temp[5];             // Temporary Variable to check for $AMJ, $DTA, $END
    memset(buffer, '\0', 40); // to make the buffer empty by assigning it with '\0'

    while (!fin.eof())
    {
        fin.getline(buffer, 41);
        memset(temp, '\0', 5);
        memcpy(temp, buffer, 4);

        if (!strcmp(temp, "$AMJ"))
        {
            init();

            int jobId, TTL, TLL;         //$AMJ 0001 0002 0003
            memcpy(temp, buffer + 4, 4); //+4 bcz jobID
            jobId = atoi(temp);
            memcpy(temp, buffer + 8, 4); //+8 bcz Total time limit
            TTL = atoi(temp);
            memcpy(temp, buffer + 12, 4); //+12 bcz Total Line Limit
            TLL = atoi(temp);
            pcb.setPCB(jobId, TTL, TLL); // set PCB with all the parameters

            PTR = allocate() * 10;   // PTR- Page Table Register it points to the starting address.
            memset(M[PTR], '*', 40); // TO make memory assigned with '*'
            currPTR = PTR;
        }
        else if (!strcmp(temp, "$DTA"))
        {
            start_execution();
        }
        else if (!strcmp(temp, "$END"))
        {
            continue;
        }
        else
        {
            if (breakFlag)
                continue;

            do
            {
                m = allocate();
            } while (M[m * 10][0] != '\0'); // To check if the frame is free

            itoa(m, M[currPTR], 10);
            currPTR++;

            cout<<"m= "<<m<<endl;

            strcpy(M[m * 10], buffer); // *10 bcz page size is 10

            cout << "PTR = " << PTR << endl;
            for (int i = 0; i < 300; i++)
            {
                cout << "M[" << i << "] :";
                for (int j = 0; j < 4; j++)
                {
                    cout << M[i][j];
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}

int main()
{
    load();
    fin.close();
    fout.close();
    return 0;
}
