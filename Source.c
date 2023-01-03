#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PATH_FOLDER "match_record\\"
#define SIZE 200

#pragma warning(disable: 4996)

typedef struct player {
	int runs;
	int outs;
	int oversBowled;
	char name[SIZE];
}player;

typedef struct team {
	int runs;
	int outs;
	char name[SIZE];
	player* players;
}team;

int findPlayer(char p[], team t, int teamSize);
team innings(team t1, team* t2, int format, int teamSize, int target);
void save(team t1, team t2, int format, int teamSize);
void scan(team* t1, team* t2, int* format, int* teamSize);
void result(team t1, team t2);
void showData(team t1, team t2, int format, int teamSize);
void strToLower(char* ptr);

int main() {
	int format = NULL, teamSize = NULL;
	team t1 = { NULL }, t2 = {NULL};
	
	int choice = 40, dataRead = 0;
	while (choice != 0)
	{
		printf("1: Enter Data\n2: Read Data\n3: Save Data\n4: Show Data\n5: Quit\n>> ");
		scanf_s("%d", &choice);
		switch (choice)
		{
		case 1:
			do
			{
				printf("How many overs? (100 MAX) \n>> ");
				scanf_s("%d", &format);
			} while (format <= 0 || format > 100);
			do
			{
				printf("How many players per team? (11 MAX; 2 MIN):\n>> ");
				scanf_s("%d", &teamSize);
			} while (teamSize <= 1 || teamSize > 11);

			t1.players = (player*)malloc(teamSize * sizeof(player));
			t2.players = (player*)malloc(teamSize * sizeof(player));

			do
			{
				printf("Enter batting first team name:\n>> ");
				gets_s(t1.name, SIZE);
			} while (t1.name[0] == ' ' || t1.name[0] == '\0');
			do
			{
				printf("Enter batting second team name:\n>> ");
				gets_s(t2.name, SIZE);
			} while (t2.name[0] == ' ' || t2.name[0] == '\0');

			strToLower(t1.name);
			strToLower(t2.name);

			player* startPoint = t1.players;
			for (int i = 0; i < teamSize; i++)
			{
				printf("Enter first team playing %d (in batting order):\n\n", teamSize);
				int index = -1;
				do
				{
					printf("Enter player %d name:\n>> ", i + 1);
					scanf_s("%s", t1.players->name, SIZE);
					strToLower(t1.players->name);
					/*index = findPlayer(t1.players->name, t1, i);
					if (index >= 0)
					{
						printf("Player already exists! %d\n", index);
					}*/
				} while (t1.players->name[0] == ' ' || t1.players->name[0] == '\0' || index >= 0);
				t1.players->runs = 0;
				t1.players->outs = 0;
				t1.players->oversBowled = 0;
				t1.players++;
			}
			t1.players = startPoint;

			startPoint = t2.players;
			for (int i = 0; i < teamSize; i++)
			{
				int index = -1;
				printf("Enter second team playing %d (in batting order):\n\n", teamSize);
				do
				{
					printf("Enter player %d name (no spaces):\n>> ", i + 1);
					scanf_s("%s", t2.players->name, SIZE);
					strToLower(t2.players->name);
					/*index = findPlayer(t2.players->name, t2, i);
					if (index >= 0)
					{
						printf("Player already exists!\n");
					}*/
				} while (t2.players->name[0] == ' ' || t2.players->name[0] == '\0' || index >= 0);
				t2.players->runs = 0;
				t2.players->outs = 0;
				t2.players->oversBowled = 0;
				t2.players++;
			}
			t2.players = startPoint;

			t1 = innings(t1, &t2, format, teamSize, -1);
			t2 = innings(t2, &t1, format, teamSize, t1.runs + 1);
			result(t1, t2);
			dataRead = 1;
			break;

		case 2:
			scan(&t1, &t2, &format, &teamSize);
			result(t1, t2);
			dataRead = 1;
			break;

		case 3:
			if (dataRead == 1)
			{
				save(t1, t2, format, teamSize);
				result(t1, t2);
			}
			break;

		case 4:
			if (dataRead == 1)
			{
				showData(t1, t2, format, teamSize);
			}
			break;

		case 5:
			choice = 0;
			break;

		default:
			printf("Invalid input\n");
			choice = 40;
			break;
		}
	}
}

