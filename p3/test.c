// Xiaohong Zhu
// CS220 HW#3

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int key_t; 
typedef int object_t; 
typedef struct {key_t key; object_t *object;} heap_el_t;
typedef struct {int    max_size; 
                int    current_size; 
                heap_el_t  *heap;    } heap_t;

struct edge_list {int u; int v; struct edge_list * next;};

typedef int item_t;

typedef struct {item_t *base; item_t *top; int size;} stack_t;

stack_t *create_stack(int size)
{   stack_t *st;
    st = (stack_t *) malloc( sizeof(stack_t) );
    st->base = (item_t *) malloc( size * sizeof(item_t) );
    st->size = size;
    st->top = st->base;
    return( st );
}

int stack_empty(stack_t *st)
{   return( st->base == st->top );
}

int push( item_t x, stack_t *st)
{   if ( st->top < st->base + st->size )
    {   *(st->top) = x; st->top += 1;  return( 0 );
    }
    else
       return( -1 );
}

item_t pop(stack_t *st)
{   st->top -= 1;
    return( *(st->top) );
}

item_t top_element(stack_t *st)
{   return( *(st->top -1) );
}

void remove_stack(stack_t *st)
{   free( st->base ); 
    free( st );
}

heap_t *create_heap(int size)
{   heap_t *hp;
    hp = (heap_t *) malloc( sizeof(heap_t) );
    hp->heap = (heap_el_t *) malloc( size * sizeof(heap_el_t) );
    hp->max_size = size;
    hp->current_size = 0;
    return( hp );
}

int heap_empty(heap_t *hp)
{   return( hp->current_size == 0 );
}

heap_el_t *find_min(heap_t *hp)
{     return( hp->heap );
}

int insert( key_t new_key, object_t *new_object, heap_t *hp)
{   if ( hp->current_size < hp->max_size )
    {  int gap;
       gap = hp->current_size++;
       while(gap > 0 && new_key < (hp->heap[(gap-1)/2]).key )
       { (hp->heap[gap]).key    = (hp->heap[(gap-1)/2]).key;
         (hp->heap[gap]).object = (hp->heap[(gap-1)/2]).object;
         gap = (gap-1)/2;
       }
       (hp->heap[gap]).key    = new_key;
       (hp->heap[gap]).object = new_object;
       return( 0 );
    }
    else
       return( -1 ); /* Heap overflow */
}

object_t *delete_min(heap_t *hp)
{  object_t *del_obj;
   int reached_top = 0;
   int gap, newgap, last;
   if( hp->current_size == 0 )
     return( NULL ); /*delete from empty heap, failed */
   del_obj = (hp->heap[0]).object;
   gap = 0;
   while( ! reached_top )
   {  if( 2*gap + 2 < hp->current_size )
      { if( (hp->heap[2*gap+1]).key < (hp->heap[2*gap+2]).key)
          newgap = 2*gap + 1;
        else
          newgap = 2*gap + 2;
        (hp->heap[gap]).key    = (hp->heap[newgap]).key;
        (hp->heap[gap]).object = (hp->heap[newgap]).object;
        gap = newgap;
      }
      else if ( 2*gap + 2 == hp->current_size )
      {  newgap = 2*gap + 1;
         (hp->heap[gap]).key    = (hp->heap[newgap]).key;
         (hp->heap[gap]).object = (hp->heap[newgap]).object;
         hp->current_size -= 1;
         return(del_obj); 
         /* finished, came out exactly on last element */
      }
      else
          reached_top = 1;
   }
   /* propagated gap to the top, now move gap down again */
   /* to insert last object in the right place */
   last = --hp->current_size;
   while(gap > 0 && 
           (hp->heap[last]).key < (hp->heap[(gap-1)/2]).key )
   { (hp->heap[gap]).key    = (hp->heap[(gap-1)/2]).key;
     (hp->heap[gap]).object = (hp->heap[(gap-1)/2]).object;
     gap = (gap-1)/2;
   }
   (hp->heap[gap]).key    = (hp->heap[last]).key;
   (hp->heap[gap]).object = (hp->heap[last]).object;
   /* filled gap by moving last element in it*/
   return( del_obj );
}

void remove_heap(heap_t *hp)
{   free( hp->heap ); 
    free( hp );
}

