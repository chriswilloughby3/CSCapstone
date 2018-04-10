/* EECS 370 LC-2K Instruction-level simulator */
// Christopher Willoughby
// 24 March 2018
// Implments a five stage look ahead pipeline using the LC-2K instruction set

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5 /* JALR will not implemented for Project 3 */
#define HALT 6
#define NOOP 7

#define NOOPINSTRUCTION 0x1c00000

typedef struct IFIDStruct {
	int instr;
	int pcPlus1;
} IFIDType;

typedef struct IDEXStruct {
	int instr;
	int pcPlus1;
	int readRegA;
	int readRegB;
	int offset;
} IDEXType;

typedef struct EXMEMStruct {
	int instr;
	int branchTarget;
	int aluResult;
	int readRegB;
} EXMEMType;

typedef struct MEMWBStruct {
	int instr;
	int writeData;
} MEMWBType;

typedef struct WBENDStruct {
	int instr;
	int writeData;
} WBENDType;

typedef struct stateStruct {
	int pc;
	int instrMem[NUMMEMORY];
	int dataMem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
	IFIDType IFID;
	IDEXType IDEX;
	EXMEMType EXMEM;
	MEMWBType MEMWB;
	WBENDType WBEND;
	int cycles; /* number of cycles run so far */
} stateType;

void printState(stateType *);
int convertNum(int num);
int opcode(int instruction);
void printInstruction(int instr);

