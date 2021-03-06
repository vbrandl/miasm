// Inspired from JitCore_mep.c

#include <Python.h>
#include "structmember.h"
#include <stdint.h>

#include <inttypes.h>
#include "../queue.h"
#include "../vm_mngr.h"
#include "../vm_mngr_py.h"
#include "../bn.h"
#include "../JitCore.h"
#include "JitCore_mep.h"


reg_dict gpreg_dict[] = {
	{.name = "R0", .offset = offsetof(vm_cpu_t, R0), .size = 32},
	{.name = "R1", .offset = offsetof(vm_cpu_t, R1), .size = 32},
	{.name = "R2", .offset = offsetof(vm_cpu_t, R2), .size = 32},
	{.name = "R3", .offset = offsetof(vm_cpu_t, R3), .size = 32},
	{.name = "R4", .offset = offsetof(vm_cpu_t, R4), .size = 32},
	{.name = "R5", .offset = offsetof(vm_cpu_t, R5), .size = 32},
	{.name = "R6", .offset = offsetof(vm_cpu_t, R6), .size = 32},
	{.name = "R7", .offset = offsetof(vm_cpu_t, R7), .size = 32},
	{.name = "R8", .offset = offsetof(vm_cpu_t, R8), .size = 32},
	{.name = "R9", .offset = offsetof(vm_cpu_t, R9), .size = 32},
	{.name = "R10", .offset = offsetof(vm_cpu_t, R10), .size = 32},
	{.name = "R11", .offset = offsetof(vm_cpu_t, R11), .size = 32},
	{.name = "R12", .offset = offsetof(vm_cpu_t, R12), .size = 32},
	{.name = "TP", .offset = offsetof(vm_cpu_t, TP), .size = 32},
	{.name = "GP", .offset = offsetof(vm_cpu_t, GP), .size = 32},
	{.name = "SP", .offset = offsetof(vm_cpu_t, SP), .size = 32},

	{.name = "PC", .offset = offsetof(vm_cpu_t, PC), .size = 32},
	{.name = "LP", .offset = offsetof(vm_cpu_t, LP), .size = 32},
	{.name = "SAR", .offset = offsetof(vm_cpu_t, SAR), .size = 32},
	{.name = "S3", .offset = offsetof(vm_cpu_t, S3), .size = 32},
	{.name = "RPB", .offset = offsetof(vm_cpu_t, RPB), .size = 32},
	{.name = "RPE", .offset = offsetof(vm_cpu_t, RPE), .size = 32},
	{.name = "RPC", .offset = offsetof(vm_cpu_t, RPC), .size = 32},
	{.name = "HI", .offset = offsetof(vm_cpu_t, HI), .size = 32},
	{.name = "LO", .offset = offsetof(vm_cpu_t, LO), .size = 32},
	{.name = "S9", .offset = offsetof(vm_cpu_t, S9), .size = 32},
	{.name = "S10", .offset = offsetof(vm_cpu_t, S10), .size = 32},
	{.name = "S11", .offset = offsetof(vm_cpu_t, S11), .size = 32},
	{.name = "MB0", .offset = offsetof(vm_cpu_t, MB0), .size = 32},
	{.name = "ME0", .offset = offsetof(vm_cpu_t, ME0), .size = 32},
	{.name = "MB1", .offset = offsetof(vm_cpu_t, MB1), .size = 32},
	{.name = "ME1", .offset = offsetof(vm_cpu_t, ME1), .size = 32},
	{.name = "PSW", .offset = offsetof(vm_cpu_t, PSW), .size = 32},
	{.name = "ID", .offset = offsetof(vm_cpu_t, ID), .size = 32},
	{.name = "TMP", .offset = offsetof(vm_cpu_t, TMP), .size = 32},
	{.name = "EPC", .offset = offsetof(vm_cpu_t, EPC), .size = 32},
	{.name = "EXC", .offset = offsetof(vm_cpu_t, EXC), .size = 32},
	{.name = "CFG", .offset = offsetof(vm_cpu_t, CFG), .size = 32},
	{.name = "S22", .offset = offsetof(vm_cpu_t, S22), .size = 32},
	{.name = "NPC", .offset = offsetof(vm_cpu_t, NPC), .size = 32},
	{.name = "DBG", .offset = offsetof(vm_cpu_t, DBG), .size = 32},
	{.name = "DEPC", .offset = offsetof(vm_cpu_t, DEPC), .size = 32},
	{.name = "OPT", .offset = offsetof(vm_cpu_t, OPT), .size = 32},
	{.name = "RCFG", .offset = offsetof(vm_cpu_t, RCFG), .size = 32},
	{.name = "CCFG", .offset = offsetof(vm_cpu_t, CCFG), .size = 32},
	{.name = "S29", .offset = offsetof(vm_cpu_t, S29), .size = 32},
	{.name = "S30", .offset = offsetof(vm_cpu_t, S30), .size = 32},
	{.name = "S31", .offset = offsetof(vm_cpu_t, S31), .size = 32},
	{.name = "S32", .offset = offsetof(vm_cpu_t, S32), .size = 32},
	{.name = "take_jmp", .offset = offsetof(vm_cpu_t, take_jmp), .size = 32},
	{.name = "last_addr", .offset = offsetof(vm_cpu_t, last_addr), .size = 32},
	{.name = "is_repeat_end", .offset = offsetof(vm_cpu_t, is_repeat_end), .size = 32},

	{.name = "PC_end", .offset = offsetof(vm_cpu_t, PC_end), .size = 32},
	{.name = "RPE_instr_count", .offset = offsetof(vm_cpu_t, RPE_instr_count), .size = 32},
	{.name = "RPC_current", .offset = offsetof(vm_cpu_t, RPC_current), .size = 32},

};

