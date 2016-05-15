#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.h"

int time;

//proc struct
struct proc {
	int pid;
	int burn;
	struct list_head list_member;
};

//cpu struct
struct cpu {
	int cid;
	int q;
	struct list_head queue;
};

//add proc node to a queue
void add_node(int pid,int burn, struct list_head *head)
{
    struct proc *procPtr = (struct proc *)malloc(sizeof(struct proc));
    assert(procPtr != NULL);
    
    procPtr->pid = pid;
    procPtr->burn = burn;
    INIT_LIST_HEAD(&procPtr->list_member);
    list_add_tail(&procPtr->list_member, head);
}

//display queue elements (proccess id and execution time
void display(struct list_head *head)
{
    struct list_head *iter;
    struct proc *objPtr;
    __list_for_each(iter, head) {
        objPtr = list_entry(iter, struct proc, list_member);
        printf("%d ", objPtr->pid);
        printf("%d|", objPtr->burn);
    }
    printf("\n");
}

//fifo level 
void FIFO(struct list_head *head){
	struct list_head *iter;
    struct proc *objPtr;
   	redo:
    __list_for_each(iter,head) {
        objPtr = list_entry(iter, struct proc, list_member);
        time+=objPtr->burn;
        printf("time %d \n",time);
		list_del(&objPtr->list_member);
		free(objPtr);
        goto redo;
    }
}


void schedual(struct list_head *head, struct list_head *head2,int q1){
	struct list_head *iter;
    struct proc *objPtr;
   	redo:
    __list_for_each(iter,head) {
		objPtr = list_entry(iter, struct proc, list_member);
		int q=q1;
		while ( q && objPtr->burn){
		q--;
		time++;
		objPtr->burn--;}
        printf("time %d \n",time);
        int pid=objPtr->pid;
        int burn=objPtr->burn;
		list_del(&objPtr->list_member);
		free(objPtr);
        if (burn>0 && head2){add_node(pid,burn,head2);}
        goto redo;
        
    }
}
	
	
int main() 
{
	//creating cpus
	struct cpu *array[3];
	int i=0;
	for (i=0;i<3;i++){
		struct cpu *cpuPtr1 = (struct cpu *)malloc(sizeof(struct cpu));
		assert(cpuPtr1 != NULL);
		INIT_LIST_HEAD(&cpuPtr1->queue);
		cpuPtr1->cid=i;
		array[i]=cpuPtr1;
		}
	
    //adding nodes to first cpu queue
    add_node(1,10, &array[0]->queue);
    add_node(2,15, &array[0]->queue);
    add_node(3,20, &array[0]->queue);
    add_node(4,25, &array[0]->queue);
    
    display(&array[0]->queue);
    
    for(i=0;i<2;i++){
    schedual(&array[i]->queue, &array[i+1]->queue,4*(i+1) );}
    FIFO(&array[2]->queue);
    
    return 0;
}