int
main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state, newState;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
	
	// Open an infile stream
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }
    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {
		
        if (sscanf(line, "%d", state.instrMem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.instrMem[state.numMemory]);
		state.dataMem[state.numMemory] = state.instrMem[state.numMemory];
    }
	// Used to track mircoprocessor stats
	state.cycles = 0;
	state.pc = 0;
	
	// Initial state register values
	int i = 0;	
	for(i = 0; i < 8; i++)
	{
		state.reg[i] = 0;
	}
	state.IFID.instr = NOOPINSTRUCTION;
	state.IDEX.instr = NOOPINSTRUCTION;
	state.EXMEM.instr = NOOPINSTRUCTION;
	state.MEMWB.instr = NOOPINSTRUCTION;
	state.WBEND.instr = NOOPINSTRUCTION;
	
	while (1) {

		printState(&state);

		/* check for halt */
		if (opcode(state.MEMWB.instr) == HALT) {
			printf("machine halted\n");
			printf("total of %d cycles executed\n", state.cycles);
			exit(0);
		}

		newState = state;
		newState.cycles++;

		/* --------------------- IF stage --------------------- */
		newState.IFID.instr = state.instrMem[state.pc];
		newState.IFID.pcPlus1 = state.pc + 1;
		 
		/* --------------------- ID stage --------------------- */
		int ldUseHaz = 0;		
		if((opcode(state.IFID.instr) != NOOP) || 
			(opcode(state.IFID.instr) != HALT))
		{
			int regA = (0x7 & (state.IFID.instr >> 19)); 
			
			if(opcode(state.IDEX.instr) == LW)
			{
				// Checks if a the load/use hazard exists
				if((0x7 & (state.IDEX.instr >> 16)) == regA)
				{
					// Inserts a noop instruction for a load/use hazard
					newState.IDEX.instr = NOOPINSTRUCTION;
					newState.IFID.instr = state.IFID.instr;
					newState.IFID.pcPlus1 = state.pc;
					ldUseHaz = 1;
				}
			}
		}
		if((opcode(state.IFID.instr) != NOOP) || 
			(opcode(state.IFID.instr) != HALT) ||
			(opcode(state.IFID.instr) != LW))
		{
			int regB = (0x7 & (state.IFID.instr >> 16)); 
			
			if(opcode(state.IDEX.instr) == LW)
			{
				if((0x7 & (state.IDEX.instr >> 16)) == regB)
				{
					newState.IDEX.instr = NOOPINSTRUCTION;
					newState.IFID.instr = state.IFID.instr;
					newState.IFID.pcPlus1 = state.pc;
					ldUseHaz = 1;
				}
			}
		}
		// For normal pipeline operations without load/use hazards
		if(ldUseHaz != 1)
		{
			newState.pc++;
			newState.IDEX.instr = state.IFID.instr;
			newState.IDEX.pcPlus1 = state.IFID.pcPlus1;
			
			if((opcode(state.IFID.instr) != NOOP) || 
				(opcode(state.IFID.instr) != HALT))
			{
				newState.IDEX.readRegA = 
					state.reg[0x7 & (state.IFID.instr >> 19)];
			}
			if((opcode(state.IFID.instr) != NOOP) || 
				(opcode(state.IFID.instr) != HALT) ||
				(opcode(state.IFID.instr) != LW))
			{
				newState.IDEX.readRegB = 
					state.reg[0x7 & (state.IFID.instr >> 16)];
			}
			if((opcode(state.IFID.instr) == BEQ) || 
				(opcode(state.IFID.instr) == LW) ||
				(opcode(state.IFID.instr) == SW))
			{
				newState.IDEX.offset = 	
					(convertNum(state.IFID.instr & 0xffff));
			}
		}		

		/* --------------------- EX stage --------------------- */
		// Transfers the values from the ID to the EX stage
		newState.EXMEM.instr = state.IDEX.instr;
		newState.EXMEM.readRegB = state.IDEX.readRegB;
		
		// Sets the intial values for the EX stage
		int lwRegA = 0;
		int lwRebB = 0;
		int dataRegA = state.IDEX.readRegA;
		int dataRegB = state.IDEX.readRegB;
		
		
		if((opcode(state.IDEX.instr) != NOOP) || 
			(opcode(state.IDEX.instr) != HALT))
		{
			int regA = 0x7 & (state.IDEX.instr >> 19);
			
			// Loads dataRegA with value in WBEND register when the LW 
			// instruction is in the WBEND register
			if((opcode(state.WBEND.instr) == LW))
			{
				if((0x7 & (state.WBEND.instr >> 16)) == regA)
				{
					dataRegA = state.WBEND.writeData;
				}
			}
			
			// Loads dataRegA with value in WBEND register when the ADD or NOR
			// instruction is in the WBEND register
			if((opcode(state.WBEND.instr) == ADD) || 
				(opcode(state.WBEND.instr) == NOR))
			{
				if((0x7 & state.WBEND.instr) == regA)
				{
					dataRegA = state.WBEND.writeData;
				}
			}
			
			// Loads dataRegA with value in MEMWB register when the LW
			// instruction is in the MEMWB register			
			if((opcode(state.MEMWB.instr) == LW))
			{
				if((0x7 & (state.MEMWB.instr >> 16)) == regA)
				{
					dataRegA = state.MEMWB.writeData;
				}			
			}
			
			// Loads dataRegA with value in MEMWB register when the ADD or NOR
			// instruction is in the MEMWB register
			if((opcode(state.MEMWB.instr) == ADD) || 
				(opcode(state.MEMWB.instr) == NOR))
			{
				if((0x7 & state.MEMWB.instr) == regA)
				{
					dataRegA = state.MEMWB.writeData;
				}
			}
			
			// Loads dataRegA with ALU value in EXMEM register when the ADD or 
			// NOR instruction is in the EXMEM register
			if((opcode(state.EXMEM.instr) == ADD) || 
				(opcode(state.EXMEM.instr) == NOR))
			{
				if((0x7 & state.EXMEM.instr) == regA)
				{
					dataRegA = state.EXMEM.aluResult;
				}
			}
		}
		
		if((opcode(state.IDEX.instr) != NOOP) || 
			(opcode(state.IDEX.instr) != HALT) ||
			(opcode(state.IDEX.instr) != LW))
		{
			int regB = 0x7 & (state.IDEX.instr >> 16);
			
			// Loads dataRegB with value in WBEND register when the LW 
			// instruction is in the WBEND register
			if((opcode(state.WBEND.instr) == LW))
			{
				if((0x7 & (state.WBEND.instr >> 16)) == regB)
				{
					dataRegB = state.WBEND.writeData;
				}
			}
			
			// Loads dataRegB with value in WBEND register when the ADD or NOR
			// instruction is in the WBEND register
			if((opcode(state.WBEND.instr) == ADD) || 
				(opcode(state.WBEND.instr) == NOR))
			{
				if((0x7 & state.WBEND.instr) == regB)
				{
					dataRegB = state.WBEND.writeData;
				}
			}
			
			// Loads dataRegB with value in MEMWB register when the LW
			// instruction is in the MEMWB register			
			if((opcode(state.MEMWB.instr) == LW))
			{
				if((0x7 & (state.MEMWB.instr >> 16)) == regB)
				{
					dataRegB = state.MEMWB.writeData;
				}			
			}
			
			// Loads dataRegB with value in MEMWB register when the ADD or NOR
			// instruction is in the MEMWB register
			if((opcode(state.MEMWB.instr) == ADD) || 
				(opcode(state.MEMWB.instr) == NOR))
			{
				if((0x7 & state.MEMWB.instr) == regB)
				{
					dataRegB = state.MEMWB.writeData;
				}
			}
			
			// Loads dataRegB with ALU value in EXMEM register when the ADD or 
			// NOR instruction is in the EXMEM register
			if((opcode(state.EXMEM.instr) == ADD) || 
				(opcode(state.EXMEM.instr) == NOR))
			{
				if((0x7 & state.EXMEM.instr) == regB)
				{
					dataRegB = state.EXMEM.aluResult;
				}
			}
		}
		// Performs the ALU operation for the ADD instruction
		if(opcode(state.IDEX.instr) == ADD)
		{	
			newState.EXMEM.aluResult = dataRegA + dataRegB;
		}
		
		// Performs the ALU operation for the NOR instruction
		else if(opcode(state.IDEX.instr) == NOR)
		{
			newState.EXMEM.aluResult = ~(dataRegA | dataRegB);
		}
		
		// Performs the ALU operation for the BEQ instruction and the return
		// address
		else if(opcode(state.IDEX.instr) == BEQ)
		{
			newState.EXMEM.aluResult = dataRegA - dataRegB;
			newState.EXMEM.branchTarget = state.IDEX.pcPlus1 +
				state.IDEX.offset;
		}
		
		// Performs the ALU operation to calculate the memory location for the 
		// SW and LW instructions
		else if((opcode(state.IDEX.instr) == SW) || 
			(opcode(state.IDEX.instr) == LW))
		{	
			newState.EXMEM.aluResult = dataRegA + state.IDEX.offset;
			
			// Puts the value that will be written to memory in the EXMEM 
			// register
			if(opcode(state.IDEX.instr) == SW)
			{
				newState.EXMEM.readRegB = dataRegB;
			}
		}
		

		/* --------------------- MEM stage --------------------- */
		
		// Transfers the data from the EXMEM register to the MEMWB register for
		// the AND and NOR instructions
		if ((opcode(state.EXMEM.instr) == ADD) || 
			(opcode(state.EXMEM.instr) == NOR))
		{
			newState.MEMWB.writeData = state.EXMEM.aluResult;
		}
		
		// Flushes the pipeline with NOOP instructions for the new instructions
		// after a branch instruction
		else if(opcode(state.EXMEM.instr) == BEQ)
		{
			if(state.EXMEM.aluResult == 0)
			{
				newState.pc = state.EXMEM.branchTarget;
				newState.IFID.instr = NOOPINSTRUCTION;
				newState.IDEX.instr = NOOPINSTRUCTION;
				newState.EXMEM.instr = NOOPINSTRUCTION;
			}
		}
		
		// Writes a value into memory
		else if(opcode(state.EXMEM.instr) == SW)
		{
			newState.dataMem[state.EXMEM.aluResult] = state.EXMEM.readRegB; 
		}
		
		// Reads in data from a memory location
		else if(opcode(state.EXMEM.instr) == LW)
		{
			newState.MEMWB.writeData = state.dataMem[state.EXMEM.aluResult];
		}
		
		newState.MEMWB.instr = state.EXMEM.instr;
		

		/* --------------------- WB stage --------------------- */
		
		// Writes the results of the ADD and NOR operations into one of the 
		// eight on chip multi-purpose registers
		
		if ((opcode(state.MEMWB.instr) == ADD) ||
			(opcode(state.MEMWB.instr) == NOR))
		{
			newState.reg[0x7 & state.MEMWB.instr] = state.MEMWB.writeData;
		}
		
		// Writes the value read in from memory with the LW instruction into one 
		// of the eight on chip multi-purpose registers
		else if(opcode(state.MEMWB.instr) == LW)
		{
			newState.reg[0x7 & (state.MEMWB.instr >> 16)] = 
				state.MEMWB.writeData;
		}
		
		newState.WBEND.instr = state.MEMWB.instr;
		newState.WBEND.writeData = state.MEMWB.writeData;

		state = newState; /* this is the last statement before end of the loop.
				It marks the end of the cycle and updates the
				current state with the values calculated in this
				cycle */	
	}
    return(0);
}

