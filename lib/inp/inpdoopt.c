/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

    /* INPdoOpts(ckt,option card)
     *  parse the options off of the given option card and add them to
     *  the given circuit 
     */

#include "spice.h"
#include <stdio.h>
#include "strext.h"
#include "inpdefs.h"
#include "ifsim.h"
#include "util.h"
#include "cpdefs.h"
#include "fteext.h"
#include "suffix.h"




/*
	Used to process .option cards.
	The analysis anal is assumed to e an options analysis already created, and any legal parameters found after the first
	token of the card image optCard are added to that options analysis.
	The first token on the card image is assumed to be the word ".options" or one of its legal synonyms
	and is ignored.
*/
///<param name = "*ckt"> ircuit being parsed </param>
///<param name = "*anal"> The options analysis to add to </param>
///<param name = "*optCard"> The card image to get data from </param>
///<param name = "*tab"> The parse tables for this circuit </param>
void INPdoOpts(GENERIC *ckt, GENERIC *anal, card *optCard, INPtables *tab)
{
	char *line;
	char *token;
	char *errmsg;
	IFvalue *val = NULL;
	int error;
	int i;
	int which;
	IFanalysis *prm = NULL;

	which = -1;
	i = 0;

	for (i = 0; i < ft_sim->numAnalyses; i++)
	{
		prm = ft_sim->analyses[i];
		if (strcmp(prm->name, "options") == 0)
		{
			which = i;
			break;
		}

		i++;
	}

	if (which == -1)
	{
		optCard->error = INPerrCat(optCard->error, INPmkTemp("errr:  analysis options table not found\n"));
		return;
	}

	line = optCard->line;
	INPgetTok(&line, &token, 1);    /* throw away '.option' */
	while (*line)
	{
		INPgetTok(&line, &token, 1);

		for (i = 0; i < prm->numParms; i++)
		{
			if (strcmp(token, prm->analysisParms[i].keyword) == 0)
			{
				if (!(prm->analysisParms[i].dataType & IF_UNIMP_MASK))
				{
					errmsg = (char *)MALLOC((45 + strlen(token)) * sizeof(char));
					(void)sprintf(errmsg, " Warning: %s not yet implemented - ignored \n", token);
					optCard->error = INPerrCat(optCard->error, errmsg);
					val = INPgetValue(ckt, &line, prm->analysisParms[i].dataType, tab);
					break;
				}

				if (prm->analysisParms[i].dataType & IF_SET)
				{
					val = INPgetValue(ckt, &line, prm->analysisParms[i].dataType&IF_VARTYPES, tab);
					error = (*(ft_sim->setAnalysisParm))(ckt, anal, prm->analysisParms[i].id, val, (IFvalue*)NULL);
					if (error)
					{
						errmsg = (char *)MALLOC((35 + strlen(token))*sizeof(char));
						(void)sprintf(errmsg, "Warning:  can't set optione %s\n", token);
						optCard->error = INPerrCat(optCard->error, errmsg);
					}
					break;
				}
			}
		}
		if (i == prm->numParms)
		{
			errmsg = (char *)MALLOC(100 * sizeof(char));
			(void)strcpy(errmsg, " Error: unknown option - ignored\n");
			optCard->error = INPerrCat(optCard->error, errmsg);
		}
	}
}
