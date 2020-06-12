# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h> 
# include <string.h>

# define SIZE 10

typedef struct Data
{
    int id;
    char content[20];
} Data;

Data * data1 = NULL;
Data * data2 = NULL;
Data * data3 = NULL;
Data * data4 = NULL;
Data * data5 = NULL;

Data * create_data(int id, char content[] )
{
    Data * d = (Data *) malloc(sizeof(Data));
    d->id = id;
    strcpy(d->content,  content);

    printf("id:%d\tcontent:%s\n", id, content);
    return d;
}

void save_data()
{
    FILE * fp = fopen("data.txt", "w");
    Data out;
    printf("AAAA 1\n");
    out.id = data1->id;
    printf("AAAA 2\n");  
    strcpy(out.content, data1->content);  
    printf("AAAA 3\n");

    fwrite(&out, sizeof(Data), 1, fp);
    printf("AAAA 4\n");
    out.id = data2->id;  
    strcpy(out.content, data2->content);  
    printf("AAAA 5\n");
    fwrite(&out, sizeof(Data), 1, fp);
printf("AAAA 6\n");
    out.id = data3->id;  
    strcpy(out.content, data3->content); 
printf("AAAA 7\n");
    fwrite(&out, sizeof(Data), 1, fp);
printf("AAAA 8\n");
    out.id = data4->id;  
    strcpy(out.content, data4->content); 
printf("AAAA 9\n");
    fwrite(&out, sizeof(Data), 1, fp);
printf("AAAA 10\n");
    out.id = data5->id;  
    strcpy(out.content, data5->content);  
printf("AAAA 11\n");
    fwrite(&out, sizeof(Data), 1, fp);

    printf("AAAA 12\n");
    printf("Contents to file written successfully !\n");
    
    fclose(fp);
}

int restore_data()
{
    
    FILE * fp = fopen("data.txt", "r");
    if(fp == NULL)
    {
        printf("No data to read\n");
        return 1;
    }
    Data inp;
    fread(&inp, sizeof(Data),1, fp);
    printf("");
    data1 = create_data(inp.id, inp.content);
    fread(&inp, sizeof(Data),1, fp);
    data2 = create_data(inp.id, inp.content);
    fread(&inp, sizeof(Data),1, fp);
    data3 = create_data(inp.id, inp.content);
    fread(&inp, sizeof(Data),1, fp);
    data4 = create_data(inp.id, inp.content);
    fread(&inp, sizeof(Data),1, fp);
    data5 = create_data(inp.id, inp.content);
        
    fclose(fp);
    return 0;
}

void handle_sigterm(int sig) 
{ 
    printf("Saving data...  signal %d\n", sig); 
    
    save_data();

    printf("Bye...\n");
    exit(0);
}  

void leak()
{
    
    Data * aux = data5;
    
    data5 = (Data *) malloc(1024*SIZE);
    
    data5 = aux;
    
}

void populate()
{
    printf("Creating data...\n\n");
    data1 = create_data(1, "data1");
    data2 = create_data(2, "data2");
    data3 = create_data(3, "data3");
    data4 = create_data(4, "data4");
    data5 = create_data(5, "data5");
}
int main(int argc, char ** argv )
{
    printf("Arg:%d\t%s\n", argc, argv[1]);

    if(argc == 2 && strcmp(argv[1], "--restore") == 0)
    {
        if(restore_data() == 1){
            sleep(2);
            populate();
        }
        else
        {
            printf("Restaured data from file...\n\n");
            sleep(2);

            printf("Restored\n id:%d\tcontent:%s\n", data1->id, data1->content);
            printf("Restored\n id:%d\tcontent:%s\n", data2->id, data2->content);
            printf("Restored\n id:%d\tcontent:%s\n", data3->id, data3->content);
            printf("Restored\n id:%d\tcontent:%s\n", data4->id, data4->content);
            printf("Restored\n id:%d\tcontent:%s\n", data5->id, data5->content);
        }
    }
    else if(argc == 1)
    {
        printf("New!\n");
        sleep(2);
        populate();
    }
    else{
        printf("Invalid entry... \n");
        exit(1);
    }
    signal(SIGTERM, handle_sigterm);

    while(1)
    {
        leak();
        usleep(100);
    }
}