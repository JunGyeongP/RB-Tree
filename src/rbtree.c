#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {

  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  node_t *nil_node = malloc(sizeof(node_t));

  nil_node->parent = NULL;
  nil_node->left = NULL;
  nil_node->right = NULL;
  nil_node->key = 0;



  
  nil_node->color = RBTREE_BLACK;

  p->nil = nil_node;
  p->root = p->nil;
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  node_t *new_node = malloc(sizeof(node_t));

  new_node->key = key;
  new_node->parent = t->nil;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_BLACK;

  t ->root = new_node;
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t * tmp = t->root;
  while (tmp != t->nil)
  {
    if (tmp->key > key)
      tmp = tmp-> left;
    else if (tmp->key < key)
      tmp = tmp->right;
    else
      return tmp;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  t->root = t->nil;
  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
