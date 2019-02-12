%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>

int line_count=1;

FILE *logout;
FILE *tokenout;

%}

WHITESPACE [ \t\f\r\v,]+ 
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \n

%%

{WHITESPACE} {}
{NEWLINE} {line_count++;}

[1-9]+{DIGIT}*	{
			fprintf(logout,"Line no %d: Lexeme %s found. matched pattern 1\n",line_count,yytext);
			printf("Line no %d:  %s -> pattern 1\n",line_count,yytext);
		}
 
[a-zA-Z_]+[a-zA-Z0-9_]*		{
			fprintf(logout,"Line no %d: Lexeme %s found. matched pattern 2\n",line_count,yytext);
			printf("Line no %d: Lexeme %s -> pattern 2\n",line_count,yytext);
		}
		
[0-9]*[.][0-9]+ 	{
			fprintf(logout,"Line no %d: Lexeme %s found. matched pattern 3\n",line_count,yytext);
			printf("Line no %d: Lexeme %s -> pattern 3\n",line_count,yytext);
		}

[0-9]+[.][0-9]+[E][+-]?[1-9]+{DIGIT}* 	{
			fprintf(logout,"Line no %d: Lexeme %s found. matched pattern 4\n",line_count,yytext);
			printf("Line no %d: Lexeme %s -> pattern 4\n",line_count,yytext);
		}	
		
n[^0-9 \t\f\r\v,]+[a-zA-Z0-9_]*	{
			fprintf(logout,"Line no %d: Lexeme %s found. matched pattern 5\n",line_count,yytext);
			printf("Line no %d: Lexeme %s -> pattern 5\n",line_count,yytext);
		}
		
<<EOF>> 	{
			printf("Total Line Number: %d \n",line_count); 
			return 0;
		}
. 		{
			fprintf(logout,"unrecognized pattern %s\n",yytext);
		}
			

%%

int main(int argc,char *argv[]){
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	logout= fopen("log.txt","w");
	tokenout= fopen("token.txt","w");

	yyin= fin;
	yylex();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;
}