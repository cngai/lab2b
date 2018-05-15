//NAME: Christopher Ngai
//EMAIL: cngai1223@gmail.com
//ID: 404795904

//lab2_list.c
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include "SortedList.h"

/********************/
/* GLOBAL VARIABLES */
/********************/

#define BILLION 1000000000L

//miscellaneous variables
int errno;
char temp_name[20] = "";	//holds test name printed out to stdout
char key_choices[53] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

//thread-related variables
int num_threads = 1;		//default is 1
int num_iterations = 1;		//default is 1
int num_lists = 1;
long long lock_time = 0;    //holds time thread waiting for lock in ns

//list-related variables
SortedList_t* sublist_array;
SortedListElement_t* list_elements;
int* sublist_num;           //holds number of sublist that specific element placed in

//mutex-related variables
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t* mutex_array;

//spin-lock variable
//int spin_val = 0;
int* spin_array;

//argument flags
int yield_flag = 0;
int opt_yield = 0;
char yield_opt[5] = "";
char sync_opt = 'n';		//initialized to 'none'

/********************/
/* HELPER FUNCTIONS */
/********************/

//if segmentation fault caught, report to stderr and exit
void signal_handler(){
    fprintf(stderr, "Error: caught segmentation fault. %s.\n", strerror(errno));
    exit(2);
}

//get test name based on parameters
char* get_test_name(){
    strcat(temp_name, "list");

    if (yield_flag){
        strcat(temp_name, "-");

        const char* yield_string = yield_opt;
        strcat(temp_name, yield_string);
    }
    else{
        strcat(temp_name, "-none");
    }
    if (sync_opt == 'n'){
        strcat(temp_name, "-none");
    }
    else if (sync_opt == 'm'){
        strcat(temp_name, "-m");
    }
    else if (sync_opt == 's'){
        strcat(temp_name, "-s");
    }

    return temp_name;
}

void initialize_keys(int total_elements){
    //generate random numbers
    srand((unsigned)time(NULL));

    int i, j;
    for (i = 0; i < total_elements; i++){
        //array that will hold random key of 1024 bytes
        char* key_val = malloc(1024 * sizeof(char));
        if (key_val == NULL){
            fprintf(stderr, "Error allocating memory for random key. %s.\n", strerror(errno));
            exit(1);
        }

        //generate 1024 random bytes for each key
        for (j = 0; j < 1024; j++){
            key_val[j] = key_choices[rand() % 52];
        }
        key_val[1023] = '\0';      //null character

        list_elements[i].key = key_val;
    }
}

//initialize all sublist heads
void initialize_sublists(){
    sublist_array = malloc(num_lists * sizeof(SortedList_t));
    if (sublist_array == NULL){
        fprintf(stderr, "Error allocating memory for sublist array. %s.\n", strerror(errno));
        exit(1);
    }
    int i;
    for (i = 0; i < num_lists; i++){
        sublist_array[i].key = NULL;
        sublist_array[i].next = &sublist_array[i];
        sublist_array[i].prev = &sublist_array[i];
    }
}

//initialize mutex/spin-locks
void initialize_locks(){
    if (sync_opt == 'm'){
        mutex_array = malloc(num_lists * sizeof(pthread_mutex_t));
        if (mutex_array == NULL){
            fprintf(stderr, "Error allocating memory for mutex array. %s.\n", strerror(errno));
            exit(1);
        }
        int i;
        for (i = 0; i < num_lists; i++){
            pthread_mutex_init(&mutex_array[i], NULL);
        }
    }
    if (sync_opt == 's'){
        spin_array = malloc(num_lists * sizeof(int));
        if (spin_array == NULL){
            fprintf(stderr, "Error allocating memory for spin-lock array. %s.\n", strerror(errno));
            exit(1);
        }
        int j;
        for (j = 0; j < num_lists; j++){
            spin_array[j] = 0;                          //initialize all spin values to 0
        }
    }
}

