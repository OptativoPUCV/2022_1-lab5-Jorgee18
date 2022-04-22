#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
  Pair* pair;
  TreeNode * left;
  TreeNode * right;
  TreeNode * parent;
};

struct TreeMap {
  TreeNode * root;
  TreeNode * current;
  int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
  if(tree->lower_than(key1,key2)==0 &&  
      tree->lower_than(key2,key1)==0) return 1;
  else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
  TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL) return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
  TreeMap* variableAux;
  variableAux = (TreeMap*) malloc(sizeof(TreeMap));

  variableAux->root = NULL;
  variableAux->current = NULL;
  variableAux->lower_than = lower_than;

  //new->lower_than = lower_than;
  return variableAux;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  TreeNode* aux;
  TreeNode* variableAux2;
  aux = tree->root;

  while(aux != NULL)
  {
    tree->current = aux;
    if(tree->lower_than(key, aux->pair->key) == 1)
    {
      aux = aux->left;
    }
    else
    {
      if(tree->lower_than(aux->pair->key, key) == 1)
      {
        aux = aux->right;
      }
      else
      {
        tree->current = aux;
        return;
      }
    }
  }
  variableAux2 = createTreeNode(key, value);
  if(tree->lower_than(key, tree->current->pair->key) == 1)
  {
    tree->current->left = variableAux2;
    variableAux2->parent = tree->current;
  }
  else
  {
    tree->current->right = variableAux2;
    variableAux2->parent = tree->current;
  }
  tree->current = variableAux2;
}

TreeNode * minimum(TreeNode * x)
{
  TreeNode* aux;
  aux = x;
  if(x == NULL)
    return NULL;
  if(x->left == NULL)
    return aux;
  
  while(aux->left != NULL)
  {
    aux = aux->left;
  }
  return aux;

}

void removeNode(TreeMap * tree, TreeNode* node) 
{
  if(node->left == NULL && node->right == NULL)
  {
    /*if(tree->lower_than(node->parent->pair->key, node->pair->key) == 1)
    {
      node->parent->right = NULL;
    }
    else
    {
      node->parent->left = NULL;
    }
  }
  if(((node->left == NULL) && (node->right != NULL)) || ((node->left != NULL) && (node->right == NULL)) )
  {
    if((node->left == NULL) && (node->right != NULL))
    {
      if(tree->lower_than(node->parent->pair->key, node->pair->key) == 1)
      {
        node->parent->right = node->right;
        node->right->parent = node->parent;
      }
      else
      {
        node->parent->left = node->right;
        node->right->parent = node->parent;
      }
    }
    else
    {
      if(tree->lower_than(node->parent->pair->key, node->pair->key) == 1)
      {
        node->parent->right = node->left;
        node->left->parent = node->parent;
      }
      else
      {
        node->parent->left = node->left;
        node->left->parent = node->parent;
      } 
    }
  }
  /*else
  {
    TreeNode* aux;
    aux = minimum(node->right);
    node->pair = aux->pair;
    removeNode(tree, aux);
  }*/
}


void eraseTreeMap(TreeMap * tree, void* key){
  if (tree == NULL || tree->root == NULL) return;

  if (searchTreeMap(tree, key) == NULL) return;
  TreeNode* node = tree->current;
  removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  TreeNode* aux;
  aux = tree->root;

  while(aux != NULL)
  {
    tree->current = aux;
    if(tree->lower_than(key, aux->pair->key) == 1)
      aux = aux->left;
    else
    {
      if(tree->lower_than(aux->pair->key, key) == 1)
      {
        aux = aux->right;
      }
      else
      {
        tree->current = aux;
        return aux->pair;
      }
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
  TreeNode* aux;
  Pair* mayorCercano = NULL;
  
  aux = tree->root;

  while(aux != NULL)
  {
    tree->current = aux;
    if(tree->lower_than(key, aux->pair->key) == 1)
    {
      mayorCercano = aux->pair;
      aux = aux->left;
    }
    else
    {
      if(tree->lower_than(aux->pair->key, key) == 1)
      {
        aux = aux->right;
      }
      else
      {
        tree->current = aux;
        return aux->pair;
      }
    }
  }
  
  return mayorCercano;
}

Pair * firstTreeMap(TreeMap * tree) 
{
  tree->current = tree->root;
  if(tree->current != NULL)
  {
    tree->current = minimum(tree->current);
    return tree->current->pair;
  }

  return NULL;
}

Pair * nextTreeMap(TreeMap * tree) 
{
  if(tree->current->right != NULL)
  {
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }
  while(tree->current != tree->root)
  {
    if(tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) == 1)
    {
      tree->current = tree->current->parent;
      return tree->current->pair;
    }
    tree->current = tree->current->parent;
  }
  return NULL;
}