#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists Продолжайте проводить повторные розыгрыши, пока не выявится победитель
    while (true)
    {
        // Calculate votes given remaining candidates Подсчет голосов с учетом оставшихся кандидатов
        tabulate();

        // Check if election has been won Проверить, выиграли ли выборы
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates Устранение кандидатов, занявших последнее место
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
//Запишите предпочтение, если голос действителен
bool vote(int voter, int rank, string name)
{
    // TODO

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {


            preferences[voter][rank] = i;
            return true;

        }

    }
    return false;
}

// Tabulate votes for non-eliminated candidates
//Подсчет голосов за невыбывших кандидатов
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {

            if (candidates[preferences[i][j]].eliminated == false)
            {
                candidates[preferences[i][j]].votes += 1;
                break;
            }
        }

    }
    return;
}

// Print the winner of the election, if there is one
//Выведите победителя выборов, если он есть
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("%s\n",candidates[i].name);

        if (candidates[i].votes > (voter_count / 2))
        {

            printf("%s\n", candidates[i].name);
            return true;

        }

    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
//Возвращает минимальное количество голосов, которое есть у любого оставшегося кандидата.
int find_min(void)
{
    // TODO
    int loss = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes < loss)
        {
            loss = candidates[i].votes;
        }
    }

    return loss;
}

// Return true if the election is tied between all candidates, false otherwise
// Возвращает true, если выборы равны между всеми кандидатами, иначе false
bool is_tie(int min)
{
    int t1 = 0;
    int t2 = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes == min)
        {
            t1 += 1;
            t2 += 1;
        }
        else if (candidates[i].eliminated == false)
        {
            t2 += 1;
        }
    }
    if (t1 == t2)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
// Исключите кандидата (или кандидатов) на последнем месте
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}