void* thread_list(void* ptr){
    int start = *(int*) ptr;
    int num_elements = num_threads * num_iterations;

    struct timespec lock_start, lock_end;

    //iterate through specific elements and insert them into list
    int i;
    for (i = start; i < num_elements; i += num_threads){
        //protect with mutex
        if (sync_opt == 'm'){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            pthread_mutex_lock(&mutex_array[sublist_num[i]]);
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);

            SortedList_insert(&sublist_array[sublist_num[i]], &list_elements[i]);
            pthread_mutex_unlock(&mutex_array[sublist_num[i]]);
        }
        //protect with spin-lock
        else if (sync_opt == 's'){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            while (__sync_lock_test_and_set(&spin_array[sublist_num[i]], 1));
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);

            SortedList_insert(&sublist_array[sublist_num[i]], &list_elements[i]);
            __sync_lock_release(&spin_array[sublist_num[i]]);
        }
        //no sync option
        else{
            SortedList_insert(&sublist_array[sublist_num[i]], &list_elements[i]);
        }
    }


    //get list length
    int list_length = 0;
    int k;
    //protect with mutex
    if (sync_opt == 'm'){
        for (k = 0; k < num_lists; k++){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            pthread_mutex_lock(&mutex_array[k]);
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);

            list_length = SortedList_length(&sublist_array[k]);
            pthread_mutex_unlock(&mutex_array[k]);
        }
    }
    //protect with spin-lock
    else if (sync_opt == 's'){
        for (k = 0; k < num_lists; k++){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            while (__sync_lock_test_and_set(&spin_array[k], 1));
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);

            list_length = SortedList_length(&sublist_array[k]);
            __sync_lock_release(&spin_array[k]);
        }
    }
    //no sync option
    else{
        for (k = 0; k < num_lists; k++){
            list_length = SortedList_length(&sublist_array[k]);
        }
    }
    if (list_length < 0){
        fprintf(stderr, "Error getting list length. %s.\n", strerror(errno));
        exit(2);
    }


    //look up and delete previously inserted key
    int j;
    SortedListElement_t* curr_element = NULL;
    for (j = start; j < num_elements; j += num_threads){
        //protect with mutex
        if (sync_opt == 'm'){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            pthread_mutex_lock(&mutex_array[sublist_num[j]]);
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);

            curr_element = SortedList_lookup(&sublist_array[sublist_num[j]], list_elements[j].key);
            if (curr_element == NULL){
                fprintf(stderr, "Error looking up element. %s.\n", strerror(errno));
                exit(2);
            }
            if (SortedList_delete(curr_element) == 1){
                fprintf(stderr, "Error deleting element. %s.\n", strerror(errno));
                exit(2);
            }

            pthread_mutex_unlock(&mutex_array[sublist_num[j]]);
        }
        //protect with spin-lock
        else if (sync_opt == 's'){
            if (clock_gettime(CLOCK_MONOTONIC, &lock_start) == -1){
                fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
                exit(1);
            }
            while (__sync_lock_test_and_set(&spin_array[sublist_num[j]], 1));
            if (clock_gettime(CLOCK_MONOTONIC, &lock_end) == -1){
                fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
                exit(1);
            }
            lock_time += BILLION * (lock_end.tv_sec - lock_start.tv_sec) + (lock_end.tv_nsec - lock_start.tv_nsec);
            
            curr_element = SortedList_lookup(&sublist_array[sublist_num[j]], list_elements[j].key);
            if (curr_element == NULL){
                fprintf(stderr, "Error looking up element. %s.\n", strerror(errno));
                exit(2);
            }
            if (SortedList_delete(curr_element) == 1){
                fprintf(stderr, "Error deleting element. %s.\n", strerror(errno));
                exit(2);
            }

            __sync_lock_release(&spin_array[sublist_num[j]]);
        }
        //no sync option
        else{
            curr_element = SortedList_lookup(&sublist_array[sublist_num[j]], list_elements[j].key);

            if (curr_element == NULL){
                fprintf(stderr, "Error looking up element. %s.\n", strerror(errno));
                exit(2);
            }

            if (SortedList_delete(curr_element) == 1){
                fprintf(stderr, "Error deleting element. %s.\n", strerror(errno));
                exit(2);
            }
        }
    }

    return NULL;
}

/*****************/
/* MAIN FUNCTION */
/*****************/

