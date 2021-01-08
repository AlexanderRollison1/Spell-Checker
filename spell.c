//Alexander Rollison
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "spell.h"

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as 
they are declared before the array in the parameter list. See: 
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

Worst case time complexity to compute the edit distance from T test words
 to D dictionary words where all words have length MAX_LEN:
Student answer:  Theta(T*D*(MAX_LEN^2))
This is because the TC depends on the lengths of the test word and dictionary word. Because we have to compare the words to each other inside of the edit distance function
and loop through them it is MAX_LEN^2


Worst case to do an unsuccessful binary search in a dinctionary with D words, when 
all dictionary words and the searched word have length MAX_LEN 
Student answer:  Theta(MAX_LEN + Log(D))

This is because it's cut in half every search, using mid.
*/

/* You can write helper functions here */

#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
void ReadFileInto2DArray(FILE *filename, char array[][100],int dictionaryLength);
FILE* OpenFile(char filename[], char mode[]);
int edit_distance(char * first_string, char * second_string, int print_table);
void spell_check(char * testname, char * dictname, int printOn);
int binarySearch(char (*arr)[100],int l,int r, char * x, int verbose, int *count);
FILE* OpenFile(char filename[], char mode[]);
void printBA(char dictionary[][100],int dictionaryLength, int mode);
void getWord(FILE *fp, FILE *out, char dictionary[][100],int dictionaryLength, int printOn);
void sortDictionary(char dictionary[][100], int dictionaryLength);
int cmp(const void *a, const void *b);
int cmp_string(const void* a, const void* b);

int edit_distance(char * first_string, char * second_string, int print_table)
{
	int editDistance;					//Edit Distance -- 10/29/2020
	int size1 = strlen(first_string); 	//Size of first string
	int size2 = strlen(second_string); 	//Size of second string
	int Dist[size1+1][size2+1]; 		//Create Edit Distance Array (2D)
	int i; //Add 1,2,3,4,5 to array at i
	int j; //Add 1,2,3,4 to array at j
	int k; //Compute Edit Distance and place into [k][l]
	int l; //Compute Edit Distance and place into [k][l]
	int m; //Zero Array
	int n; //Zero Array
	
	//Zero the 2D Array
	for(m = 0; m < size1; m++)
	{
		for(n = 0; n < size2; n++)
		{
			Dist[m][n] = 0;
		}
	}
	
	//Make sure the first position of 2D Array is 0
	Dist[0][0] = 0;
	
	//Add numbers 1,2,3,4.. for first string
	for(i = 0; i < size1;i++)
	{
		Dist[i+1][0] = i+1;
		//printf("  %d|",Dist[i+1][0]);
	}
	//printf("\n");
	//Add numbers 1,2,3,4,5.. for second string
	for(j = 0; j < size2; j++)
	{
		Dist[0][j+1] = j+1;
		//printf("  %d|",Dist[0][j+1]);
	}
	
	//Compute the Edit Distances and place them into the array positions
	for(k = 1; k < size1+1; k++)
	{
		for(l = 1; l < size2+1; l++)
		{
			if(first_string[k-1] == second_string[l-1])
			{
				Dist[k][l] = min(min(Dist[k-1][l]+1,Dist[k][l-1]+1),Dist[k-1][l-1]);
			}
			if(first_string[k-1] != second_string[l-1])
			{
				Dist[k][l] = min(min(Dist[k-1][l]+1,Dist[k][l-1]+1),Dist[k-1][l-1]+1);
			}
		}
	}
	
	//Update Edit Distance
	editDistance = Dist[size1][size2];
	
	//If 1, print table, Else continue
	if(print_table == 1)
	{
		int o;
		int p;
		int q;
		int counter = 0;
		//printf(" ");
		for(o = 0; o < size2+2; o++)//  |  | D| o| G|\n
		{
			if(o > 1)
			{
				printf("%3c|", second_string[o-2]);
			}
			else
			{
				printf("   |");
			}
			counter++;
		}
		printf("\n");
		
		for(q = 0; q < counter; q++)
		{
			printf("----");
		}
		printf("\n");
		
		for(o = 0; o < size1+1; o++)
		{
			if(o > 0)
			{
				printf("%3c|", first_string[o-1]);
			}
			else
			{
				printf("   |");
			}
			
			for(p = 0; p < size2+1; p++)
			{
				printf("%3d|", Dist[o][p]);
			}
			printf("\n");
			
			for(q = 0; q < counter; q++)
			{
				printf("----");
			}
			printf("\n");
		}
		
		//printf("\nedit distance: %d\n", editDistance);
	}
	
	return editDistance;
}
	  