int findPlayer(char p[], team t, int teamSize) {
	int matchingIndex = -1;
	for (int i = 0; i < teamSize; i++)
	{
		if (strcmp(p, t.players[i].name) == 0)
		{
			matchingIndex = i;
			break;
		}
	}
	return matchingIndex;
}

team innings(team t1, team *t2, int format, int teamSize, int target) {
	int batterIn = 0; //increment when a new batter comes to bat
	player* batter1 = &t1.players[batterIn];
	batterIn++;
	player* batter2 = &t1.players[batterIn], * bowler = NULL, * currentBatter = batter1;
	int previousBowlerIndex = -1;
	int index = -1;
	for (int ball = 0; ball < format * 6; ball++)
	{
		if (target > 0 && t1.runs >= target)
		{
			break;
		}
		//overs
		if (ball % 6 == 0)
		{
			char bowlerName[SIZE];
			previousBowlerIndex = index;
			do
			{
				printf("Enter bowler name (no spaces):\n>> ");
				scanf_s("%s", bowlerName, SIZE);
				strToLower(bowlerName);
				index = findPlayer(bowlerName, *t2, teamSize);
				if (index < 0)
				{
					printf("Player not found\n");
				}
				else if (index == previousBowlerIndex) {
					int innerChoice;
					printf("Bowler bowled previous over! Are you sure you want to allow the bowler to bowl? (1: yes; else: no)\n");
					scanf_s("%d", &innerChoice);
					if (innerChoice != 1)
					{
						index = -1;
					}
				}
				/*else
				{
					//*bowler = t2->players[index];
				}*/
			} while (bowlerName[0] == ' ' || bowlerName[0] == '\0' || index < 0);

			//crossing
			if (currentBatter == batter1 && ball != 0)
			{
				currentBatter = batter2;
			}
			else
			{
				currentBatter = batter1;
			}
			t2->players[index].oversBowled++;
		}
		//bowler->oversBowled++;

		//runs
		int runs, choice;
		do
		{
			printf("How many runs scored? (non-extra runs):\n>> ");
			scanf_s("%d", &runs);
		} while (runs < 0);
		t1.runs += runs;
		currentBatter->runs += runs;
		if (runs % 2 != 0)
		{
			if (currentBatter == batter1)
			{
				currentBatter = batter2;
			}
			else
			{
				currentBatter = batter1;
			}
		}

		//wides & no-balls
		do
		{
			printf("Were there any wides or no-balls? (1 = Yes; 2 = No):\n>> ");
			//they're the same bcz free hits are counted and run outs happen
			scanf_s("%d", &choice);
		} while (choice != 1 && choice != 2);
		if (choice == 1)
		{
			runs++;
			t1.runs++;
			ball--;
		}

		//extra runs
		do
		{
			printf("How many EXTRA runs scored? (exclude wide and no-ball penalty run but enter 4 if wide lead to boundary):\n>> ");
			scanf_s("%d", &runs);
		} while (runs < 0);
		t1.runs += runs;

		//outs
		do
		{
			printf("Were there any outs? (1 = Yes; 2 = No):\n>> ");
			//runouts still happen on no-balls and free hits
			scanf_s("%d", &choice);
		} while (choice != 1 && choice != 2);
		if (choice == 1)
		{
			batterIn++;
			t1.outs = t1.outs + 1;
			t2->players[index].outs = t2->players[index].outs + 1;
			//bowler->outs++;

			int innerChoice = 0;
			do
			{
				printf("Which batsman got out? (1: %s OR 2: %s)\n>> ", batter1->name, batter2->name);
				scanf_s("%d", &innerChoice);
			} while (innerChoice != 1 && innerChoice != 2);
			if (batterIn == teamSize)
			{
				printf("%s all out\n", t1.name);
				break;
			}
			if (innerChoice == 1)
			{
				batter1 = &t1.players[batterIn];
			}
			else
			{
				batter2 = &t1.players[batterIn];
			}
		}

		//penalty runs
		printf("How many penalty runs awarded? (Enter negative to give the runs to the bowling side):\n>> ");
		scanf_s("%d", &runs);
		if (runs >= 0)
		{
			t1.runs += runs;
		}
		else
		{
			t2->runs += -runs;
		}
		system("cls");
	}
	return t1;
}

