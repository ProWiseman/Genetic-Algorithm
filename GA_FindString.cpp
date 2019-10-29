#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#define CHROMOSOME_CNT 50


int biggestFitPercentIndex;

void initChromosome(char(*chromosome)[50], int answerStringLen);
void fitness(char(*chromosome)[50], char* answerString, int answerStringLen, int *fitPercent);
void swap(int x, int y);
int biggestFitPercent(int fitPercent[]);
int addFitPercent(int *fitPercent);
int ranNumOfRoulette(int fitPercentSum, int cnt);
void selectChromosome(int* fitPercent, int fitPercentSum, char(*chromosome)[50], char(*parentChromosome)[50]);
void print(int generation, char(*chromosome)[50], int *fitPercent, char *answerString, int biggest);
void crossover(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen, int sequence);
void mutation(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen, int sequence);
void genOffspring(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen);


int main() {
	char chromosome[CHROMOSOME_CNT][50] = { 0, }, answerString[50], parentChromosome[CHROMOSOME_CNT][50] = { 0, };
	int fitPercent[CHROMOSOME_CNT] = { 0, }, fitPercentSum = 0;
	int generation = 1, answerStringLen, biggest;

	srand((unsigned)time(NULL));

	printf("찾을 문자열을 입력하세요 : ");
	answerStringLen = strlen(gets_s(answerString));

fitPrecentSumIsZero:
	initChromosome(chromosome, answerStringLen);

	fitness(chromosome, answerString, answerStringLen, fitPercent);

	fitPercentSum = addFitPercent(fitPercent);
	if (fitPercentSum == 0) {
		goto fitPrecentSumIsZero;
	}
	biggest = biggestFitPercent(fitPercent);

	print(generation, chromosome, fitPercent, answerString, biggest);

	while (biggest != 100) {

		selectChromosome(fitPercent, fitPercentSum, chromosome, parentChromosome);
		genOffspring(parentChromosome, chromosome, answerStringLen);

		generation++;

		fitness(chromosome, answerString, answerStringLen, fitPercent);
		fitPercentSum = addFitPercent(fitPercent);

		biggest = biggestFitPercent(fitPercent);

		print(generation, chromosome, fitPercent, answerString, biggest);
	}

	return 0;
}

void initChromosome(char(*chromosome)[50], int answerStringLen) {
	int i, j;

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		for (j = 0; j < answerStringLen; j++) {
			chromosome[i][j] = rand() % 94 + 32; // 아스키코드 값 32 ~ 126까지의 난수 생성
		}
	}
}

void fitness(char(*chromosome)[50], char* answerString, int answerStringLen, int *fitPercent) {
	int cnt, i, j;

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		cnt = 0;
		for (j = 0; j < answerStringLen; j++) {
			if (chromosome[i][j] == answerString[j]) {
				cnt++;
			}
		}
		fitPercent[i] = floor((cnt * 100. / answerStringLen) + 0.5); // fitPercent 소숫점 첫번째 자리에서 반올림
	}
}

void swap(int x, int y) {
	int t;
	t = x;
	x = y;
	y = t;
}

int biggestFitPercent(int fitPercent[]) {
	int m, t, i, j;
	int biggest[CHROMOSOME_CNT];

	biggestFitPercentIndex = 0;

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		biggest[i] = fitPercent[i];
	}

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		m = i;
		for (j = i; j < CHROMOSOME_CNT; j++) {
			if (biggest[m] > biggest[j]) {
				m = j;
			}
		}
		if (biggest[m] > biggest[biggestFitPercentIndex]) {
			biggestFitPercentIndex = m;
		}
		swap(biggest[i], biggest[m]);
	}

	return biggest[CHROMOSOME_CNT - 1];
}

int addFitPercent(int *fitPercent) {
	int i, sum = 0;

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		sum += fitPercent[i];
	}

	return sum;
}

int ranNumOfRoulette(int fitPercentSum, int cnt) {
	int random[CHROMOSOME_CNT], i;

	for (i = 0; i < CHROMOSOME_CNT; i++)
		random[i] = rand() % fitPercentSum + 1;
	return random[cnt];
}

void selectChromosome(int* fitPercent, int fitPercentSum, char(*chromosome)[50], char(*parentChromosome)[50]) {
	int rouletteNum, rouletteRange, i, j;

	for (i = 0; i < CHROMOSOME_CNT; i++) {
		rouletteNum = ranNumOfRoulette(fitPercentSum, i);
		for (j = 0, rouletteRange = 0; j < CHROMOSOME_CNT; j++) {
			if (rouletteRange < rouletteNum && rouletteNum <= fitPercent[j] + rouletteRange) {
				strcpy(parentChromosome[i], chromosome[j]);
				break;
			}
			rouletteRange += fitPercent[j];
		}
	}
}

void crossover(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen, int sequence) {
	int i, dividePoint = rand() % answerStringLen;
	char parent1[50], parent2[50];

	strcpy(parent1, parentChromosome[sequence]);
	strcpy(parent2, parentChromosome[sequence + 1]);

	for (i = 0; i < answerStringLen - dividePoint; i++) {
		parent1[i] = parent2[i];
	}

	strcpy(chromosome[sequence], parent1);

	strcpy(parent1, parentChromosome[sequence]);
	strcpy(parent2, parentChromosome[sequence + 1]);

	for (i = answerStringLen - dividePoint; i < answerStringLen; i++) {
		parent1[i] = parent2[i];
	}

	strcpy(chromosome[sequence + 1], parent1);
}

void mutation(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen, int sequence) {
	int i, random[50] = { 0, }, dividePoint = rand() % answerStringLen;
	char parent1[50];

	for (i = 0; i < answerStringLen; i++) {
		random[i] = rand() % 94 + 32;
	}

	strcpy(parent1, parentChromosome[sequence]);

	for (i = 0; i < answerStringLen - dividePoint; i++) {
		parent1[i] = random[i];
	}

	strcpy(chromosome[sequence], parent1);

	strcpy(parent1, parentChromosome[sequence + 1]);

	for (i = answerStringLen - dividePoint; i < answerStringLen; i++) {
		parent1[i] = random[i];
	}

	strcpy(chromosome[sequence + 1], parent1);
}

void genOffspring(char(*parentChromosome)[50], char(*chromosome)[50], int answerStringLen) {
	int chance[50] = { 0, }, i;

	for (i = 0; i < 50; i++) {
		chance[i] = rand() % 99;
	}

	for (i = 0; i < 50; i += 2) {
		if (chance[i] % 3 == 0) {
			mutation(parentChromosome, chromosome, answerStringLen, i);
		}
		else {
			crossover(parentChromosome, chromosome, answerStringLen, i);
		}
	}
}

void print(int generation, char(*chromosome)[50], int *fitPercent, char *answerString, int biggest) {
	int i;

	system("cls");

	printf("%d세대\n\n", generation);
	printf("찾으려는 문자열 : %s\t\t", answerString);
	printf("정답에 가장 유사한 문자열 : %s 정확도 : %3d%%\n\n", chromosome[biggestFitPercentIndex], biggest);
	for (i = 0; i < CHROMOSOME_CNT; i++) {
		printf("%2d번째 염색체 : %s 정확도 : %3d%%\t", i + 1, chromosome[i], fitPercent[i]);
		if ((i + 1) % 2 == 0) {
			printf("\n");
		}
	}
}