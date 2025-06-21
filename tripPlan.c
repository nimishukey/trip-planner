#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_LANDMARKS 20    // Maximum number of landmarks that can be visited in a day by walk
#define MAX_LANDMARK_NAME_LEN 32    // Maximum length of the landmark name is 31
#define MAX_FERRIES 20  // Maximum number of landmarks that can be visited in a day by ferry
#define INF INT_MAX // Represents infinity for Dijkstra's algorithm

// Structure for the name of the landmark
typedef struct {
    char name[MAX_LANDMARK_NAME_LEN];
} Landmark;

// Structure for a Walking Link
typedef struct {
    int from;
    int to;
    int time; // Walking time in minutes
} WalkingLink;

// Structure for a Ferry Schedules
typedef struct {
    int from;
    int to;
    int departureTime;
    int arrivalTime;
} FerrySchedule;

Landmark landmarks[MAX_LANDMARKS];
WalkingLink walkingLinks[MAX_LANDMARKS * MAX_LANDMARKS];
FerrySchedule ferrySchedules[MAX_FERRIES];
int n, m, p;

// Function to return the index of the landmark by name or -1 if not found
int getLandmarkIndex(char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(landmarks[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Prints time in HHMM format
void printTime(int time) {
    printf("%02d%02d", time / 60, time % 60);
}

void printPath(int *prev, int *arrivalTime, int current, int start) {
    if (current == start) {
        return;
    }
    printPath(prev, arrivalTime, prev[current], start);

    int prevTime = arrivalTime[prev[current]];
    int currTime = arrivalTime[current];
    int duration = currTime - prevTime;

    bool isFerry = false;
    int ferrydepartureTime = prevTime;
    for (int i = 0; i < p; i++) {
        if (ferrySchedules[i].from == prev[current] && ferrySchedules[i].to == current &&
            ferrySchedules[i].arrivalTime == currTime) {
            ferrydepartureTime = ferrySchedules[i].departureTime;
            duration = currTime - ferrydepartureTime;
            printf("Ferry %d minute(s):\n", duration);
            isFerry = true;
            break;
        }
    }
    
    if (!isFerry) {
        printf("Walk %d minute(s):\n", duration);
    }

    printf("  ");
    printTime(isFerry ? ferrydepartureTime : prevTime);
    printf(" %s\n", landmarks[prev[current]].name);
    printf("  ");
    printTime(currTime);
    printf(" %s\n", landmarks[current].name);
}

// Function to find a route and plan a trip
void findRoute(int start, int end, int departureTime) {
    int arrivalTime[MAX_LANDMARKS];
    int prev[MAX_LANDMARKS];
    bool visited[MAX_LANDMARKS] = {false};

    for (int i = 0; i < n; i++) {
        arrivalTime[i] = INF;
        prev[i] = -1;
    }
    arrivalTime[start] = departureTime;

    while (true) {
        int minTime = INF;
        int u = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && arrivalTime[i] < minTime) {
                minTime = arrivalTime[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        for (int i = 0; i < m; i++) {
            int v = -1;
            if (walkingLinks[i].from == u) {
                v = walkingLinks[i].to;
            } else if (walkingLinks[i].to == u) {
                v = walkingLinks[i].from;
            }
            if (v != -1 && arrivalTime[u] != INF && arrivalTime[u] + walkingLinks[i].time < arrivalTime[v]) {
                arrivalTime[v] = arrivalTime[u] + walkingLinks[i].time;
                prev[v] = u;
            }
        }

        for (int i = 0; i < p; i++) {
            if (ferrySchedules[i].from == u && arrivalTime[u] <= ferrySchedules[i].departureTime &&
                ferrySchedules[i].arrivalTime < arrivalTime[ferrySchedules[i].to]) {
                arrivalTime[ferrySchedules[i].to] = ferrySchedules[i].arrivalTime;
                prev[ferrySchedules[i].to] = u;
            }
        }
    }

    if (arrivalTime[end] != INF) {
        printPath(prev, arrivalTime, end, start);
    } else {
        printf("No route.\n");
    }
}

int main() {
    printf("Number of landmarks: ");
    scanf("%d", &n); 
    for (int i = 0; i < n; i++) {   // Accept "n" number of landmarks in the city
        scanf("%s", landmarks[i].name);
    }

    printf("Number of walking links: ");
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        char from[MAX_LANDMARK_NAME_LEN], to[MAX_LANDMARK_NAME_LEN];
        int time;
        scanf("%s %s %d", from, to, &time); // Read the 2 landmarks and walking time
        walkingLinks[i].from = getLandmarkIndex(from);  // Find the index of starting (departure) landmark
        walkingLinks[i].to = getLandmarkIndex(to);  // Find the index of arrival (destination) landmark
        walkingLinks[i].time = time;    // Store walking time
    }

    printf("Number of ferry schedules: ");
    scanf("%d", &p);
    for (int i = 0; i < p; i++) {
        char from[MAX_LANDMARK_NAME_LEN], to[MAX_LANDMARK_NAME_LEN];
        int departureTime, arrivalTime;
        scanf("%s %d %s %d", from, &departureTime, to, &arrivalTime);
        ferrySchedules[i].from = getLandmarkIndex(from);    // Find the index of the starting (departure) landmark
        ferrySchedules[i].to = getLandmarkIndex(to);    // Find the index of the arrival (destination) landmark
        ferrySchedules[i].departureTime = departureTime / 100 * 60 + departureTime % 100;   // Convert departure time from HHMM format to minutes
        ferrySchedules[i].arrivalTime = arrivalTime / 100 * 60 + arrivalTime % 100; // Convert arrival time from HHMM format to minutes
    }

    char from[MAX_LANDMARK_NAME_LEN], to[MAX_LANDMARK_NAME_LEN];
    int departureTime;
    // Handle trip planning queries
    while (1) {
        printf("\nFrom: ");
        scanf("%s", from);
        if (strcmp(from, "done") == 0) {
            printf("Happy travels!\n");
            break;
        }
        printf("To: ");
        scanf("%s", to);
        printf("Departure time: ");
        scanf("%d", &departureTime);
        printf("\n");

        int start = getLandmarkIndex(from);
        int end = getLandmarkIndex(to);

        // Check for invalid landmarks
        if (start == -1 || end == -1) {
            printf("Invalid landmark names.\n");
        } else {
            findRoute(start, end, departureTime / 100 * 60 + departureTime % 100);
        }
    }

    return 0;
}
