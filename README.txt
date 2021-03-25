README for CS 3013 project 4, Colby Frechette and Jason Dykstra

Project Description:

In our project, Colby Frechette and I tackled building a scheduler by using arrays to keep track of different aspects of the program. For FIFO and SJF we use an array to keep track of the lengths of jobs input by a text file, and use a separate array to keep track of the IDs of those jobs. For SJF specifically we do a pair sort on the job lengths and job IDs so that when job lengths are sorted they are still coupled with their corresponding job ID. After this initial sort, we go back through the array of job lengths and find segments where there are duplicates. For each segment, we take out the corresponding job IDs and sort them from lowest to highest, then re-insert them. This solves the issue where if three jobs have the same length, for example, the job with the lowest job ID will always come first. Our data is then processed and stored in a linked list of structs of type job, which contain and id and a length.

For RR, we split a job and re-add it to the end of the linked list of jobs as we are executing. This means that no matter how many times a job must be split, the execution will run until all requirements are satisfied. To do this we simply wrap the execution in a while loop that finishes when we reach the end of the linked list, so if we are always adding to the end of the linked list within the while loop, it will not finish until RR is done splitting all jobs appropriately.

For the analysis of each policy we store response times, turnaround times, and wait times in their own arrays. The index of these arrays corresponds to a job id, e.g. waitTimes[6] = 4 would mean job of id 6 has a wait time of 4. We go through the linked list which already has all the jobs in it (including the splits from RR in the execution phase) and update each list of times accordingly. We keep track of the overall time that has passed with a simple integer. After the entire list has been parsed, we sort the results by response time while making sure to keep the other times and job IDs coupled to their respective times. This is again done with a modified selection sort algorithm. While we do this, we add up the total times in order to calculate the averages, then print the jobs and their time details one at a time, followed by the averages for that policy.

Novel Workloads:
The first novel workload is simply a bunch of 3's. This results in the response time being the same as the wait time for RR with a time slice of 3.

The second workload makes FIFO's turnaround time roughly 10 times larger than SJF's turnaround time by having a very large first entry, followed by many small entries. In FIFO the many small entries will have a large turnaround time due to the first entry, but in SJF the small entries will go first, massively lowering the average turnaround time.

The third workload was the same approach as the first workload. By having all elements be 3, FIFO did not differ from SJF, and by having all elements be less than or equal to the time slice, RR acted the same as FIFO.

The fourth workload was similar to the second one in the sense that it has a very large first entry and smaller entries for the rest of the workload. Again, the large initial entry makes the turnaround time extremely large, while the smaller entries make the average wait time very low.

The fifth workload was a matter of tinkering until we got it to work. by manipulating the size of the second entry we could fine tune response time, and by fine tuning the third entry we could adjust turnaround time. 