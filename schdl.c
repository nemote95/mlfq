#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.h"

int time;

//process struct
struct proc {
	int pid;
	int execution_time;
	struct list_head list_member;
};

//cpu struct
struct cpu {
	int cid;
	int quantum_time;
	struct list_head queue;
};

//insert process node to a queue
void add_node(int pid,int execution_time, struct list_head *head)
{
    struct proc *procPtr = (struct proc *)malloc(sizeof(struct proc));
    assert(procPtr != NULL);
    
    procPtr->pid = pid;
    procPtr->execution_time = execution_time;
    INIT_LIST_HEAD(&procPtr->list_member);
    list_add_tail(&procPtr->list_member, head);
}

//display queue elements (proccess id and execution time)
void display(struct list_head *head)
{
    struct list_head *iter;
    struct proc *objPtr;
    __list_for_each(iter, head) {
        objPtr = list_entry(iter, struct proc, list_member);
        printf("p%d ", objPtr->pid);
        printf("%d|", objPtr->execution_time);
    }
    printf("\n");
}

//display the state of cpus
void display_all(struct cpu **array,int length)
{
	int i;
	for(i=0;i<length;i++){
		printf("cpu %d : " ,i);
		display(&array[i]->queue);
		printf("\n");}
}
	

//fifo level algorithm
void FIFO(struct list_head *head){
	struct list_head *iter;
    struct proc *objPtr;
   	redo:
    __list_for_each(iter,head) {
        objPtr = list_entry(iter, struct proc, list_member);
        time+=objPtr->execution_time;
		list_del(&objPtr->list_member);
		free(objPtr);
        goto redo;
    }
    printf("time : %d \n",time);
	display(head);
	printf("--- \n");
    
}

//main MLFQ shcedual algorithm 
void schedual(struct cpu **array,int length){
	int i;
	for(i=0;i<length-1;i++)
	{
		
	struct list_head *head1=&array[i]->queue;
	struct list_head *head2=&array[i+1]->queue;
	
	struct list_head *iter;
    struct proc *objPtr;
   	redo:
    __list_for_each(iter,head1) {
		printf("time : %d \n",time);
		display_all(array,length);
		printf("--- \n");
		objPtr = list_entry(iter, struct proc, list_member);
		int q=array[i]->quantum_time;
		while ( q && objPtr->execution_time){
		q--;
		time++;
		objPtr->execution_time--;}
        int pid=objPtr->pid;
        int execution_time=objPtr->execution_time;
		list_del(&objPtr->list_member);
		free(objPtr);
        if (execution_time>0 && head2){add_node(pid,execution_time,head2);}
        goto redo;}
        
    }
    FIFO(&array[length-1]->queue);
}
	
	
int main() 
{
	//struct cpus
	struct cpu *array[3];
	int length=3;
	int i;
	for (i=0;i<length;i++){
		struct cpu *cpuPtr1 = (struct cpu *)malloc(sizeof(struct cpu));
		assert(cpuPtr1 != NULL);
		INIT_LIST_HEAD(&cpuPtr1->queue);
		cpuPtr1->cid=i;
		cpuPtr1->quantum_time=(i+1)*4;
		array[i]=cpuPtr1;
		}
	
    //add process nodes to the first cpu queue
    add_node(1,10, &array[0]->queue);
    add_node(2,15, &array[0]->queue);
    add_node(3,20, &array[0]->queue);
    add_node(4,25, &array[0]->queue);
    
    schedual(array,length);
    
    return 0;
}
