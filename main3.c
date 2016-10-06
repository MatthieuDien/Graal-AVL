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

#define N 15

int main()
{
  avl_tree t;
  int tab[N];

  srand(time(NULL));

  t = avl_make_empty_tree();
  
  for(int i = 0; i < N; i++)
    {
      tab[i] = 1 + rand() % 100;
      printf("Inserting %d\n", tab[i]);
      t = avl_insert(t, (void*)(intptr_t)tab[i]);
    }

  printf("\nTraversal:\n");  
  avl_traverse_and_print(t);
  
  printf("\nIs %d in the tree ?\n", tab[N/2]);
  printf("%d (1 means yes, 0 means no)\n", NULL != avl_search(t, (void*)(intptr_t)tab[N/2]));

  printf("\nIs %d in the tree ?\n", 142);
  printf("%d (1 means yes, 0 means no)\n", NULL != avl_search(t, (void*)(intptr_t)1422));

  printf("\nRemoving %d\n", tab[N/2]);
  t = avl_remove(t, (void*)(intptr_t)tab[N/2]);

  printf("\nIs %d in the tree ?\n", tab[N/2]);
  printf("%d (1 means yes, 0 means no)\n\n", NULL != avl_search(t, (void*)(intptr_t)tab[N/2]));
  
  avl_erase_tree(t);

  return 0;
}
