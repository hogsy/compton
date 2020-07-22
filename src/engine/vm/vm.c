/* Copyright (C) 2020 Mark Sowden <markelswo@gmail.com>
 * */

#include <PL/pl_llist.h>
#include <PL/platform_console.h>
#include <PL/platform_filesystem.h>

#include "vm.h"

/* Virtual Machine
 * This is my first attempt at writing a virtual machine
 * and obviously, could be a heck better; total learning experience.
 *
 * Feel free to use this for whatever you want if it's for some reason
 * useful to you!
 *
 * todo
 * 	- separate log outputs per VM instance
 * 	- log output for VM manager
 */

static int vmLogIndex;
#define VM_Print( ... ) plLogMessage( vmLogIndex, __VA_ARGS__ )

/*********************************/
/** ERROR HANDLING **/

static char vmErrorMsg[ 256 ];

static void VM_ClearError( void ) {
	vmErrorMsg[ 0 ] = '\0';
}

static void VM_SetError( const char *msg, ... ) {

}

const char *VM_GetError( void ) {
	return vmErrorMsg;
}

/*********************************/

static PLLinkedList *vmPrograms;

typedef enum VMVariableType {
	VM_VAR_INTEGER,
	VM_VAR_FLOAT,
	VM_VAR_STRING,
} VMVariableType;

typedef enum VMRegisterIndex {
	VM_REG_0,
	VM_REG_1,
	VM_REG_2,
	VM_REG_3,
	VM_REG_4,
	VM_REG_5,
	VM_REG_6,
	VM_REG_7,
	VM_REG_PC,
	VM_REG_COND,
	VM_REG_COUNT,

	VM_MAX_REGISTERS
} VMRegisterIndex;

typedef struct VMRegister {
	VMVariableType varType;
	union {
		int         varI;
		float       varF;
		char        varS[ 256 ];
	};
} VMRegister;

typedef enum VMOpCode {
	VM_OP_NOP, /* invalid instruction, throws error */
	VM_OP_RETURN,
	VM_OP_OR,
	VM_OP_AND,
	VM_OP_CALL,

	/* integer operations */
	VM_OP_MUL_I,
	VM_OP_INC_I,
	VM_OP_ADD_I,
	VM_OP_SUB_I,
	VM_OP_NEG_I,

	VM_MAX_OPCODES
} VMOpCode;

typedef struct VMInstruction {
	uint8_t opCode;
} VMInstruction;

#define VM_PROGRAM_NAME_LENGTH	16

typedef struct VMProgram {
	VMRegister registers[ VM_MAX_REGISTERS ];

	bool isRunning; /* whether or not the program should be ticked */

	unsigned int clockSpeed; /* frequency of ticks */
	unsigned int lastTick; /* last time we ticked */
	unsigned int numTicks; /* number of ticks total */
	unsigned int curInstruction; /* current instruction */
	unsigned int numInstructions; /* total number of instructions */
	VMInstruction *instructions; /* array of instructions */

	char name[ VM_PROGRAM_NAME_LENGTH ]; /* name of the program */
	char path[ PL_SYSTEM_MAX_PATH ]; /* path to where the program was loaded from */

	PLLinkedListNode *node; /* instance in the linked list */
} VMProgram;

VMProgram *VM_GetProgramByName( const char *programName ) {
	VM_ClearError();

	PLLinkedListNode *curNode = plGetRootNode( vmPrograms );
	while ( curNode != NULL ) {
		VMProgram *program = ( VMProgram* ) plGetLinkedListNodeUserData( curNode );
		if ( strcmp( program->name, programName ) == 0 ) {
			return program;
		}

		curNode = plGetNextLinkedListNode( curNode );
	}

	VM_SetError( "Failed to find the specified VM program!\n" );

	return NULL;
}

void VM_ExecuteProgram( VMProgram *program ) {
	program->isRunning = true;
}

