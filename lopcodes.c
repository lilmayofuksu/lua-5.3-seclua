/*
** $Id: lopcodes.c,v 1.55.1.1 2017/04/19 17:20:42 roberto Exp $
** Opcodes for Lua virtual machine
** See Copyright Notice in lua.h
*/

#define lopcodes_c
#define LUA_CORE

#include "lprefix.h"


#include <stddef.h>

#include "lopcodes.h"


/* ORDER OP */

LUAI_DDEF const char *const luaP_opnames[NUM_OPCODES+1] = {
  "ADD",
  "SUB",
  "MUL",
  "MOD",
  "POW",
  "DIV",
  "IDIV",
  "BAND",
  "BOR",
  "BXOR",
  "SHL",
  "SHR",
  "UNM",
  "BNOT",
  "NOT",
  "LEN",
  "CONCAT",
  "JMP",
  "EQ",
  "LT",
  "LE",
  "MOVE",
  "LOADK",

  "OP_TFORCALL1",
  "OP_TFORCALL2",
  "GETTABUPM",

  "EXTRAARG",
  "LOADBOOL",
  "RETURN",
  "GETUPVAL",
  "SETLIST",
  "LOADKX",
  "SELF",
  "FORLOOP",
  "SETTABLE",
  "TAILCALL",
  "TFORLOOP",
  "SETTABUP",
  "TESTSET",
  "GETTABUP",
  "VARARG",
  "LOADNIL",
  "TFORCALL",
  "SETUPVAL",
  "GETTABLE",
  "FORPREP",
  "NEWTABLE",
  "CALL",
  "CLOSURE",
  "TEST",
  NULL
};


LUAI_DDEF const char* const luaP_opnames_stock[NUM_OPCODES + 1] = {
  "MOVE",
  "LOADK",
  "LOADKX",
  "LOADBOOL",
  "LOADNIL",
  "GETUPVAL",
  "GETTABUP",
  "GETTABLE",
  "SETTABUP",
  "SETUPVAL",
  "SETTABLE",
  "NEWTABLE",
  "SELF",
  "ADD",
  "SUB",
  "MUL",
  "MOD",
  "POW",
  "DIV",
  "IDIV",
  "BAND",
  "BOR",
  "BXOR",
  "SHL",
  "SHR",
  "UNM",
  "BNOT",
  "NOT",
  "LEN",
  "CONCAT",
  "JMP",
  "EQ",
  "LT",
  "LE",
  "TEST",
  "TESTSET",
  "CALL",
  "TAILCALL",
  "RETURN",
  "FORLOOP",
  "FORPREP",
  "TFORCALL",
  "TFORLOOP",
  "SETLIST",
  "CLOSURE",
  "VARARG",
  "EXTRAARG",
  NULL
};


#define opmode(t,a,b,c,m) (((t)<<7) | ((a)<<6) | ((b)<<4) | ((c)<<2) | (m))

LUAI_DDEF const lu_byte luaP_opmodes[NUM_OPCODES] = {
/*       T  A    B       C     mode		   opcode	*/
  opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_ADD */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SUB */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_MUL */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_MOD */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_POW */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_DIV */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_IDIV */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BAND */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BOR */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BXOR */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SHL */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SHR */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_UNM */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_BNOT */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_NOT */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_LEN */
 ,opmode(0, 1, OpArgR, OpArgR, iABC)		/* OP_CONCAT */
 ,opmode(0, 0, OpArgR, OpArgN, iAsBx)		/* OP_JMP */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_EQ */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_LT */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_LE */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_MOVE */
 ,opmode(0, 1, OpArgK, OpArgN, iABx)		/* OP_LOADK */

 ,opmode(0, 0, OpArgN, OpArgU, iABC)		/* OP_TFORCALL1 */
 ,opmode(0, 0, OpArgN, OpArgU, iABC)		/* OP_TFORCALL2 */
 ,opmode(0, 1, OpArgU, OpArgK, iABC)		/* OP_GETTABUPM */

 ,opmode(0, 0, OpArgU, OpArgU, iAx)			/* OP_EXTRAARG */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_LOADBOOL */
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/* OP_RETURN */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_GETUPVAL */
 ,opmode(0, 0, OpArgU, OpArgU, iABC)		/* OP_SETLIST */
 ,opmode(0, 1, OpArgN, OpArgN, iABx)		/* OP_LOADKX */
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/* OP_SELF */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_FORLOOP */
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/* OP_SETTABLE */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_TAILCALL */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_TFORLOOP */
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/* OP_SETTABUP */
 ,opmode(1, 1, OpArgR, OpArgU, iABC)		/* OP_TESTSET */
 ,opmode(0, 1, OpArgU, OpArgK, iABC)		/* OP_GETTABUP */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_VARARG */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_LOADNIL */
 ,opmode(0, 0, OpArgN, OpArgU, iABC)		/* OP_TFORCALL */
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/* OP_SETUPVAL */
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/* OP_GETTABLE */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_FORPREP */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_NEWTABLE */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_CALL */
 ,opmode(0, 1, OpArgU, OpArgN, iABx)		/* OP_CLOSURE */
 ,opmode(1, 0, OpArgN, OpArgU, iABC)		/* OP_TEST */
};

