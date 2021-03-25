#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct job {
    int id;
    int length;
    // Other meta data
    struct job *next;
} job;

struct job *head = NULL;

struct sjfNode {
        int id;
        int length;
};

void push(int id, int len) {
    struct job *current = head;

    if (current != NULL) {
        // Get to the end of the list
        while (current->next != NULL) {
            current = current->next;
        }

        // Add new variable
        current->next = (struct job *) malloc(sizeof(struct job));
        current->next->id = id;
        current->next->length = len;
        current->next->next = NULL;

    } else {
        head = (struct job *) malloc(sizeof(struct job));
        head->id = id;
        head->length = len;
    }
}

void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Function to perform Selection Sort for jobs with jobIds associated (pair sorting)
void selectionSortWithIds(int arr[], int ids[], int n) {
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
        swap(&ids[min_idx], &ids[i]);
    }
}

// Function to perform Selection Sort for a single array
void selectionSort(int arr[], int n) {
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

// Function to perform Selection Sort for all arrays involved in analysis, based off of response times
void analyzeSort(int responseTimes[], int jobIds[], int turnaroundTimes[], int waitTimes[], int n) {
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {

        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (responseTimes[j] < responseTimes[min_idx] && responseTimes[j] != -1)
                min_idx = j;

        // Swap the found minimum element
        // with the first element
        swap(&responseTimes[min_idx], &responseTimes[i]);
        swap(&jobIds[min_idx], &jobIds[i]);
        swap(&turnaroundTimes[min_idx], &turnaroundTimes[i]);
        swap(&waitTimes[min_idx], &waitTimes[i]);
    }
}

// "Zeroes" an array; fills it with -1
void fillArr(int arr[], int n, int value){
    for(int i = 0; i < n; i++){
        arr[i] = value;
    }
}

void arrPush(int arr[], int num){
    int counter = 0;

    while(arr[counter] != -1){
        counter++;
    }
    arr[counter] = num;
}

void printArr(int arr[], int n){
    int counter = 0;
    while(arr[counter] != -1 && counter < n){
        printf(" %d", arr[counter]);
        counter++;
    }
}

int appearsAgain(struct job *currJob){
    int id = currJob->id;
    while(currJob->next != NULL){
        currJob = currJob->next;
        // If a job further in the linked list has the same ID, this job appears again
        if(currJob->id == id){
            return 1;
        }
    }

    // If no jobs further in the linked list have the same ID, this job does not appear again
    return 0;
}



int main(int argc, char **argv) {
    // Scheduler takes in three command line arguments:
    // Policy name: type of scheduling policy
    // Job trace: the location of the file for input job numbers
    // Time slice: only used for round robin

    char *policyName = argv[1];
    char *jobTrace = argv[2];
    char *timeSlice = argv[3];

    // Read the file and record jobs
    FILE *fp;
    char *line = "";
    size_t len = 0;
    ssize_t read;

    // TODO: Change file path when we use makefile
    // TODO: make makefile
    if ((fp = fopen(jobTrace, "r")) == NULL) {
        printf("Can't find file.\n");
        exit(1);
    }

    int jobIdCounter = 0;
    int n = 200;

    int jobArray[n];
    int jobIdArray[n];

    for (int i = 0; i < n; i++) {
        jobArray[i] = -1;
    }


    while ((read = getline(&line, &len, fp)) != -1) {
        jobArray[jobIdCounter] = atoi(line);
        jobIdCounter++;
    }

    for (int i = 0; i < n; i++) {
        if (i < jobIdCounter) jobIdArray[i] = i;
        else jobIdArray[i] = -1;
    }

    if (strcmp(policyName, "SJF") == 0) {
        selectionSortWithIds(jobArray, jobIdArray, jobIdCounter);

        // Where there are duplicates in jobArray, sort the job Ids associated with them
        int unique = -1;
        int startIndex = 0;
        int ids[n];
        //int counter = 0;

        fillArr(ids, n, -1);

        // Loop over arr1
        for(int i = 0; i < n; i++){
            // If you come across a new unique value
            if(jobArray[i] != unique){

                // Handle previous ids before resetting unique and array
                if(ids[1] != -1){
                    int idsSize = 0;
                    while(ids[idsSize] != -1) idsSize++;

                    // Sort the ids array list
                    selectionSort(ids, idsSize);

                    // Loop over real array of IDs and overwrite with new sorted IDs
                    for(int j = startIndex; j < idsSize+startIndex; j++){
                        jobIdArray[j] = ids[j-startIndex];
                    }
                }

                // Set unique to the new value, zero the ids array, and reset start index at current position
                unique = jobArray[i];
                fillArr(ids, n, -1);
                arrPush(ids, jobIdArray[i]);
                startIndex = i;
            } else {
                // If the value is the same as the unique value
                arrPush(ids, jobIdArray[i]);
            }
        }
    }





    jobIdCounter = 0;


    while (jobArray[jobIdCounter] != -1) {
        // Implement RR
        if (strcmp(policyName, "SJF") == 0) {
            push(jobIdArray[jobIdCounter], jobArray[jobIdCounter]);
        } else {
            push(jobIdCounter, jobArray[jobIdCounter]);
        }

        jobIdCounter++;
    }

    fclose(fp);
    if (line) {
        free(line);
    }

    struct job *runningJob;
    runningJob = head;
    int timeSliceInt = atoi(timeSlice);
    printf("Execution trace with %s:\n", policyName);


    while (runningJob != NULL) {

        if (strcmp(policyName, "RR") == 0) {
            // If job is more than timeslice, split it up and add it to the end of the list
            if (runningJob->length > timeSliceInt) {
                push(runningJob->id, (runningJob->length - timeSliceInt));
                runningJob->length = timeSliceInt;
            }
        }

        usleep(runningJob->length);
        printf("Job %d ran for: %d\n", runningJob->id, runningJob->length);
        runningJob = runningJob->next;

    }
    printf("End of execution with %s.\n", policyName);while (runningJob != NULL) {

        // Adjust length if policy is RR
        if (strcmp(policyName, "RR") == 0) {
            // If job is more than timeslice, split it up and add it to the end of the list
            if (runningJob->length > timeSliceInt) {
                push(runningJob->id, (runningJob->length - timeSliceInt));
                runningJob->length = timeSliceInt;
            }
        }

        // Increment t

        usleep(runningJob->length);
        printf("Job %d ran for: %d\n", runningJob->id, runningJob->length);
        runningJob = runningJob->next;

    }

    /*
     * Policy Analysis Glossary
     * T = 0 (starting time for all jobs is 0)
     * Ts = the time at which a job starts running
     * Tc = the time at which a job finishes running
     * Response time = Ts - T. How long a job had to wait before starting
     * Turnaround time = Tc - T. How long a job had to wait before finishing
     * Wait time = time spent waiting and not running. For FIFO and SJF wait time = response time.
     */

    // Counter to keep track of total "time" elapsed
    int t = 0;

    // Make three arrays for all the times we need to record
    int responseTimes[n];
    int turnaroundTimes[n];
    int waitTimes[n];

    // Array to keep track of whether or not a RR job has already appeared once
    int numOccurrences[n];

    // Initialize the arrays
    fillArr(responseTimes, n, -1);
    fillArr(turnaroundTimes, n, -1);
    fillArr(waitTimes, n, -1);
    fillArr(numOccurrences, n, 0);

    printf("Begin analyzing %s:\n", policyName);

    // Perform the jobs and fill the time arrays, THEN print the results after
    // This prevents us from having to make a special condition for RR to wait for all jobs to finish before printing

    // Reset runningJob to head
    runningJob = head;

    // Re-initialize job ids
    for (int i = 0; i < n; i++) {
        if (i < jobIdCounter) jobIdArray[i] = i;
        else jobIdArray[i] = -1;
    }

    // What we have now is a linked list of all jobs, with RR operations already performed if policy is RR.
    while (runningJob != NULL) {

        // "Perform" the job
        usleep(runningJob->length);

        // Update response and wait times before incrementing t
        // If policy is RR, only update response time if it's the first time the job is run
        if(strcmp(policyName, "RR") == 0){
            if(numOccurrences[runningJob->id] == 0){
                responseTimes[runningJob->id] = t;

            }
        } else {
            responseTimes[runningJob->id] = t;
        }

        // Increment the number of occurrences of the job
        numOccurrences[runningJob->id] += 1;

        // Update wait times, this works for any policy
        if(waitTimes[runningJob->id] == -1){
            waitTimes[runningJob->id] = t;
        } else {
            // If previous wait time was 3 for example, and time slice was 5, and program ran for second time at t = 10
            // wait time previously stored would be 3, need to add 2 which is t (10) - previous wait time (3) - time slice (5) = 2
            waitTimes[runningJob->id] += (t - waitTimes[runningJob->id] - timeSliceInt);
            // Must take into account how many times the number has appeared in the past
            // If this is the third occurrence of the number, we need to account for the time it ran in the past TWICE, if it is the fourth occurrence, we need to account for timeSlice three times, and so on.
            if(numOccurrences[runningJob->id] - 2 >= 1)
                // Have to do numOccurrences - 2 since we increment numOccurrences right before we check it
                waitTimes[runningJob->id] -= ((numOccurrences[runningJob->id] - 2)*timeSliceInt);
        }

        // Increment t
        t += runningJob->length;

        // Update turnaround times
        // If this was the last appearance for RR, add the time to the job's turnaround time
        if(strcmp(policyName, "RR") == 0){
            if(appearsAgain(runningJob) == 0){
                turnaroundTimes[runningJob->id] = t;
            }
        } else {
            // If the policy is not RR, no need to check if it appears again because it won't
            turnaroundTimes[runningJob->id] = t;
        }

        // Go to next job
        runningJob = runningJob->next;
    }

    // Sort results by response time to get the order that jobs ran in
    analyzeSort(responseTimes, jobIdArray, turnaroundTimes, waitTimes, n);

    // Print the results while adding times to totals for calculating average later
    int totalResponseTime = 0;
    int totalTurnaroundTime = 0;
    int totalWaitTime = 0;
    int counter = 0;
    while(waitTimes[counter] != -1){
        printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n", jobIdArray[counter], responseTimes[counter],turnaroundTimes[counter],waitTimes[counter]);
        totalResponseTime += responseTimes[counter];
        totalTurnaroundTime += turnaroundTimes[counter];
        totalWaitTime += waitTimes[counter];
        counter++;
    }

    // Calculate and print average times
    float averageResponseTime = (float)totalResponseTime/(float)counter;
    float averageTurnaroundTime = (float)totalTurnaroundTime/(float)counter;
    float averageWaitTime = (float)totalWaitTime/(float)counter;

    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n", averageResponseTime, averageTurnaroundTime, averageWaitTime);

    printf("End analyzing %s.\n", policyName);


}