void save(team t1, team t2, int format, int teamSize) {
	char fileName[SIZE], path[SIZE];
	FILE* file;
	int overWrite = 0;

	do
	{
		printf("Enter file name (without extension): ");
		gets_s(fileName, SIZE);
		sprintf(path, "%s%s.txt", PATH_FOLDER, fileName);
	} while (fileName[0] == '\0' || fileName[0] == ' ');

	if (fopen(path, "r") != NULL)
	{
		printf("File found! Overwrite it? (1: Yes; else: no)\n");
		scanf_s("%d", &overWrite);
		if (overWrite != 1)
		{
			return;
		}
	}

	file = fopen(path, "w");
	if (file == NULL)
	{
		printf("An error occured\n");
		fclose(file);
		return;
	}

	fprintf(file, "Format: %d\nTeam Size: %d\n", format, teamSize);

	//t1
	fprintf(file, "\nTeam: %s\n%d for %d\n", t1.name, t1.runs, t1.outs);
	for (int i = 0; i < teamSize; i++)
	{
		fprintf(file, "%s %d %d %d\n", t1.players[i].name, t1.players[i].runs, t1.players[i].oversBowled, t1.players[i].outs);
	}

	//t2
	fprintf(file, "\nTeam: %s\n%d for %d\n", t2.name, t2.runs, t2.outs);
	for (int i = 0; i < teamSize; i++)
	{
		fprintf(file, "%s %d %d %d\n", t2.players[i].name, t2.players[i].runs, t2.players[i].oversBowled, t2.players[i].outs);
	}
	fclose(file);
}

void scan(team *t1, team *t2, int *format, int *teamSize) {
	char fileName[SIZE], path[SIZE];
	FILE* file;
	do
	{
		printf("Enter file name (without extension): ");
		gets_s(fileName, SIZE);
		sprintf(path, "%s%s.txt", PATH_FOLDER, fileName);
		file = fopen(path, "r");
		if (file == NULL)
		{
			printf("File not found!\n");
		}
	} while (fileName[0] == '\0' || fileName[0] == ' ' || file == NULL);
	fscanf(file, "Format: %d\nTeam Size: %d\n", format, teamSize);

	t1->players = (player*)malloc(*teamSize * sizeof(player));
	t2->players = (player*)malloc(*teamSize * sizeof(player));

	//t1
	fscanf(file, "\nTeam: %s\n%d for %d\n", t1->name, &t1->runs, &t1->outs);
	for (int i = 0; i < *teamSize; i++)
	{
		fscanf(file, "%s %d %d %d\n", t1->players[i].name, &t1->players[i].runs, &t1->players[i].oversBowled, &t1->players[i].outs);
	}
	
	//t2
	fscanf(file, "\nTeam: %s\n%d for %d\n", t2->name, &t2->runs, &t2->outs);
	for (int i = 0; i < *teamSize; i++)
	{
		fscanf(file, "%s %d %d %d\n", t2->players[i].name, &t2->players[i].runs, &t2->players[i].oversBowled, &t2->players[i].outs);
	}
	
	fclose(file);
}

void result(team t1, team t2) {
	if (t1.runs > t2.runs)
	{
		printf("\n%s WON\n", t1.name);
	}
	else if (t2.runs > t1.runs)
	{
		printf("\n%s WON\n", t2.name);
	}
	else
	{
		printf("\nThe match is a draw\n");
	}
}

void showData(team t1, team t2, int format, int teamSize) {
	printf("Format: %d\nTeam Size: %d\n", format, teamSize);

	//t1
	printf("\nTeam: %s\n%d for %d\n", t1.name, t1.runs, t1.outs);
	for (int i = 0; i < teamSize; i++)
	{
		printf("Name: %s Runs: %d Overs: %d Wickets: %d\n", t1.players[i].name, t1.players[i].runs, t1.players[i].oversBowled, t1.players[i].outs);
	}

	//t2
	printf("\nTeam: %s\n%d for %d\n", t2.name, t2.runs, t2.outs);
	for (int i = 0; i < teamSize; i++)
	{
		printf("Name: %s Runs: %d Overs: %d Wickets: %d\n", t2.players[i].name, t2.players[i].runs, t2.players[i].oversBowled, t2.players[i].outs);
	}
}

void strToLower(char* ptr) {
	int i = 0;
	while (*ptr != '\0' && i < SIZE)
	{
		*ptr = tolower(*ptr);
		ptr++;
	}
}