/************************** JitCpu object **************************/



PyObject* cpu_get_gpreg(JitCpu* self)
{
    PyObject *dict = PyDict_New();
    PyObject *o;

    get_reg(R0);
    get_reg(R1);
    get_reg(R2);
    get_reg(R3);
    get_reg(R4);
    get_reg(R5);
    get_reg(R6);
    get_reg(R7);
    get_reg(R8);
    get_reg(R9);
    get_reg(R10);
    get_reg(R11);
    get_reg(R12);
    get_reg(TP);
    get_reg(GP);
    get_reg(SP);

    get_reg(PC);
    get_reg(LP);
    get_reg(SAR);
    get_reg(S3);
    get_reg(RPB);
    get_reg(RPE);
    get_reg(RPC);
    get_reg(HI);
    get_reg(LO);
    get_reg(S9);
    get_reg(S10);
    get_reg(S11);
    get_reg(MB0);
    get_reg(ME0);
    get_reg(MB1);
    get_reg(ME1);
    get_reg(PSW);
    get_reg(ID);
    get_reg(TMP);
    get_reg(EPC);
    get_reg(EXC);
    get_reg(CFG);
    get_reg(S22);
    get_reg(NPC);
    get_reg(DBG);
    get_reg(DEPC);
    get_reg(OPT);
    get_reg(RCFG);
    get_reg(CCFG);
    get_reg(S29);
    get_reg(S30);
    get_reg(S31);
    get_reg(S32);

    get_reg(PC_end);
    get_reg(RPE_instr_count);
    get_reg(RPC_current);


    return dict;
}


PyObject* cpu_set_gpreg(JitCpu* self, PyObject *args)
{
    PyObject* dict;
    PyObject *d_key, *d_value = NULL;
    Py_ssize_t pos = 0;
    uint64_t val;
    unsigned int i, found;

    if (!PyArg_ParseTuple(args, "O", &dict))
        return NULL;
    if(!PyDict_Check(dict))
        RAISE(PyExc_TypeError, "arg must be dict");
    while(PyDict_Next(dict, &pos, &d_key, &d_value)){
        if(!PyString_Check(d_key))
            RAISE(PyExc_TypeError, "key must be str");

        PyGetInt(d_value, val);

        found = 0;
        for (i=0; i < sizeof(gpreg_dict)/sizeof(reg_dict); i++){
            if (strcmp(PyString_AsString(d_key), gpreg_dict[i].name))
                continue;
            *((uint32_t*)(((char*)(self->cpu)) + gpreg_dict[i].offset)) = val;
            found = 1;
            break;
        }

        if (found)
            continue;
        fprintf(stderr, "unknown key: %s\n", PyString_AsString(d_key));
        RAISE(PyExc_ValueError, "unknown reg");
    }
    Py_INCREF(Py_None);
    return Py_None;
}




PyObject * cpu_init_regs(JitCpu* self)
{
    memset(self->cpu, 0, sizeof(vm_cpu_t));

    Py_INCREF(Py_None);
    return Py_None;

}

