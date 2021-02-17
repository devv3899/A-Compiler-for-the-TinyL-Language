#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
 The dead code elimination optimizer expect the input file to be provided at the standard input (stdin), and will write the generated code back to standard output (stdout).
 The basic algorithm identifies “crucial” instructions. The initial crucial instructions are all READ and WRITE instructions. For all WRITE instruction, the algorithm has to detect all instructions that contribute to the value of the variable that is written out. First, you will need to find the store instruction that stores a value into the variable that is written out. This STORE instruction is marked as critical and will reference a register, let’s say Rm. Then you will find the instructions on which the computation of the register Rm depends on and mark them as critical. This marking process terminates when no more instructions can be marked as critical. If this basic algorithm is performed for all WRITE instructions, the instructions that were not marked critical can be deleted.
 Instructions that are deleted as part of the optimization process have to be explicitly deallocated using the C free command in order to avoid memory leaks. You will implement your dead code elimination pass in the file Optimizer.c. All of your “helper” functions should be implemented in this file.
 */

//Algo - Main Aim of this program is to optimize the "TinyL.out" file which was made during the compilation of the testcases. We optimize it using the values which are manually entered or being assigned in the file. The final answers is the value obtained after the exceution of all the instructions as needed.

int main() {
    
	Instruction *head;
    Instruction *Before;
    Instruction *Pointer;
    Instruction *End;

	head = ReadInstructionList(stdin);
    
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
    
    Pointer = head;
    
	int Length = 1;
    int X = 0;
    
    while(X == 0) {
	
	    if(Pointer->next != NULL){
            //Assigning Different values to iterate.
	 	Pointer = Pointer->next;
	 	Before =  Pointer->prev;
	 	End = Pointer->next;

	 	} else {
            
	  	if(Length == 0){
	  	break;
	  }
            
	  Pointer = head;
            
        if(Pointer->next != NULL){
         //Assigning Different values to iterate.
	 	Pointer = Pointer->next;
	 	Before = Pointer->prev;
	 	End = Pointer->next;
	 	Length = 0;
	 	}
    }
        
	  if(Before->opcode == LOADI && Pointer->opcode == LOADI ){
	  		
          if(End->opcode == OR){
              
          Length = 1;
              //Assigning Different values to iterate.
          int Parser1 = Before->field2;
          int Parser2 = Pointer->field2;
          int Parser3 = Parser1|Parser2;
          
             // Pointer->opcode = LOADI;
              //Pointer->field1 = End->field1;
              //Pointer->field2 = Parser3;
             // Pointer->next = End->next;
              //Pointer->prev = Before->prev;
              
          Pointer->opcode = LOADI;
              //Assigning Different values to iterate.
          Pointer->field1 = End->field1;
          Pointer->field2 = Parser3;
          Pointer->next = End->next;
          Pointer->prev = Before->prev;

              if(Before == head){
                  head = Pointer;
                  
              } else {
                  
                  Before->next = Pointer;
                  Before->prev->next =Pointer;
              }
          }
          
          
          if(End->opcode == AND){
              
          Length = 1;
           
              //Assigning Different values to iterate.
          int Parser1 = Before->field2;
          int Parser2 = Pointer->field2;
          int Parser3 = Parser1&Parser2;
              
              //Pointer->opcode = LOADI;
              //Pointer->field1 = End->field1;
              //Pointer->field2 = Parser3;
              //Pointer->next = End->next;
              //Pointer->prev = Before->prev;
          
              //Assigning Different values to iterate.
          Pointer->opcode = LOADI;
          Pointer->field1 = End->field1;
          Pointer->field2 = Parser3;
          Pointer->next = End->next;
          Pointer->prev = Before->prev;

              if(Before == head){
                  head = Pointer;
             
              } else {
                  
                  Before->next = Pointer;
                  Before->prev->next =Pointer;
              }
          }
          
        if(End->opcode == ADD){
            
        Length = 1;
            //Assigning Different values to iterate.
        int Parser1 = Before->field2;
        int Parser2 = Pointer->field2;
        int Parser3 = Parser1+Parser2;
            
            /*
            Pointer->opcode = LOADI;
            Pointer->field1 = End->field1;
            Pointer->field2 = Parser3;
            Pointer->next = End->next;
            Pointer->prev = Before->prev;
             */
          
            //Assigning Different values to iterate.
        Pointer->opcode = LOADI;
        Pointer->field1 = End->field1;
        Pointer->field2 = Parser3;
        Pointer->next = End->next;
        Pointer->prev = Before->prev;

                if(Before == head){
                    head = Pointer;
	  				
                    } else {
                        
                    Before->next = Pointer;
                    Before->prev->next =Pointer;
                }
            }
          
        if(End->opcode == SUB) {
                    
            int Parser1 = 0;
            int Parser2 = 0;
                      
                  if(End->field2 == Before->field2){
                  Parser1 = Before->field2;
                  Parser2 = Pointer->field2;
                              
                          } else {
                              
                  Parser2 = Before->field2;
                  Parser1 = Pointer->field2;
              }

            int Parser3 = Parser1-Parser2;
            Length = 1;
            
            /*
            Pointer->opcode = LOADI;
            Pointer->field1 = End->field1;
            Pointer->field2 = Parser3;
            Pointer->next = End->next;
            Pointer->prev = Before->prev;
            */
            
            
            //Assigning Different values to iterate.
            Pointer->opcode = LOADI;
            Pointer->field2 = Parser3;
            Pointer->next = End->next;
            Pointer->prev = Before->prev;
                    
            if(Before == head){
            head = Pointer;
                              
            } else {
                                  
            Before->next = Pointer;
            Before->prev->next =Pointer;
            }
        }
          
        if(End->opcode == MUL){
                
        Length = 1;
            //Assigning Different values to iterate.
        int Parser1 = Before->field2;
        int Parser2 = Pointer->field2;
        int Parser3 = Parser1*Parser2;
            
            /*
            Pointer->opcode = LOADI;
            Pointer->field1 = End->field1;
            Pointer->field2 = Parser3;
            Pointer->next = End->next;
            Pointer->prev = Before->prev;
        */
            
            //Assigning Different values to iterate.
            
        Pointer->opcode = LOADI;
        Pointer->field1 = End->field1;
        Pointer->field2 = Parser3;
        Pointer->next = End->next;
        Pointer->prev = Before->prev;

                if(Before == head){
                    head = Pointer;
                        
                    } else {
                                
                    Before->next = Pointer;
                    Before->prev->next =Pointer;
                }
            }
        }
	}

        if (head) {
		PrintInstructionList(stdout, head);	
        DestroyInstructionList(head);
	}
    
	return EXIT_SUCCESS;
}


