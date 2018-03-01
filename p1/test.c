#include <stdio.h>
#include <stdlib.h>

struct listnode { struct listnode * next;
    			unsigned int key; };
struct listnode * sort( struct listnode * list) {
	struct listnode * tmp;
	unsigned int i, a;
	struct listnode * bucket_begin[65536];
	struct listnode * bucket_end[65536];
	for (i = 0; i<65536; i++) {
		bucket_begin[i]=NULL;
		bucket_end[i]=NULL;
	}
	tmp = list;
	while (list != NULL)
	{
		list=list->next;
		tmp->next=NULL;
		a = tmp->key;
		i = a&0xFFFF;
		if (bucket_begin[i] == NULL)
		{
			bucket_begin[i] = tmp;
			bucket_end[i] = tmp;
		} else {
			bucket_end[i]->next = tmp;
			bucket_end[i] = tmp;
		}
		tmp = list;
	}
	i = 0;
	list = bucket_begin[i];
	tmp = bucket_end[i];
	i++;
	while (i < 65536)
	{
		if (bucket_begin[i] != NULL)
		{
			tmp->next = bucket_begin[i];
			tmp = bucket_end[i];
		}
		i++;
	}
	// NEXT ROUND //
	for (i = 0; i<65536; i++) {
	bucket_begin[i]=NULL;
	bucket_end[i]=NULL;
	}	
	tmp = list;
	while (list != NULL)
	{
		list=list->next;
		tmp->next=NULL;
		a = tmp->key;
		i = (a>>16)&0xFFFF;
		if (bucket_begin[i] == NULL)
		{
			bucket_begin[i] = tmp;
			bucket_end[i] = tmp;
		} else {
			bucket_end[i]->next = tmp;
			bucket_end[i] = tmp;
		}
		tmp = list;
	}
	i=0;
	list = bucket_begin[i];
	tmp = bucket_end[i];
	i++;
	while (i < 65536)
	{
		if (bucket_begin[i] != NULL)
		{
			tmp->next = bucket_begin[i];
			tmp = bucket_end[i];
		}
		i++;
	}	
	return list;
}

int main(void)
{  
   unsigned int i; 
   struct listnode *list, *tmp;
   struct listnode space[10000000];
   for( i=0; i< 10000000; i++ )
      (space[i]).key  = 2*((139*i)%10000000);   
   for( i=0; i< 10000000 -1; i++ )
     (space[i]).next = &(space[i+1]);
   (space[10000000 -1]).next = NULL;
   printf(" Finished preparing list\n"); fflush(stdout); 
   list = &(space[0]);
	list = sort(list);

   for( tmp = list, i=0; tmp != NULL; tmp = tmp->next, i++)
	; 
   printf(" before sort: %d list items.\n", i); fflush( stdout); 

   for( tmp = list, i=0; tmp != NULL; tmp = tmp->next, i++)
     ;
   printf(" after  sort: %d list items.\n", i); fflush( stdout); 
   for( i=0; i< 10000000; i++ )
     {  if( list-> key != 2*i )
	 { printf("something wrong\n"); fflush(stdout); exit(-1);
	 }
       list = list-> next;
     }
   printf(" all sorted\n"); fflush(stdout);
}
  

