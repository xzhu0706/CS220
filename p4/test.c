// Xiaohong Zhu
// CS220 HW#4
// Reference: I used the queue sample code from: 
// 			  www-cs.ccny.cuny.edu/~peter/dstest/ar_queue.c
#include <stdio.h>
#include <stdlib.h>

typedef int item_t;

typedef struct {item_t *base; 
                int    front; 
                int     rear; 
                int     size;} queue_t;

queue_t *create_queue(int size)
{   queue_t *qu;
    qu = (queue_t *) malloc( sizeof(queue_t) );
    qu->base = (item_t *) malloc( size * sizeof(item_t) );
    qu->size = size;
    qu->front = qu->rear = 0;
    return( qu );
}

int queue_empty(queue_t *qu)
{   return( qu->front == qu->rear );
}

int enqueue( item_t x, queue_t *qu)
{   if ( qu->front != ((qu->rear +2)% qu->size) )
    {   qu->base[qu->rear] = x; 
        qu->rear = ((qu->rear+1)%qu->size);  
        return( 0 );
    }
    else
       return( -1 );
}

item_t dequeue(queue_t *qu)
{   int tmp;
    tmp = qu->front;
    qu->front = ((qu->front +1)%qu->size);
    return( qu->base[tmp] );
}

item_t front_element(queue_t *qu)
{   return( qu->base[qu->front] );
}

void remove_queue(queue_t *qu)
{   free( qu->base ); 
    free( qu );
}

void maximum_flow(int n, int s, int t, int *capacity, int *flow)
{
	int *res_cap = (int *)malloc(sizeof(int) * n * n);
	int *current_flow = (int *)malloc(sizeof(int) * n * n);
	int *prev = (int *)malloc(sizeof(int) * n);
	/* Initialize residual capacity and current flow */
	for (int i = 0; i < n; i++)
	{	
		for (int j = 0; j < n; j++)
		{
			*(res_cap + i*n +j) = *(capacity + i*n +j);
			*(current_flow + i*n +j) = 0;
		}
	}
	/* Search for augmenting path using BFS */
	bool no_path = false;
	while (!no_path)
	{
		bool visited[1000];
		int x, y;
		for (int i = 0; i < n; i++)
			visited[i] = false;
		queue_t *q = create_queue(n);
		enqueue(s, q);
		prev[s] = -1;
		while( !queue_empty(q) )
		{
			x = dequeue(q);
			visited[x] = true;
			for (y = 0; y < n; y++)
			{
				if ( !visited[y] && *(res_cap + x*n +y) > 0 )
				{
					enqueue(y, q);
					prev[y] = x;
					visited[y] = true;
				}
			}
		}
		remove_queue(q);
		if ( !visited[t] )
		{	
			no_path = true;    // If no path from s to t is found, exit the loop.
			break;
		}
		/* find the minimum flow along the augmenting path */
		int min_flow = *(res_cap +prev[t]*n +t);
		int tmp = prev[t];
		while ( tmp != s )
		{
			if ( *(res_cap +prev[tmp]*n +tmp) < min_flow)
				min_flow = *(res_cap +prev[tmp]*n +tmp);
			tmp = prev[tmp];
		}
		/* update the residual capacity and current flow */
		tmp = t;
		while ( tmp != s )
		{
			*(current_flow +prev[tmp]*n +tmp) += min_flow;
			*(res_cap +tmp*n + prev[tmp]) += min_flow;
			*(res_cap +prev[tmp]*n +tmp) -= min_flow;
			tmp = prev[tmp];
		}
	}
	/* No more path is found, copy the current flow to the flow array */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			*(flow + i*n + j) = *(current_flow +i*n +j);
	free(res_cap);
	free(prev);
	free(current_flow);
}

int main(void)
{  int cap[1000][1000], flow[1000][1000];
   int i,j, flowsum;
   for(i=0; i< 1000; i++)
     for( j =0; j< 1000; j++ )
       cap[i][j] = 0;
     
   for(i=0; i<499; i++)
     for( j=i+1; j<500; j++) 
       cap[i][j] = 2;
   for(i=1; i<500; i++)
     cap[i][500 + (i/2)] =4;
   for(i=500; i < 750; i++ )
   { cap[i][i-250]=3;
     cap[i][750] = 1;
     cap[i][751] = 1;
     cap[i][752] = 5;
   }
   cap[751][753] = 5;
   cap[752][753] = 5;
   cap[753][750] = 20;
   for( i=754; i< 999; i++)
   {  cap[753][i]=1;
      cap[i][500]=3;
      cap[i][498]=5;
      cap[i][1] = 100;
   }
   cap[900][999] = 1;
   cap[910][999] = 1;
   cap[920][999] = 1;
   cap[930][999] = 1;
   cap[940][999] = 1;
   cap[950][999] = 1;
   cap[960][999] = 1;
   cap[970][999] = 1;
   cap[980][999] = 1;
   cap[990][999] = 1;
   printf("prepared capacity matrix, now executing maxflow code\n");
   maximum_flow(1000,0,999,&(cap[0][0]),&(flow[0][0]));
   for(i=0; i<=999; i++)
     for(j=0; j<=999; j++)
     {  if( flow[i][j] > cap[i][j] )
        {  printf("Capacity violated\n"); exit(0);}
     }    
   flowsum = 0;
   for(i=0; i<=999; i++)
     flowsum += flow[0][i];
   printf("Outflow of  0 is %d, should be 10\n", flowsum);
   flowsum = 0;
   for(i=0; i<=999; i++)
     flowsum += flow[i][999];
   printf("Inflow of 999 is %d, should be 10\n", flowsum);

   printf("End Test\n");
}
