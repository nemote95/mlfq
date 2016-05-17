#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.h"

int time;

//cpu struct
struct cpu_core {
	int cid;
	int quantum_time;
	struct list_head queue;
};

//process struct
struct process {
	int pid;
	char status;
	int execution_time;
	struct list_head node;
};

//insert process node to a queue
void insert(int pid,int execution_time, char status,struct list_head *head)
{
    struct process *processPtr = (struct process *)malloc(sizeof(struct process));
    assert(processPtr != NULL);
    
    processPtr->pid = pid;
    processPtr->status = status;
    processPtr->execution_time = execution_time;
    INIT_LIST_HEAD(&processPtr->node);
    list_add_tail(&processPtr->node, head);
}

//display queue elements (proccess id and execution time)
void print_queu_state(struct list_head *head)
{
    struct list_head *iter;
    struct process *objPtr;
    __list_for_each(iter, head) {
        objPtr = list_entry(iter, struct process, node);
        printf("p%d:", objPtr->pid);
        printf(" %c ", objPtr->status);
        printf("%d|", objPtr->execution_time);
    }
    printf("\n");
}

//display the state of cpu_cores
void print_total_state(struct cpu_core **array,int length)
{
	int i;
	for(i=0;i<length;i++){
		printf("cpu_core %d : " ,i);
		print_queu_state(&array[i]->queue);}
}
	

//fifo level algorithm
void FIFO(struct list_head *head){
	struct list_head *iter;
    struct process *objPtr;
   	redo:
    __list_for_each(iter,head) {
        objPtr = list_entry(iter, struct process, node);
        time+=objPtr->execution_time;
		list_del(&objPtr->node);
		free(objPtr);
        goto redo;
    }
    printf("time : %d \n",time);
	print_queu_state(head);
	printf("---------- \n");
    
}

//main MLFQ shcedual algorithm 
void schedual(struct cpu_core **array,int length){
	int i;
	for(i=0;i<length-1;i++)
	{
		
	struct list_head *head1=&array[i]->queue;
	struct list_head *head2=&array[i+1]->queue;
	
	struct list_head *iter;
    struct process *objPtr;
   	redo:
    __list_for_each(iter,head1) {
		objPtr = list_entry(iter, struct process, node);
		objPtr->status='r';
		//print the status
		printf("time : %d \n",time);
		print_total_state(array,length);
		printf("---------- \n");
		//
		
		int q=array[i]->quantum_time;
		while ( q && objPtr->execution_time){
		q--;
		time++;
		objPtr->execution_time--;}
        int pid=objPtr->pid;
        int execution_time=objPtr->execution_time;
		list_del(&objPtr->node);
		free(objPtr);
        if (execution_time>0 && head2){insert(pid,execution_time,'w',head2);}
        goto redo;}
        
    }
    FIFO(&array[length-1]->queue);
}
	
	
int main() 
{
	//struct cpu cores
	struct cpu_core *array[3];
	int length=3;
	int i;
	for (i=0;i<length;i++){
		struct cpu_core *cpu_corePtr = (struct cpu_core *)malloc(sizeof(struct cpu_core));
		assert(cpu_corePtr != NULL);
		INIT_LIST_HEAD(&cpu_corePtr->queue);
		cpu_corePtr->cid=i;
		cpu_corePtr->quantum_time=(i+1)*4;
		array[i]=cpu_corePtr;
		}
	
    //add process nodes to the first cpu_core queue
    insert(1,10,'w', &array[0]->queue);
    insert(2,15,'w', &array[0]->queue);
    insert(3,20,'w', &array[0]->queue);
    insert(4,25,'w', &array[0]->queue);
    
    schedual(array,length);
    
    return 0;
}
