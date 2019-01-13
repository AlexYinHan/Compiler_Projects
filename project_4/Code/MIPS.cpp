#include "MIPS.h"

Operand MIPS32Translater::getSimplifiedOperand(Operand operand)
{
    // simplify duplicated ADDRESS/DEREFER
    if(operand->kind == ADDRESS && operand->u.operand->kind == DEREFER)
    {
        // &*t -> t
        return operand->u.operand->u.operand;
    }
    if(operand->kind == DEREFER && operand->u.operand->kind == ADDRESS)
    {
        // &*t -> t
        return operand->u.operand->u.operand;
    }
    return operand;
}
void MIPS32Translater::translateInterCodes(list<InterCode> interCodeList)
{
    list<InterCode>::iterator it;
    for(it = interCodeList.begin(); it != interCodeList.end(); it ++)
    {
        switch((*it)->kind)
        {
            case ASSIGN: translate_assign(*it); break;
			case DEC: translate_dec(*it); break;
			case ADD:
			case SUB:
			case MUL:
			case DIV: translate_binop(*it); break;
			case FUNC_DEF: translate_funcDef(*it); break;
			case PARAM: translate_param(*it); break;
			case RETURN: translate_return(*it); break;
			case ARG: translate_arg(*it); break;
			case READ: translate_read(*it); break;
			case WRITE: translate_write(*it); break;
			case ASSIGN_CALL: translate_assignCall(*it); break;
			case LABEL_DEC: translate_labelDec(*it); break;
			case GOTO: translate_goto(*it); break;
			case COND_GOTO: translate_condGoto(*it); break;
            default: continue;
        }
    }
}
void MIPS32Translater::translate_assign(InterCode interCode)
{
    Operand left = getSimplifiedOperand(interCode->u.assign.left), 
            right = getSimplifiedOperand(interCode->u.assign.right);
    int xRegID = -1, yRegID = -1;
    if(left->kind == VARIABLE || left->kind == TMP)
    {
        switch(right->kind)
        {
            case CONSTANT:
            {
                // x := #k
                xRegID = getRegID(left);
                // code - li reg(x), k
                string code = "li " + getRegName(xRegID) + ", " + to_string(right->u.int_const);
                codeList.push_back(code);
                break;
            }
            case TMP:
            case VARIABLE:
            {
                // x := y
                xRegID = getRegID(left), yRegID = getRegID(right);
                // code - move reg(x), reg(y)
                string code = "move " + getRegName(xRegID) + ", " + getRegName(yRegID);
                codeList.push_back(code);
                break;
            }
            case ADDRESS:
            {
                // x := &y
                right = right->u.operand;
                xRegID = getRegID(left);
                int varIndex = memManager.getVarIndexByName(InterCodeTranslater::toString(right));
                int offset = memManager.varList[varIndex].offset;
                codeList.push_back("addi " + getRegName(xRegID) + ", $fp, " + to_string(offset));
                break;
            }
            case DEREFER:
            {
                // x := *y
                right = right->u.operand;
                xRegID = getRegID(left), yRegID = getRegID(right);
                // code - lw reg(x), 0(reg(y))
                string code = "lw " + getRegName(xRegID) + ", 0(" + getRegName(yRegID) + ")";
                codeList.push_back(code);
                break;
            }
            default:break;
        }
    }
    else if(left->kind == DEREFER)
    {
        left = left->u.operand;
        if(right->kind == CONSTANT)
        {
            // *x := #k
            xRegID = getRegID(left);
            // can't directly sw a value to memory, use $v1 to pass value
            // code - li $v1, k
            string code1 = "li $v1, " + to_string(right->u.int_const);
            codeList.push_back(code1);
            // code - sw $v1, 0(reg(x))
            string code2 = "sw $v1, 0(" + getRegName(xRegID) + ")";
            codeList.push_back(code2);
        }
        else if(right->kind == VARIABLE || right->kind == TMP)
        {
            // *x := y
            xRegID  = getRegID(left), yRegID = getRegID(right);
            // code - sw reg(y), 0(reg(x))
            string code = "sw " + getRegName(yRegID) + ", 0(" + getRegName(xRegID) + ")";
            codeList.push_back(code);
        }
        // not considered:
        // *x := *y
        // *x := &y
    }
    if(!(xRegID >= 4 && xRegID <= 7))
    {
        moveReg2Stack(xRegID, memManager.regs[xRegID].var->offset);
    }
}
void MIPS32Translater::translate_dec(InterCode interCode)
{
    // add to var list, won't load from memory here
    memManager.allocRegId(interCode->u.dec.tableItem_p->name, interCode->u.dec.size);
    // allocate space on stack
}
void MIPS32Translater::translate_binop(InterCode interCode)
{
    Operand result = getSimplifiedOperand(interCode->u.binop.result); 
    Operand op1 = getSimplifiedOperand(interCode->u.binop.op1); 
    Operand op2 = getSimplifiedOperand(interCode->u.binop.op2); 
    int resultRegID = getRegID(result);
    bool hasConstant = op1->kind == CONSTANT || op2->kind == CONSTANT;
    bool bothConstant = op1->kind == CONSTANT && op2->kind == CONSTANT;
    int op1Value = ((op1->kind == CONSTANT) ? op1->u.int_const : getRegID(op1));
    int op2Value = ((op2->kind == CONSTANT) ? op2->u.int_const : getRegID(op2));
    string code;
    switch(interCode->kind)
    {
        case ADD:
        {
            if(bothConstant){
                code = "li " + getRegName(resultRegID) + ", " + to_string(op1Value + op2Value);
            } else if(hasConstant){
                // reg first, constant second
                code = op2->kind == CONSTANT ? 
                    "addi " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", " + to_string(op2Value) :
                    "addi " + getRegName(resultRegID) + ", " + getRegName(op2Value) + ", " + to_string(op1Value);
            } else {
                code = "add " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", " + getRegName(op2Value);
            }
            codeList.push_back(code);
            break;
        }
        case SUB:
        {
            if(bothConstant){
                code = "li " + getRegName(resultRegID) + ", " + to_string(op1Value - op2Value);
            } else if(hasConstant){
                // reg first, constant second
                if(op2->kind == CONSTANT) {
                    code = "addi " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", -" + to_string(op2Value);
                } else {
                    codeList.push_back("li $v1, " + to_string(op1Value));
                    code = "sub " + getRegName(resultRegID) + ", $v1, " + getRegName(op2Value);
                }
            } else {
                code = "sub " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", " + getRegName(op2Value);
            }
            codeList.push_back(code);
            break;
        }
        case MUL:
        {
            if(bothConstant){
                code = "li " + getRegName(resultRegID) + ", " + to_string(op1Value * op2Value);
                codeList.push_back(code);
            } else if(hasConstant){
                string code2;
                if(op2->kind == CONSTANT){
                    code = "li $v1, " + to_string(op2Value);
                    code2 = "mul " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", $v1";
                } else {
                    code = "li $v1, " + to_string(op1Value);
                    code2 = "mul " + getRegName(resultRegID) + ", " + getRegName(op2Value) + ", $v1";
                }
                codeList.push_back(code);
                codeList.push_back(code2);
            } else {
                code = "mul " + getRegName(resultRegID) + ", " + getRegName(op1Value) + ", " + getRegName(op2Value);
                codeList.push_back(code);
            }
            break;
        }
        case DIV:
        {
            if(bothConstant){
                code = "li " + getRegName(resultRegID) + ", " + to_string(op1Value / op2Value);
                codeList.push_back(code);
            } else if(hasConstant){
                string code2, code3;
                if(op2->kind == CONSTANT){
                    code = "li $v1, " + to_string(op2Value);
                    code2 = "div " + getRegName(op1Value) + ", $v1";
                } else {
                    code = "li $v1, " + to_string(op1Value);
                    code2 = "div $v1, " + getRegName(op2Value);
                }
                code3 = "mflo " + getRegName(resultRegID);
                codeList.push_back(code);
                codeList.push_back(code2);
                codeList.push_back(code3);
            } else {
                code = "div " + getRegName(op1Value) + ", " + getRegName(op2Value);
                codeList.push_back(code);
                code = "mflo " + getRegName(resultRegID);
                codeList.push_back(code);
            }
            break;
        }
    }
    if(!(resultRegID >= 4 && resultRegID <= 7))
    {
        // save result to stack
        moveReg2Stack(resultRegID, memManager.regs[resultRegID].var->offset);
    }
}
void MIPS32Translater::translate_funcDef(InterCode interCode)
{
    list<string> codes = {
        interCode->u.function->name + ":"
        // "addi $sp, $sp, -4",
        // "sw $fp, 0($sp)",
        // "move $fp, $sp"
    };
    codeList.splice(codeList.end(), codes);
    if(interCode->u.function->name.compare("main") == 0)
    {
        codeList.push_back("sw $ra, 0($sp)");
        codeList.push_back("sw $fp, -4($sp)");
        codeList.push_back("move $fp, $sp");
    }
    curParamIndex = 0; // prepare for param parsing
    memManager.spOffSet = -44;
}
void MIPS32Translater::translate_param(InterCode interCode)
{
    // param IRs actually tell where to find these variables
    string varName = InterCodeTranslater::toString(interCode->u.sinop.op);
    if(curParamIndex <= 3)
    {
        // in reg $a0-$a3 (4-7)
        Var* var = new Var(varName, 0, 4 + curParamIndex);
        memManager.varList.push_back(*var);
        memManager.regs[4 + curParamIndex].var = var;
    } 
    else
    {
        // on stack
        Var* var = new Var(varName, 4 * (curParamIndex - 3), -1);
        memManager.varList.push_back((*var));
    }
    curParamIndex ++;
}
void MIPS32Translater::translate_return(InterCode interCode)
{
    Operand op = interCode->u.sinop.op;
    string value = (op->kind == CONSTANT) ? ("$" + op->u.int_const) : getRegName(getRegID(op));
    list<string> codes = {
        "move $v0, " + value,
        // "addi $sp, $fp, 4"
        "move $v1, $ra",
        "lw $ra, 0($fp)",
        "lw $fp, -4($fp)",
        "jr $v1"
    };
    codeList.splice(codeList.end(), codes);
}
void MIPS32Translater::translate_arg(InterCode interCode)
{
    // save args to regs or stack
    Operand op = interCode->u.sinop.op;
    string code = "";
    int curArgIndex = interCode->u.arg.argIndex;
    if(curArgIndex <= 3)
    {
        // in reg $a0-$a3 (4-7)
        code = (op->kind == CONSTANT) ? 
                "li " + getRegName(4 + curArgIndex) + ", " + to_string(op->u.int_const) :
                "move " + getRegName(4 + curArgIndex) + ", " + getRegName(getRegID(op));
        codeList.push_back(code);
    }
    else
    {
        // on stack
        code = (op->kind == CONSTANT) ? 
                "li $v1, " + to_string(op->u.int_const) :
                "move $v1, " + getRegName(getRegID(op)); // to make it simple, move to $v1 first
        memManager.spOffSet -= 4;
        list<string> codes = {
            code,
            // "addi $sp, $fp, " + to_string(memManager.spOffSet),
            "sw $v1, " + to_string(memManager.spOffSet) + "($fp)"
        };
        codeList.splice(codeList.end(), codes);
    }
    // curArgIndex ++;
}

