/************************************************************************
* A big part of this code come from http://www.eternallyconfuzzled.com. *
* Thanks to the author of this website for its sharing.                 *
*                                                                       *
* Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
* This work is free. You can redistribute it and/or modify it under the *
* terms of the Do What The Fuck You Want To Public License, Version 2,  *
* as published by Sam Hocevar. See the COPYING file for more details.   *
************************************************************************/

#ifndef __GRAAL_AVL__
#define __GRAAL_AVL__

typedef struct _avl_tree* avl_tree;

avl_tree avl_make_empty_tree(void);

void avl_erase_tree(avl_tree t);

void* avl_search(avl_tree tree, void* data);

void* avl_remove(avl_tree tree, void* data);

void avl_insert(avl_tree tree, void* data);

#endif
