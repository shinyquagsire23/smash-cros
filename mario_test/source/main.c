#include <stdint.h>
#include "debug.h"
#include "CommonTranslated_fighter.h"

extern void* _Z8NewmarioPv_orig(CommonTranslated_fighter* fighter);
extern int _ZN3MSC32TranslatedMotionScriptEngineCore9fastshiftEi(CommonTranslated_fighter* fighter, void* func);

// We're intentionally avoiding .bss here, .bss is extremely touchy at the moment
void* vtable_test[44] = {-1};
void* intercept_funbox_init_ptr = -1;
void* original_funbox_init_ptr = -1;
void* original_677 = -1;

int perframe(CommonTranslated_fighter* fighter)
{
   fighter->msc_state->unk_0_variable_related->moduleAccessor->workModuleImpl->vtable->variable_fadd(fighter->msc_state->unk_0_variable_related->moduleAccessor->workModuleImpl, 0x1000004, 123.0f);

   float test = fighter->msc_state->unk_0_variable_related->moduleAccessor->workModuleImpl->vtable->variable_fget(fighter->msc_state->unk_0_variable_related->moduleAccessor->workModuleImpl, 0x1000004);
   printf("perframe! %f", test);
   for (int i = 0; i < 10; i++)
      printf("%x", fighter->msc_state->unk_0_variable_related->unk_0[i]);

   return 0;
}

int nullsub(CommonTranslated_fighter* fighter)
{
   printf("nullsub! %x", fighter->stick_y_digital);
   /*for (int i = 0; i < 0x44; i++)
   {
      printf("CommonTranslated_fighter %x: %08x", i*4, ((void**)fighter)[i]);
   }*/
   
   fighter->funbox_every_frame_2 = perframe;
   
   //void* (*func)(void *arg1) = (void*)original_677;
   //func(fighter);
   //_ZN3MSC32TranslatedMotionScriptEngineCore9fastshiftEi(fighter, original_677);

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
   
   original_677 = fighter->vtable_677;
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
