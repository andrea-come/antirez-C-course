/*
 * Implementation of the `tac` command
 * Prints lines of a text file in reverse order 
 * Uses dynamic buffer allocation to handle arbitrarily long lines
 */


#include <stdio.h>
#include <stdlib.h>

#define INPUT_ERROR 1
#define MEM_ERROR 2


int main(int argc, char **argv)
{
	if (argc != 2) {
  	puts("Missing file name");
	 	return INPUT_ERROR;
	}
    
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
  	puts("File not found");
  	return INPUT_ERROR;
	}
    
	// Get file length
	int c, flen = 0;
	while ((c = fgetc(fp)) != EOF) {
  	flen++;
	}
    
	// Handle empty file
	if (flen == 0) {
  	fclose(fp);
  	return 0;
	}
	
	int line_len = 256; 
	char *buf = malloc(line_len);
	if( buf == NULL) {
		printf("%s\n", "Not enough memory to process line");
		fclose(fp);
		return MEM_ERROR;
	}
	int line_end = 0;    // stores end position of current line
  
	for (int pos = flen - 1; pos >= 0; pos--) {
	  fseek(fp, pos, SEEK_SET);
		c = fgetc(fp);
		buf[line_end] = c;
		line_end++;
        
		if (c == '\n' || pos == 0) {  // Handle newline or start of file
			// Print `buf` in reverse order (excluding the newline if present)
			int start_idx = (c == '\n') ? line_end - 2 : line_end - 1;
			for (int j = start_idx; j >= 0; j--) {
				printf("%c", buf[j]);
			}
			printf("\n");  
            
			// Reset line_end for next line
			line_end = 0;
		}
		// Buffer cleaning is not necessary since we print only up to `start_idx`     
   
		// Check for buffer overflow
		if (line_end >= line_len) {
			line_len *= 2;
			char *new_buf = realloc(buf, line_len);
			if( new_buf == NULL){
				printf("%s\n", "Not enough memory to process line");
				free(buf);				
				fclose(fp);				
				return MEM_ERROR;
			}
			buf = new_buf;
		}
	}
    
	fclose(fp);
	return 0;
}
