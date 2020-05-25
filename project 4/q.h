    #include<stdio.h>  
    #include<stdlib.h> 
    #include"tcb.h"

    void create(int);  
    int counter =1;
    void deletion_beginning();  
    struct node  
    {  
        struct TCB_t* tcb_t; 
        int data;
        struct node *next;  
        struct node *prev;  
    };  
    //struct node *head;  
     
	//method to create and return a new item 
    struct node* newItem(){
      struct node* qElement = (struct node *) malloc(sizeof(struct node));
      qElement->data =counter++;
      qElement->next=NULL;
      qElement->prev=NULL;
      return qElement;
    }

    struct node* newQueue(){
      return NULL;
    }

	//adding an item to the end of the queue
    void AddQueue(struct node** head,struct node* item)  
    {  
  
       if(item == NULL){
        return;
       }


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
   
	//deleting an item from front of the queue
    struct node* deleteItem(struct node** head) {
      struct node* temp_head = (*head);
      if((*head) == NULL)
        return NULL;

      else if((*head)->next==(*head) && (*head)->prev){ //redundant
        (*head) = NULL;
        //free(temp_head);
      }
      else {
        (*head)->prev->next = (*head)->next;
        (*head)->next->prev = (*head)->prev;
        (*head) = (*head)->next;
        
      }

      return temp_head;
    }

    
    // //retrive top item of the queue
    // struct node* top(struct node** head){
        
    //     struct node* temp_head = (*head);
    //   if((*head) == NULL)
    //     exit(-1);

    //   return temp_head;

    // }

    