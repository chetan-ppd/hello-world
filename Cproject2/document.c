/*Hiteeesh Nukalapati, 116421157, hnuk*/ 
#include<stdio.h>
#include<string.h>
#include "document.h"


int init_document(Document *doc, const char *name) {
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }
    
    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);
    return SUCCESS;
}

int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int print_document(Document *doc) {
    int i = 0, j = 0;
    
    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: %c%s%c\n", '"', doc->name, '"');
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        if (doc->paragraphs[i].number_of_lines == 0) {
            continue;
        }

        for(j  = 0; j < doc->paragraphs[i].number_of_lines ; j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);   
	    }

        if (i == doc->number_of_paragraphs - 1) {
          break;
        }

      printf("\n");
    }
    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || 
        paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    doc->number_of_paragraphs++;
    doc->paragraphs[paragraph_number].number_of_lines = 0; 
    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
	               const char *new_line) {
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs || 
        doc->paragraphs[paragraph_number-1].number_of_lines == 
        MAX_PARAGRAPH_LINES ||
        line_number > doc->paragraphs[paragraph_number-1].number_of_lines) {
            return FAILURE;
    }

    doc->paragraphs[paragraph_number-1].number_of_lines++;
    strcpy((char *)doc->paragraphs[paragraph_number-1].lines[line_number], 
            new_line);
    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, 
                               int *number_of_lines) {
    if (doc == NULL || number_of_lines == NULL || 
        paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int line = doc->paragraphs[paragraph_number-1].number_of_lines ;
    
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs || 
        doc->paragraphs[paragraph_number].number_of_lines >= 
        MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }

    strcpy((char *)doc->paragraphs[paragraph_number-1].lines[line], new_line);
    doc->paragraphs[paragraph_number-1].number_of_lines++;
    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number){
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs || 
        line_number > doc->paragraphs[paragraph_number-1].number_of_lines) {
        return FAILURE;
    }

    
    
    if (line_number == doc->paragraphs[paragraph_number-1].number_of_lines) {
        doc->paragraphs[paragraph_number-1].lines[line_number-1][0] = '\0';
        doc->paragraphs[paragraph_number-1].number_of_lines--; 
    } else {
        int total_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
        int index = 0;
        
        for (index = line_number - 1; index < total_lines; index++) {
            if (index == total_lines - 1) {
                doc->paragraphs[paragraph_number-1].lines[index][0] = '\0';
                doc->paragraphs[paragraph_number-1].number_of_lines--;
                break;
            }

            strcpy(doc->paragraphs[paragraph_number-1].lines[index], 
                    doc->paragraphs[paragraph_number-1].lines[index+1]);
        }
    }
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
                  int data_lines) { 
	    int index = 0, paragraph_counter = 0, line_counter = 0; 
	    
        if (doc == NULL || data == NULL || data_lines == 0) { 
            return FAILURE; 
	    } 
 
	    for (index  = 0; index < data_lines; index++) { 
	        if (data[index][0] == '\0') {
	            paragraph_counter++; 
	            line_counter = 0; 
	            continue;/*goes to next ln, if current ln is empty*/ 
            } 
	        strcpy(doc->paragraphs[paragraph_counter].lines[line_counter++], 
                   data[index]); 
	        doc->paragraphs[paragraph_counter].number_of_lines =  line_counter; 
	    }

	    doc->number_of_paragraphs = paragraph_counter + 1; 
	    return SUCCESS;
} 

int replace_text(Document *doc, const char *target, const char *replacement) {
    int target_len = 0, replacement_len = 0, i = 0, j = 0, start = 0, end = 0;
    
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }
    
    target_len = strlen(target);
    replacement_len = strlen(replacement);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        int temp_start = 0;
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            while(1) {
                char *curr_str = doc->paragraphs[i].lines[j];
                char *temp_result = strstr(curr_str + temp_start, target);
                char temp_str[MAX_STR_SIZE] = {'\0'};
                int index = 0, size = 0, line_len = 0;
                
                if (temp_result == NULL) {
                    temp_start = 0;
                    break;
                }
                
                line_len = strlen(doc->paragraphs[i].lines[j]);
                start = temp_result - curr_str;
                end = start + target_len;
                
                for (index = 0; index < start; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }
                
                for (index = 0; index < replacement_len; index++) {
                    temp_str[size++] = replacement[index];
                }
                
                /*changing temp start index for this specific line here 
                  (used for checking recurring instances of taget)*/
                temp_start = size; 
                
                for (index = end; index < line_len; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }

                strcpy(doc->paragraphs[i].lines[j], (const char *)temp_str);
            }
        }
    }
    return SUCCESS;

}   

int highlight_text(Document *doc, const char *target) {
    int target_len = 0, i = 0, j = 0, start = 0, end = 0;
    
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    
    target_len = strlen(target);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        int temp_start = 0;
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            while(1) {
                char *curr_str = doc->paragraphs[i].lines[j];
                char *temp_result = strstr(curr_str + temp_start, target);
                char temp_str[MAX_STR_SIZE] = {'\0'};
                char temp_str2[MAX_STR_SIZE] = {'\0'};
                int line_len = 0, index = 0, size = 0;
                
                if (temp_result == NULL) {
                    temp_start = 0;
                    break;/*goes to the next line if target isnt in current ln*/
                }
                
                line_len = strlen(doc->paragraphs[i].lines[j]);
                start = temp_result - curr_str;
                end = start + target_len;
                
                for (index = 0; index < start; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }
                
                strcpy(temp_str, (const char *)strcat(temp_str, 
                                                      HIGHLIGHT_START_STR) );
                size++;
                
                strcpy(temp_str2, (const char *)strcat(temp_str, target));
                strcpy(temp_str, (const char *)strcat(temp_str2, 
                                                      HIGHLIGHT_END_STR));
                size += target_len+1;

                for (index = end; index < line_len; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }

                strcpy(doc->paragraphs[i].lines[j], (const char *)temp_str);
                
                /*changing temp start index for this specific line here 
                  (used for checking recurring instances of taget)*/
                temp_start = end+1;
            }    
        }
    }       
    return SUCCESS;
}

int remove_text(Document *doc, const char *target){
    int target_len = 0, i = 0, j = 0, start = 0, end = 0;
    
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    target_len = strlen(target);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        int temp_start = 0;
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            while(1) {
                char *curr_str = doc->paragraphs[i].lines[j];
                char *temp_result = strstr(curr_str + temp_start, target);
                char temp_str[MAX_STR_SIZE] = {'\0'};
                int index = 0, size = 0, line_len = 0;
                
                if (temp_result == NULL) {
                    temp_start = 0;
                    break;/*goes to the next line if target isnt in current ln*/
                }
                
                line_len = strlen(doc->paragraphs[i].lines[j]);
                start = temp_result - curr_str;
                end = start + target_len;
                
                for (index = 0; index < start; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }
                
                for (index = end; index < line_len; index++) {
                    temp_str[size++] = doc->paragraphs[i].lines[j][index];
                }
                
                strcpy(doc->paragraphs[i].lines[j], (const char *)temp_str);
                
                /*changing temp start index for this specific line here 
                  (used for checking recurring instances of taget)*/
                temp_start = end;
            }
        }
    }
    return SUCCESS;
} 