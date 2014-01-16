#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
 
void die(const char *message){
        if(errno){
                perror(message);
        }else{
                printf("ERROR: %s\n",message);
        }
        exit(1);
}
 
FILE *get_file(char *filename){
        FILE *file = fopen(filename,"r");
        if(file){
                return file;
        }else{
                die("Not a valid filename.");
        }
        return file;
}
 
void close_file(FILE *f, FILE *g){
        int err = fclose(f);
        if(err!=0){
                die("Error closing readfile.");
        }
        err=fclose(g);
        if(err!=0){
                die("Error closing output");
        }
}
 
void read_file(FILE *f, FILE *outf){
        //fprintf(outf,"#include <stdio.h>\nint main(int argc, char *argv[]){\nputs(\"Hello World\");\nreturn 0;\n}"); 
        fprintf(outf,"#include <stdio.h>\nint main(int argc, char *argv[]){\n");
        char *str="";
        size_t len =0;
        fprintf(outf,"char array[100]={0};\nchar *ptr=array;\n");
        while(getline(&str,&len,f)>0){
                int i =0;
                int c = 1;
                int d = 1;
                while(str[i]!='\n'){
                        switch(str[i]){
                                case '>':
                                        fprintf(outf,"ptr++;\n");
                                        break;
                                case '<':
                                        fprintf(outf,"ptr--;\n");
                                        break;
                                case '+':
                                        if(str[i+1]!='+'){
                                                fprintf(outf,"(*ptr)+=%d;\n",c);
                                        }else{
                                                c++;
                                        }
                                        break;
                                case '-':
                                        if(str[i+1]!='-'){
                                                fprintf(outf,"(*ptr)-=%d;\n",d);
                                        } else{
                                                d++;
                                        }
                                        break;
                                case '.':
                                        fprintf(outf,"putchar(*ptr);\n");
                                        break;
                                case '[':
                                        fprintf(outf,"while(*ptr){\n");
                                        break;
                                case ']':
                                        fprintf(outf,"}\n");
                                        break;
                                default:
                                        die("ERROR: incorrect syntax.");
                        }
                i++;
                }
        }
        if(str) free(str);
 
        fprintf(outf,"return 0;\n}");
}
 
int main(int argc, char *argv[]){
                FILE *file= get_file(argv[1]);
                FILE *outfile = fopen(argv[2],"w");
        if(argc==3){
                read_file(file, outfile);
                close_file(file, outfile);
        }else{
                die("Format: bf <brainf**k textfile> <C file name>");
        }
        return 0;
}