LUAI_DDEF const lu_byte luaP_opmodes_stock[NUM_OPCODES_STOCK] = {
/*       T  A    B       C     mode		   opcode	*/
  opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_MOVE */
 ,opmode(0, 1, OpArgK, OpArgN, iABx)		/* OP_LOADK */
 ,opmode(0, 1, OpArgN, OpArgN, iABx)		/* OP_LOADKX */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_LOADBOOL */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_LOADNIL */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_GETUPVAL */
 ,opmode(0, 1, OpArgU, OpArgK, iABC)		/* OP_GETTABUP */
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/* OP_GETTABLE */
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/* OP_SETTABUP */
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/* OP_SETUPVAL */
 ,opmode(0, 0, OpArgK, OpArgK, iABC)		/* OP_SETTABLE */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_NEWTABLE */
 ,opmode(0, 1, OpArgR, OpArgK, iABC)		/* OP_SELF */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_ADD */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SUB */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_MUL */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_MOD */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_POW */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_DIV */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_IDIV */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BAND */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BOR */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_BXOR */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SHL */
 ,opmode(0, 1, OpArgK, OpArgK, iABC)		/* OP_SHR */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_UNM */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_BNOT */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_NOT */
 ,opmode(0, 1, OpArgR, OpArgN, iABC)		/* OP_LEN */
 ,opmode(0, 1, OpArgR, OpArgR, iABC)		/* OP_CONCAT */
 ,opmode(0, 0, OpArgR, OpArgN, iAsBx)		/* OP_JMP */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_EQ */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_LT */
 ,opmode(1, 0, OpArgK, OpArgK, iABC)		/* OP_LE */
 ,opmode(1, 0, OpArgN, OpArgU, iABC)		/* OP_TEST */
 ,opmode(1, 1, OpArgR, OpArgU, iABC)		/* OP_TESTSET */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_CALL */
 ,opmode(0, 1, OpArgU, OpArgU, iABC)		/* OP_TAILCALL */
 ,opmode(0, 0, OpArgU, OpArgN, iABC)		/* OP_RETURN */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_FORLOOP */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_FORPREP */
 ,opmode(0, 0, OpArgN, OpArgU, iABC)		/* OP_TFORCALL */
 ,opmode(0, 1, OpArgR, OpArgN, iAsBx)		/* OP_TFORLOOP */
 ,opmode(0, 0, OpArgU, OpArgU, iABC)		/* OP_SETLIST */
 ,opmode(0, 1, OpArgU, OpArgN, iABx)		/* OP_CLOSURE */
 ,opmode(0, 1, OpArgU, OpArgN, iABC)		/* OP_VARARG */
 ,opmode(0, 0, OpArgU, OpArgU, iAx)		/* OP_EXTRAARG */
};


LUAI_DDEF const OpCodeStock OpCodeMapping[NUM_OPCODES] = {
	STOCK_OP_ADD,
	STOCK_OP_SUB,
	STOCK_OP_MUL,
	STOCK_OP_MOD,
	STOCK_OP_POW,
	STOCK_OP_DIV,
	STOCK_OP_IDIV,
	STOCK_OP_BAND,
	STOCK_OP_BOR,
	STOCK_OP_BXOR,
	STOCK_OP_SHL,
	STOCK_OP_SHR,
	STOCK_OP_UNM,
	STOCK_OP_BNOT,
	STOCK_OP_NOT,
	STOCK_OP_LEN,
	STOCK_OP_CONCAT,
	STOCK_OP_JMP,
	STOCK_OP_EQ,
	STOCK_OP_LT,
	STOCK_OP_LE,
	STOCK_OP_MOVE,
	STOCK_OP_LOADK,
	STOCK_OP_TFORCALL,
	STOCK_OP_TFORCALL,
	STOCK_OP_GETTABUP,
	STOCK_OP_EXTRAARG,
	STOCK_OP_LOADBOOL,
	STOCK_OP_RETURN,
	STOCK_OP_GETUPVAL,
	STOCK_OP_SETLIST,
	STOCK_OP_LOADKX,
	STOCK_OP_SELF,
	STOCK_OP_FORLOOP,
	STOCK_OP_SETTABLE,
	STOCK_OP_TAILCALL,
	STOCK_OP_TFORLOOP,
	STOCK_OP_SETTABUP,
	STOCK_OP_TESTSET,
	STOCK_OP_GETTABUP,
	STOCK_OP_VARARG,
	STOCK_OP_LOADNIL,
	STOCK_OP_TFORCALL,
	STOCK_OP_SETUPVAL,
	STOCK_OP_GETTABLE,
	STOCK_OP_FORPREP,
	STOCK_OP_NEWTABLE,
	STOCK_OP_CALL,
	STOCK_OP_CLOSURE,
	STOCK_OP_TEST
};