void MIPS32Translater::translate_read(InterCode interCode)
{
    Operand op = interCode->u.sinop.op;
    int regId = getRegID(op);
    list<string> codes = {
        "sw $ra, " + to_string(memManager.spOffSet - 4) + "($fp)",
        "sw $fp, " + to_string(memManager.spOffSet - 8) + "($fp)",
        "addi $fp, $fp, " + to_string(memManager.spOffSet - 4),
        "jal read",
        "lw $fp, -4($fp)",
        "lw $ra, 0($fp)",
    };
    codeList.splice(codeList.end(), codes);
    Var var = memManager.varList[memManager.getVarIndexByName(InterCodeTranslater::toString(op))];
    if(var.regId >= 4 && var.regId <= 7)
    {
        // it's a param
        codeList.push_back("move " + getRegName(var.regId) + ", $v0");
    }
    else
    {
        // should be stored on stack
        codeList.push_back("move " + getRegName(var.regId) + ", $v0");
        codeList.push_back("sw $v0, " + to_string(var.offset) + "($fp)");
    }
}
void MIPS32Translater::translate_write(InterCode interCode)
{
    Operand op = interCode->u.sinop.op;
    int regId = getRegID(op);
    list<string> codes = {
        "move $a0, " + getRegName(regId),
        "sw $ra, " + to_string(memManager.spOffSet - 4) + "($fp)",
        "sw $fp, " + to_string(memManager.spOffSet - 8) + "($fp)",
        "addi $fp, $fp, " + to_string(memManager.spOffSet - 4),
        "jal write",
        "lw $fp, -4($fp)",
        "lw $ra, 0($fp)",
    };
    codeList.splice(codeList.end(), codes);
}
void MIPS32Translater::translate_assignCall(InterCode interCode)
{
    Operand op = interCode->u.assign_call.result;
    IRFunction func = interCode->u.assign_call.function;
    list<string> codes = {
        "sw $ra, " + to_string(memManager.spOffSet - 4) + "($fp)",
        "sw $fp, " + to_string(memManager.spOffSet - 8) + "($fp)",
        "sw $t0, " + to_string(memManager.spOffSet - 12) + "($fp)",
        "sw $t1, " + to_string(memManager.spOffSet - 16) + "($fp)",
        "sw $t2, " + to_string(memManager.spOffSet - 20) + "($fp)",
        "sw $t3, " + to_string(memManager.spOffSet - 24) + "($fp)",
        "sw $t4, " + to_string(memManager.spOffSet - 28) + "($fp)",
        "sw $t5, " + to_string(memManager.spOffSet - 32) + "($fp)",
        "sw $t6, " + to_string(memManager.spOffSet - 36) + "($fp)",
        "sw $t7, " + to_string(memManager.spOffSet - 40) + "($fp)",
        "sw $t8, " + to_string(memManager.spOffSet - 44) + "($fp)",
        "sw $t9, " + to_string(memManager.spOffSet - 48) + "($fp)",
        "add $fp, $fp, " + to_string(memManager.spOffSet - 4),
        // "addi $fp, $fp, " + to_string(memManager.spOffSet - 4),
        // "addi $sp, $sp, -4",
        // "sw $ra, 0($sp)",
        "jal " + func->name,
        "lw $t0, " + to_string(memManager.spOffSet - 12) + "($fp)",
        "lw $t1, " + to_string(memManager.spOffSet - 16) + "($fp)",
        "lw $t2, " + to_string(memManager.spOffSet - 20) + "($fp)",
        "lw $t3, " + to_string(memManager.spOffSet - 24) + "($fp)",
        "lw $t4, " + to_string(memManager.spOffSet - 28) + "($fp)",
        "lw $t5, " + to_string(memManager.spOffSet - 32) + "($fp)",
        "lw $t6, " + to_string(memManager.spOffSet - 36) + "($fp)",
        "lw $t7, " + to_string(memManager.spOffSet - 40) + "($fp)",
        "lw $t8, " + to_string(memManager.spOffSet - 44) + "($fp)",
        "lw $t9, " + to_string(memManager.spOffSet - 48) + "($fp)"
    };
    memManager.spOffSet -= 4;
    codeList.splice(codeList.end(), codes);
    int regId = getRegID(op);
    Var var = memManager.varList[memManager.getVarIndexByName(InterCodeTranslater::toString(op))];
    if(var.regId >= 4 && var.regId <= 7)
    {
        // it's a param
        codeList.push_back("move " + getRegName(var.regId) + ", $v0");
    }
    else
    {
        // should be stored on stack
        codeList.push_back("move " + getRegName(var.regId) + ", $v0");
        codeList.push_back("sw $v0, " + to_string(var.offset) + "($fp)");
    }
    // curArgIndex = 0;
}
void MIPS32Translater::translate_labelDec(InterCode interCode)
{
    codeList.push_back("label" + to_string(interCode->u.sinop.op->u.label_no) + ":");
}
void MIPS32Translater::translate_goto(InterCode interCode)
{
    codeList.push_back("j label" + to_string(interCode->u.sinop.op->u.label_no));
}

