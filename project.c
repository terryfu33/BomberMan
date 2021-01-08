#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>


// Terry Fu - 30088948
//Project part 1 - bomberman in c
// X - unknown * - exit tile @ - extra bomb $ - extra bomb range # - extra life 
//random number generation used code from tutorial as inspiration


//global variables
float scoreSum;
bool done = false;
bool bonus = false;
int lives = 3;
int bombs;

//create empty 2d array
void initialize(float *table, int m, int n)
{
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			*((table+i*n) + j) = 0.0;
		}
	}
} 

//print uncovered board for easier grading
void display(float *table, int m, int n)
{
	printf("%s\n","Numbers behind game board: ");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {	
			if(*((table+i*n) + j) == 200){
				printf("   *   ");
			}
			else if(*((table+i*n) + j) == 400){
				printf("   $   ");
			}
			else if(*((table+i*n) + j) == 600){
				printf("   @   ");
			}
			else if(*((table+i*n) + j) == 800){
				printf("   #   ");
			}
			else{
            	printf(" ");
        		printf("%+7.2f", *((table+i*n) + j));
			}
        }
        printf("\n");
    }
}

//print covered board for in game use
void gameDisplay(float *table, int m, int n){
	printf("%s\n", "Current game board: ");
	for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {	
			if( *((table+i*n)+j) == 100){
				printf(" ");
				printf("%s","+ ");
			}
			else if( *((table+i*n)+j) == -100){
				printf(" ");
				printf("%s","- ");
			}
			else if( *((table+i*n)+j) == 300){
				printf(" ");
				printf("%s","* ");
			}
			else if( *((table+i*n)+j) == 500){
				printf(" ");
				printf("%s","$ ");
			}
			else if( *((table+i*n)+j) == 700){
				printf(" ");
				printf("%s","@ ");
			}
			else if( *((table+i*n)+j) == 900){
				printf(" ");
				printf("%s","# ");
			}
			else{
            	printf(" ");
           	 	printf("%s", "X ");
			}
        }
        printf("\n");
    }
}

//place bomb at position and uncover surrounding area -- range = size of bomb detonated
void bomb(float *table, int M, int N, int m, int n,int range)
{
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if(abs(m-i) <= range && abs(n-j) <= range)
			{	
				//exit tile was uncovered
				if(*((table + i*N) + j) == 200){
					done = true;
					*((table + i*N) + j) = 300;
				}
				else if(*((table + i*N) + j) == 400){
					bonus = true;
					printf("Power Up! Next bomb has increased range!!\n");
					*((table + i*N) + j) = 500;
				}
				else if(*((table + i*N) + j) == 600){
					bombs = bombs + 1;
					printf("Power Up! You have recieved an extra bomb!!\n");
					*((table + i*N) + j) = 700;
				}
				else if(*((table + i*N) + j) == 800){
					lives = lives + 1;
					printf("Power Up! You have recieved an extra life!!\n");
					*((table + i*N) + j) = 900;
				}
				else if((*((table + i*N) + j) != 100) && (*((table + i*N) + j) != -100)){
					scoreSum = scoreSum + *((table + i*N) + j);

				}
				//change to impossible numbers to store that the area is uncovered, while keeping the sign
				if(*((table + i*N) + j) > 0 && *((table + i*N) + j) != 300 && *((table + i*N) + j) != 500 && *((table + i*N) + j) != 700&& *((table + i*N) + j) != 900){
					*((table + i*N) + j) = 100;
				}
				if(*((table + i*N) + j) < 0){
					*((table + i*N) + j) = -100;
				}
			}
		}
	}
}

//generate a random number
int randomNum(int min, int max){
    int num;
    num = rand()%(max-min)+min;
    return num;
}

//save score to log file "projectfile.log"
void logScore(char username,float score,int turns){
	
	FILE *fl;
    fl = fopen("projectfile.log","w");
	printf("%s",&username);
	fprintf(fl,"Username: %s   Score: %+7.2f   Turns: %d",&username,&score,&turns);
}

