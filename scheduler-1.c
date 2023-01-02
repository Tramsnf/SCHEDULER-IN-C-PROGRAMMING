#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process{             // process type
    int pid;                        // process id
    int time;                       // process time
    int initial_time;               //  initial_time of process when it is created 
    struct process *next;
}process;

process *new_process(int pid, int time){        //create new process function
    process *new = malloc(sizeof(process));     //malloc the memory size of new process
    new->pid = pid;                             // set pid of new process
    new->time = time;                           // set time of new process
    new->initial_time = time;                   // set initial_time of new process
    new->next = NULL;                           // set next process of new process to NULL
    return new;
}

typedef struct queue{           // queue type
    process *front;             // front process of queue
    char *name;                 // name of queue
}queue;

queue *new_queue(char *name){               // create new queue
    queue *new = malloc(sizeof(queue));     // malloc the memory size of new queue
    new->front = NULL;                      // set the front process of new queue to NULL i.e. set the new queue to empty
    new->name = name;                       // set the name of new queue
    return new;
}

void push(queue *q, process *p){            // push process p to the last of queue q
    if(q->front == NULL){                   // if q is empty
        p->next = NULL;                     // set the next process of p to NULL
        q->front = p;                       // set the front of q to p
        return;
    }
    else{                                   // if q is not empty
        process *c = q->front;              // find the last process in q
        while(c->next != NULL){
            c = c->next;
        }
        p->next = NULL;
        c->next = p;                        // set the next process of last process to p, then p becomes the last process of q
    }
}

process* pop(queue *q){                     // pop front process of q
    if(q->front == NULL) return NULL;       // if q is empty, return NULL
    process* c = q->front;                  // else pop out front process of q
    q->front = q->front->next;              // change the front process of q
    return c;
}

void display(queue *q){
    printf("%s->", q->name);                // print q's name, ex: Q0->
    process *c = q->front;                  // print one by one from q->front
    while(c != NULL){
        printf("P%d(%d)->",c->pid, c->time);
        c = c->next;
    }
    printf("\n");
}

int ptot = 0;

void init(queue *q, int n){                 // input q which length is n
    int t;
    int i;
    for(i = 1; i <= n; i ++){               
        scanf("%d", &t);                    // input time
        process *p = new_process(ptot, t);  // create new process which pid=ptot, time=t
        ptot ++;                                      
        push(q, p);                         // push new process to q
    }
}

int main(int argc, char * argv[]){
    queue *Q0 = new_queue("Q0"), *Q1 = new_queue("Q1"), *Q2 = new_queue("FCFS");
    
    int n0, n1, n2;
    scanf("%d", &n0);           // input the number of process in Q0
    init(Q0, n0);               // init the Q0 with n0 times
    scanf("%d", &n1);           // ...
    init(Q1, n1);
    scanf("%d", &n2);
    init(Q2, n2);

    display(Q0);                // display Q0, Q1, Q2
    display(Q1);
    display(Q2);

    int current_time = 0;           // current time from beginning
    int completion_time = 0;        // Sum of completion time of processes
    int waiting_time = 0;           // Sum of waiting time of processes

    int promote = 0;                // promote count

    while(Q0->front != NULL || Q1->front != NULL || Q2->front != NULL){     //while Q0, Q1, Q2 are not all empty
        if(Q0->front != NULL){                                              // if Q0 is not empty
            process *c = pop(Q0);                                           // pop the front process of Q0
            printf("P%d(%d)\n", c->pid, c->time);
            if(c->time > 8){                                                // if c is not finished
                c->time -= 8;                                               // reduce the time of c by 8
                current_time += 8;                                          // add current time by 8
                push(Q1, c);                                                // push c to Q1
            }
            else{                                                           // if c is finished
                current_time += c->time;                                    // add current_time by the time of c
                c->time = 0;            
                completion_time += current_time;                            // add sum of completion time by current time(the completion time of c)
                waiting_time += current_time - c->initial_time;             // waiting time = completion time - burst time so add sum of waiting time by the difference between current time and initial_time
            }    
            display(Q0);                                                    // display Q0, Q1, Q2
            display(Q1);
            display(Q2);        
        }
    
        else if(Q1->front != NULL){                                         // if Q0 is empty but Q1 is not empty
            process *c = pop(Q1);                                           // pop the front process of Q1
            printf("P%d(%d)\n", c->pid, c->time);                           
            if(c->time > 16){                                               // if c is not finished
                c->time -= 16;                                              // reduce the time of c by 16
                current_time += 16;                                         // add current time by 16
                if(promote % 4 == 0){                                       // once per 4 times, i.e. 25% chance
                    push(Q0, c);                                            // push c to Q0
                    printf("Promote\n");
                }
                else{                                                       // no chance
                    push(Q2, c);                                            // push c to Q2
                }
                promote ++;                                                 // increase promote by 1
            }
            else{
                current_time += c->time;                                    // if c is finished(same as Q0)
                c->time = 0;
                completion_time += current_time;
                waiting_time += current_time - c->initial_time;
            } 
            display(Q0);
            display(Q1);
            display(Q2);
        }

        else if(Q2->front != NULL){                                         // if Q0, Q1 is empty but Q2 is not empty
            process *c = pop(Q2);                                           // pop the front process of Q2
            printf("P%d(%d)\n", c->pid, c->time);
            display(Q0);
            display(Q1);
            display(Q2);
            current_time += c->time;                                        // calculate current_time, completion_time, waiting_time (same as upper cases)
            c->time = 0;
            completion_time += current_time;
            waiting_time += current_time - c->initial_time;
        }
    }

    float average_turnaround_time = 1.0 * completion_time / ptot;
    float average_wait_time = 1.0 * waiting_time / ptot;
    printf("Average wait time: %.6f\n", average_wait_time);
    printf("Average turnarount time: %.6f", average_turnaround_time);
    
}