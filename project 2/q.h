#include<stdio.h>  
#include<stdlib.h> 
#include"TCB.h"

int c = 1;  
struct node  
{  
  struct TCB_t* tcb_t; 
  int data;
  struct node *next;  
  struct node *prev;  
};  

struct node* newItem() //returns a new queue item that contains the TCB information
{
  struct node* qEle = (struct node *) malloc(sizeof(struct node));
  qEle->data =c++;
  qEle->next=NULL;
  qEle->prev=NULL;
  return qEle;
}

struct node* newQueue() //creates a queue and returns the head pointer
{
  return NULL;
}

void AddQueue(struct node** head,struct node* item)  
{  
  if((*head) == NULL)  
  {  
    (*head)=item;
    (*head)->next = item;
    (*head)->prev = item;
  }  
else  
  {  
    item->prev = (*head)->prev;
    item->next = (*head);
    (*head)->prev->next = item;
    (*head)->prev = item; 
  }          
}  


struct node* DelQueue(struct node** head) 
{
  struct node* tempHead = (*head);
  if((*head) == NULL) exit(-1);

  else if((*head)->next==(*head) && (*head)->prev) 
  { 
    (*head) = NULL;
    free(tempHead);
  }
  else 
  {
    (*head)->prev->next = (*head)->next;
    (*head)->next->prev = (*head)->prev;
    (*head) = (*head)->next;
    return tempHead;
  }
}