void dump_gpregs(vm_cpu_t* vmcpu)
{
        printf("R0  %.4"PRIX32" ", vmcpu->R0);
        printf("R1  %.4"PRIX32" ", vmcpu->R1);
        printf("R2  %.4"PRIX32" ", vmcpu->R2);
        printf("R3  %.4"PRIX32" ", vmcpu->R3);
        printf("R4  %.4"PRIX32" ", vmcpu->R4);
        printf("R5  %.4"PRIX32" ", vmcpu->R5);
        printf("R6  %.4"PRIX32" ", vmcpu->R6);
        printf("R7  %.4"PRIX32" ", vmcpu->R7);
        printf("R8  %.4"PRIX32" ", vmcpu->R8);
        printf("R9  %.4"PRIX32" ", vmcpu->R9);
        printf("R10  %.4"PRIX32" ", vmcpu->R10);
        printf("R11  %.4"PRIX32" ", vmcpu->R11);
        printf("R12  %.4"PRIX32" ", vmcpu->R12);
        printf("TP  %.4"PRIX32" ", vmcpu->TP);
        printf("GP  %.4"PRIX32" ", vmcpu->GP);
        printf("SP  %.4"PRIX32" ", vmcpu->SP);
        printf("\n");
}


PyObject * cpu_dump_gpregs(JitCpu* self, PyObject* args)
{
    vm_cpu_t* vmcpu;

    vmcpu = self->cpu;
    dump_gpregs(vmcpu);
    Py_INCREF(Py_None);
    return Py_None;
}