#if 1
struct edge_list * Dijkstra(int n, int *graph, int start)
{
#if 0
	/* Find the most distant vertex using BFS and an array that records 
	 * the length of path to each vertex. */
	queue_t *queue = create_queue(n);
	enqueue(start, queue);
	bool visited[n];
	int length[n];
	int x, tmp, neighbors, most_distant, max_length = 0;
	for (int i = 0; i < n; i++)
	{
		length[n] = 0;
	}
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	while ( !(queue_empty(queue)) )
	{
		x = dequeue(queue);
		if ( !(visited[x]) )
		{		
			visited[x] = true;
			neighbors = 0;
			for (int j = 0; j < n; j++)
			{
				if ( j != x )
				{
					tmp = *(graph +n*x +j);
					if (tmp < 100)
					{	
						if ( !(visited[j]) )  
						{
							length[j] += tmp;
							enqueue(j, queue);
							neighbors++;
						}
					}
					if (neighbors == 0)  // reached a possible most-distant vertex
					{
						if ( length[j] > max_length )
						{
							max_length = length[j];
							most_distant = j;
						}
					}
				}
			}
		}
	}
#endif	
	
//	int[n] distance;
	typedef enum {unknown, active, finished} State;
	State state[n];
	for (int i = 0; i < n; i++)
	   state[i] = unknown;
//	for (int i = 0; i < n; i++)
//	   distance[i] = INFINITY;
	stack_t *visited = create_stack(n);
	heap_t *active_v = create_heap(n);
	int finished_v = 0;
//	distance[start] = 0;
	state[start] = active;
	insert(start, 0, active_v);
	int min_dist, edge_length, v;
	heap_el_t* current;
	while (finished_v < n)
	{
		current = find_min(active_v);
		v = current->key;
		delete_min(active_v);
		push(v, visited);
		state[current->key] = finished;
		int no_good = 0;
		for (int j = 0; j < n; j++)
		{
			edge_length = *(graph +n*v +j);
			if ( edge_length < 100 && state[j] != finished )
			{
				if (state[j] == unknown)
				{
					min_dist = edge_length;
					state[j] = active;
					int *insobj = (object_t *) malloc(sizeof(object_t));
					*insobj = min_dist;
					insert(j, insobj, active_v);
				} else
				{
					if ( find_min(active_v)->object < (current->object + edge_length))
						find_min(active_v)->object = current->object + edge_length;
					else
						no_good++;
				}
			}
		}
		if (no_good > 0 && finished_v != (n-1) )
			pop(visited);
		finished_v++;
	}
	struct edge_list *result, *last, *tmp;
	result = (edge_list *) malloc(sizeof(edge_list));
	last   = (edge_list *) malloc(sizeof(edge_list));
	last->v = pop(visited);
	last->u = pop(visited);
	last->next = NULL;
	tmp = last;
	while (!stack_empty(visited))
	{
		result->v    = tmp->u;
		result->u    = pop(visited);
		result->next = tmp;
		tmp = result;
	}
	return result;
}
#endif

int main()
{  
#if 0
	heap_t *heap;
   char nextop;
   heap = create_heap(1000);
   printf("Made Heap\n");
   while( (nextop = getchar())!= 'q' )
   { if( nextop == 'i' )
     { int inskey,  *insobj, success;
       insobj = (object_t *) malloc(sizeof(object_t));
       scanf(" %d,%d", &inskey, insobj);
       success = insert( inskey, insobj, heap );
       if ( success == 0 )
         printf("  insert successful, key = %d, object value = %d, \
current heap size is %d\n",
        	  inskey, *insobj, heap->current_size );
       else
           printf("  insert failed, success = %d\n", success);
     }  
     if( nextop == 'd' )
     { object_t *delobj; 
       getchar();
       delobj = delete_min( heap);
       if( delobj == NULL )
         printf("  delete failed\n");
       else
         printf("  delete successful, deleted object %d\n", 
             *delobj);
     }
   }
#endif
#if 0
   int a[3][3] = {{1, 2, 3},
	   			  {4, 5, 6},
				  {7, 8, 9}};
   for (int i = 0; i < 3; i++)
   {
	   for (int j = 0; j < 3; j++)
	   {
			printf("%d ", **(a +9*i +j));
	   }
	   printf("\n");
   }
#endif
//  list_node_t *path;
  long i,j,k,l, dist, length; 
  int *graph;
  struct edge_list * result, *tmp;
   graph = (int *) malloc( 1000000*sizeof(int) );
   for(i=0; i<1000; i++)
     for(j=0; j<1000;j++)
       *(graph+1000*i+j) = 100;
   /* now create 30 by 30 grid */
   for(k=0; k<29; k++)
     for(l=0; l<30;l++)
     {  i = 30*k + l;
        j = 30*(k+1 ) + l;
        *(graph+1000*i+j) = 1;
        *(graph+1000*j+i) = 1;
     }
   for(k=0; k<30; k++)
     for(l=0; l<29;l++)
     {  i = 30*k + l;
        j = 30*k + l+1;
        *(graph+1000*i+j) = 1;
        *(graph+1000*j+i) = 1;
     }
   for(k=0; k<10; k++)
     for(l=0; l<10;l++)
     {  i = 900+10*k + l;
        j = 30*3*k + 3*l;
        *(graph+1000*i+j) = 18;
        *(graph+1000*j+i) = 18;
     }
   for(k=0; k<10; k++)
     for(l=0; l<9;l++)
     {  i = 900+10*k + l;
        j = 900+10*k + l +1;
        *(graph+1000*i+j) = 2;
        *(graph+1000*j+i) = 2;
     }
   for(k=0; k<9; k++)
     for(l=0; l<10;l++)
     {  i = 900+10*k + l;
       j = 900+10*(k+1) + l;
        *(graph+1000*i+j) = 2;
        *(graph+1000*j+i) = 2;
     }
   result = Dijkstra(1000, graph, 31);
   dist = 0;
   if( result-> v != 31 && result-> u != 31 )
   {  printf("Wrong startvertex; should be 31, but first edges goes from %d to %d.\n", result->v, result->u ); fflush(stdout);
        exit(-1);
   }
   tmp = result;
   printf("Path is: ");
   while( tmp != NULL )
   {  printf("%d-%d, ", tmp->u, tmp->v);
      tmp = tmp-> next;
   }
   printf("\n"); fflush(stdout);
   dist = *(graph + 1000*(result->v) + (result->u));
   while( result->next != NULL)
   {  result = result-> next;
      length = *(graph + 1000*(result->v) + (result->u));
      if( length >= 100 )
      {  printf("Path uses non-edge: no edge between %d and %d\n", 
                  result->u, result->v ); fflush(stdout);
	 exit(-1);
      }
      dist += length;
   }
   if( length != 54 )
   {  printf("Length of path is %d, should be 54.\n"); fflush(stdout);
      exit(-1);
   }
   printf("Passed Test\n"); fflush(stdout);

   return(0);
}

