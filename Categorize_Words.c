//ARDA BAYRAM - 150116029
//This program categorize a newspaper's words

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include <locale.h>

//Crate some structures
struct MLL{
	char termName[100];
	int totNum;
	struct MLL *next;
	struct LL *nextLL;
};
typedef struct MLL MLL;

struct LL{
	int docID;
	int totNum;
	int length;
	char catName[20];
	struct LL *nextLL;
};
typedef struct LL LL;

//Function prototypes
char *pathcat(const char *str1, char *str2);
void txtAccess(const char *path);
int insert(MLL** header, char[100]);
void txtAccessForSearch(const char *path, MLL **p);
void insertLL(MLL *mll, int id, int length, int totNum, const char *catName);
void stopWords(MLL **hdr);
void discWords(MLL **hdr);

MLL *hdr;
LL *hdrLL;

int main()
{	
	setlocale(LC_ALL, "Turkish"); //Set language for Turkish words
	
	const char *path1="econ/"; // Directory target
    const char *path2="health/"; 
    const char *path3="magazin/";
    
    MLL *mll; //Declare structures
    LL *ll;

	hdr = NULL; //Set header 
	    
    txtAccess(path1); //Call functions to read different directories
    txtAccess(path2);
    txtAccess(path3);
   
	mll=hdr; //Set mll to linked list header
	
	while(mll != NULL) { //Set some variables in the list
		mll->totNum = 0;
		mll=mll->next;
	}
	
	mll=hdr; //Set mll to linked list header
	
	txtAccessForSearch(path1, &mll); //Call functions to read different directories
	txtAccessForSearch(path2, &mll);	
	txtAccessForSearch(path3, &mll);
	
	ll=mll->nextLL; //Set ll
	
	stopWords(&mll); //Call function to find stop words
	
	mll=hdr; //Set mll to linked list header
	
	discWords(&mll); //Call function to find dicriminating words

    return 0;
}

//Use this function for merging path name
char *pathcat(const char *str1, char *str2)
{
	char *res;
    size_t strlen1 = strlen(str1);
    size_t strlen2 = strlen(str2);
    int i, j;
    res = malloc((strlen1+strlen2+1)*sizeof *res);
    strcpy(res, str1);
    for (i=strlen1, j=0; ((i<(strlen1+strlen2)) && (j<strlen2)); i++, j++)
        res[i] = str2[j];
    res[strlen1+strlen2] = '\0';
    return res;
}

//This function read txt's
void txtAccess(const char *path){
	
	char word[100];
	FILE *entry_file;
    struct dirent *dp; 
    char *fullpath;
 
    DIR *dir = opendir(path); //Open the directory - dir contains a pointer to manage the dir
    while (dp=readdir(dir)) //If dp is null, there's no more content to read
    {
    	if (!strcmp (dp->d_name, ".")) //Use this to avoid unwanted texts
            continue;
	
        if (!strcmp (dp->d_name, "..")) //Use this to avoid unwanted texts   
            continue;
        
		fullpath = pathcat(path, dp->d_name); //Create full path name
                
        entry_file = fopen(fullpath, "r"); //Open the file
        
        if (entry_file == NULL) //Check the file open
        {
          fprintf(stderr, "Error : Failed to open entry file\n");
          exit(1);
        }

		//Add each word to linked list with calling insert function
        while((fscanf(entry_file, "%99s", word)) == 1){ 
			insert(&hdr, word);
		}
	
        fclose(entry_file); //Close the file
        free(fullpath);	
    }
    closedir(dir); //Close the handle (pointer)
	
}

