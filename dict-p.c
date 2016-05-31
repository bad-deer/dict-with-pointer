#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dict
{
    char *key;
    char *content;
    struct dict *next;  //pointer chain
};
// function list
int open_file(struct dict **p, char *file_name);          //two level pointer needed, tried to build pointer chain and the same data struct
int search_file(const struct dict *p, const char *key, char *content);
void dict_free(struct dict *p);


int main(int argc, char *args[]) // array of pointers to store the string in the openning process
{
    if (argc < 2){
        printf("usage: %s is not right\n", args[0]);
        return 0;
    }
    long start_ms = 0;
    long end_ms = 0;

    struct dict *p = NULL;    //the head of the chain; set an empty pointer 
    int size = 0;
    start_ms = clock();
    size = open_file(&p, args[1]);   //to open the dict_file to heap memory
    if (size == 0)
        return 0;
    end_ms = clock();
    printf("loading time used: %ld\n",end_ms - start_ms);
    char key[1024] = {0};
    char content[1024] = {0};
    while(1){
        memset(key, 0, sizeof(key));
        memset(content, 0, sizeof(content));
        printf("Please input the word:");
        scanf("%s", key);
        if(!strncmp(key,"command=exit",12))
            break;
        start_ms = clock();

        if(search_file(p,key,content)){
                printf("%s\n", content);
        }else{
            printf("not found\n");
        }
        end_ms = clock();
        printf("finding time used: %ld",end_ms - start_ms);
    }
    start_ms = clock();
    dict_free(p);
    end_ms = clock();
    printf("free time used %ld", end_ms - start_ms);

    return 0;
}

//function accomplishment
int open_file(struct dict **p, char *file_name)
{
    FILE *pfile = fopen(file_name,"r");
    if(pfile == NULL) //to make sure we get the data
        return 0;
    *p = (struct dict *)malloc(sizeof(struct dict)); //create a heap to store struct dict chain
    memset(*p, 0, sizeof(struct dict));
    struct dict *pd = *p;

    char buf[2048] = {0};//to store the data by different size;
    int i = 0;
    size_t len = 0;
    while(!feof(pfile)){
        memset(buf, 0, sizeof(buf));
        fgets(buf,sizeof(buf),pfile);
        len = strlen(buf);
        if(len > 0){
            pd->key = (char *)malloc(len);
            memset(pd->key, 0, len);
            strcpy(pd->key,&buf[1]);
        }

        memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), pfile);
            len = strlen(buf);

        if (len > 0){
            pd->content = (char *)malloc(len);
            memset(pd->content, 0, len);
            strcpy(pd->content, &buf[6]);
        }
        pd->next = (struct dict *)malloc(sizeof(struct dict)); //build next pointer of the chain;
        memset(pd->next, 0, sizeof(struct dict));

        pd = pd->next;//the way we build pointer chain;
        i++;
    }
    fclose(pfile);
    return i;
}

int search_file(const struct dict *p, const char *key, char *content)
{
    const struct dict *pd = p;
    while(pd){
            if((pd->key)&&(pd->content)){
                 if(strncmp(key,pd->key,strlen(key))){
                    pd = pd->next;
                 }else{
                    strcpy(content,pd->content);
                    return 1;
                }
            }

    }
    return 0;
}

void dict_free(struct dict *p) // free every heap you assigned
{
    struct dict *pd = p;
    while(pd->next){
        if(pd->key)
            free(pd->key);
        if(pd->content)
            free(pd->content);

    struct dict *tmp;
        tmp = pd->next;
        free(pd);
        pd = tmp;
    }
}

















