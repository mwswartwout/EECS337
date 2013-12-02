/*******************************************************************************
*
* FILE:		mustang.c
*
* DESC:		EECS 337 Project Extra Credit
*
* AUTHOR:	caseid
*
* DATE:		December 5, 2013
*
* EDIT HISTORY:	
*
*	mustang tail lights are defined using 4 bits per side
*	PORTA RA5 input switch - right turn signal
*	PORTB RB0, RB1, RB2, RB3 output LEDs
*	0000 all off  
*	1000 right on 1
*	1100 right on 2
*	1110 right on 3
*	1111 right on 4
*******************************************************************************/
char	lights = 0;
delay()
{
	char	index;
	char	count;

	index = 255;
	while( index)
	{
		index = index - 1;
		count = 255;
		while( count)
			count = count - 1;
	}
}

clear()
{
	lights = 0x00;
	printf( "%d", lights);
	delay();
	return;
}

right()
{
	if( lights == 0x00)
	       lights = 0x08;
	else if( lights == 0x08)
	       lights = 0x0c;
	else if( lights == 0x0c)
	       lights = 0x0e;
	else if( lights == 0x0e)
	       lights = 0x0f;
	else // if( lights == 0x0f)
	       lights = 0x00;
	printf( "%d", lights);
	delay();
	return;
}

int	main()
{
	char	flags;
	while( 1)
	{
		if( flags & 0x10)
		    right();
		else 
		    clear();
	}
	return 0;
}
