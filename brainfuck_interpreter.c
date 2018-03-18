// gcc brainfuck_interpreter.c -o gobrainfuck

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_instructions(FILE * fp) {
	if(fp==NULL){
		return -1 ;
	}


	int result = 0 ;
	char c;

	while((c=fgetc(fp))!=EOF){
		if(c=='<'|| c=='>' || c=='+' || c=='-' || c=='.' || c==',' || c=='[' || c==']'){
			result ++;
		}
	}

	return result;
}

unsigned char * clean_input(FILE *fp) {
	if(fp==NULL){
		return NULL ;
	}

	unsigned char * result = NULL;
	int nb_instructions;
	if((nb_instructions=count_instructions(fp))==-1){
		return NULL ;
	}
	rewind(fp);
	result = malloc((nb_instructions + 1)*sizeof(char));
	char c ;
	int index = 0 ;
	while((c=fgetc(fp))!=EOF){
		if(c=='<'|| c=='>' || c=='+' || c=='-' || c=='.' || c==',' || c=='[' || c==']'){
			result[index]=c;
			index ++ ;
		}
	}
	result[nb_instructions] = 0;

	return result ;
}
/*
int count_data_slots(char* instructions) {

	int result= 0 ;

	return result ;

}
*/

int find_closing_bracket(unsigned char * instructions, int ip){
	
	int good_one = 0 ;
	int index = ip ;
	int result = -1 ;
	do{
		index ++ ;
		if(instructions[index] == '[') {
			good_one ++;
		}else if (instructions[index] == ']'){
			good_one --;
		}
	}while (instructions[index]!= ']' || good_one != -1);
	return index;
}

int find_opening_bracket(unsigned char * instructions, int ip){
	
	int good_one = 0 ;
	int index = ip ;
	int result = -1 ;
	do{
	 	//printf("\ton recule\n");
		index -- ;
		if(instructions[index] == ']') {
			//printf("\trencontre ] intermédiaire \n");
			good_one ++;
		}else if (instructions[index] == '['){
			//printf("\trencontre [ intermédiaire \n");
			good_one --;
		}
		//printf("\tinstructions = %c good_one = %d",instructions[index],good_one);
	}while (instructions[index]!= '[' || good_one != -1);
	
	
	return index ;
}

int exec_environment(unsigned char * instructions){


	int ip = 0 ;
	int pdata = 0 ;
	unsigned char data[30000];
	memset(data,0,30000 * sizeof(data[0]));


	while (instructions[ip]!=0){
		//printf("Je suis en %d, je parse le %c : ",ip,instructions[ip]);
		if(instructions[ip]=='<'){
			//printf("cellule prcédente \n");
			if(pdata-1 >= 0){
				pdata -- ;
			}else{
				return -3 ;
			}
			if(ip+1<strlen(instructions)){
				ip++;
			}else{
				return -1 ;
			}
		}else if(instructions[ip]=='>'){
			//printf("cellule suivante \n");
			if(pdata +1 < sizeof(data)){
				pdata ++ ;
			}else{
				return -2 ;
			}
			if(ip +1 < strlen(instructions)){
				ip++;		
			}else {
				return -1 ;
			}
		}else if(instructions[ip]=='+'){
			//printf("cellule ++ \n");
			data[pdata] ++ ;
			ip++;
		}else if(instructions[ip]=='-'){
			//printf("cellule -- \n");
			data[pdata] -- ;
			ip++;
		}else if(instructions[ip]=='.'){
			//printf("affichage \n");
			printf("%c",data[pdata]);
			ip++;
		}else if(instructions[ip]==','){
//ajouter l'input		
		}else if(instructions[ip]=='['){
			if(data[pdata]==0){
				ip = find_closing_bracket(instructions,ip)+1;
			}else{
				ip ++ ;
			}
		}else if(instructions[ip]==']'){
			ip = find_opening_bracket(instructions, ip);
			//printf("on saute en %d\n",ip);
		}

	}	
	/*
	//Affichage des 10 premières valeurs
	printf("\n");
	int i ;
	for(i = 0 ; i < 10 ; i++){
		printf("%p",data[i]);
	}
	printf("\n");
	*/
	return 0 ;
}

int main(int argc, char ** argv){

	FILE * fp ;	
	unsigned char * instructions ;

	if(argc!=2){
		printf("Use : ./gobrainfuck <file>\n");
		return -1 ;
	}

	if((fp = fopen(argv[1],"r"))==NULL){
		printf("Error while opening the input file \n");
		return -2 ;
	}

	if((instructions = clean_input(fp))==NULL){
		return -3 ;
	}

	if(fp){
		fclose(fp);
	}
	int result ;
	result = exec_environment(instructions);
	/*
	//Affichage test input nettoyé
	int i ;
	for(i = 0 ; i < strlen(instructions);i++){
		printf("%c",instructions[i]);
	}	
	printf("\n");
	//fin Affichage test
	*/

	return result ;
}