void spell_check(char * testname, char * dictname, int printOn)
{
	//Creating Files
	FILE * testFILE = NULL;
	FILE * dictFILE = NULL;
	FILE * outputFILE = NULL;
	char mode[2] = "r+";
	
	//Concatenate to get the output filename out_filename.txt
	char concatout[101] = "";
	char outputName[101] = "out_";
	strcpy(concatout,testname);
	strcat(outputName,concatout);
	//Open dictionary file that we will be geting input from
	dictFILE = OpenFile(dictname, mode);
	printf("\nLoading the dictionary file: %s",dictname);
	
	//Get first line from dictionary file which is the amt of lines
	char indexStr1[100];
	char er1 = '\0';
	char er2 = '\n';
	
	fgets(indexStr1, sizeof(indexStr1)-1, dictFILE);
	if(indexStr1[strlen(indexStr1)-1] == er2)
	{
		indexStr1[strlen(indexStr1)-1] = er1;
	}
	int dictionaryLength = atoi(indexStr1);
	printf("\n\nDictionary has size: %d\n", dictionaryLength);
	
	//Store Lenth of Dictionary into the dictionary Array.
	char dictionary[dictionaryLength][100];
	
	//Call function to store every value into each position in the array
	//struct TreeNode *root = NULL;
	ReadFileInto2DArray(dictFILE, dictionary, dictionaryLength);
	
	//Print dictionary before sort
	if(printOn == 1)
	{
		printBA(dictionary,dictionaryLength,0);
	}
	if(printOn == 1)
	{
		printf("\n\n");
	}
	//sortDictionary(dictionary, dictionaryLength);
	//qsort(dictionary, dictionaryLength, sizeof(dictionary), int(*cmp)(const void *, const void *));
	qsort(dictionary, dictionaryLength, sizeof(*dictionary), (int (*)(const void*, const void*))strcmp);
	
	//Print dictionary after sort
	if(printOn == 1)
	{
		printBA(dictionary,dictionaryLength, 1);
	}
	
	
	//printf("\nLoading the Test file: %s\n", testname);
	testFILE = OpenFile(testname, mode);
	//printf("\nCreating the Output file: %s\n", outputName);
	outputFILE = OpenFile(outputName, "w");
	
	char * word;
	int binaryCount = 0;
	int searchNum;
	
	getWord(testFILE,outputFILE, dictionary,dictionaryLength,printOn);
	fclose(testFILE);
	fclose(outputFILE);
}

void getWord(FILE *fp, FILE *out, char dictionary[][100],int dictionaryLength, int printOn)
{
	char word[101] = "";
	char ch;
	int i = 0;
	while(!feof(fp))
	{
		ch = fgetc(fp);
		
		if(isalpha(ch) && ch != 10 && ch < 256 && ch > 0)
		{
			word[i] = ch;
			i++;
		}
		else
		{
			if(i>0)
			{
				int binaryCount = 0;
				int searchNum;
				
				binaryCount = 0;
				if(printOn != 0 && printOn == 1)
				{
				printf("\nBinary search for: %s\n", word);
				}
				
				int z;
				char wordLower[101];
				char tempChar = 'c';
				
				strcpy(wordLower,word);
				for(int i = 0; wordLower[i]; i++)
				{
					tempChar = wordLower[i];
					wordLower[i] = tolower(tempChar);
				}
				searchNum = binarySearch(dictionary, 0, dictionaryLength, wordLower, printOn, &binaryCount);
				
				if(searchNum == -1)
				{
					if(printOn != 0 && printOn == 1)
					{
						printf("\nNot found\n");
					}
				}
				printf("\n\n--->|%s|", word);
				printf(" (words compared when searching: %d)\n", binaryCount);
				
				if(searchNum != -1)
				{
					printf("\n    -OK\n");
					fprintf(out, "%s", word);
				}
				else if(searchNum == -1)
				{
					printf("\n-1 - type correction");
					printf("\n 0 - leave word as is (do not fix spelling)\n");
					
					int w;
					int minDist = 0;
					int tempDist;
					char minArray[dictionaryLength][100];
					int minCounter = 0;
					for(w = 0; w < dictionaryLength;w++)
					{
						if(minDist == 0)
						{
							minDist = edit_distance(word, dictionary[w], 0);
							minCounter++;
						}
						else
						{
							tempDist = edit_distance(word,dictionary[w], 0);
							minCounter++;
							if(minDist > tempDist)
							{
								minDist = tempDist;
							}
						}
					}
					printf("     Minimum distance: %d  (computed edit distances: %d)\n     Words that give minimum distance:\n",minDist,minCounter);
					int minArrayCounter = 0;
					for(w = 0; w < dictionaryLength; w++)
					{
						tempDist = edit_distance(word,dictionary[w],0);
						//printf("\n--%d--\n",tempDist);
						if(tempDist == minDist)
						{
							strcpy(minArray[minArrayCounter],dictionary[w]);
							minArrayCounter++;
						}
					}
					
					for(w = 1; w <= minArrayCounter; w++)
					{
						printf("\n %d - %s",w, minArray[w-1]);
					}
					
					printf("\nEnter your choice from the above options: ");
					int choice;
					scanf("%d",&choice);
					
					if(choice == -1)
					{
						printf("\nEnter correct word: ");
						char newWord[100];
						scanf("%s",newWord);
						fprintf(out, "%s", newWord);
					}
					else if(choice == 0)
					{
						fprintf(out, "%s", word);
					}
					else if(choice > 0)
					{
						fprintf(out,"%s", minArray[choice-1]);
					}
				}
			}
			
			if(!feof(fp))
			{
				fprintf(out, "%c", ch);
			}
			i=0;
			memset(word, 0, 101);
		}
    }
}
/*
void sortDictionary(char dictionary[][100], int dictionaryLength)
{
	int i;
	int j;
	char temp[100];
	
	for(i=0;i<dictionaryLength;i++)
	{
		for(j=i+1;j<dictionaryLength;j++)
		{
			if(strcmp(dictionary[j],dictionary[i])<0)
			{
				strcpy(temp,dictionary[i]);
				strcpy(dictionary[i],dictionary[j]);
				strcpy(dictionary[j],temp);
			}
		}
	}
}
*/

