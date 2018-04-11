// Xiaohong Zhu
// CS220
// Homework 2

#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256

typedef int object_t;
typedef struct tr_n_t {
                    int 		  leaves;
					struct tr_n_t  *left;
                    struct tr_n_t *right;
                    int           height; 
                      } tree_node_t;

tree_node_t *currentblock = NULL;
int    size_left;
tree_node_t *free_list = NULL;

tree_node_t *get_node()
{ tree_node_t *tmp;
  if( free_list != NULL )
  {  tmp = free_list;
     free_list = free_list -> left;
  }
  else
  {  if( currentblock == NULL || size_left == 0)
     {  currentblock = 
                (tree_node_t *) malloc( BLOCKSIZE * sizeof(tree_node_t) );
        size_left = BLOCKSIZE;
     }
     tmp = currentblock++;
     size_left -= 1;
  }
  return( tmp );
}

void return_node(tree_node_t *node)
{  node->left = free_list;
   free_list = node;
}

tree_node_t *create_tree(void)
{  tree_node_t *tmp_node;
   tmp_node = get_node();
   tmp_node->left = NULL;
   return( tmp_node );
}

void left_rotation(tree_node_t *n)
{  tree_node_t *tmp_node;
   tmp_node = n->left; 
   n->left  = n->right;        
   n->right = n->left->right;  
   n->left->right = n->left->left;
   n->left->left  = tmp_node;
   n->left->leaves = tmp_node->leaves + n->left->right->leaves;  // update leaves
}

void right_rotation(tree_node_t *n)
{  tree_node_t *tmp_node;
   tmp_node = n->right;
   n->right = n->left;        
   n->left  = n->right->left;  
   n->right->left = n->right->right;
   n->right->right = tmp_node;
   n->right->leaves = tmp_node->leaves + n->right->left->leaves;  // update leaves
}

object_t *find_by_number(tree_node_t *tree, int k) {
	tree_node_t *tmp_node;
	if (tree->leaves == 0)       // tree empty
		return(NULL);
	else if (tree->leaves < k)   // the k-th leaf doesn't exist
		return(NULL);
	else
	{
		tmp_node = tree;
		while (tmp_node->leaves != 1)
		{
			if (k <= tmp_node->left->leaves) 
				tmp_node = tmp_node->left;
			else 
			{
				k = k - tmp_node->left->leaves;
				tmp_node = tmp_node->right;
			}
		}
		return ( (object_t *) tmp_node->left );
	}
}

