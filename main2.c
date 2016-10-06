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
  int N, k = 0;
  avl_tree t;
  clock_t start_time;
  float ti, td, ts;
  void** tab;
  char buf[200];
  FILE* fic;
  
  /* initialize random seed: */
  srand(time(NULL));

  fic = fopen("plot.dat", "w");
  
  if(fic == NULL)
    return 1;
  
  if(argc > 1)
    N = atoi(argv[1]);
  else
    N = 100000;

  if(argc > 2)
    k = atoi(argv[2]);
  else
    k = 5;
  
  tab = (void**)malloc(N*sizeof(void*));

  fputs("n insertions searches deletions\n", fic);

  for(int n = 999; n < N; n += 1000)
    {
  
      for(int i = 0; i < n; i++)
	tab[i] = (void*)(intptr_t)rand();
  
      t = avl_make_empty_tree();

      ti = td = ts = 0;
      
      for(int j = 0; j < k; j++)
	{
	  start_time = clock();
  
	  for(int i = 0; i < n; i++)
	    avl_insert(t, tab[i]);

	  ti += (clock() - start_time)/((float)CLOCKS_PER_SEC);

	  start_time = clock();
  
	  for(int i = 0; i < n; i++)
	    avl_search(t, tab[i]);

	  ts += (clock() - start_time)/((float)CLOCKS_PER_SEC);

	  start_time = clock();
  
	  for(int i = 0; i < n; i++)
	    avl_remove(t, tab[i]);

	  td += (clock() - start_time)/((float)CLOCKS_PER_SEC);

	}

      sprintf(buf, "%d %f %f %f\n", n, ti/k, ts/k, td/k);
      fputs(buf, fic);

      avl_erase_tree(t);
    }

  fclose(fic);

  system("gnuplot plot.plt");
  
  return 0;
}
