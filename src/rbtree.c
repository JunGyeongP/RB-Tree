#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  nil->key = 0;
  nil->parent = NULL;
  nil->left = NULL;
  nil->right = NULL;

  p->nil = nil;
  p->root = p->nil;

  return p;
}

void delete_rbtree_DFS(rbtree *t, node_t *p) {
  if (p == t->nil) {
    return;
  }
  else {
    delete_rbtree_DFS(t, p->left);
    delete_rbtree_DFS(t, p->right);
    free(p);
  }
}

void delete_rbtree(rbtree *t) {
  delete_rbtree_DFS(t, t->root);
  free(t->nil);
  free(t);
}

void rbtree_left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;

  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }

  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void rbtree_right_rotate(rbtree *t, node_t *y) {
  node_t *x = y->left;

  y->left = x->right;
  if (x->right != t->nil) {
    x->right->parent = y;
  }

  x->parent = y->parent;
  if (y->parent == t->nil) {
    t->root = x;
  }
  else if (y == y->parent->left) {
    y->parent->left = x;
  }
  else {
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}

void rbtree_insert_fix_up(rbtree *t, node_t *p) {
  while (p->parent->color == RBTREE_RED) {
    if (p->parent == p->parent->parent->left) {
      node_t *uncle = p->parent->parent->right;
      if (uncle->color == RBTREE_RED) {
        p->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;  // move pointer to grandparent
      }
      else {
        if (p == p->parent->right) {
          p = p->parent;  // move pointer to parent
          rbtree_left_rotate(t, p);
        }
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, p->parent->parent);
      }
    }
    else {
      node_t *uncle = p->parent->parent->left;
      if (uncle->color == RBTREE_RED) {
        p->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        p = p->parent->parent;  // move pointer to grandparent
      }
      else {
        if (p == p->parent->left) {
          p = p->parent;  // move pointer to parent
          rbtree_right_rotate(t, p);
        }
        p->parent->color = RBTREE_BLACK;
        p->parent->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, p->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_make_new_node(rbtree *t, const key_t key) {
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;
  new_node->left = t->nil;
  new_node->right = t->nil;

  return new_node;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = rbtree_make_new_node(t, key);

  node_t *p = t->root;  // p means current pointer
  if (p == t->nil) {  // if root is empty
    t->root = new_node;
  }
  else {
    node_t *tmp;
    char flag;
    while (p != t->nil) {
      tmp = p;
      if (key <= p->key) {
        p = p->left;
        flag = 'L';
      }
      else {
        p = p->right;
        flag = 'R';
      }
    }
    if (flag == 'L') {
      tmp->left = new_node;
    }
    else {
      tmp->right = new_node;
    }
    new_node->parent = tmp;
  }
  rbtree_insert_fix_up(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *p = t->root;
  while (p != t->nil) {
    if (key == p->key) {
      return p;
    }
    else if (key < p->key) {
      p = p->left;
    }
    else {
      p = p->right;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *p = t->root;
  if (p == t->nil) {
    return NULL;
  }
  else {
    while (p->left != t->nil) {
      p = p->left;
    }
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *p = t->root;
  if (p == t->nil) {
    return NULL;
  }
  else {
    while (p->right != t->nil) {
      p = p->right;
    }
  }
  return p;
}

void rbtree_transplant(rbtree *t, node_t *p, node_t *replace_p) {
  if (p->parent == t->nil) {
    t->root = replace_p;
  }
  else if (p == p->parent->left) {
    p->parent->left = replace_p;
  }
  else {
    p->parent->right = replace_p;
  }
  replace_p->parent = p->parent;
}

node_t *rbtree_subtree_min(rbtree *t, node_t *sub_root) {
  node_t *sub_min = sub_root;
  while (sub_min->left != t->nil) {
    sub_min = sub_min->left;
  }
  return sub_min;
}

void rbtree_erase_fix_up(rbtree *t, node_t *p) {
  while (p != t->root && p->color == RBTREE_BLACK) {
    if (p == p->parent->left) {
      node_t *sibling = p->parent->right;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, p->parent);
        sibling = p->parent->right;
      }
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        p = p->parent;
      }
      else {
        if (sibling->right->color == RBTREE_BLACK) {
        sibling->left->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        rbtree_right_rotate(t, sibling);
        sibling = p->parent->right;
        }
        sibling->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, p->parent);
        p = t->root;
      }
    }
    else {
      node_t *sibling = p->parent->left;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, p->parent);
        sibling = p->parent->left;
      }
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        p = p->parent;
      }
      else {
        if (sibling->left->color == RBTREE_BLACK) {
        sibling->right->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        rbtree_left_rotate(t, sibling);
        sibling = p->parent->left;
        }
        sibling->color = p->parent->color;
        p->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, p->parent);
        p = t->root;
      }
    }
  }
  p->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *erased) {
  // the name of variables means : 'node' that should be *erased, *modified, *replaced
  node_t *modified = erased;
  color_t modified_origin_color = modified->color;
  node_t *replaced;
  if (erased->left == t->nil) {
    replaced = erased->right;
    rbtree_transplant(t, erased, erased->right);
  }
  else if (erased->right == t->nil) {
    replaced = erased->left;
    rbtree_transplant(t, erased, erased->left);
  }
  else {
    modified = rbtree_subtree_min(t, erased->right);
    modified_origin_color = modified->color;
    replaced = modified->right;
    if (modified->parent == erased) {
      replaced->parent = modified;
    }
    else {
      rbtree_transplant(t, modified, modified->right);
      modified->right = erased->right;
      modified->right->parent = modified;
    }
    rbtree_transplant(t, erased, modified);
    modified->left = erased->left;
    modified->left->parent = modified;
    modified->color = erased->color;
  }
  if (modified_origin_color == RBTREE_BLACK) {
    rbtree_erase_fix_up(t, replaced);
  }
  free(erased);
  return 0;
}

void rbtree_to_array_mid_DFS(const rbtree *t, key_t *arr, int *i_pointer, node_t *p) {
  if (p == t->nil) {
    return;
  }
  else {
    rbtree_to_array_mid_DFS(t, arr, i_pointer, p->left);
    arr[*i_pointer] = p->key;
    *i_pointer += 1;
    rbtree_to_array_mid_DFS(t, arr, i_pointer, p->right);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *p = t->root;
  int i = 0;
  int *i_pointer = &i;
  rbtree_to_array_mid_DFS(t, arr, i_pointer, p);
  return 0;
}