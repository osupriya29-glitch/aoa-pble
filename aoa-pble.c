#include <stdio.h>
#include <string.h>

#define MAX 20
#define INF 999999

int graph[MAX][MAX];
char location[MAX][50];
int n;

int distanceArr[MAX];
int visited[MAX];
int parent[MAX];


/* Function to enter campus graph */
void enterGraph()
{
    int edges;
    int i, u, v, weight;

    printf("\nEnter number of locations (maximum %d): ", MAX);
    scanf("%d", &n);
    getchar();

    if (n <= 0 || n > MAX)
    {
        printf("Invalid number of locations!\n");
        return;
    }

    /* Enter location names */
    for (i = 0; i < n; i++)
    {
        printf("Enter name of location %d: ", i + 1);
        fgets(location[i], sizeof(location[i]), stdin);

        location[i][strcspn(location[i], "\n")] = '\0';
    }

    /* Initialize adjacency matrix */
    for (i = 0; i < n; i++)
    {
        int j;

        for (j = 0; j < n; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    printf("\nEnter number of roads: ");
    scanf("%d", &edges);

    printf("\nEnter road details:\n");
    printf("Format: Source_Number Destination_Number Distance\n");

    for (i = 0; i < edges; i++)
    {
        printf("Road %d: ", i + 1);
        scanf("%d %d %d", &u, &v, &weight);

        if (u < 1 || u > n || v < 1 || v > n || weight < 0)
        {
            printf("Invalid road details! Try again.\n");
            i--;
            continue;
        }

        /*
           Campus roads are considered undirected.
           Therefore, both directions are stored.
        */
        graph[u - 1][v - 1] = weight;
        graph[v - 1][u - 1] = weight;
    }

    printf("\nCampus graph created successfully!\n");
}


/* Function to display adjacency matrix */
void displayMatrix()
{
    int i, j;

    if (n <= 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    printf("\n========== ADJACENCY MATRIX ==========\n\n");

    printf("%-20s", "");

    for (i = 0; i < n; i++)
    {
        printf("%-15d", i + 1);
    }

    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("%-20d", i + 1);

        for (j = 0; j < n; j++)
        {
            if (graph[i][j] == INF)
                printf("%-15s", "INF");
            else
                printf("%-15d", graph[i][j]);
        }

        printf("\n");
    }

    printf("\nLocation Numbers:\n");

    for (i = 0; i < n; i++)
    {
        printf("%d. %s\n", i + 1, location[i]);
    }
}


/* Find vertex with minimum distance */
int findMinimum()
{
    int min = INF;
    int minIndex = -1;
    int i;

    for (i = 0; i < n; i++)
    {
        if (!visited[i] && distanceArr[i] < min)
        {
            min = distanceArr[i];
            minIndex = i;
        }
    }

    return minIndex;
}


/* Dijkstra's Algorithm */
void dijkstra(int source)
{
    int i, j;

    /* Initialize arrays */
    for (i = 0; i < n; i++)
    {
        distanceArr[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    distanceArr[source] = 0;

    /* Dijkstra algorithm */
    for (i = 0; i < n; i++)
    {
        int u = findMinimum();

        if (u == -1)
            break;

        visited[u] = 1;

        for (j = 0; j < n; j++)
        {
            if (!visited[j] &&
                graph[u][j] != INF &&
                distanceArr[u] != INF &&
                distanceArr[u] + graph[u][j] < distanceArr[j])
            {
                distanceArr[j] = distanceArr[u] + graph[u][j];
                parent[j] = u;
            }
        }
    }
}


/* Print shortest path recursively */
void printPath(int vertex)
{
    if (parent[vertex] == -1)
    {
        printf("%s", location[vertex]);
        return;
    }

    printPath(parent[vertex]);
    printf(" -> %s", location[vertex]);
}


/* Select source location */
int selectSource()
{
    int source;

    if (n <= 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return -1;
    }

    printf("\n========== CAMPUS LOCATIONS ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d. %s\n", i + 1, location[i]);
    }

    printf("\nEnter source location number: ");
    scanf("%d", &source);

    if (source < 1 || source > n)
    {
        printf("Invalid source location!\n");
        return -1;
    }

    return source - 1;
}


/* Display shortest paths */
void displayShortestPaths(int source)
{
    int i;

    printf("\n========== SHORTEST PATHS ==========\n");

    printf("Source: %s\n\n", location[source]);

    for (i = 0; i < n; i++)
    {
        if (i == source)
            continue;

        printf("Destination: %s\n", location[i]);

        if (distanceArr[i] == INF)
        {
            printf("Shortest Distance: Not Reachable\n");
            printf("Shortest Path: No path exists\n");
        }
        else
        {
            printf("Shortest Distance: %d\n", distanceArr[i]);

            printf("Shortest Path: ");
            printPath(i);
            printf("\n");
        }

        printf("\n");
    }
}


/* Display only shortest distances */
void displayDistances(int source)
{
    int i;

    printf("\n========== DISTANCES FROM SOURCE ==========\n");

    printf("Source: %s\n\n", location[source]);

    printf("%-25s %-20s\n", "Destination", "Shortest Distance");
    printf("------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        if (i == source)
            continue;

        if (distanceArr[i] == INF)
            printf("%-25s %-20s\n",
                   location[i], "Not Reachable");
        else
            printf("%-25s %-20d\n",
                   location[i], distanceArr[i]);
    }
}


/* Main function */
int main()
{
    int choice;
    int source = -1;
    int graphEntered = 0;
    int shortestPathCalculated = 0;

    n = 0;

    do
    {
        printf("\n\n============================================\n");
        printf("     CAMPUS SHORTEST ROUTE FINDER\n");
        printf("          DIJKSTRA'S ALGORITHM\n");
        printf("============================================\n");

        printf("\n1. Enter Campus Graph");
        printf("\n2. Display Adjacency Matrix");
        printf("\n3. Select Source Location");
        printf("\n4. Find Shortest Distance");
        printf("\n5. Display Shortest Paths");
        printf("\n6. Display Distance from Source to All Locations");
        printf("\n7. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                enterGraph();
                graphEntered = 1;
                shortestPathCalculated = 0;
                source = -1;
                break;

            case 2:
                if (!graphEntered)
                {
                    printf("\nPlease enter the campus graph first.\n");
                }
                else
                {
                    displayMatrix();
                }
                break;

            case 3:
                if (!graphEntered)
                {
                    printf("\nPlease enter the campus graph first.\n");
                }
                else
                {
                    source = selectSource();

                    if (source != -1)
                    {
                        printf("\nSource selected: %s\n",
                               location[source]);

                        shortestPathCalculated = 0;
                    }
                }
                break;

            case 4:
                if (!graphEntered)
                {
                    printf("\nPlease enter the campus graph first.\n");
                }
                else if (source == -1)
                {
                    printf("\nPlease select a source location first.\n");
                }
                else
                {
                    dijkstra(source);

                    shortestPathCalculated = 1;

                    printf("\nShortest distances calculated successfully!\n");
                }
                break;

            case 5:
                if (!graphEntered)
                {
                    printf("\nPlease enter the campus graph first.\n");
                }
                else if (source == -1)
                {
                    printf("\nPlease select a source location first.\n");
                }
                else if (!shortestPathCalculated)
                {
                    printf("\nPlease find the shortest distance first.\n");
                }
                else
                {
                    displayShortestPaths(source);
                }
                break;

            case 6:
                if (!graphEntered)
                {
                    printf("\nPlease enter the campus graph first.\n");
                }
                else if (source == -1)
                {
                    printf("\nPlease select a source location first.\n");
                }
                else if (!shortestPathCalculated)
                {
                    printf("\nPlease find the shortest distance first.\n");
                }
                else
                {
                    displayDistances(source);
                }
                break;

            case 7:
                printf("\nThank you for using Campus Shortest Route Finder!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}
