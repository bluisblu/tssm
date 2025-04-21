#ifndef XMEM_H
#define XMEM_H

#include <types.h>
#include <rpworld.h>
#include <rwcore.h>
#include <rpworld.h>

struct xBase;

struct RyzMemGrow
{
    S32 flg_grow;
    S32 amt;
    signed char* ptr;
    xBase* user;
    S32 amt_last;
    signed char* ptr_last;
    xBase* user_last;
};

struct RyzMemData
{
    void __dl();
    void* __nw(U32 amt, RyzMemGrow* growCtxt);
};

struct xFactoryInst : RyzMemData
{
    S32 itemType;
    xFactoryInst* nextprod;
    xFactoryInst* prevprod;
};

struct xMemPool
{
    void* FreeList;
    U16 NextOffset;
    U16 Flags;
    void* UsedList;
    void (*InitCB)(xMemPool*, void*);
    void* Buffer;
    U16 Size;
    U16 NumRealloc;
    U32 Total;
};

#endif
