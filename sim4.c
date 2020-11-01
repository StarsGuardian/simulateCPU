#include <stdio.h>
#include "sim4.h"


WORD getInstruction(WORD curPC, WORD *instructionMemory){
        return instructionMemory[curPC/4];
}

void extract_instructionFields(WORD instruction, InstructionFields *fieldsOut){
        fieldsOut -> opcode = (instruction >> 26) & 0x3f;
        fieldsOut -> rs = (instruction >> 21) & 0x1f;
        fieldsOut -> rt = (instruction >> 16) & 0x1f;
        fieldsOut -> rd = (instruction >> 11) & 0x1f;
        fieldsOut -> shamt = (instruction >> 6) & 0x1f;
        fieldsOut -> funct = (instruction) & 0x3f;
        fieldsOut -> imm16 = (instruction) & 0xffff;
        fieldsOut -> imm32 = signExtend16to32(fieldsOut -> imm16);
        fieldsOut -> address = (instruction) & 0x03ffffff;
}

int fill_CPUControl(InstructionFields *fields, CPUControl *controlOut){
        //perform add instruction
        //perform addu instruction
        if (fields -> opcode == 0 && (fields -> funct == 0x20 || fields -> funct == 0x21)){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
	//perform sub instruction
        //perform subu instruction
        else if (fields -> opcode == 0 && (fields -> funct == 0x22 || fields -> funct == 0x23)){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 1;
                return 1;
        }
        //perform addi instruction
        //perform addiu instruction
        else if (fields -> opcode == 0x08 || fields -> opcode == 0x09){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 1;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform and instruction
        else if (fields -> opcode == 0 && fields -> funct == 0x24){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 0;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform or instruction
        else if (fields -> opcode == 0 && fields -> funct == 0x25){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 1;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform xor instruction
        else if (fields -> opcode == 0 && fields -> funct == 0x26){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 4;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform slt instrction
        else if (fields -> opcode == 0 && fields -> funct == 0x2a){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 3;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 1;
                return 1;
        }
        //perform slti instruction
        else if (fields -> opcode == 0x0a){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 3;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 1;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 1;
                return 1;
        }
        //perform lw instruction
        else if (fields -> opcode == 0x23){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 1;
                controlOut -> memToReg = 1;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 1;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform sw instruction
        else if (fields -> opcode == 0x2b){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 1;
                controlOut -> ALUsrc = 1;
                controlOut -> regWrite = 0;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform beq instruction
        else if (fields -> opcode == 0x04){
                controlOut -> regDst = 0;
                controlOut -> branch = 1;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 2;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 0;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 1;
                return 1;
        }
        //perform j instruction
        else if (fields -> opcode == 0x02){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 0;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 0;
                controlOut -> jump = 1;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform ori instruction
        else if (fields -> opcode == 0x0d){
                controlOut -> regDst = 0;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 1;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 1;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform sll instruction
        else if (fields -> opcode == 0 && fields -> funct == 0x00){
                controlOut -> regDst = 1;
                controlOut -> branch = 0;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 5;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 1;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 0;
                return 1;
        }
        //perform bne instruction
        else if (fields -> opcode == 0x05){
                controlOut -> regDst = 0;
                controlOut -> branch = 1;
                controlOut -> memRead = 0;
                controlOut -> memToReg = 0;
                controlOut -> ALU.op = 6;
                controlOut -> memWrite = 0;
                controlOut -> ALUsrc = 0;
                controlOut -> regWrite = 0;
                controlOut -> jump = 0;
                controlOut -> ALU.bNegate = 1;
                return 1;
        }
        else{
                return 0;
        }
}
WORD getALUinput1(CPUControl *controlIn,
                  InstructionFields *fieldsIn,
                  WORD rsVal, WORD rtVal, WORD reg32, WORD reg33,
                  WORD oldPC){
        if (fieldsIn -> opcode == 0 && fieldsIn -> funct == 0x00){
                return rtVal;
        }
        else{
                return rsVal;
        }
}

WORD getALUinput2(CPUControl *controlIn,
                  InstructionFields *fieldsIn,
                  WORD rsVal, WORD rtVal, WORD reg32, WORD reg33,
                  WORD oldPC){
        if (fieldsIn -> opcode == 0 && fieldsIn -> funct == 0x00){
                return fieldsIn -> shamt;
        }
        else if (controlIn -> ALUsrc == 0){
                return rtVal;
        }
        else{
                return fieldsIn -> imm32;
        }
}
void execute_ALU(CPUControl *controlIn,
                 WORD input1, WORD input2,
                 ALUResult  *aluResultOut){
        //perform and operation
        if (controlIn -> ALU.op == 0){
                aluResultOut -> result = input1 & input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }
        }
        //perform or operation
        else if (controlIn -> ALU.op == 1){
                aluResultOut -> result = input1 | input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }
        }
        //perform add operation
        else if (controlIn -> ALU.op == 2 && controlIn -> ALU.bNegate == 0){
                aluResultOut -> result = input1 + input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }

        }
        //perform sub operation if bNegate is 1
        else if (controlIn -> ALU.op == 2 && controlIn -> ALU.bNegate == 1){
                aluResultOut -> result = input1 - input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }

        }
        //perform less operation
        else if (controlIn -> ALU.op == 3){
                if (input1 < input2){
                        aluResultOut -> result = 1;
                        aluResultOut -> extra = 0;
                        aluResultOut -> zero = 0;
                }
                else{
                        aluResultOut -> result = 0;
                        aluResultOut -> extra = 0;
                        aluResultOut -> zero = 1;
                }
        }
        //perform xor operation
        else if (controlIn -> ALU.op == 4){
                aluResultOut -> result = input1 ^ input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }

        }
        //perform sll operation
        else if (controlIn -> ALU.op == 5){
                aluResultOut -> result = input1 <<  input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 1;
                }
                else{
                        aluResultOut -> zero = 0;
                }
        }
        //perform bne operation
        else if (controlIn -> ALU.op == 6){
                aluResultOut -> result = input1 - input2;
                aluResultOut -> extra = 0;
                if (aluResultOut -> result == 0){
                        aluResultOut -> zero = 0;
                }
                else{
                        aluResultOut -> zero = 1;
                }
        }
}

void execute_MEM(CPUControl *controlIn,
                 ALUResult  *aluResultIn,
                 WORD        rsVal, WORD rtVal,
                 WORD       *memory,
                 MemResult  *resultOut){
        if (controlIn -> memRead == 1){
                resultOut -> readVal = memory[(aluResultIn -> result)/4];
        }
        else if (controlIn -> memWrite == 1){
                memory[(aluResultIn -> result)/4] = rtVal;
                resultOut -> readVal = 0;
        }
        else{
                resultOut -> readVal = 0;
        }
}
WORD getNextPC(InstructionFields *fields, CPUControl *controlIn, int aluZero,
               WORD rsVal, WORD rtVal,
               WORD oldPC){
        if (controlIn -> jump == 1){
                return (((oldPC + 4) & 0xf0000000) | ((fields -> address << 2) & 0x0fffffff));
        }
        else{
                if (controlIn -> branch && aluZero == 1){
                        return ((fields -> imm32 << 2) + (oldPC + 4));
                }
                else{
                        return (oldPC + 4);
                }
        }
}

void execute_updateRegs(InstructionFields *fields, CPUControl *controlIn,
                        ALUResult  *aluResultIn, MemResult *memResultIn,
                        WORD       *regs){
        if (controlIn -> memToReg == 1 && controlIn -> regWrite == 1){
                regs[fields -> rt] = memResultIn -> readVal;
        }
        else if (fields -> opcode == 0 && controlIn -> regWrite == 1){
                regs[fields -> rd] = aluResultIn -> result;
        }
        else if (controlIn -> regWrite == 1){
                regs[fields -> rt] = aluResultIn -> result;
        }
}
