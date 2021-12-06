#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct dynamic_element{
    char *time;
    struct dynamic_element *pointer;
};

void push(char *time, struct dynamic_element **top){
    struct dynamic_element *element = (struct dynamic_element*)malloc(sizeof(struct dynamic_element));
    (**top).pointer = element;
    (**top).time = (char*)malloc(26 * sizeof(char));
    strcpy((**top).time, time);
    *top = element;
}

char* pop(struct dynamic_element **tail){
    struct dynamic_element *new_tail;
    new_tail = (**tail).pointer;
    char *time;
    strcpy(time, (**tail).time);
    free((*tail)->time);
    free(*tail);
    *tail = new_tail;
    return time;
}

struct bool_t{
    unsigned short value : 1;
};

unsigned get_TimeDiff(char* time1, char *time2){
    short sec1 = (int)(time1[19] - '0') + ((int)(time1[18] - '0') * 10);
    short sec2 = (int)(time2[19] - '0') + ((int)(time2[18] - '0') * 10);
    short min1 = (int)(time1[16] - '0') + ((int)(time1[15] - '0') * 10);
    short min2 = (int)(time2[16] - '0') + ((int)(time2[15] - '0') * 10);
    short hou1 = (int)(time1[13] - '0') + ((int)(time1[12] - '0') * 10);
    short hou2 = (int)(time2[13] - '0') + ((int)(time2[12] - '0') * 10);
    short day1 = (int)(time1[1] - '0') + ((int)(time1[0] - '0') * 10);
    short day2 = (int)(time2[1] - '0') + ((int)(time2[0] - '0') * 10);
    if (sec1 >= 60){
        min1++;
        sec1 -= 60;
    }
    if (sec2 >= 60){
        min2++;
        sec2 -= 60;
    }
    if (min1 >= 60){
        hou1++;
        min1 -= 60;
    }
    if (min2 >= 60){
        hou2++;
        min2 -= 60;
    }
    if (hou1 >= 24){
        day1++;
        hou1 -= 24;
    }
    if (hou2 >= 24){
        day2++;
        hou2 -= 24;
    }
    return (sec2 - sec1) + (min2 - min1) * 60 + (hou2 - hou1) * 3600 + (day2 - day1) * 86400;
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Error, File Not Found\n");
        exit(2);
    }

    FILE *log = fopen(argv[1], "r+");

    if (fgetc(log) && feof(log) != 0){
        printf("File is empty\n");
        exit(4);
    }

    fseek(log, 0, 2);
    fprintf(log, '\0');
    fseek(log, 0, 0);

    FILE *Errors_5xx = fopen("Errors.txt", "w");
    FILE *Time_Window;
    long ConstTimeDiff;

    if (argc > 2){
        ConstTimeDiff = atoi(argv[2]);
        Time_Window = fopen("Time_Window.txt", "w");
    }
    else
        ConstTimeDiff = -1;

    struct dynamic_element *top = (struct dynamic_element*)malloc(sizeof(struct dynamic_element));
    struct dynamic_element *tail = top;
    char *line = (char*)calloc(400, sizeof(char));
    char *local_time = (char*)calloc(30, sizeof(char));
    char *act_local_time = (char*)calloc(30, sizeof(char));
    char *check_local_time = (char*)calloc(30, sizeof(char));
    char *status = (char*)calloc(5, sizeof(char));
    char letter;
    int len = 0;
    long num_line = 0;
    short i;
    short l;
    short s;
    int Count5xxError = 0;
    char *save_time = (char*)calloc(26, sizeof(char));
    int *count_of_equals_time = (int*)calloc(ConstTimeDiff + 2, sizeof(int));
    int pc = 0;
    int rc = 0;
    unsigned long num_of_line = 0;
    unsigned long TimeWindowCount = 1;
    unsigned long MaxTimeWindowCount = 1;
    long count = 1;
    long TimeDiff;
    long PrevTimeDiff = -1;
    long SaveTimeDiff = 0;
    struct bool_t start_line;
    start_line.value = 1;
    struct bool_t check;
    check.value = 1;

    while (!feof(log)){
        line[0] = '\0';
        local_time[0] = '\0';
        status[0] = '\0';
        fgets(line, 400, log);
        i = 0;
        l = 0;
        s = 0;
        letter = line[i++];
        while (letter != '[' && letter != '\0'){
            letter = line[i++];
        }
        if (letter == '\0')
            break;
        letter = line[i++];

        while (letter != ']' && letter != '\0'){
            local_time[l++] = letter; 
            letter = line[i++];
        }
        local_time[l] = '\0';
        if (letter == '\0')
            break;
        i += 2;
        letter = line[i++];

        while (letter != '"' && letter != '\0')
            letter = line[i++];
        if (letter == '\0')
            break;
        i++;
        letter = line[i++];

        while (letter != ' ' && letter != '\0'){
            status[s++] = letter;
            letter = line[i++];
        }
        status[s] = '\0';
        if (letter == '\0')
            break;
        letter = line[i++];

        while (letter != '\n' && letter != '\0')
            letter = line[i++];
        len = i - 1;
        line[len] = '\0';

        num_line++;

        if (status[0] == '5'){
            Count5xxError++;
            fprintf(Errors_5xx, "%s", line);
            fprintf(Errors_5xx, "   lenght = %d\n", len);
        }

        if (ConstTimeDiff < 0)
            continue;
        if (start_line.value){
            strcpy(act_local_time, local_time);
            start_line.value = 0;
        }
        else{
            TimeDiff = get_TimeDiff(act_local_time, local_time);
            printf("%ld.  %s    %s   SaveTimeDiff = %ld  rc = %d  check = %hi  ", num_line, act_local_time, local_time, SaveTimeDiff, rc, check.value);
            printf("TimeDiff = %ld  ", TimeDiff);
            if (TimeDiff != PrevTimeDiff && !check.value){
                if (rc >= ConstTimeDiff - 1)
                    rc = 0;
                else
                    rc++;
            }
            if (TimeDiff == PrevTimeDiff || check.value)
                count_of_equals_time[rc]++;
            if (TimeDiff > SaveTimeDiff){
                printf("~~~~~~");
                push(local_time, &top);
                SaveTimeDiff = TimeDiff;
            }
            if (TimeDiff == 0)
                count++;
            printf("\n\n");
            if (TimeDiff <= ConstTimeDiff){
                TimeWindowCount++;
                check.value = 0;
            }
            else{
                if (MaxTimeWindowCount < TimeWindowCount){
                    num_of_line = num_line;
                    MaxTimeWindowCount = TimeWindowCount;
                }
                if (check.value){
                    if (rc >= ConstTimeDiff - 1)
                        rc = 0;
                    else
                        rc++;
                }
                printf("HERE");
                strcpy(act_local_time, pop(&tail));
                printf(" %s  %ld count_of_equals_time[0] = %d count_of_equals_time[1] = %d pc = %d rc = %d\n\n", act_local_time, TimeWindowCount, count_of_equals_time[0], count_of_equals_time[1], pc, rc);
                TimeWindowCount -= count_of_equals_time[pc];
                count_of_equals_time[pc] = 0;
                if (pc >= ConstTimeDiff - 1)
                    pc = 0;
                else
                    pc++;
                TimeDiff = get_TimeDiff(act_local_time, local_time);
                check.value = 1;
                SaveTimeDiff = TimeDiff;
                if (TimeDiff <= ConstTimeDiff)
                    TimeWindowCount++;
            }
            PrevTimeDiff = TimeDiff;
        }
    }
    if (MaxTimeWindowCount < TimeWindowCount){
        num_of_line = num_line;
        MaxTimeWindowCount = TimeWindowCount;
    }
    printf("Finish %ld %ld  ", MaxTimeWindowCount, ConstTimeDiff);
    printf("%s\n", pop(&tail));
    fprintf(Errors_5xx, "Count of errors 5xx = %d", Count5xxError);
    free(count_of_equals_time);
    free(check_local_time);
    free(tail);
    free(top);
    free(save_time);
    free(line);
    free(local_time);
    free(act_local_time);
    free(status);
    fclose(log);
    fclose(Errors_5xx);
    if (argc > 2)
        fclose(Time_Window);
}