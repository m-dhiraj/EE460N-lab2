/*
    Name 1: Dhiraj Manukonda
    Name 2: Kolbe Bauer
    UTEID 1: DM48254
    UTEID 2: kb37324
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.
   MEMORY
   CURRENT_LATCHES
   NEXT_LATCHES
   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.
   Begin your code here 	  			       */

/***************************************************************/
int regSEXT(int num){
  if(num&0x8000>=1){
    return num|0xffff0000;
  }
  return num;
}

void updateCond(int num){
  if((num<<16)>0){
    NEXT_LATCHES.N=0;
    NEXT_LATCHES.Z=0;
    NEXT_LATCHES.P=1;
  }
  else if((num<<16)==0){
    NEXT_LATCHES.N=0;
    NEXT_LATCHES.Z=1;
    NEXT_LATCHES.P=0;
  }
  else if((num<<16)<0){
    NEXT_LATCHES.N=1;
    NEXT_LATCHES.Z=0;
    NEXT_LATCHES.P=0;
  }
  /*else{
    printf("you a dum dum\n");
  }*/
}

void updateMem(int instruction){
  int operation=instruction>>12;
  int condition=instruction;
  int dr=instruction&0x0E00;
  dr>>=9;
  //ADD
  if(operation==0x1){
    condition&=0x0020;
    int sr1=instruction&0x01C0;
    sr1>>=6;
    if(condition==0){//register add
      int sr2=instruction&0x0007;
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1])+regSEXT(CURRENT_LATCHES.REGS[sr2]);
    }
    else{//imediate add
      int imm5=instruction&0x0F;
      int sign=instruction&0x10;
      if(sign>=1){
        imm5|=0xFFFFFFF0;
      }
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1])+imm5;
    }
    updateCond(NEXT_LATCHES.REGS[dr]);
    NEXT_LATCHES.REGS[dr]=Low16bits(NEXT_LATCHES.REGS[dr]);
  }

  //AND
  if(operation==0x5){
    condition&=0x0020;
    int sr1=instruction&0x01C0;
    sr1>>=6;
    if(condition==0){//register and
      int sr2=instruction&0x0007;
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1])&regSEXT(CURRENT_LATCHES.REGS[sr2]);
    }
    else{//imediate and
      int imm5=instruction&0x0F;
      int sign=instruction&0x10;
      if(sign>=1){
        imm5|=0xFFFFFFF0;
      }
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1])&imm5;
    }
    updateCond(NEXT_LATCHES.REGS[dr]);
    NEXT_LATCHES.REGS[dr]=Low16bits(NEXT_LATCHES.REGS[dr]);
  }

  //XOR
  if(operation==0x9){
    condition&=0x00000020;
    int sr1=instruction&0x000001C0;
    sr1>>=6;
    if(condition==0){//register xor
      int sr2=instruction&0x00000007;
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1]) ^ regSEXT(CURRENT_LATCHES.REGS[sr2]);
    }
    else{//imediate xor
      int imm5=instruction&0x0000000F;
      int sign=instruction&0x00000010;
      if(sign>=1){
        imm5|=0xFFFFFFF0;
      }
      NEXT_LATCHES.REGS[dr]=regSEXT(CURRENT_LATCHES.REGS[sr1]) ^ imm5;
    }
    updateCond(NEXT_LATCHES.REGS[dr]);
    NEXT_LATCHES.REGS[dr]=Low16bits(NEXT_LATCHES.REGS[dr]);
  }

  //BR
  if(operation==0){
    int n=instruction&0x0800;
    int z=instruction&0x0400;
    int p=instruction&0x0200;
    if((n&&CURRENT_LATCHES.N)||(p&&CURRENT_LATCHES.P)||(z&&CURRENT_LATCHES.Z)){
      int offset=instruction&0x0FF;
      int sign=instruction&0x100;
      if(sign>=1){
        offset|=0xFFFFFF00;
      }
      offset<<=1;
      NEXT_LATCHES.PC=NEXT_LATCHES.PC+offset;
    }
  }

  //JMP
  if(operation==0xC){
    int bReg=instruction&0x01C0;
    bReg>>=6;
    NEXT_LATCHES.PC=regSEXT(CURRENT_LATCHES.REGS[bReg]);
  }

  //JSR and JSRR
  if(operation==0x4){
    condition=instruction&0x0800;
    int temp=NEXT_LATCHES.PC;
    if(condition>0){
      int offSet=instruction&0x03FF;
      int sign=instruction&0x0400;
      if(sign>=1){
        offSet|=0xFFFFFC00;
      }
      offSet<<=1;
      NEXT_LATCHES.PC=NEXT_LATCHES.PC+offSet;
    }
    else{
      int baseR=instruction&0x01C0;
      baseR>>=6;
      NEXT_LATCHES.PC=CURRENT_LATCHES.REGS[baseR];
    }
    NEXT_LATCHES.REGS[7]=temp;
  }

  //SHF
  if(operation==0xD){
    condition=instruction&0x0030;
    condition>>=4;
    int sr=instruction&0x01C0;
    sr>>=6;
    int amount=instruction&0x000F;
    int sign=0;
    if(condition==0){
      NEXT_LATCHES.REGS[dr]=CURRENT_LATCHES.REGS[sr] << amount;
    }
    if(condition==1){
      NEXT_LATCHES.REGS[dr]=CURRENT_LATCHES.REGS[sr] >> amount;
    }
    if(condition==3){
      int sign=CURRENT_LATCHES.REGS[sr]&0x8000;
      int temp=CURRENT_LATCHES.REGS[sr];
      int i;
      for(i=0;i<amount;i++){
        temp>>=1;
        temp+=sign;
      }
      NEXT_LATCHES.REGS[dr]=temp;
    }
    updateCond(NEXT_LATCHES.REGS[dr]);
    NEXT_LATCHES.REGS[dr]=Low16bits(NEXT_LATCHES.REGS[dr]);
  }

  //TRAP
  if(operation==0xf){
    NEXT_LATCHES.PC=0;
  }
  
  //LEA
  if(operation==0xe){
    int offset=instruction&0xff;
    int sign=instruction&0x100;
    if(sign>=1){
      offset|=0xffffff00;
    }
    NEXT_LATCHES.REGS[dr]=NEXT_LATCHES.PC+(offset<<1);
    NEXT_LATCHES.REGS[dr]=Low16bits(NEXT_LATCHES.REGS[dr]);
  }

  //LDB
  if(operation==0x2){
    int offset=instruction&0x001f;
    int sign=instruction&0x0020;
    if(sign>=1){
      offset|=0xffffffC0;
    }
    int bR=instruction&0x01C0;
    bR>>=6;
    int loc = CURRENT_LATCHES.REGS[bR]+offset;
    int pos=loc%2;
    //shouldnt this be one?
    //loc>>=1;
    loc=Low16bits(loc);
    NEXT_LATCHES.REGS[dr]=(MEMORY[loc>>1][loc%2]);
    /*sign=NEXT_LATCHES.REGS[dr]&0x0080;
    if(sign>=1){
      offset|=0xffffff00;
    }*/
    updateCond(NEXT_LATCHES.REGS[dr]);
  }

  //LDW
  if(operation==0x6){
    int offset=instruction&0x001f;
    int sign=instruction&0x0020;
    if(sign>=1){
      offset|=0xffffffC0;
    }
    offset<<=1;
    int bR=instruction&0x01C0;
    bR>>=6;
    int loc = CURRENT_LATCHES.REGS[bR]+offset;
    int pos=loc%2;
    //this will always be an even number. you dummy
    //this should also just be one
        //we're multiplying by two to divide by two... redundant
    loc=Low16bits(loc);
    loc>>=1;
    if(pos==0){
      NEXT_LATCHES.REGS[dr]=(MEMORY[loc][1]<<8)+(MEMORY[loc][0]);
    }
    else{
      NEXT_LATCHES.REGS[dr]=(MEMORY[loc][0]<<8)+(MEMORY[loc+1][1]);
    }
    updateCond(NEXT_LATCHES.REGS[dr]);
  }

  //STB
  if(operation==0x3){
    int sr=dr;
    int offset=instruction&0x0000001f;
    int sign=instruction&0x00000020;
    if(sign>=1){
      offset|=0xffffffC0;
    }
    dr=instruction&0x000001C0;
    dr>>=6;
    int loc=CURRENT_LATCHES.REGS[dr]+offset;
    int pos=loc%2;
    //shouldnt this be a one??
    loc=Low16bits(loc);
    loc>>=1;
    //this needs to be masked to only store one byte of data.
    //rn you're putting 16bits of data into an 8 bit space and it works bc theyre stored as ints
    //but we need to mask this to only store the low byte of data
    MEMORY[loc][pos]=CURRENT_LATCHES.REGS[sr]&0x000000ff;
  }

  //STW
  if(operation==0x7){
    int sr=dr;
    int offset=instruction&0x001f;
    int sign=instruction&0x0020;
    if(sign>=1){
      offset|=0xffffffC0;
    }
    offset<<=1;
    //offset needs to be leftshifted by one
    dr=instruction&0x01C0;
    dr>>=6;
    int loc=CURRENT_LATCHES.REGS[dr]+offset;
    int pos=loc%2;
    loc=Low16bits(loc);
    loc>>=1;
    if(pos==0){
      MEMORY[loc][0]=CURRENT_LATCHES.REGS[sr]&0x000000FF;
      MEMORY[loc][1]=(CURRENT_LATCHES.REGS[sr]&0x0000FF00)>>8;
    }
    else{
      MEMORY[loc+1][1]=CURRENT_LATCHES.REGS[sr]&0x00FF;
      MEMORY[loc][0]=(CURRENT_LATCHES.REGS[sr]&0xFF00)>>8;
    }
  }
}

char* toBinary(int num){
  
  //stringa1 = (char*) malloc((n+1)*sizeof(char));
  char* word=(char*)malloc((16)*sizeof(char));
  int i;
  for(i=15;i>=0;i--){
    word[i]=(num%2)+'0';
    num/=2;
  }
  word[16]='\0';
  return word;
}

void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */     
  
  //get pc line num from struct
  //fetch instruction from memory
  //decode instruction
  //call function for different instructions and pass parameters

  int instruction=(MEMORY[CURRENT_LATCHES.PC>>1][1]<<8)+MEMORY[CURRENT_LATCHES.PC>>1][0];
  NEXT_LATCHES.PC=CURRENT_LATCHES.PC+2;
  //char* binary=toBinary(instruction);
  //printf("instruction: %s  %x\n",binary,instruction);
  
  updateMem(instruction);
  
  //to process trap functions set pc to 0
  //don't have to implement RTI
  //be sure to do 16-bit arithmetic
  //lea doesnt change condition codes
}