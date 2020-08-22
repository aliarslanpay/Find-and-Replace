#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int max (int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

void bad_char_heuristic( char *pattern, int size, int badchar[256], int case_s) {
    int i;
    
    for (i = 0; i < 256; i++)
         badchar[i] = -1;
         
    for (i = 0; i < size; i++)
         badchar[(int) pattern[i]] = i;

}

void search( char *text,  char *pattern, int case_s, int *index) {
	int i=0;  
    int m = strlen(pattern);
    int n = strlen(text);
    int badchar[256];
    int s = 0;  
    
	bad_char_heuristic(pattern, m, badchar, case_s);
    

	if (case_s == 1){
		while(s <= (n - m)) {
        	int j = m-1;  
        	
        	while(j >= 0 && pattern[j] == text[s+j])  
            	j--;
            	
        	if (j < 0) {  
        		index[i] = s; 
        		i++;
            	s += (s+m < n) ? m-badchar[text[s+m]] : 1; 
    		}
        	else
            	s += max(1, j - badchar[text[s+j]]);
    	}
	}
	
	//not case sensitive
	else{
		while(s <= (n - m)) {
        	int j = m-1; 
        	
        	while(j >= 0 && (pattern[j] == tolower(text[s+j]) || pattern[j] == tolower(text[s+j]+32) || pattern[j] == tolower(text[s+j]-32)))
            	j--;
            	
        	if (j < 0) {  
        		index[i] = s;  
        		i++;
            	s += (s+m < n)? m-badchar[tolower(text[s+m])] : 1;
			}
        	else
            	s += max(1, j - badchar[tolower(text[s+j])]); 
    	}
	}

	if (i == 0){
		printf("The entered pattern is not in the given text!");
		exit(0);
	}
}

void replace_pattern (char *txt,  char *find, char *replace, int *index){
	int f_a_r=0;
	int g=strlen(txt);
	char txt_bastir[g];  
	strcpy(txt_bastir,txt);    
	
	int i=1, w=0, u, v;  
	int n = strlen(txt);  
	int j = strlen(find);  
	int k = strlen(replace);  
	int uzunluk_farki = k-j;  
	
	while (index[i] > 0){
    	w++;  
    	i++;  
	}
	
	int iter;  
	
	if (uzunluk_farki <= 0){
		int p = 1;
		while (index[p] != 0){
			if (index[p] - index[p-1] < j){
				index[p]=-1;
			}
			p++;
		}
		i=0, u=0;  
		while (i <=n && w >= 0){
		iter = i+j;
		if (i == index[u] && w >= 0){
			while (iter <= n-1){  
				txt[iter-(j-k)] = txt[iter];
				iter++;
			}
			
			for(v=0; v<k; v++){
				txt[i] = replace[v];
				i++;
			}
			
			f_a_r++; 

			int z = j-k, q=iter-1;  
			while (z > 0){
				txt[q]='\0';
				q--;
				z--;
			}
		
			w--;
			u++;
			
			p=1;
			while (index[p] != 0){
				index[p] = index[p]-(j-k);
				p++;
			}
			if (index[u] < 0){
				u++;
			}
		}
		else  
			i++;
		n = strlen(txt);  
	}
	}
	
	else{
		int p = 1;
		while (index[p] != 0){
			if (index[p] - index[p-1] < j){
				index[p]=-1;
			}
			p++;
		}
		i = n-1, u=0;   
		while (i >=0 && w >= 0){   
			iter = n-1+uzunluk_farki;
			if (i == index[u] && w >= 0){   
				while (iter > index[u]){
				txt[iter] = txt[iter-(uzunluk_farki)];
				iter--;
				}
			
				for(v=0; v<k; v++){
					txt[iter] = replace[v];
					iter++;	
				}
				f_a_r++;
			
				i--;
				w--;
				u++;
				
				p=1;
				
				while (index[p] != 0){
					if (index[p] != -1)
						index[p] = index[p]+(k-j);
					p++;
				}
				if (index[u] < 0){
					u++;
					w--;
				}
				n = strlen(txt);  
				i=n-1;  
				
			}
			else  
				i--;
		}
	}
	printf("\nText: %s",txt_bastir);
	printf("\nNew Text: %s",txt);
	printf("\nFound and Replaced: %d",f_a_r);
}

int main() {
	struct timespec tstart={0,0}, tend={0,0};
    int i=1, secenek = 3, case_s = 1, txtUzunluk;
    int index[100] = {0};
	char txt[700] = {}, find[50] , replace[50] , ch;
    FILE *fp;
	
    while (secenek != 0 && secenek != 1){
		printf("Press 0 to read as a string, Press 1 to read from the 'input.txt' file!\n");
		scanf("%d",&secenek);
		if ( secenek != 0 && secenek != 1)
			printf("\nYou made a wrong choose! Please try again!\n");
	}
	
	if (secenek == 0){
    	ch = getchar();
    	printf("Enter a string:");
		scanf("%s",txt);
	}
	
	else{
		fp = fopen("input.txt","r+"); 
		if (fp == NULL){
			printf("The file could not be opened!");
			return 0;
		}
		else{
			do {
				fgets(txt,650,fp);
  			} while (!feof(fp));
		}
	}
	
	printf("\n%s\n",txt);
	printf("\nFind: ");
	ch = getchar();
	gets(find);
	printf("Replace: ");
	gets(replace);
	printf("Option (Press 0 for not case sensitive, press 1 for case sensitive): ");
	scanf("%d",&case_s);
	
    clock_gettime(CLOCK_MONOTONIC, &tstart);  
    if (case_s == 0){  
    	int i = 0;
		while( find[i] ) {
      		find[i] = tolower(find[i]);
      		i++;
   		}
	}
	
	search(txt, find, case_s, index); 
	replace_pattern(txt, find, replace, index);  
	txtUzunluk = strlen(txt);   
	fprintf(fp,"\n");
	fwrite(txt,1,txtUzunluk,fp);  
    fclose (fp);
	clock_gettime(CLOCK_MONOTONIC, &tend); 
    printf("\nRunning Time: %.5f seconds\n",  
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
           
    return 0;
}
