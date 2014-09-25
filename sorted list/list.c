#include	<stdio.h>
#include	<stdlib.h>
#include	"malloc.h"

// Construct a singly-linked list.

struct Node {
	int		value;
	struct Node *	next;
};

void forever(){
	int	a[10],i;

	for ( i = 0 ; i <= 11 ; i++ )
	{
		a[i] = 0;
//		printf( "BKR i is %d %s line %i\n", i, __FILE__, __LINE__ );
	}
	printf( "return from forever()\n" );		// Yeah, right.
}

// shared data:
struct Node *	head = 0;
struct Node *	tail = 0;

int main(){
	int		x;
	struct Node *	p;
//	struct Node *	head = 0;
//	struct Node *	tail = 0;
	struct Node *	next;

	forever();
	while ( printf( "enter an integer->" ), scanf( " %d", &x ) > 0 ){
		p = (struct Node *)malloc( sizeof( struct Node ) );
		p->value = x;
		p->next = 0;
		if ( head == 0 ){	/* is first? */
			head = p;
			tail = p;
		}
		else {
			tail->next = p;
			tail = p;
		}
	}
	printf( "\nNow the output:\n" );
	for ( p = head ; p != 0 ; p = p->next ){
		x = p->value;
		printf( "%d\n", x );
	}
	for ( p = head ; p != 0 ; p = next ){	/* free everything you take */
		next = p->next;
		free( p );
	}
	return 0;
}