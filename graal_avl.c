/************************************************************************
* A big part of this code come from http://www.eternallyconfuzzled.com. *
* Thanks to the author of this website for its sharing.                 *
*                                                                       *
* Copyright © 2016 Matthieu Dien <matthieu.dien@lip6.fr>                *
* This work is free. You can redistribute it and/or modify it under the *
* terms of the Do What The Fuck You Want To Public License, Version 2,  *
* as published by Sam Hocevar. See the COPYING file for more details.   *
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct _avl_node {
  int balance;
  void* data;
  struct _avl_node* sons[2];
}* avl_node;

typedef struct _avl_tree {
  avl_node root;
}* avl_tree;

/*******************
*   Constructors   *
*******************/

avl_node make_node(void* data)
{
  avl_node r = (avl_node)malloc(sizeof(struct _avl_node));
  
  r->data = data;
  r->balance = 0;
  r->sons[0] = r->sons[1] = NULL;
  return r;
}

avl_tree avl_make_empty_tree(void)
{
  avl_tree r = (avl_tree)malloc(sizeof(struct _avl_tree));
  
  r->root = NULL;
  return r;
}

/*******************
*    Destructor    *
*******************/

void erase_tree(avl_node root)
{
  if(root != NULL) {
    avl_node l = root->sons[0], r = root->sons[1];
    
    free(root);
    erase_tree(l);
    erase_tree(r);
  }    
}

void avl_erase_tree(avl_tree tree)
{
  erase_tree(tree->root);
}

/*******************
*      Search      *
*******************/

void* search_r(avl_node root, void* data)
{
  if (root == NULL)
    return NULL;
  else
    if (data == root->data)
      return root->data;
    else
      if ((intptr_t)data < (intptr_t)root->data)
        return search_r(data, root->sons[0]);
      else
        return search_r(data, root->sons[1]);
}
  
void* avl_search(avl_tree tree, void* data)
{
  return search_r(tree->root, data);
}


/*******************
*     Rotation     *
*******************/

// dir == 0 means left rotation and 1 means right
avl_node single_rotation(avl_node root, int dir)
{
  avl_node save = root->sons[!dir];

  root->sons[!dir] = save->sons[dir];
  save->sons[dir] = root;

  return save;
}

// dir == 0 means right-left and 1 means left-right
avl_node double_rotation(avl_node root, int dir)
{
    root = single_rotation(root->sons[!dir], !dir);
    return single_rotation(root, dir);
}

/* Adjust the balances before the application of double_rotation
   (right-left rotation on the following example)
      x
     / \
    A   y
       / \
      z   D
     / \
    B   C
*/
void adjust_balance(avl_node x, int dir, int bal)
{
  avl_node y = x->sons[dir];
  avl_node z = y->sons[!dir];

  // Case: (A ,(x, +2), ((B, (z, 0), C), (y, -1), D))
  if (z->balance == 0)
    {
      x->balance = y->balance = 0;
    }
  // Case: (A ,(x, +2), ((B, (z, +1), C), (y, -1), D))
  else if (z->balance == bal)
    {
      x->balance = -bal;
      y->balance = 0;
    }
  // Case: (A ,(x, +2), ((B, (z, -1), C), (y, -1), D))
  else
    {
      x->balance = 0;
      y->balance = bal;
    }

  z->balance = 0;
}


/*******************
*    Insertion     *
*******************/

avl_node insert_balance(avl_node root, int dir)
{
    avl_node n = root->sons[dir];
    int bal = dir == 0 ? -1 : +1;

    if (n->balance == bal)
    {
        root->balance = n->balance = 0;
        root = single_rotation(root, !dir);
    }
    else /* n->balance == -bal */
    {
        adjust_balance(root, dir, bal);
        root = double_rotation(root, !dir);
    }

    return root;
}

void insert_node(avl_node root, void* data)
{
  /* Empty tree case */
  if(root == NULL)
    root = make_node(data);
  else
    {
      avl_node head = make_node((void*)0); /* False tree root */
      avl_node s, t;     /* Place to rebalance and parent */
      avl_node p, q;     /* Iterator and save pointer */
      int dir;
      
      t = head;
      t->sons[1] = root;
      
      /* Search down the tree, saving rebalance points */
      for (s = p = t->sons[1];; p = q)
        {
          dir = p->data < data;
          q = p->sons[dir];
          
          if (q == NULL)
            break;

          if (q->balance != 0)
            {
              t = p;
              s = q;
            }
        }

      /* Insert the new node */
      p->sons[dir] = q = make_node(data);
      
      /* Update balance factors */
      for (p = s; p != q; p = p->sons[dir])
        {
          dir = p->data < data;
          p->balance += dir == 0 ? -1 : +1;
        }

      q = s; /* Save rebalance point for parent fix */

      /* Rebalance if necessary */
      if (abs(s->balance) > 1)
        {
          dir = s->data < data;
          s = insert_balance(s, dir);
        }

      /* Fix parent */
      if (q == head->sons[1])
        root = s;
      else
        t->sons[q == t->sons[1]] = s;
    }
}

avl_tree avl_insert(avl_tree tree, void* data)
{
  insert_node(tree->root, data);

  return tree;
}


/*******************
*     Deletion     *
*******************/

avl_node remove_balance(avl_node root, int dir, int *done)
{
    avl_node n = root->sons[!dir];
    int bal = dir == 0 ? -1 : +1;

    if (n->balance == -bal)
    {
        root->balance = n->balance = 0;
        root = single_rotation(root, dir);
    }
    else if (n->balance == bal)
    {
        adjust_balance(root, !dir, -bal);
        root = double_rotation(root, dir);
    }
    else /* n->balance == 0 */
    {
        root->balance = -bal;
        n->balance = bal;
        root = single_rotation(root, dir);
        *done = 1;
    }

    return root;
}

avl_node remove_node(avl_node root, void* data, int* done)
{
  if (root != NULL)
    {
      int dir;
      
      /* Remove node */
      if (root->data == data)
        {
          /* Unsons and fix parent */
          if (root->sons[0] == NULL || root->sons[1] == NULL)
            {
              avl_node save;
              
              dir = root->sons[0] == NULL;
              save = root->sons[dir];
              free(root);

              return save;
            }
          else
            {
              /* Find inorder predecessor */
              avl_node heir = root->sons[0];

              while (heir->sons[1] != NULL)
                heir = heir->sons[1];

              /* Copy and set new search data */
              root->data = heir->data;
              data = heir->data;
            }
        }

      dir = root->data < data;
      root->sons[dir] = remove_node(root->sons[dir], data, done);

      if (!*done)
        {
          /* Update balance factors */
          root->balance += dir != 0 ? -1 : +1;

          /* Terminate or rebalance as necessary */
          if (abs(root->balance) == 1)
            *done = 1;
          else if (abs(root->balance) > 1)
            root = remove_balance(root, dir, done);
        }
    }

  return root;
}

avl_tree avl_remove(avl_tree tree, void* data)
{
  int done = 0;
  
  remove_node(tree->root, data, &done);

  return tree;
}
