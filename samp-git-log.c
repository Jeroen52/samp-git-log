#include <stdio.h>

/* samp-git-log - Parse git log into something that can be interpreted by SAMP.
 * Copyright (C) 2018  Jeroen de Neef
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "samp-git-log.h"

#include "lib/replace.c"

//#define DEBUG

#define MAX_LOG_LINES 22
#define MAX_LOG_LINE_LENGTH 178
#define MAX_LOG_LENGTH (MAX_LOG_LINES*MAX_LOG_LINE_LENGTH)
#define MAX_INCLUDE_LENGTH (262+MAX_LOG_LENGTH)

#define GIT_LOG_COMMAND "git log --no-merges --pretty=format:'\\n%<(8,trunc)%aN\\t%ad\\t%<(128,trunc)%s\\' --abbrev-commit HEAD^^^^^^^^^^^^..HEAD"

#ifdef __unix__

#elif defined(_WIN32) || defined(WIN32)

	#ifdef DEBUG
		#define OS_Windows
	#endif
	#define popen	_popen
	#define pclose	_pclose
#endif

char str[MAX_LOG_LENGTH];

int main()
{
	if(!IsCurrentlyInGitRepo())
		return 1;
	strcpy(str, "#if defined _INC_SAMP_GIT_LOG\n");
	strcat(str, "	#endinput\n");
	strcat(str, "#endif\n\n");
	strcat(str, "/*\n");
	strcat(str, " * samp-git-log 1.0 (https://github.com/Jeroen52/samp-git-log)\n");
	strcat(str, " * This file has been generated at ");
	AppendCurrentTimeToString(str);
	strcat(str, " */\n\n");
	strcat(str, "#define _INC_SAMP_GIT_LOG \"Committer\\tTimestamp\\tCommit message\\\n");
	AppendGitLogToString(str);
	str[(strlen(str)-1)] = '\0';
	strcat(str, "\"");
	store_data("samp-git-log.inc", str);
	#ifdef DEBUG
		printf("%d\n", strlen(str));
		#ifdef OS_Windows
			getchar();
		#endif
	#endif
}

int AppendGitLogToString(char *target)
{
	FILE *log = popen(GIT_LOG_COMMAND, "r");
	char buf[MAX_LOG_LENGTH];
	char *tempstr = malloc(MAX_LOG_LENGTH*sizeof(char));
	tempstr[0] = '\0'; // A weird bug causes a "P" to be the first char, this fixes that.
	
	while (fgets(buf, sizeof(buf), log) != 0) {
		strcat(tempstr, buf);
	}
	
	tempstr = replace(tempstr, "'", "`", NULL);
	tempstr = replace(tempstr, "\"", "`", NULL);
	
	strcat(target, tempstr);
	
	free(tempstr);
	
	pclose(log);
	return 0;
}

int IsCurrentlyInGitRepo()
{
	if(system(GIT_LOG_COMMAND))
		return 0;
	return 1;
}

int AppendCurrentTimeToString(char *target)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strcat(str, asctime (timeinfo));
	return 0;
}

int store_data(const char *filepath, const char *data)
{
	FILE *fp = fopen(filepath, "w");
	if (fp != NULL)
	{
		fputs(data, fp);
		fclose(fp);
		return 0;
	}
	return 0;
}