//This function read txt's
void txtAccessForSearch(const char *path, MLL ** mll){
	
	char word[100];
	FILE *entry_file;
    struct dirent *dp;
    char *fullpath;
    MLL *q;
    q=*mll;
 
    int IDcnt = 0;
    int length = 0;
    int totNum = 0;
        
    DIR *dir = opendir(path); //Open the directory - dir contains a pointer to manage the dir
    while (dp=readdir(dir)) //If dp is null, there's no more content to read
    {	
    	   	
    	if (!strcmp (dp->d_name, ".")) //Use this to avoid unwanted texts
            continue;

        if (!strcmp (dp->d_name, "..")) //Use this to avoid unwanted texts    
            continue;
            
        IDcnt++;
		fullpath = pathcat(path, dp->d_name); //Create full path name
       	
		while(q != NULL) {
			
			length=0;
			totNum=0;
			
			entry_file = fopen(fullpath, "r"); //Open the file
        
        	if (entry_file == NULL){ //Check the file open
        	    fprintf(stderr, "Error : Failed to open entry file\n");
	            exit(1);
      	    }
        	
        	//Set values in linked list
			while((fscanf(entry_file, "%99s", word)) == 1){
				if(strcmp(word, q->termName) == 0){
  					q->totNum++;
  					break;
  				} 
    		}
    		
    		fclose(entry_file); //Close the file
    		      		
    		entry_file = fopen(fullpath, "r"); //Open the file
        
       		if (entry_file == NULL){ //Check the file open
        		fprintf(stderr, "Error : Failed to open entry file\n");
        		exit(1);
        	}
        	
			//Set variables to use them to create ll        
    	   	while((fscanf(entry_file, "%99s", word)) == 1){
        		length++;
				if(strcmp(word, q->termName) == 0){
					totNum++;
  		  		} 
    		}
    		
    		insertLL(q, IDcnt, length, totNum, path); //Call function to create ll
    		
    		fclose(entry_file); //Close the file
    		q=q->next;
		}
		if(q == NULL) q = *mll;
       
        free(fullpath);
    }
    closedir(dir); //Close the handle (pointer)
}

//This function create ll
void insertLL(MLL *mll, int id, int length, int totNum, const char *catName){
	
	LL *p, *q;
	
	p=malloc(sizeof(LL)); //Create a node and set its variables
	p->docID = id;
    p->length = length;
    p->totNum = totNum;
    
    if(catName[0] == 'm')strcpy(p->catName, "magazin");
	if(catName[0] == 'h')strcpy(p->catName, "health");    
	if(catName[0] == 'e')strcpy(p->catName, "econ");

	p->nextLL = NULL;
    
	if(mll->nextLL==NULL) mll->nextLL=p; //Connect nodes in the list
	
	else{
		q=mll->nextLL;
			
		while(q->nextLL != NULL){
			q=q->nextLL;
		}
		q->nextLL=p;
	}
}