// Prints the results of each mircoprocessor cycle
void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate before cycle %d starts\n", statePtr->cycles);
    printf("\tpc %d\n", statePtr->pc);

    printf("\tdata memory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tdataMem[ %d ] %d\n", i, statePtr->dataMem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("\tIFID:\n");
	printf("\t\tinstruction ");
	printInstruction(statePtr->IFID.instr);
	printf("\t\tpcPlus1 %d\n", statePtr->IFID.pcPlus1);
    printf("\tIDEX:\n");
	printf("\t\tinstruction ");
	printInstruction(statePtr->IDEX.instr);
	printf("\t\tpcPlus1 %d\n", statePtr->IDEX.pcPlus1);
	printf("\t\treadRegA %d\n", statePtr->IDEX.readRegA);
	printf("\t\treadRegB %d\n", statePtr->IDEX.readRegB);
	printf("\t\toffset %d\n", statePtr->IDEX.offset);
    printf("\tEXMEM:\n");
	printf("\t\tinstruction ");
	printInstruction(statePtr->EXMEM.instr);
	printf("\t\tbranchTarget %d\n", statePtr->EXMEM.branchTarget);
	printf("\t\taluResult %d\n", statePtr->EXMEM.aluResult);
	printf("\t\treadRegB %d\n", statePtr->EXMEM.readRegB);
    printf("\tMEMWB:\n");
	printf("\t\tinstruction ");
	printInstruction(statePtr->MEMWB.instr);
	printf("\t\twriteData %d\n", statePtr->MEMWB.writeData);
    printf("\tWBEND:\n");
	printf("\t\tinstruction ");
	printInstruction(statePtr->WBEND.instr);
	printf("\t\twriteData %d\n", statePtr->WBEND.writeData);
}

