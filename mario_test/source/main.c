#include <stdint.h>
#include "debug.h"
#include "CommonTranslated_fighter.h"

extern void* _Z8NewmarioPv_orig(CommonTranslated_fighter* fighter);

// We're intentionally avoiding .bss here, .bss is extremely touchy at the moment
void* vtable_test[44] = {-1};
void* intercept_funbox_init_ptr = -1;
void* original_funbox_init_ptr = -1;

int nullsub(CommonTranslated_fighter* fighter)
{
   printf("nullsub!");

   return 0;
}

void intercept_init(CommonTranslated_fighter* fighter)
{
   if (original_funbox_init_ptr)
   {
      void* (*func)(void *arg1) = (void*)original_funbox_init_ptr;
      func(fighter);
   }

   // CRO-placed functions
   for (int i = 0; i < 44; i++)
   {
      vtable_test[i] = ((void**)fighter)[i+2506];
      //((void**)fighter)[i+2506] = nullsub;
   }
   
   // funbox and initialized functions?
   /*for (int i = 0; i < 2463 - 722; i++)
   {
      ((void**)fighter)[i+722] = nullsub;
   }*/
   
   fighter->vtable_677 = nullsub; // Double jump or something
   
   /*for (int i = 0; i < 0x9f7; i++)
   {
      printf("CommonTranslated_fighter %x: %08x", i*4, ((void**)fighter)[i]);
   }*/
}

void* _Z8NewmarioPv(CommonTranslated_fighter* fighter)
{
   // Pre-hook stuff

   // Call the original function
   _Z8NewmarioPv_orig(fighter);
   
   // Call our init once
   original_funbox_init_ptr = 0;
   intercept_init(fighter);

   // Change the init pointer
   intercept_funbox_init_ptr = intercept_init;
   original_funbox_init_ptr = *fighter->funbox_init;
   fighter->funbox_init = &intercept_funbox_init_ptr;
   
   // Post-hook stuff
   debug_print("Mario Instantiated!\n");
   
   return fighter;
}