string relop2mips(string relop)
{
    if(relop.compare("==") == 0){
        return "beq";
    } else if(relop.compare("!=")==0){
        return "bne";
    } else if(relop.compare(">")==0){
        return "bgt";
    } else if(relop.compare("<")==0){
        return "blt";
    } else if(relop.compare(">=")==0){
        return "bge";
    } else if(relop.compare("<=")==0){
        return "ble";
    }
}
bool relopConstant(string relop, Operand x, Operand y)
{
    int xc = x->u.int_const, yc = y->u.int_const;
    if(relop.compare("==") == 0){
        return xc == yc;
    } else if(relop.compare("!=")==0){
        return xc != yc;
    } else if(relop.compare(">")==0){
        return xc > yc;
    } else if(relop.compare("<")==0){
        return xc < yc;
    } else if(relop.compare(">=")==0){
        return xc >= yc;
    } else if(relop.compare("<=")==0){
        return xc <= yc;
    }
}
void MIPS32Translater::translate_condGoto(InterCode interCode)
{
    Operand x = interCode->u.condGoto->x;
    Operand y = interCode->u.condGoto->y;
    Operand label = interCode->u.condGoto->label;
    string relop = interCode->u.condGoto->relop;
    string code = "";
    if(x->kind == CONSTANT && y->kind == CONSTANT) {
        if(relopConstant(relop, x, y))
        {
            codeList.push_back("j label" + to_string(label->u.label_no));
        }
    }
    else
    {
        int xid = getRegID(x), yid = getRegID(y);
        code = relop2mips(relop) + " " + 
                getRegName(xid) + ", " + getRegName(yid) + 
                ", label" + to_string(label->u.label_no);
        codeList.push_back(code);
    }
}
void MIPS32Translater::translate(list<InterCode> interCodeList)
{
    this->codeList.clear();
    // headings
    list<string> headings = {
        ".data",
        "_prompt: .asciiz \"Enter an integer:\"",
        "_ret: .asciiz \"\\n\"",
        ".globl main",
        ".text",
        // FUNCTION read
        "read:",
        "li $v0, 4",
        "la $a0, _prompt",
        "syscall",
        "li $v0, 5",
        "syscall",
        "jr $ra\n",
        // FUNCTION write
        "write:",
        "li $v0, 1",
        "syscall",
        "li $v0, 4",
        "la $a0, _ret",
        "syscall",
        "move $v0, $0",
        "jr $ra\n",
    };
    codeList.assign(headings.begin(), headings.end());
    // codes
    translateInterCodes(interCodeList);
}