int
field0(int instruction)
{
	return( (instruction>>19) & 0x7);
}

int
field1(int instruction)
{
	return( (instruction>>16) & 0x7);
}

int
field2(int instruction)
{
	return(instruction & 0xFFFF);
}

int
opcode(int instruction)
{
	return(instruction>>22);
}

void
printInstruction(int instr)
{
	
	char opcodeString[10];
	
	if (opcode(instr) == ADD) {
		strcpy(opcodeString, "add");
	} else if (opcode(instr) == NOR) {
		strcpy(opcodeString, "nor");
	} else if (opcode(instr) == LW) {
		strcpy(opcodeString, "lw");
	} else if (opcode(instr) == SW) {
		strcpy(opcodeString, "sw");
	} else if (opcode(instr) == BEQ) {
		strcpy(opcodeString, "beq");
	} else if (opcode(instr) == JALR) {
		strcpy(opcodeString, "jalr");
	} else if (opcode(instr) == HALT) {
		strcpy(opcodeString, "halt");
	} else if (opcode(instr) == NOOP) {
		strcpy(opcodeString, "noop");
	} else {
		strcpy(opcodeString, "data");
    }
    printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),
		field2(instr));
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit integer */
    if (num & (1 << 15) ) {
        num -= (1 << 16);
    }
    return num;
}

