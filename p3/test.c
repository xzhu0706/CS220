// Xiaohong Zhu
// CS220 HW#3

#include <stdlib.h>
#include <stdio.h>

struct edge_list {int u; int v; struct edge_list * next;};

struct edge_list * Dijkstra(int n, int *graph, int start)
{
    typedef enum {unknown, active, finished} State;
    State state[n];
    int distance[n];
	int prev[n];
    for (int i = 0; i < n; i++)
        distance[i] = 100;
    for (int i = 0; i < n; i++)
        state[i] = unknown;
    int count = 0;
	int edge_length, v, tmp_v, tmp_dist;
    state[start] = active;
    distance[start] = 0;
	v = start;             // select start vertex
	while (count < n)
	{
        state[v] = finished;
        for (int j = 0; j < n; j++)
        {
            edge_length = *(graph +n*v +j);
            if ( edge_length < 100 && state[j] != finished )
            {
                if (state[j] == unknown)
                    state[j] = active;
                if (distance[j] > (distance[v] + edge_length))
				{
                    distance[j] = distance[v] + edge_length;
					prev[j] = v;
				}
            }
        }
		/* find the next vertex to be selected from the active vertices */
		for (int i = 0; i < n; i++)
		{
			if (state[i] == active)
			{
				tmp_dist = distance[i];
				tmp_v = i;
				break;
			}
		}
		for (int i = tmp_v; i < n; i++)
		{
			if (state[i] == active && distance[i] < tmp_dist)
			{
				tmp_dist = distance[i];
				tmp_v = i;
			}
		}
		v = tmp_v;
        count++;
    }
	/* find the most distant vertex */
	int max_dist = distance[0];
	int farthest;
	for (int i = 0; i < n; i++)
	{
		if ( distance[i] > max_dist )
		{
			farthest = i;
			max_dist = distance[i];
		}
	}
	/* make the edge_list*/
	struct edge_list *result, *last, *tmp;
    last    = (struct edge_list *) malloc(sizeof(struct edge_list));
    last->v = farthest;
    last->u = prev[farthest];
    last->next = NULL;
	result = last;
	while( result->u != start )
	{
		struct edge_list *tmp = (struct edge_list *) malloc(sizeof(struct edge_list));
		tmp->v    = result->u;
		tmp->u    = prev[result->u];
		tmp->next = result;
		result = tmp;
	}
	return result;
}

int main()
{ 
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
        *(graph+1000*i+j) = 21;
        *(graph+1000*j+i) = 21;
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
   if( dist != 57 )
     {  printf("Length of path is %d, should be 57.\n", dist); fflush(stdout);
      exit(-1);
   }
   printf("Passed Test\n"); fflush(stdout);
}