void MIPS32Translater::output()
{
    list<string>::iterator it;
    for(it = codeList.begin(); it != codeList.end(); it ++)
    {
        cout << *it << endl;
    }
}

void MIPS32Translater::output(string filename)
{
    ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        return;
    }
    list<string>::iterator it;
    for(it = codeList.begin(); it != codeList.end(); it ++)
    {
        file << *it << endl;
    }
    file.close();
}
string MIPS32Translater::getRegName(int index)
{
    if(index == -1)
    {
        return "";
    }
    return this->memManager.regNames[index];
}
void MIPS32Translater::moveStack2Reg(int regID, int offset)
{
    string code = "lw " + getRegName(regID) + ", " + to_string(offset) + "($fp)";
    codeList.push_back(code);
}
void MIPS32Translater::moveReg2Stack(int regID, int offset)
{
    string code = "sw " + getRegName(regID) + ", " + to_string(offset) + "($fp)";
    codeList.push_back(code);
}
int MIPS32Translater::getRegID(Operand operand)
{
    if(operand->kind == CONSTANT)
    {
        // for MUL/DIV/COND_GOTO, at most one constant, simply load it to v1
        codeList.push_back("li $v1, " + to_string(operand->u.int_const));
        return 3;
    }
    AllocateRegResult ret = memManager.allocRegId(operand);
    if(ret.needLoadingFromMem)
    {
        moveStack2Reg(ret.regID, ret.offset);   // code - lw reg, offset($fp)
    }
    return ret.regID;
}