VMProgram *VM_LoadProgram( const char *path ) {
	VM_ClearError();

	PLFile *filePtr = plOpenFile( path, false );
	if ( filePtr == NULL ) {
		VM_SetError( "Failed to open CVM, \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	/* validate it */

	char identifier[ 4 ];
	if ( plReadFile( filePtr, identifier, sizeof( char ), 4 ) != 4 ) {
		VM_SetError( "Failed to read identifier for \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	if ( identifier[ 0 ] != 'C' || identifier[ 1 ] != 'V' || identifier[ 2 ] != 'M' || identifier[ 3 ] != '0' ) {
		VM_SetError( "Unexpected identifier \"%s\", expected CVM0!\n", identifier );
		return NULL;
	}

	char programName[ VM_PROGRAM_NAME_LENGTH ];
	if ( plReadFile( filePtr, programName, sizeof( char ), VM_PROGRAM_NAME_LENGTH ) != VM_PROGRAM_NAME_LENGTH ) {
		VM_SetError( "Failed to read in program name for \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	/* read in all of the instructions */
	bool status;
	unsigned int numInstructions = plReadInt32( filePtr, false, &status );
	VMInstruction *instructions = pl_calloc( numInstructions, sizeof( VMInstruction ) );
	for ( unsigned int i = 0; i < numInstructions; ++i ) {
		instructions[ i ].opCode = plReadInt8( filePtr, &status );

	}

	if ( !status ) {
		VM_SetError( "Failed to read in instructions for \"%s\"!\nPL: %s\n", path, plGetError() );
		return NULL;
	}

	/* now we can actually setup the VM */

	VMProgram *program = pl_calloc( 1, sizeof( VMProgram ) );
	program->clockSpeed = 0;
	program->numInstructions = numInstructions;
	program->instructions = instructions;
	program->node = plInsertLinkedListNode( vmPrograms, program );

	return program;
}

static void VM_Evaluate( VMProgram *vmHandle, VMInstruction *curInstruction ) {
	switch( curInstruction->opCode ) {
		case VM_OP_ADD_I:
			break;
		case VM_OP_MUL_I:
			break;
		case VM_OP_NEG_I:
			break;
		case VM_OP_RETURN:
			break;
	}
}

static void VM_TickProgram( VMProgram *program ) {
	if ( !program->isRunning ) {
		return;
	}

	VMInstruction *curInstruction = &program->instructions[ program->curInstruction++ ];
	VM_Evaluate( program, curInstruction );

	if ( program->curInstruction >= program->numInstructions ) {
		VM_SetError( "Overrun in program, \"%s\"!\n", program->name );
		return;
	}
}

void VM_TerminateProgram( VMProgram *program ) {

}

static void VM_SetClockSpeed( unsigned int argc, char **argv ) {
}

static void VM_FreezeCallback( unsigned int argc, char **argv ) {
	if ( argc <= 1 ) {
		VM_SetError( "Invalid arguments!\n" );
		return;
	}

	const char *programName = argv[ 1 ];
	VMProgram *program = VM_GetProgramByName( programName );
	if ( program == NULL ) {
		return;
	}

	program->isRunning = false;
}

static void VM_TerminateCallback( unsigned int argc, char **argv ) {
}

static void VM_ExecuteCallback( unsigned int argc, char **argv ) {
}

static void VM_AssembleCallback( unsigned int argc, char **argv ) {
	if ( argc <= 2 ) {
		VM_SetError( "Invalid arguments!\n" );
		return;
	}

	const char *asmPath = argv[ 1 ];
	const char *outPath = argv[ 2 ];

	VM_Print( "Assembling \"%s\"...\n", asmPath );

	PLFile *filePtr = plOpenLocalFile( asmPath, false );
	if ( filePtr == NULL ) {
		VM_SetError( "Failed to open \"%s\"!\nPL: %s\n", asmPath, plGetError() );
		return;
	}

	VM_Print( "Wrote \"%s\"\n" );
}

void VM_Initialize( void ) {
	vmLogIndex = plAddLogLevel( "vm", PL_COLOUR_WHITE, true );

	VM_Print( "Initializing Virtual Machine...\n" );

	plRegisterConsoleCommand( "Vm.SetClockSpeed", VM_SetClockSpeed, "Set the clock speed of the specified program." );
	plRegisterConsoleCommand( "Vm.Freeze", VM_FreezeCallback, "Freeze the specified program." );
	plRegisterConsoleCommand( "Vm.Terminate", VM_TerminateCallback, "Terminate the specified program." );
	plRegisterConsoleCommand( "Vm.Execute", VM_ExecuteCallback, "Execute the specified program." );
	plRegisterConsoleCommand( "Vm.Assemble", VM_AssembleCallback, "Assembles the specified ASM." );

	vmPrograms = plCreateLinkedList();
	if ( vmPrograms == NULL ) {
		VM_SetError( "Failed to create vmPrograms list!\nPL: %s\n", plGetError() );
		return;
	}
}

void VM_Shutdown( void ) {
	plDestroyLinkedList( vmPrograms );
}
