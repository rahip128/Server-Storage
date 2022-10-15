#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>
#include <ctype.h>
#include <netinet/in.h>
#include <limits.h>

#define BACKLOG 5

struct connection {
    struct sockaddr_storage addr;
    socklen_t addr_len;
    int fd;
};

struct t_args{
	int connection;
	int connection2;
	//FILE* fIn;
	//FILE* fOut;
};



struct node{
	char* keyName;
	char* value;
	struct node* next;
};

struct request{
	char* command;
	char* length;
	char* key;
	char* value;

};

struct request* r = 0;
pthread_mutex_t lock;




struct request* allocateRequest(char* c, char* l, char* key, char* v){

	struct request* temp = malloc(sizeof(struct request));
	
	char* hold = calloc(strlen(c)+1, sizeof(char)); 
	for(int i = 0; i< strlen(c); i++){
		hold[i] = c[i];
	}
	hold[strlen(hold)]='\0';

	temp->command = hold;
	if(strcmp(c," ") != 0){
		free(c);
	}

	char* hold2 = calloc(strlen(l)+1, sizeof(char)); 
	for(int i = 0; i< strlen(l); i++){
		hold2[i] = l[i];
	}
	hold2[strlen(hold2)]='\0';
	temp->length = hold2;
	if(strcmp(l," ") != 0){
		free(l);
	}
	char* hold3 = calloc(strlen(key)+1, sizeof(char)); 
	for(int i = 0; i< strlen(key); i++){
		hold3[i] = key[i];
	}
	hold3[strlen(hold3)]='\0';
	temp->key = hold3;

	if(strcmp(key," ") != 0){
		free(key);
	}
	char* hold4 = calloc(strlen(v)+1, sizeof(char)); 
	for(int i = 0; i< strlen(v); i++){
		hold4[i] = v[i];
	}
	hold4[strlen(hold4)]='\0';
	temp->value = hold4;
	if(strcmp(v," ") != 0){
		free(v);
	}
	


		

	return temp;
}

struct node* allocateNode(char* k, char* v){
	struct node* temp = malloc(sizeof(struct node));
	temp->keyName = k;
	temp->value = v;
	temp->next = 0;

	return temp;
}

struct node* LL;

char* concat (char* a, char* b, int nL){
	int length = strlen(a) + strlen(b);
	char* final =  calloc(length+2+nL, sizeof(char));
	for(int i = 0; i < strlen(a); i++){
		final[i] = a[i];
	}
	final[strlen(a)] = '\n';
	for(int i = 0; i < strlen(b); i++){
		final[i + strlen(a)+1] = b[i];
	}
	if(nL == 1){
		final[strlen(final)] = '\n';
	}


	return final;
}



char* GET(struct node* head, char* key){
	pthread_mutex_lock(&lock);
	struct node* ptr = head;
	char* stringL;
	char* lengthS = (char*)malloc(100* sizeof(char));
	char* tempo;
	
	
	
	while(ptr!=0){
		if(strcmp(key, ptr->keyName) == 0){

			sprintf(lengthS, "%ld", (strlen(ptr->value))+1);
			stringL = concat("OKG",lengthS,0);
			tempo = stringL;
			stringL = concat(stringL, ptr->value,1);
			free(tempo);
			free(lengthS);
			pthread_mutex_unlock(&lock);
			return stringL;


			
		}
		
		ptr = ptr->next;
	}
	free(lengthS);
	pthread_mutex_unlock(&lock);
	return "KNF\n";
}

char* SET(struct node* head, char* key, char* val){
	pthread_mutex_lock(&lock);
	
	struct node* ptr = head;
	
	

	while(ptr!=0){
		if(strcmp(key, ptr->keyName) == 0){
			ptr->value = val;
			pthread_mutex_unlock(&lock);
			return "OKS\n";
		}
		
		ptr = ptr->next;
	}
	
	struct node* temp = allocateNode(key,val);
	ptr = head;
	while(ptr->next!=NULL){
		ptr = ptr->next;
	}
	ptr->next = temp;
	

	pthread_mutex_unlock(&lock);
	return "OKS\n";

}