//This function create mll
int insert(MLL **header, char word[100])
{ 
   MLL *p,*q,*r;
      
   p=malloc(sizeof(MLL)); //Create a node and set its variables
   strcpy(p->termName, word);
   p->next = NULL;
   p->nextLL = NULL;
   
   if (*header == NULL) *header=p; //Connect nodes in the list
   
   else {
   	 q=*header;
   	 while(q != NULL && strcmp(q->termName, p->termName) < 0){
   	 	  r=q;
   	 	  q=q->next;
		}
		
	 if(q != NULL && strcmp(q->termName, p->termName) == 0){
	 	return 1;
	 }
	 
	 if(q != NULL) p->next = q;
	 
	 if(q==*header) *header = p;
	 
	 else r->next=p;
   }
   
   return 1;
 } 
 
 //Find and print stop words
 void stopWords(MLL **hdr){
 	
 	//Create and set some variables
 	MLL *mll, *temp, *prevTemp, *prevTemp2, *prevTemp3, *prevTemp4;
 	LL *ll;
 	int totNum=0, prevNum=0, i=0, l=0;
 	
 	MLL mllArray[5], mllArray2[5], tempArray[1];
 	
 	temp=NULL;
 	prevTemp=NULL;
 	prevTemp2=NULL;
 	prevTemp3=NULL;
 	prevTemp4=NULL;
 	mll=*hdr;
 	ll=mll->nextLL;
 	
 	for(i=0; i<5; i++){ //Do work and check its conditions
		prevNum=0;
 		while(mll!=NULL){
 			totNum=0;
 			
 			while(ll!=NULL){
 				totNum+=ll->totNum;
 				ll=ll->nextLL;
			}
			
 			if(totNum>prevNum){
 				if(prevTemp != mll){
 					if(prevTemp2 != mll){
 						if(prevTemp3 != mll){
 							if(prevTemp4 != mll){
 								temp=mll;
 								prevNum=totNum;
							}
							else{
								mll=mll->next;
								if(mll!=NULL) ll=mll->nextLL;
								continue;
							}
						}
						else{
							mll=mll->next;
							if(mll!=NULL) ll=mll->nextLL;
							continue;
						}
					}
					else{
						mll=mll->next;
						if(mll!=NULL) ll=mll->nextLL;
						continue;
					}
				}
				else{
					mll=mll->next;
					if(mll!=NULL) ll=mll->nextLL;
					continue;
				}
			} 
 		
 			mll=mll->next;
 			if(mll!=NULL) ll=mll->nextLL;
		}
		if(i>=3) prevTemp4=prevTemp3;
		if(i>=2) prevTemp3=prevTemp2;
		if(i>=1) prevTemp2=prevTemp;
		prevTemp=temp;
		mllArray[i]=*temp;
 		mll=*hdr;
	}
 
 	for(i=0; i<5; i++){ //Create another array from backward
 		mllArray2[i]=mllArray[4-i];
 	}
 	
 	for(i=0;i<5;i++){ //Sort the array
 		for(l=0;l<4;l++){
 			if(strcmp(mllArray2[l].termName, mllArray2[l+1].termName)>0){
 				tempArray[0]=mllArray2[l];
 				mllArray2[l]=mllArray2[l+1];
 				mllArray2[l+1]=tempArray[0];
			}	
 			
		}
	}
	for(i=0;i<5;i++){ //Print array in ascending order
 		printf("Term-%d: %s\n",i+1,mllArray2[i].termName);
	}
 }	
 
 //Find and print discriminating words
 void discWords(MLL **header){
 	
 	//Create and set some variables
 	MLL *mll;
 	LL *ll, *prevLL;
 	int check,econCnt=0,magCnt=0,healthCnt=0,i, genCnt=0;
 	
 	mll=*header;
 	ll=mll->nextLL;
 	prevLL=ll;
 	
 	MLL econ[5],magazin[5],health[5];
 	
 	while(mll!=NULL){ //Check list ending
 		
 		while(ll!=NULL){ //Check ll list and eliminate empty texts
 			if(ll->totNum > 0){
 				prevLL=ll;
 				break;
			}
			ll=ll->nextLL;	
		}
		ll=mll->nextLL;
 		
 		while(ll!=NULL){ //Do work and check its conditions
 			
 			if(ll->totNum > 0){
			 
 				if(strcmp(ll->catName, prevLL->catName) != 0){
 					check=0;
 					break;
				} 
 				else check=1;
 			
 				if(ll->totNum > 0) prevLL=ll; 
			}
 			ll=ll->nextLL;
		}
		ll=mll->nextLL;
		
		if(check==1){ //Set ll value
			while(ll!=NULL){
 				if(ll->totNum > 0){
 					break;
				}
			ll=ll->nextLL;	
			}
		}
		
		if(check==1){ //Insert the correct nodes inside of arrays
			if(strcmp(ll->catName, "econ") == 0){
				if(econCnt>=5){
					mll=mll->next;	
 					if(mll!=NULL) ll=mll->nextLL;
 					if(ll->totNum > 0) prevLL=ll;
 					continue;
				}
				econ[econCnt]=*mll;
				econCnt++;
				genCnt++;
				
			}
			if(strcmp(ll->catName, "magazin") == 0){
				if(magCnt>=5){
					mll=mll->next;	
					if(mll!=NULL) ll=mll->nextLL;
 					if(ll->totNum > 0) prevLL=ll;
 					continue;
				}
				magazin[magCnt]=*mll;
				magCnt++;
				genCnt++;
				
			}
			if(strcmp(ll->catName, "health") == 0){
				if(healthCnt>=5){
					mll=mll->next;	
 					if(mll!=NULL) ll=mll->nextLL;
 					if(ll->totNum > 0) prevLL=ll;
 					continue;
				} 
				health[healthCnt]=*mll;
				healthCnt++;
				genCnt++;
			}
		}
		
		if(genCnt==15){ //Prints dicriminating words
			
			printf("\n%-10s%-10s%-10s\n", "econ", "health", "magazin");
			printf("------------------------------\n");
			
			for(i=0; i<5;i++){
				printf("%-10s%-10s%-10s\n",econ[i].termName,health[i].termName,magazin[i].termName);
			}
			genCnt++;
		}
		
 		mll=mll->next;
		if(mll!=NULL){
			ll=mll->nextLL;
 			if(ll->totNum > 0) prevLL=ll;
		} 	
 		
	}
 }
  
 

