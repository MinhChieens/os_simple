/*
 * Copyright (C) 2024 pdnguyen of the HCMC University of Technology
 */
/*
 * Source Code License Grant: Authors hereby grants to Licensee
 * a personal to use and modify the Licensed Source Code for
 * the sole purpose of studying during attending the course CO2018.
 */
// #ifdef MM_TLB
/*
 * Memory physical based TLB Cache
 * TLB cache module tlb/tlbcache.c
 *
 * TLB cache is physically memory phy
 * supports random access
 * and runs at high speed
 */

#include "mm.h"
#include <stdlib.h>
#include <stdio.h>
#define init_tlbcache(mp, sz, ...) init_memphy(mp, sz, (1, ##__VA_ARGS__))

/*
 *  tlb_cache_read read TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */
int tlb_cache_read(struct memphy_struct *mp, int pid, int pgnum, BYTE value)
{
   /* TODO: the identify info is mapped to
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */

   if (mp == NULL || mp->tlbtable == NULL)
      return -1;

   // for (int i = 0; i < mp->maxsz; i++) {
   //    if(mp->tlbtable[i].pid == pid && mp->tlbtable[i].pgnum == pgnum) return mp->tlbtable[i].fpn;
   // }

   // int cache_index = (pid * mp->maxsz + pgnum) % mp->maxsz;
   // if (mp->tlbtable[cache_index].pid == pid && mp->tlbtable[cache_index].pgnum == pgnum)
   // {
   //    return mp->tlbtable[cache_index].pgnum;
   // }

   int tag = pid % mp->maxsz;
   if (mp->tlbtable[tag].tag == tag)
   {
      for (int i = 0; i < 10; i++)
      {
         if (mp->tlbtable[tag].cache_line[i].pid == pid && mp->tlbtable[tag].cache_line[i].pgnum == pgnum)
            return mp->tlbtable[tag].cache_line[i].frame;
      }
   }
   return -1;
   // return 0;
}

/*
 *  tlb_cache_write write TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */
int tlb_cache_write(struct memphy_struct *mp, int pid, int pgnum, BYTE value, int frame)
{
   /* TODO: the identify info is mapped to
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
   if (mp == NULL || mp->tlbtable == NULL)
      return -1;
   int tag = pid % mp->maxsz;
   mp->tlbtable[tag].tag = tag;
   for (int i = 0; i < 10; i++)
   {
      if (mp->tlbtable[tag].cache_line[i].pid == -1)
      {

         mp->tlbtable[tag].cache_line[i].pid = pid;
         mp->tlbtable[tag].cache_line[i].pgnum = pgnum;
         mp->tlbtable[tag].cache_line[i].frame = frame;
         return 0;
      }
   }

   for (int i = 0; i < 9; i++)
   {
      mp->tlbtable[tag].cache_line[i].pid = mp->tlbtable[tag].cache_line[i + 1].pid;
      mp->tlbtable[tag].cache_line[i].pgnum = mp->tlbtable[tag].cache_line[i + 1].pgnum;
      mp->tlbtable[tag].cache_line[i].frame = mp->tlbtable[tag].cache_line[i + 1].frame;
   }
   mp->tlbtable[tag].cache_line[9].pid = pid;
   mp->tlbtable[tag].cache_line[9].pgnum = pgnum;
   mp->tlbtable[tag].cache_line[9].frame = frame;
   return 0;
}

/*
 *  TLBMEMPHY_read natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @value: obtained value
 */
int TLBMEMPHY_read(struct memphy_struct *mp, int addr, BYTE *value)
{
   if (mp == NULL)
      return -1;

   /* TLB cached is random access by native */
   *value = mp->storage[addr];

   return 0;
}

/*
 *  TLBMEMPHY_write natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @data: written data
 */
int TLBMEMPHY_write(struct memphy_struct *mp, int addr, BYTE data)
{
   if (mp == NULL)
      return -1;

   /* TLB cached is random access by native */
   mp->storage[addr] = data;

   return 0;
}

/*
 *  TLBMEMPHY_format natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 */

int TLBMEMPHY_dump(struct memphy_struct *mp)
{
   /*TODO dump memphy contnt mp->storage
    *     for tracing the memory content
    */
   printf("==========DUMP TLB_MEMPHY ==========\n");
   for (int i = 0; i < mp->maxsz; i++)
   {
      if (mp->tlbtable[i].tag != -1)
      {
         for (int j = 0; j < 10; j++)
         {
            if (mp->tlbtable[i].cache_line[j].pid != -1)
               printf("Content in tlbtable is numpage %d - frame %d - pid %d\n", mp->tlbtable[i].cache_line[j].pgnum, mp->tlbtable[i].cache_line[j].frame, mp->tlbtable[i].cache_line[j].pid);
         }
      }
   }

   printf("===============================\n");

   return 0;
}

/*
 *  Init TLBMEMPHY struct
 */
int init_tlbmemphy(struct memphy_struct *mp, int max_size)
{
   mp->storage = (BYTE *)malloc(max_size * sizeof(BYTE));
   mp->maxsz = max_size;

   mp->rdmflg = 1;
   mp->tlbtable = malloc(max_size * sizeof(struct tlbentry_struct));
   for (int i = 0; i < max_size; i++)
   {
      mp->tlbtable[i].tag = -1;
      for (int j = 0; j < 10; j++)
      {
         mp->tlbtable[i].cache_line[j].frame = -1;
         mp->tlbtable[i].cache_line[j].pgnum = -1;
         mp->tlbtable[i].cache_line[j].pid = -1;
      }
   }
   return 0;
}

// #endif