char* DEL(struct node* head, char* key){
	pthread_mutex_lock(&lock);
	struct node* ptr = head->next;
	struct node* temp = head;
	char* stringL;
	char* lengthS = (char*)malloc(100* sizeof(char));
	char* tempo;
	

	while(ptr!=0){
		if(strcmp(key, ptr->keyName) == 0){
			//char* value = ptr->value;
			
			sprintf(lengthS, "%ld", (strlen(ptr->value))+1);
			//printf("lengthS = %s\n", lengthS);
			stringL = concat("OKD",lengthS,0);
			tempo = stringL;
			stringL = concat(stringL, ptr->value,1);
			temp->next = ptr->next;
			free(ptr);
			free(lengthS);
			free(tempo);
			//printf("hello \n%s", stringL);
			pthread_mutex_unlock(&lock);
			return stringL;
			
		}
		temp = temp->next;
		ptr = ptr->next;
	}
	free(lengthS);
	pthread_mutex_unlock(&lock);
	return "KNF\n";
}

void decrypt(char* string){
	char* word = (char*)calloc(1, sizeof(char));
	//char* c = (char*)calloc(1, sizeof(char));
	int count = 0;
	int counter = 0;
	char* com = " ";
	char* len = " ";
	char* pay1 = " ";
	char* pay2 = " ";
	int n = 0;
	//char* payload;
	//printf("RAHIII\n");
	while(counter < strlen(string)){
		//printf("%d\n", n);
		if(string[counter] != '\n'){
			word[count] = string[counter];
			count++;
			word = (char*) realloc(word, (count+1));
		

		}else{
			
			word[count] = '\0';
			count = 0;
			char* hold = calloc(strlen(word)+1, sizeof(char)); 
			for(int i = 0; i< strlen(word); i++){
				hold[i] = word[i];
			}
			hold[strlen(hold)]='\0';
			if(n == 0){
				com = hold;

			}else if(n == 1){
				len = hold;
				//break;
			}else if(n == 2){
				pay1 = hold;
			}else if(n == 3){
				pay2 = hold;
			}
			n++;
			
			
		}
		counter++;
		
	}
	
	
	

	

	free(word);
	free(string);
	r = allocateRequest(com,len,pay1,pay2);
	
	}


char* readRequest(struct request* r){
	
	
	if(strcmp(r->command, "GET") == 0){
		return GET(LL, r->key);
	}else if(strcmp(r->command, "SET") == 0){
		return SET(LL, r->key, r->value);
	}else if(strcmp(r->command, "DEL") == 0){
		return DEL(LL, r->key);
	}
	return "ERR";

}

void printLL(struct node* head){
	struct node* ptr = head;
	while(ptr!= NULL){
		printf("%s(%s)->", ptr->keyName, ptr->value);
		ptr = ptr->next;
	}
	printf("\n");
}