int main(int argc, char* argv[]){
	//hold getopt_long option
	int opt;
	static struct option long_options[] = {
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", required_argument, 0, 'y'},
		{"sync", required_argument, 0, 's'},
        {"lists", required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};

	while((opt = getopt_long(argc, argv, "t:i:y:s:l", long_options, NULL)) != -1){
    	switch(opt){
      		case 't':
        		num_threads = atoi(optarg);
        		break;
        	case 'i':
        		num_iterations = atoi(optarg);
        		break;
        	case 'y':
        		yield_flag = 1;

        		unsigned int i;
        		for (i = 0; i < strlen(optarg); i++){
        			if (optarg[i] == 'i'){
        				opt_yield |= INSERT_YIELD;
                        strcat(yield_opt, "i");
        			}
        			else if (optarg[i] == 'd'){
        				opt_yield |= DELETE_YIELD;
                        strcat(yield_opt, "d");
        			}
        			else if (optarg[i] == 'l'){
        				opt_yield |= LOOKUP_YIELD;
                        strcat(yield_opt, "l");
        			}
        			else{
        				fprintf(stderr, "Invalid yield option. %s.\n", strerror(errno));
        				exit(1);
        			}
        		}
        		break;
        	case 's':
        		//mutex
        		if (optarg[0] == 'm'){
        			sync_opt = 'm';
        		}
        		//spin-lock
        		else if (optarg[0] == 's'){
        			sync_opt = 's';
        		}
        		else{
        			fprintf(stderr, "Not a valid sync option. %s\n", strerror(errno));
        			exit(1);
        		}
        		break;
            case 'l':
                num_lists = atoi(optarg);
                break;
      		default:
       			fprintf(stderr, "Unrecognized argument.\n");
        		exit(1);
    	}
	}

    //catch segmentation fault
    signal(SIGSEGV, signal_handler);

	//initialize sublists
    initialize_sublists();

    //initialize mutexes/spin-locks
    initialize_locks();

    //create and initialize required number of list elements
    int num_elements = num_threads * num_iterations;
    list_elements = malloc(num_elements * sizeof(SortedListElement_t));
    if (list_elements == NULL){
        fprintf(stderr, "Error allocating memory for list elements. %s.\n", strerror(errno));
        exit(1);
    }
    initialize_keys(num_elements);

    //select which sublist a particular key should be in based on hash of key
    sublist_num = malloc(num_elements * sizeof(int));
    if (sublist_num == NULL){
        fprintf(stderr, "Error allocating memory for sublist number array. %s.\n", strerror(errno));
        exit(1);
    }
    int k;
    for (k = 0; k < num_elements; k++){
        sublist_num[k] = (*list_elements[k].key % num_lists);    //hash function based on key value % number of lists
    }

    //start timing for the run
    struct timespec start, end;

    //check if properly gets start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1){
        fprintf(stderr, "Error getting start clock time. %s.\n", strerror(errno));
        exit(1);
    }

    //initialize threads
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL){
        fprintf(stderr, "Error allocating memory for threads. %s.\n", strerror(errno));
        exit(1);
    }

    //array that holds thread ID numbers
    int* tids = malloc(num_threads * sizeof(int));
    if (tids == NULL){
        fprintf(stderr, "Error allocating memory for thread ID's. %s.\n", strerror(errno));
        exit(1);
    }

    //create threads
    int i;
    for (i = 0; i < num_threads; i++){
        tids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_list, &tids[i]) != 0){
            fprintf(stderr, "Error creating thread. %s.\n", strerror(errno));
            exit(1);
        }
    }

    //wait for threads to terminate
    int j;
    for (j = 0; j < num_threads; j++){
        if (pthread_join(threads[j], NULL) != 0){
            fprintf(stderr, "Error waiting for thread to terminate. %s.\n", strerror(errno));
            exit(1);
        }
    }

    //check if properly gets end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1){
        fprintf(stderr, "Error getting end clock time. %s.\n", strerror(errno));
        exit(1);
    }

    //check length of sublists to confirm it is zero
    int m;
    for (m = 0; m < num_lists; m++){
        if (SortedList_length(&sublist_array[m]) != 0){
            fprintf(stderr, "Error confirming length of sublist %d is zero. %s.\n", m, strerror(errno));
            exit(2);
        }
    }

    //print to stdout CSV record
    char* test_name = get_test_name();
    long long num_operations = num_threads * num_iterations * 3;    //insert, lookup, delete
    long long total_time = BILLION * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);   //in nanoseconds
    long long average_time = total_time / num_operations;
    long long average_lock_time = lock_time / num_operations;

    printf("%s,%d,%d,%d,%lld,%lld,%lld,%lld\n", test_name, num_threads, num_iterations, num_lists, num_operations, total_time, average_time, average_lock_time);

    //free allocated memory
    free(sublist_array);
    free(mutex_array);
    free(spin_array);
    free(list_elements);
    free(sublist_num);
    free(threads);
    free(tids);

    return 0;
}