/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/*
 */

    /* INPlist(file,deck,type)
     *  provide an input listing on the specified file of the given
     *  card deck.  The listing should be of either PHYSICAL or LOGICAL
     *  lines as specified by the type parameter.
     */

#include "spice.h"
#include <stdio.h>
#include "inpdefs.h"
#include "util.h"
#include "suffix.h"



/*
	Obsolete function used to produce a listing of an input deck, either printing all of
	the logical lines produced by stripping out the continuation cards and stringing the lines out to their
	full length, or a physical listing of the cards as they were inputs.
	This routine has been replaced by more general facilities inside of nutmeg
*/
///<param name = "*file"> The file to print the listing on </param>
///<param name = "*deck"> The input deck to list </param>
///<param name = "type"> The type of listing - logical or physical </param>
void INPlist(FILE *file, card *deck, int type)
{

	card *here;
	card *there;

	if (type == LOGICAL)
	{
		for (here = deck; here != NULL; here = here->nextcard)
		{
			fprintf(file, "%6d : %s\n", here->linenum, here->line);
			if (here->error != (char *)NULL)
			{
				fprintf(file, "%s", here->error);
			}
		}
	}
	else if (type == PHYSICAL)
	{
		for (here = deck; here != NULL; here = here->nextcard)
		{
			if (here->actualLine == (card *)NULL)
			{
				fprintf(file, "%6d : %s\n", here->linenum, here->line);

				if (here->error != (char *)NULL)
				{
					fprintf(file, "%s", here->error);
				}
			}
			else
			{
				for (there = here->actualLine; there != NULL; there = there->nextcard)
				{
					fprintf(file, "%6d : %s\n", there->linenum, there->line);
					if (there->error != (char *)NULL)
					{
						fprintf(file, "%s", there->error);
					}
				}
			}
		}
	}
}
                    
                
