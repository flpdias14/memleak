# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define SIZE 10
float* vector;


int main()
{
    int count = 0;
    long int allocaded_size = 0;
    while(1)
    {
        allocaded_size += 1024*SIZE;
        vector = (float*) malloc(1024*SIZE);
        // printf("Malloc %d\tsizeof = %d bytes\n", ++count, allocaded_size);
        //  sleep(1);
        usleep(10000);
        
    }
}