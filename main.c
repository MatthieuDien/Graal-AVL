/************************************************************************
* Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
* This work is free. You can redistribute it and/or modify it under the *
* terms of the Do What The Fuck You Want To Public License, Version 2,  *
* as published by Sam Hocevar. See the COPYING file for more details.   *
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "graal_avl.h"

int main(int argc, char* argv[])
{
  int n, nbr = 0, nbs = 0;
  avl_tree t;
  clock_t start_time;
  float total_time;
    
  /* initialize random seed: */
  srand(time(NULL));

  if(argc > 1)
    n = atoi(argv[1]);
  else
    n = 10000;

  t = avl_make_empty_tree();

  start_time = clock();
  
  for(int i = 0; i < n; i++)
    {      
      t = avl_insert(t, (void*)(intptr_t)rand());

      if(rand() % 100 == 42)
        {
      	  nbr += 1;
          t = avl_remove(t, (void*)(intptr_t)rand());
        }

      if(rand() % 100 == 42)
        {
          nbs += 1;
          avl_search(t, (void*)(intptr_t)rand());
        }
    }

  total_time = (clock() - start_time)/((float)CLOCKS_PER_SEC);

  avl_erase_tree(t);
  
  printf("I did:\n- %d insertions\n- %d deletions\n- %d research\n in %f seconds\n", n, nbr, nbs, total_time);
  
  return 0;
}