//main game loop and setup
int main(int argc, char const *argv[])
{	
	scoreSum = 0;
	char username[] = "";
    sscanf(argv[1],"%s",&username);
	int M = atoi(argv[2]);
	int N = atoi(argv[3]);
	if(M < 10 || N < 10){
		printf("Too small size\n");
		return(0);
	}
	srand(time(0));
	//printf("%s",username);

	//math for creating the grid
	float score[M][N];
	initialize((float*)score, M, N);
	int size = M * N;
	int half = (int)(0.5*M*N);
	int numNegtive = randomNum((int)half/2,half);
	int numPositive = size - numNegtive;
	bombs = (int)(0.10*M*N);


// initialize the positive scores
	int i = 0;
	while(i < numPositive)
	{
		int m = rand()%M;
		int n = rand()%N;
		if (score[m][n] == 0.0)
		{
			score[m][n] = fabs((float)(rand() & 0xf) - ((float)rand()/(float)(RAND_MAX))); //((float)rand()/(float)(RAND_MAX))*15;
			i++;
		}
	}
// initialize the negative scores
	i = 0;
	while(i < numNegtive)
	{
		int m = rand()%M;
		int n = rand()%N;
		if (score[m][n] == 0.0)
		{
			score[m][n] =  -1.0*fabs((float)(rand() & 0xf) - ((float)rand()/(float)(RAND_MAX))); //((float)rand()/(float)(RAND_MAX))*15;
			i++;
		}
	}
	printf("Total negative numbers: %d/%d \n",numNegtive,size);

	//exit tile(*) - if uncovered the game is over --- you won
	int x = randomNum(0,M);
	int y = randomNum(0,N);
	score[x][y] = 200;

	//double bonus($) - if uncovered the next bomb will deal double radius damage
	int doubleX = randomNum(0,M);
	int doubleY = randomNum(0,N);
	score[doubleX][doubleY] = 400;

	//extra bomb(@) - if uncovered the player gains another bomb
	int bombX = randomNum(0,M);
	int bombY = randomNum(0,N);
	score[bombX][bombY] = 600;
	bombX = randomNum(0,M);
	bombY = randomNum(0,N);
	score[bombX][bombY] = 600;

	//extra life(#) - if uncovered the player gains another life
	int lifeX = randomNum(0,M);
	int lifeY = randomNum(0,N);
	score[lifeX][lifeY] = 800;

	//diplay uncovered board and covered board
	display((float*)score, M, N);
	gameDisplay((float*)score, M, N);
	
	int counter = 1;
	int range = 1;
	int turns = 0;
	//game loop runs until exit tile is uncovered, score is less or equal to zero and zero lives, bombs are all used before exit tile is uncovered
	while(done == false){
		//end condition 1
		if(bombs == counter){
			done = true;
			break;
		}
		//check life
		if(lives < 0 && scoreSum < 0){
			done = true;
			break;
		}
		//procced with this turn
		printf("Bombs remaining: %d\n", bombs-counter);
		printf("Enter coordinates to place bomb: ");
		int m, n;
		scanf("%i %i", &m, &n);
		//stats for bonus
		if(bonus){
			range = 2;
			bomb(score, M, N, m, n,range);
			bonus = false;
			range = 1;
		}
		else{
			bomb(score, M, N, m, n,range);
		}
		//update board/outputs for user
		printf("%s","Total uncovered score is:");
		printf("%+7.2f\n",scoreSum);
		if(scoreSum < 0){
			lives = lives - 1;
			scoreSum = 0;
		}
		printf("Lives remaining: %d\n",lives);
		gameDisplay((float*)score, M, N);
		
		int c;
		printf("Do you wish to keep playing (0 = yes/1 = no");
		scanf("%d",&c);
		if(c == 1){
			break;
		}
		//increment counters
		turns = turns + 1;
		counter = counter + 1;
	}
	printf("game done\n");
	//logScore(username,scoreSum,turns);
	FILE *fl;
    fl = fopen("projectfile.log","w");
	fprintf(fl,"Username: %s   Score: %+7.2f   Turns: %d",&username,scoreSum,turns);
	return 0;
}