MemManager::MemManager()
{
    for(int i = 0; i < 32; i ++)
    {
        regs[i].var = NULL;
    }
}
AllocateRegResult MemManager::allocRegId(string varName, int size)
{
    int varId = getVarIndexByName(varName);
    // assign in sequence among $t0-$t7
    int allocatedID = curRegId;
    bool needLoadingFromMem = true;
    int offset;
    curRegId ++;
    if(curRegId == 16)
    {
        curRegId = 8;
    }
    if(varId == -1)
    {
        // allocate space on stack
        spOffSet -= size;
        Var* var = new Var(varName, spOffSet, allocatedID);
        varList.push_back(*var);
        regs[allocatedID].var = var;
        // it's the first time this var has appeared, no need to load from stack
        needLoadingFromMem = false;
        offset = (*var).offset;
    }
    else
    {
        int regId = varList[varId].regId;
        if(regId >= 4 && regId <= 7)
        {
            // arg, this var is still in the reg
            allocatedID = varList[varId].regId;
            needLoadingFromMem = false;
            offset = varList[varId].offset;
        }
        else
        {
            // load from memory to a register
            varList[varId].regId = allocatedID;
            regs[allocatedID].var = &(varList[varId]);
            offset = varList[varId].offset;
        }
    }
    return AllocateRegResult(allocatedID, needLoadingFromMem, offset);
}
AllocateRegResult MemManager::allocRegId(Operand operand)
{
    string varName = InterCodeTranslater::toString(operand);
    return allocRegId(varName, 4);
}


int MemManager::getVarIndexByName(string name)
{
    for(int i = 0; i < varList.size(); i ++)
    {
        if(varList[i].name.compare(name) == 0)
        {
            return i;
        }
    }
    return -1;
}