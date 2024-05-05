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

#define CACHE_LINE 4096
#define NUMBER_OF_TAG 16
/*
 *  tlb_cache_read read TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */
int tlb_cache_read(struct memphy_struct *mp, int pid, int pgnum)
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

   //cache line 4096   
   for(int i = 0; i < CACHE_LINE; i++){
      if(mp->tlbtable->cache_line[i]->pid == pid 
      && mp->tlbtable->cache_line[i]->pgnum){
         return mp->tlbtable->cache_line[i]->frame;         
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
int tlb_cache_write(struct memphy_struct *mp, int pid, int pgnum, int frame)
{
   /* TODO: the identify info is mapped to
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
   //main memory 1MB
   //line cache 4096
   if (mp == NULL || mp->tlbtable == NULL)
      return -1;
   //int tag = pid % NUMBER_OF_TAG;   
   //int index = pgnum % CACHE_LINE;
   int full = 0;
   /*duyệt mảng để kiểm tra line nào còn trống, nếu không còn trống thì dùng fifo*/
   /*để bỏ một line ra*/
   for(int i = 0; i < CACHE_LINE; i++){
      if(mp->tlbtable->cache_line[i]->pid == -1){
            mp->tlbtable->cache_line[i]->frame = frame;
            mp->tlbtable->cache_line[i]->pgnum = pgnum;
            mp->tlbtable->cache_line[i]->pid = pid;
            mp->tlbtable->cache_line[i]->hit = 0;
            return 0;
      }
      if(mp->tlbtable->cache_line[i]->pgnum == pgnum 
      && mp->tlbtable->cache_line[i]->pid == pid){
         return 0;
      }         
      full = 1;      
   }
   if(full){
      return -1;
   }
   /*bị trùng*/

   /*mp->tlbtable->cache_line[index]->frame = frame;
   mp->tlbtable->cache_line[index]->pgnum = pgnum;
   mp->tlbtable->cache_line[index]->pid = tag;
   mp->tlbtable->cache_line[index]->hit = 0;*/

   /*for (int i = 0; i < 10; i++)
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
   mp->tlbtable[tag].cache_line[9].frame = frame;*/
   return -1;
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
   for (int i = 0; i < CACHE_LINE; i++)
   {
      if(mp->tlbtable->cache_line[i]->pid != -1){
         printf("Line %d: numpage %d - frame %d- pid %d\n", i, mp->tlbtable->cache_line[i]->pgnum, mp->tlbtable->cache_line[i]->frame, mp->tlbtable->cache_line[i]->pid);
      }

      /*if (mp->tlbtable[i].tag != -1)
      {
         for (int j = 0; j < 10; j++)
         {
            if (mp->tlbtable[i].cache_line[j].pid != -1)
               printf("Content in tlbtable is numpage %d - frame %d - pid %d\n", mp->tlbtable[i].cache_line[j].pgnum, mp->tlbtable[i].cache_line[j].frame, mp->tlbtable[i].cache_line[j].pid);
         }
      }*/
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
   mp->tlbtable = (struct tlbentry_struct *)malloc(sizeof(struct tlbentry_struct));

   //cache sẽ có 4096 index
   int new_size = max_size / 16;
   mp->tlbtable->cache_line = (struct tlbline_struct**)malloc(new_size * sizeof(struct tlbline_struct*));
   for (int i = 0; i < new_size; i++)
   {      
      if(mp->tlbtable->cache_line[i] == NULL){
      	mp->tlbtable->cache_line[i] = (struct tlbline_struct*)malloc(sizeof(struct tlbline_struct));
      }
      mp->tlbtable->cache_line[i]->frame = -1;
      mp->tlbtable->cache_line[i]->pgnum = -1;
      mp->tlbtable->cache_line[i]->pid = -1;      
      mp->tlbtable->cache_line[i]->hit = 0;
   }
   return 0;
}

// #endif