PyObject* cpu_set_exception(JitCpu* self, PyObject* args)
{
    PyObject *item1;
    uint64_t i;

    if (!PyArg_ParseTuple(args, "O", &item1))
        return NULL;

    PyGetInt(item1, i);

    ((vm_cpu_t*)self->cpu)->exception_flags = i;
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject* cpu_get_exception(JitCpu* self, PyObject* args)
{
    return PyLong_FromUnsignedLongLong((uint64_t)(((vm_cpu_t*)self->cpu)->exception_flags));
}





void check_automod(JitCpu* jitcpu, uint64_t addr, uint64_t size)
{
    PyObject *result;

    if (!(((VmMngr*)jitcpu->pyvm)->vm_mngr.exception_flags & EXCEPT_CODE_AUTOMOD))
        return;
    result = PyObject_CallMethod(jitcpu->jitter, "automod_cb", "LL", addr, size);
    Py_DECREF(result);

}

void MEM_WRITE_08(JitCpu* jitcpu, uint64_t addr, uint8_t src)
{
    vm_MEM_WRITE_08(&((VmMngr*)jitcpu->pyvm)->vm_mngr, addr, src);
    check_automod(jitcpu, addr, 8);
}

void MEM_WRITE_16(JitCpu* jitcpu, uint64_t addr, uint16_t src)
{
    vm_MEM_WRITE_16(&((VmMngr*)jitcpu->pyvm)->vm_mngr, addr, src);
    check_automod(jitcpu, addr, 16);
}

void MEM_WRITE_32(JitCpu* jitcpu, uint64_t addr, uint32_t src)
{
    vm_MEM_WRITE_32(&((VmMngr*)jitcpu->pyvm)->vm_mngr, addr, src);
    check_automod(jitcpu, addr, 32);
}

void MEM_WRITE_64(JitCpu* jitcpu, uint64_t addr, uint64_t src)
{
    vm_MEM_WRITE_64(&((VmMngr*)jitcpu->pyvm)->vm_mngr, addr, src);
    check_automod(jitcpu, addr, 64);
}


PyObject* vm_set_mem(JitCpu *self, PyObject* args)
{
       PyObject *py_addr;
       PyObject *py_buffer;
       Py_ssize_t py_length;

       char * buffer;
       uint64_t size;
       uint64_t addr;
       int ret = 0x1337;

       if (!PyArg_ParseTuple(args, "OO", &py_addr, &py_buffer))
           return NULL;

       PyGetInt(py_addr, addr);

       if(!PyString_Check(py_buffer))
           RAISE(PyExc_TypeError,"arg must be str");

       size = PyString_Size(py_buffer);
       PyString_AsStringAndSize(py_buffer, &buffer, &py_length);

       ret = vm_write_mem(&(((VmMngr*)self->pyvm)->vm_mngr), addr, buffer, size);
       if (ret < 0)
           RAISE(PyExc_TypeError,"arg must be str");
       check_automod(self, addr, size*8);

       Py_INCREF(Py_None);
       return Py_None;
}

static PyMemberDef JitCpu_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef JitCpu_methods[] = {
    {"init_regs", (PyCFunction)cpu_init_regs, METH_NOARGS, "X"},
    {"dump_gpregs", (PyCFunction)cpu_dump_gpregs, METH_NOARGS, "X"},
    {"get_gpreg", (PyCFunction)cpu_get_gpreg, METH_NOARGS, "X"},
    {"set_gpreg", (PyCFunction)cpu_set_gpreg, METH_VARARGS, "X"},
    {"get_exception", (PyCFunction)cpu_get_exception, METH_VARARGS, "X"},
    {"set_exception", (PyCFunction)cpu_set_exception, METH_VARARGS, "X"},
    {"set_mem", (PyCFunction)vm_set_mem, METH_VARARGS, "X"},
    {"get_mem", (PyCFunction)vm_get_mem, METH_VARARGS, "X"},
    {NULL}  /* Sentinel */
};

static int
JitCpu_init(JitCpu *self, PyObject *args, PyObject *kwds)
{
    self->cpu = malloc(sizeof(vm_cpu_t));
    if (self->cpu == NULL) {
        fprintf(stderr, "cannot alloc vm_cpu_t\n");
        exit(0);
    }
    return 0;
}

getset_reg_u32(R0);
getset_reg_u32(R1);
getset_reg_u32(R2);
getset_reg_u32(R3);
getset_reg_u32(R4);
getset_reg_u32(R5);
getset_reg_u32(R6);
getset_reg_u32(R7);
getset_reg_u32(R8);
getset_reg_u32(R9);
getset_reg_u32(R10);
getset_reg_u32(R11);
getset_reg_u32(R12);
getset_reg_u32(TP);
getset_reg_u32(GP);
getset_reg_u32(SP);

getset_reg_u32(PC);
getset_reg_u32(LP);
getset_reg_u32(SAR);
getset_reg_u32(S3);
getset_reg_u32(RPB);
getset_reg_u32(RPE);
getset_reg_u32(RPC);
getset_reg_u32(HI);
getset_reg_u32(LO);
getset_reg_u32(S9);
getset_reg_u32(S10);
getset_reg_u32(S11);
getset_reg_u32(MB0);
getset_reg_u32(ME0);
getset_reg_u32(MB1);
getset_reg_u32(ME1);
getset_reg_u32(PSW);
getset_reg_u32(ID);
getset_reg_u32(TMP);
getset_reg_u32(EPC);
getset_reg_u32(EXC);
getset_reg_u32(CFG);
getset_reg_u32(S22);
getset_reg_u32(NPC);
getset_reg_u32(DBG);
getset_reg_u32(DEPC);
getset_reg_u32(OPT);
getset_reg_u32(RCFG);
getset_reg_u32(CCFG);
getset_reg_u32(S29);
getset_reg_u32(S30);
getset_reg_u32(S31);
getset_reg_u32(S32);

getset_reg_u32(PC_end);
getset_reg_u32(RPE_instr_count);
getset_reg_u32(RPC_current);



PyObject* get_gpreg_offset_all(void)
{
    PyObject *dict = PyDict_New();
    PyObject *o;
    get_reg_off(exception_flags);

    get_reg_off(R0);
    get_reg_off(R1);
    get_reg_off(R2);
    get_reg_off(R3);
    get_reg_off(R4);
    get_reg_off(R5);
    get_reg_off(R6);
    get_reg_off(R7);
    get_reg_off(R8);
    get_reg_off(R9);
    get_reg_off(R10);
    get_reg_off(R11);
    get_reg_off(R12);
    get_reg_off(TP);
    get_reg_off(GP);
    get_reg_off(SP);

    get_reg_off(PC);
    get_reg_off(LP);
    get_reg_off(SAR);
    get_reg_off(S3);
    get_reg_off(RPB);
    get_reg_off(RPE);
    get_reg_off(RPC);
    get_reg_off(HI);
    get_reg_off(LO);
    get_reg_off(S9);
    get_reg_off(S10);
    get_reg_off(S11);
    get_reg_off(MB0);
    get_reg_off(ME0);
    get_reg_off(MB1);
    get_reg_off(ME1);
    get_reg_off(PSW);
    get_reg_off(ID);
    get_reg_off(TMP);
    get_reg_off(EPC);
    get_reg_off(EXC);
    get_reg_off(CFG);
    get_reg_off(S22);
    get_reg_off(NPC);
    get_reg_off(DBG);
    get_reg_off(DEPC);
    get_reg_off(OPT);
    get_reg_off(RCFG);
    get_reg_off(CCFG);
    get_reg_off(S29);
    get_reg_off(S30);
    get_reg_off(S31);
    get_reg_off(S32);

    get_reg_off(PC_end);
    get_reg_off(RPE_instr_count);
    get_reg_off(RPC_current);


    return dict;
}




static PyGetSetDef JitCpu_getseters[] = {
    {"vmmngr",
     (getter)JitCpu_get_vmmngr, (setter)JitCpu_set_vmmngr,
     "vmmngr",
     NULL},

    {"jitter",
     (getter)JitCpu_get_jitter, (setter)JitCpu_set_jitter,
     "jitter",
     NULL},


    {"R0" , (getter)JitCpu_get_R0      , (setter)JitCpu_set_R0     , "R0" , NULL},
    {"R1" , (getter)JitCpu_get_R1      , (setter)JitCpu_set_R1     , "R1" , NULL},
    {"R2" , (getter)JitCpu_get_R2      , (setter)JitCpu_set_R2     , "R2" , NULL},
    {"R3" , (getter)JitCpu_get_R3      , (setter)JitCpu_set_R3     , "R3" , NULL},
    {"R4" , (getter)JitCpu_get_R4      , (setter)JitCpu_set_R4     , "R4" , NULL},
    {"R5" , (getter)JitCpu_get_R5      , (setter)JitCpu_set_R5     , "R5" , NULL},
    {"R6" , (getter)JitCpu_get_R6      , (setter)JitCpu_set_R6     , "R6" , NULL},
    {"R7" , (getter)JitCpu_get_R7      , (setter)JitCpu_set_R7     , "R7" , NULL},
    {"R8" , (getter)JitCpu_get_R8      , (setter)JitCpu_set_R8     , "R8" , NULL},
    {"R9" , (getter)JitCpu_get_R9      , (setter)JitCpu_set_R9     , "R9" , NULL},
    {"R10" , (getter)JitCpu_get_R10      , (setter)JitCpu_set_R10     , "R10" , NULL},
    {"R11" , (getter)JitCpu_get_R11      , (setter)JitCpu_set_R11     , "R11" , NULL},
    {"R12" , (getter)JitCpu_get_R12      , (setter)JitCpu_set_R12     , "R12" , NULL},
    {"TP" , (getter)JitCpu_get_TP      , (setter)JitCpu_set_TP     , "TP" , NULL},
    {"GP" , (getter)JitCpu_get_GP      , (setter)JitCpu_set_GP     , "GP" , NULL},
    {"SP" , (getter)JitCpu_get_SP      , (setter)JitCpu_set_SP     , "SP" , NULL},

    {"PC" , (getter)JitCpu_get_PC      , (setter)JitCpu_set_PC     , "PC" , NULL},
    {"LP" , (getter)JitCpu_get_LP      , (setter)JitCpu_set_LP     , "LP" , NULL},
    {"SAR" , (getter)JitCpu_get_SAR      , (setter)JitCpu_set_SAR     , "SAR" , NULL},
    {"S3" , (getter)JitCpu_get_S3      , (setter)JitCpu_set_S3     , "S3" , NULL},
    {"RPB" , (getter)JitCpu_get_RPB      , (setter)JitCpu_set_RPB     , "RPB" , NULL},
    {"RPE" , (getter)JitCpu_get_RPE      , (setter)JitCpu_set_RPE     , "RPE" , NULL},
    {"RPC" , (getter)JitCpu_get_RPC      , (setter)JitCpu_set_RPC     , "RPC" , NULL},
    {"HI" , (getter)JitCpu_get_HI      , (setter)JitCpu_set_HI     , "HI" , NULL},
    {"LO" , (getter)JitCpu_get_LO      , (setter)JitCpu_set_LO     , "LO" , NULL},
    {"S9" , (getter)JitCpu_get_S9      , (setter)JitCpu_set_S9     , "S9" , NULL},
    {"S10" , (getter)JitCpu_get_S10      , (setter)JitCpu_set_S10     , "S10" , NULL},
    {"S11" , (getter)JitCpu_get_S11      , (setter)JitCpu_set_S11     , "S11" , NULL},
    {"MB0" , (getter)JitCpu_get_MB0      , (setter)JitCpu_set_MB0     , "MB0" , NULL},
    {"ME0" , (getter)JitCpu_get_ME0      , (setter)JitCpu_set_ME0     , "ME0" , NULL},
    {"MB1" , (getter)JitCpu_get_MB1      , (setter)JitCpu_set_MB1     , "MB1" , NULL},
    {"ME1" , (getter)JitCpu_get_ME1      , (setter)JitCpu_set_ME1     , "ME1" , NULL},
    {"PSW" , (getter)JitCpu_get_PSW      , (setter)JitCpu_set_PSW     , "PSW" , NULL},
    {"ID" , (getter)JitCpu_get_ID      , (setter)JitCpu_set_ID     , "ID" , NULL},
    {"TMP" , (getter)JitCpu_get_TMP      , (setter)JitCpu_set_TMP     , "TMP" , NULL},
    {"EPC" , (getter)JitCpu_get_EPC      , (setter)JitCpu_set_EPC     , "EPC" , NULL},
    {"EXC" , (getter)JitCpu_get_EXC      , (setter)JitCpu_set_EXC     , "EXC" , NULL},
    {"CFG" , (getter)JitCpu_get_CFG      , (setter)JitCpu_set_CFG     , "CFG" , NULL},
    {"S22" , (getter)JitCpu_get_S22      , (setter)JitCpu_set_S22     , "S22" , NULL},
    {"NPC" , (getter)JitCpu_get_NPC      , (setter)JitCpu_set_NPC     , "NPC" , NULL},
    {"DBG" , (getter)JitCpu_get_DBG      , (setter)JitCpu_set_DBG     , "DBG" , NULL},
    {"DEPC" , (getter)JitCpu_get_DEPC      , (setter)JitCpu_set_DEPC     , "DEPC" , NULL},
    {"OPT" , (getter)JitCpu_get_OPT      , (setter)JitCpu_set_OPT     , "OPT" , NULL},
    {"RCFG" , (getter)JitCpu_get_RCFG      , (setter)JitCpu_set_RCFG     , "RCFG" , NULL},
    {"CCFG" , (getter)JitCpu_get_CCFG      , (setter)JitCpu_set_CCFG     , "CCFG" , NULL},
    {"S29" , (getter)JitCpu_get_S29      , (setter)JitCpu_set_S29     , "S29" , NULL},
    {"S30" , (getter)JitCpu_get_S30      , (setter)JitCpu_set_S30     , "S30" , NULL},
    {"S31" , (getter)JitCpu_get_S31      , (setter)JitCpu_set_S31     , "S31" , NULL},
    {"S32" , (getter)JitCpu_get_S32      , (setter)JitCpu_set_S32     , "S32" , NULL},

    {"PC_end" , (getter)JitCpu_get_PC_end      , (setter)JitCpu_set_PC_end     , "PC_end" , NULL},
    {"RPE_instr_count" , (getter)JitCpu_get_RPE_instr_count      , (setter)JitCpu_set_RPE_instr_count     , "RPE_instr_count" , NULL},
    {"RPC_current" , (getter)JitCpu_get_RPC_current      , (setter)JitCpu_set_RPC_current     , "RPC_current" , NULL},



    {NULL}  /* Sentinel */
};



static PyTypeObject JitCpuType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "JitCore_mep.JitCpu",   /*tp_name*/
    sizeof(JitCpu),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)JitCpu_dealloc,/*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "JitCpu objects",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    JitCpu_methods,            /* tp_methods */
    JitCpu_members,            /* tp_members */
    JitCpu_getseters,          /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)JitCpu_init,     /* tp_init */
    0,                         /* tp_alloc */
    JitCpu_new,                /* tp_new */
};



static PyMethodDef JitCore_mep_Methods[] = {

    /*

    */
    {"get_gpreg_offset_all", (PyCFunction)get_gpreg_offset_all, METH_NOARGS},
    {NULL, NULL, 0, NULL}        /* Sentinel */

};

static PyObject *JitCore_mep_Error;

PyMODINIT_FUNC
initJitCore_mep(void)
{
    PyObject *m;

    if (PyType_Ready(&JitCpuType) < 0)
    return;

    m = Py_InitModule("JitCore_mep", JitCore_mep_Methods);
    if (m == NULL)
        return;

    JitCore_mep_Error = PyErr_NewException("JitCore_mep.error", NULL, NULL);
    Py_INCREF(JitCore_mep_Error);
    PyModule_AddObject(m, "error", JitCore_mep_Error);

    Py_INCREF(&JitCpuType);
    PyModule_AddObject(m, "JitCpu", (PyObject *)&JitCpuType);

}
