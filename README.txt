Cristian Villalobos CS460

The output of my code is formatted as such,

input such as:

int main void ( )
 { int x;
 }

 will output:

int	 main	void	(	)
{
int	 x	;
}

 everything that is on one line is a sibling, and if its on a newline
 the leftmost token is the child of the rightmost token above it,
 then the siblings of this new child are to the right of the new child.
 this repeats.