void insert_by_number(tree_node_t *tree, int k, object_t *new_obj)
{
	tree_node_t *tmp_node;
	int finished;
	if (tree->leaves == 0 && k == 1)   // insert node when tree empty
	{
		tree->left = (tree_node_t *) new_obj;
		tree->leaves = 1;
		tree->height = 0;
		tree->right = NULL;
	}   
	// cannot insert to the k-th leaf because (k-1)th leaf doesn't exist
	else if (tree->leaves < (k - 1))
		return;
	else
	{
		tree_node_t * path_stack[100]; int path_st_p = 0;
		tmp_node = tree;
		while (tmp_node->leaves != 1)
		{
			path_stack[path_st_p++] = tmp_node;
			tmp_node->leaves = tmp_node->leaves + 1;  // update the leaves field along the path
			if (k <= tmp_node->left->leaves) 
				tmp_node = tmp_node->left;
			else 
			{
				k = k - tmp_node->left->leaves;
				tmp_node = tmp_node->right;
			}
		}
		/* found the candidate leaf, now perform the insert*/
		tree_node_t *old_leaf, *new_leaf;
		old_leaf = get_node();
		old_leaf->left = tmp_node->left;
		old_leaf->leaves = 1;
		old_leaf->right = NULL;
		old_leaf->height = 0;
		new_leaf = get_node();
		new_leaf->left = (tree_node_t *) new_obj;
		new_leaf->leaves = 1;
		new_leaf->right = NULL;
		new_leaf->height = 0;
		if ( k > tmp_node->leaves )    // need to insert to the rightmost leaf
		{
			tmp_node->left = old_leaf;
			tmp_node->right = new_leaf;
		} else
		{
			tmp_node->left = new_leaf;
			tmp_node->right = old_leaf;
		}
		tmp_node->height = 1;
		tmp_node->leaves = 2;
		/* rebalance */
		finished = 0;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;
         if( tmp_node->left->height - 
                                 tmp_node->right->height == 2 )
         {  if( tmp_node->left->left->height - 
                                 tmp_node->right->height == 1 )
            {  right_rotation( tmp_node );
               tmp_node->right->height = 
                            tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
            {  left_rotation( tmp_node->left );
               right_rotation( tmp_node );
               tmp_height = tmp_node->left->left->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else if ( tmp_node->left->height - 
                                tmp_node->right->height == -2 )
         {  if( tmp_node->right->right->height - 
                                  tmp_node->left->height == 1 )
            {  left_rotation( tmp_node );
               tmp_node->left->height = 
                           tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  right_rotation( tmp_node->right );
               left_rotation( tmp_node );
               tmp_height = tmp_node->right->right->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else /* update height even if there was no rotation */ 
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }
         if( tmp_node->height == old_height )
            finished = 1;
	  }
	}	
}

object_t *delete_by_number(tree_node_t *tree, int k)
{  
	tree_node_t *tmp_node, *upper_node, *other_node;
	object_t *deleted_object; int finished;
	if( tree->leaves == 0 )           // cannot delete because tree empty
		return( NULL );
	else if ( tree->leaves < k )  // cannot delete the k-th leaf becasue it doesn't exist
		return( NULL );
   	else if( tree->leaves == 1 && k == 1 )  // delete the only node ( when k is 1 )
   	{                                         
		deleted_object = (object_t *) tree->left;
     	tree->left = NULL;
	 	tree->leaves = 0;
     	return( deleted_object );
   	}
   	else
   	{  
		tree_node_t * path_stack[100]; int path_st_p = 0;
		tmp_node = tree;
		while (tmp_node->leaves != 1)
		{
			path_stack[path_st_p++] = tmp_node;
			tmp_node->leaves = tmp_node->leaves - 1; // update the leaves field along the path
			upper_node = tmp_node;
			if (k <= tmp_node->left->leaves) 
			{	
				tmp_node = upper_node->left;
				other_node = upper_node->right;
			}
			else 
			{	
				k = k - upper_node->left->leaves;
				tmp_node   = upper_node->right;
				other_node = upper_node->left;
			}
		}
		upper_node->left  = other_node->left;
		upper_node->right = other_node->right;
		upper_node->height = other_node->height;
		upper_node->leaves = other_node->leaves;
		deleted_object = (object_t *) tmp_node->left;
		return_node( tmp_node );
		return_node( other_node );
      /*start rebalance*/  
      finished = 0; path_st_p -= 1;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;
         if( tmp_node->left->height - 
                                 tmp_node->right->height == 2 )
         {  if( tmp_node->left->left->height - 
                                 tmp_node->right->height == 1 )
	    {  right_rotation( tmp_node ); 
               tmp_node->right->height = 
                            tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
	    {  left_rotation( tmp_node->left ); 
               right_rotation( tmp_node );
               tmp_height = tmp_node->left->left->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else if ( tmp_node->left->height - 
                                tmp_node->right->height == -2 )
         {  if( tmp_node->right->right->height - 
                                  tmp_node->left->height == 1 )
	    {  left_rotation( tmp_node ); 
               tmp_node->left->height = 
                           tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  right_rotation( tmp_node->right );
               left_rotation( tmp_node );
               tmp_height = tmp_node->right->right->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else /* update height even if there was no rotation */ 
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }
         if( tmp_node->height == old_height )
	    finished = 1;
      }
      /*end rebalance*/
      return( deleted_object );
   }
}

int main()
{  tree_node_t *st1, *st2;
   long i, k; 
   int o[4] = {0,2,4,6};
   st1 = create_tree();
   st2 = create_tree();
   printf("Made two Trees\n");
   for( i=0; i < 100000; i++)
   {  insert_by_number( st1, 1, &(o[1]) ); 
      insert_by_number( st2, 1, &(o[2]) ); 
   }
   /*inserted 100000 pointers each, to 2 in st1 and to 4 in st2 */ 
   printf("Passed 200000 inserts.\n"); fflush(stdout);
   for( i=100000; i >0 ; i--)
   {  insert_by_number( st1, i+1, &(o[3]) );
      insert_by_number( st1, i+1, &(o[2]) );
   }
   /* now st1 is a sequence 246246...246 of length 300000 */
   for( i=0; i < 100000; i++)
   {  insert_by_number( st2, 1, &(o[1]) ); 
   }
   /* now st2 is a sequence of 100000 2s, followed by 1000000 4s */
   for( i=0; i < 300000; i++)
   {  k = *find_by_number(st1, i+1 );
      if( 2*((i%3)+1) != k )
      {  printf("Found wrong number %d (should be %d) in tree st1 at line %d\n",
                k, 2*((i%3)+1), i+1); fflush(stdout); exit(0);
      }
   }
   for( i=300000; i >0; i--)
   {  k = *find_by_number(st1, i );
      if( k==6 )
	delete_by_number(st1, i );
   }
   /* now st1 is sequence 24242424... of length 200000 */
   for( i=0; i < 200000; i++)
   {  k = *find_by_number(st1, i+1 );
      if( 2*((i%2)+1) != k )
      {  printf("Found wrong number %d (should be %d) in tree st1 at line %d\n",
                k, 2*((i%2)+1), i+1); fflush(stdout); exit(0);
      }
   }
   printf("Passed Test\n"); fflush(stdout);
}

