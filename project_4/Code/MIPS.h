#ifndef _MIPS_H
#define _MIPS_H

#include "common.h"
#include <list>
#include <stack>
#include <fstream>
#include "InterCode.h"


struct Var
{
    string name;
    int offset; // offset to fp, i.e, ebp/rbp
    int regId;

    Var(string name, int offset, int regId)
    {
        this->name = name;
        this->offset = offset;
        this->regId = regId;
    }
};
struct Reg
{
    Var* var;
};
struct AllocateRegResult
{
    int regID;
    bool needLoadingFromMem;
    int offset; // var's offset on stack
    AllocateRegResult(int regID, bool needLoadingFromMem, int offset)
    {
        this->regID = regID;
        this->needLoadingFromMem = needLoadingFromMem;
        this->offset = offset;
    }
};
class MemManager
{
public:
    int spOffSet = -4; // sp's offset to fp, init in func_def
    vector<Var> varList;
    Reg regs[32];
    string regNames[32] = 
    {
        "$zero",
        "$at",
        "$v0","$v1",
        "$a0","$a1","$a2","$a3",
        "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
        "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
        "$t8","$t9",
        "$k0","$k1",
        "$gp",
        "$sp",
        "$fp",
        "$ra"
    };
    int curRegId = 8;

    MemManager();
    AllocateRegResult allocRegId(string varName, int size);
    AllocateRegResult allocRegId(Operand operand);
    int getVarIndexByName(string name);
    
};

class MIPS32Translater
{
private:
    list<string> codeList;
    MemManager memManager;

    /* indicate the index of current param/arg when parsing*/
    int curParamIndex = 0;  // inc in translate_param, zero in func_def
    // int curArgIndex= 0;     // inc in translate_arg, zero in call/assign_call

    Operand getSimplifiedOperand(Operand operand);
    void translateInterCodes(list<InterCode> interCodeList);
    void translate_assign(InterCode interCode);
    void translate_dec(InterCode interCode);
    void translate_binop(InterCode interCode);
    void translate_funcDef(InterCode interCode);
    void translate_param(InterCode interCode);
    void translate_return(InterCode interCode);
    void translate_arg(InterCode interCode);
    void translate_read(InterCode interCode);
    void translate_write(InterCode interCode);
    void translate_assignCall(InterCode interCode);
    void translate_labelDec(InterCode interCode);
    void translate_goto(InterCode interCode);
    void translate_condGoto(InterCode interCode);

    string getRegName(int index);
    void moveStack2Reg(int regID, int offset);
    void moveReg2Stack(int regID, int offset);
    int getRegID(Operand operand);
public:
    void translate(list<InterCode> interCodeList);
    void output();
    void output(string filename);
};


#endif