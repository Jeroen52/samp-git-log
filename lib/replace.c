/* ---------------------------------------------------------------------------
  Name       : replace - Search & replace a substring by another one. 
  Creation   : Thierry Husson, Sept 2010
  Parameters :
      str    : Big string where we search
      oldstr : Substring we are looking for
      newstr : Substring we want to replace with
      count  : Optional pointer to int (input / output value). NULL to ignore.  
               Input:  Maximum replacements to be done. NULL or < 1 to do all.
               Output: Number of replacements done or -1 if not enough memory.
  Returns    : Pointer to the new string or NULL if error.
  Notes      : 
     - Case sensitive - Otherwise, replace functions "strstr" by "strcasestr"
     - Always allocate memory for the result.
--------------------------------------------------------------------------- */
char* replace(const char *str, const char *oldstr, const char *newstr, int *count)
{
   const char *tmp = str;
   char *result;
   int   found = 0;
   int   length, reslen;
   int   oldlen = strlen(oldstr);
   int   newlen = strlen(newstr);
   int   limit = (count != NULL && *count > 0) ? *count : -1; 

   tmp = str;
   while ((tmp = strstr(tmp, oldstr)) != NULL && found != limit)
      found++, tmp += oldlen;

   length = strlen(str) + found * (newlen - oldlen);
   if ( (result = (char *)malloc(length+1)) == NULL) {
      fprintf(stderr, "Not enough memory\n");
      found = -1;
   } else {
      tmp = str;
      limit = found; /* Countdown */
      reslen = 0; /* length of current result */ 
      /* Replace each old string found with new string  */
      while ((limit-- > 0) && (tmp = strstr(tmp, oldstr)) != NULL) {
         length = (tmp - str); /* Number of chars to keep intouched */
         strncpy(result + reslen, str, length); /* Original part keeped */ 
         strcpy(result + (reslen += length), newstr); /* Insert new string */
         reslen += newlen;
         tmp += oldlen;
         str = tmp;
      }
      strcpy(result + reslen, str); /* Copies last part and ending nul char */
   }
   if (count != NULL) *count = found;
   return result;
}