int cmp_string(const void* a, const void* b)
{
    const char* s1 = *(const char**)a;
    const char* s2 = *(const char**)b;
    return strcmp(s1, s2);
}

int cmp(const void *a, const void *b) { 
    return strcmp(a, b); 
}

int binarySearch(char (*arr)[100],int low,int high, char * x, int verbose, int *count)
{
	if (high >= low) 
	{ 
        int mid = low + (high - low)/2; 
		
		if(verbose == 1 && verbose != 0)
		{
			printf("\ndict[%d] = %s", mid, arr[mid]); 
        }
		
		(*count)++;
		
		if (strcmp(arr[mid],x) == 0) 
            return mid; 
  
        if (strcmp(arr[mid],x) > 0) 
            return binarySearch(arr, low, mid - 1, x, verbose, count); 
  
        return binarySearch(arr, mid + 1, high, x, verbose, count); 
	}
	return -1;
}
/*
int binarySearch1(char arr[][100], int l, int r, char *x)
{
	while(l<=r)
	{
		int m = l+(r-1)/2
		if(arr[m] == x)
		{
			return m;
		}
		if(arr[m] < x)
		{
			l = m + 1;
		}
		else
		{
			r = m - 1;
		}
	}
}
*/

void printBA(char dictionary[][100],int dictionaryLength, int mode)
{
	if(mode == 0)
	{
		printf("   Original dictionary :\n");
	}
	if(mode == 1)
	{
		printf("   Sorted dictionary (alphabetical order) :\n");
	}
	
	int r;
	for(r = 0; r < dictionaryLength; r++)
	{
		printf("%d. %s\n", r, dictionary[r]);
	}
}

void ReadFileInto2DArray(FILE *filename, char array[][100],int dictionaryLength)
{
	char index[100];
	int count = 0;
	//char *token;
	/*
	while(fgets(index, sizeof(index)-1,filename))
	{
		if(index[strlen(index)] == '\n')
		{
			index[strlen(index)] = '\0';
		}
		if(index[strlen(index)-1] == '\n')
		{
			index[strlen(index)-1] = '\0';
		}
		strcpy(array[count], index);
		count++;
	}
	*/
	
	 int i;
	 for(i = 0; i < dictionaryLength; i++)
	 {
		 if(i == (dictionaryLength-1))
			fscanf(filename,"%s",array[i]);
			
		fscanf(filename,"%s\n",array[i]);
	 }
	
}
FILE* OpenFile(char filename[], char mode[])
{
	FILE* FH = NULL;
	FH = fopen(filename,mode);
	while(FH == NULL)
	{
		printf("\nA File you entered does not exist. Exiting...\n");
		exit(0);
	}
	return FH;
}