void freeNodes(struct node* head){

	struct node* temp = head;
	head = head->next;
	free(temp);
	while(head != 0){
		temp = head;
		head=head->next;
		free(temp);
	}
	free(head);


}
int readInput(FILE* fIn, FILE* fOut){
	char* stringLiteral = (char*) calloc(INT_MAX, sizeof(char));
	stringLiteral[0] = getc(fIn);
	int limit = 0;
	int count = 0;
	char* err;
	if(stringLiteral[0] == 'G' || stringLiteral[0] == 'D'){
		limit = 3;
	}else if(stringLiteral[0] == 'S'){
		limit = 4;
	}
	int i = 1;
	int check = 0;
	while(count < limit){
		stringLiteral[i] = getc(fIn);
		if(stringLiteral[i] == '\n'){	
			count++;
		}
	
		if(count == 1){
			if(stringLiteral[0] == 'G'){
				if(stringLiteral[1] == 'E'){
					if(stringLiteral[2] == 'T'){
						
					}else{
						//wrong
						err = concat("ERR", "BAD", 1);
						fprintf(fOut, "%s",err);
						fflush(fOut);
						free(err);
						return 1;
						
					}
				}else{
					//wrong
					err = concat("ERR", "BAD", 1);
					fprintf(fOut, "%s",err);
					fflush(fOut);
					free(err);
					return 1;
				}
			}else if(stringLiteral[0] == 'S'){
				if(stringLiteral[1] == 'E'){
					if(stringLiteral[2] == 'T'){
						
					}else{
						//wrong
						err = concat("ERR", "BAD", 1);
						fprintf(fOut, "%s",err);
						fflush(fOut);
						free(err);
						return 1;
					}
				}else{
					//wrong
					err = concat("ERR", "BAD", 1);
					fprintf(fOut, "%s",err);
					fflush(fOut);
					free(err);
					return 1;
				}
			}else if(stringLiteral[0] == 'D'){
				if(stringLiteral[1] == 'E'){
					if(stringLiteral[2] == 'L'){
						
					}else{
						//wrong
						err = concat("ERR", "BAD", 1);
						fprintf(fOut, "%s",err);
						fflush(fOut);
						free(err);
						return 1;
					}
				}else{
					//wrong
					err = concat("ERR", "BAD", 1);
					fprintf(fOut, "%s",err);
					fflush(fOut);
					free(err);
					return 1;
				}
			}
			
		}else if(count == 2 && check == 0){
			int j = i-1;
			while(stringLiteral[j]!='\n'){
				if(isdigit(stringLiteral[j]) == 0){
					err = concat("ERR", "BAD", 1);
					fprintf(fOut, "%s",err);
					fflush(fOut);
					free(err);
					return 1;
				}
				j--;
			}
			check = 1;
			
		}
		
		
		i++;
	}

	
	
	decrypt(stringLiteral);

	int payLength = atoi(r->length);
	int keyLength = strlen(r->key)+1;
	int valueLength = 0;
	if(strcmp(r->command,"GET") == 0 || strcmp(r->command,"DEL") == 0){
		valueLength = strlen(r->value)-1;
	}else{
		valueLength = strlen(r->value)+1;
	}
	
	if(keyLength + valueLength != payLength){
		err = concat("ERR", "LEN", 1);
		fprintf(fOut, "%s",err);
		fflush(fOut);
		free(err);
		return 1;
	}

	char* strl = readRequest(r); 


	
	fprintf(fOut, "%s",strl);
	fflush(fOut);
	printLL(LL->next);
	

	return 0;
}

void* helper(void* in){
	struct t_args* arg = (struct t_args*) in;

	FILE* fdIn = fdopen(arg->connection, "r");



	FILE* fdOut = fdopen(arg->connection2, "w");
			
	while(fdIn != NULL && fdOut != NULL){
			
		int x = readInput(fdIn,fdOut);

		if(x!=0){
					
			close(arg->connection);
			return NULL;

		}
	}
	return NULL;

		

	
}

int server(char *port)
{
    struct addrinfo hint, *info_list, *info;
    //struct connection *con;
    int error, sfd;
    pthread_t tid;


    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    	
    error = getaddrinfo(NULL, port, &hint, &info_list);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
	
        return -1;
    }


    for (info = info_list; info != NULL; info = info->ai_next) {
        sfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        

        if (sfd == -1) {
            continue;
        }

       
        if ((bind(sfd, info->ai_addr, info->ai_addrlen) == 0) &&
            (listen(sfd, BACKLOG) == 0)) {
            break;
        }


        close(sfd);
    }

    if (info == NULL) {

        fprintf(stderr, "Could not bind\n");
	freeaddrinfo(info_list);
        return -1;
    }

    freeaddrinfo(info_list);


    printf("Waiting for connection\n");



	for (;;) {
		struct t_args* args = malloc(sizeof(struct t_args*));
		args->connection = accept(sfd,NULL,NULL);
		printf("waiting...\n");
		args->connection2 = dup(args->connection);
		error = pthread_create(&tid, NULL, helper, args);
		if (error != 0) {
			fprintf(stderr, "Unable to create thread: %d\n", error);
			close(args->connection);
			free(args);
			continue;
		}
		if(args->connection != -1){
			pthread_detach(tid);

		}
		

	}
	
	return 0;
}



int main (int argc, char* argv[]){

	if (argc != 2) {
		printf("Usage: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	LL = allocateNode(" "," ");
	(void) server(argv[1]);
	free(r->command);
	free(r->length);
	free(r->key);
	
	free(r->value);
	free(r);
	
    	return EXIT_SUCCESS;
	



}	




