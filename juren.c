#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAXSEAT     12
#define MAXNAME     20
char * s_gets(char * st, int n);

struct seat {
    char first_name [MAXNAME];
    char last_name [MAXNAME];
    char is_booking;
    unsigned short seat_num;
};
void get_empty_seats(int *empty_seats, int *result);
struct seat airline[MAXSEAT];

int main() {
    printf("To choose a function, enter its letter label:\n\n");
    printf("a) Show number of empty seats\n\
b) Show list of empty seats\n\
c) Show alphabetical list of seats\n\
d) Assign a customer to a seat assignment\n\
e) Delete a seat assignment\n\
f) Quit\n\n");


    
    FILE * pseats;
    if ((pseats = fopen("seat.dat", "a+b"))==NULL) {
        fputs("Can't open seat.dat file.", stderr);
        exit(1);
    }

    rewind(pseats);
    int size = sizeof(struct seat);
    int count = 0;
    int index, filecount;
    while( count < MAXSEAT && fread(&airline[count], size, 1, pseats)==1) {
        // if (count == 0) puts("Current seats of seat.dat of:");
        // printf("%s %s is %s at %d\n", airline[count].first_name, 
        //                             airline[count].last_name, 
        //                             airline[count].is_booking=='y' ? "booking" : "not booking",  
        //                             airline[count].seat_num);
        count++;
    }

    filecount = count;
    // if (count == MAXSEAT) {
    //     fputs("The seat.dat file is full.", stderr);
    //     exit(2);
    // }



    // puts("\nInput FIRST NAME, press [Enter] at the start of a line to stop.");
    // char * first_name = s_gets(airline[count].first_name, MAXNAME);
    while( count < MAXSEAT && s_gets(airline[count].first_name, MAXNAME)!=NULL && airline[count].first_name[0]!='\0') {
        puts("Input LAST NAME");
        s_gets(airline[count].last_name, MAXNAME);
        puts("Input SEAT NUM");
        scanf("%d", &airline[count].seat_num);
        while (getchar() !='\n')
            continue;
        puts("Input IS BOOKING");
        scanf("%c", &airline[count++].is_booking); // Y or N
        while (getchar() !='\n')
            continue;
        if (count < MAXSEAT)
            puts ("Enter the next FIRST NAME.");
    }
    // if (count> 0) {
    //     puts ("here is the list of your seats:" );
    //     for (index = 0; index < count; index++)
    //         printf("%s %s is %s at %d\n", airline[index].first_name, 
    //                                     airline[index].last_name, 
    //                                     airline[index].is_booking=='y' ? "booking" : "not booking",  
    //                                     airline[index].seat_num);
    //     fwrite(&airline[filecount], size, count - filecount, pseats);
    // }
    // else
    //     puts ("No seats? Too bad.\n");


    int empty_seats_count;
    int empty_seat_indices[MAXSEAT];
    char cmd;
    do {
        scanf("%c", &cmd);
        switch (cmd) {
        case 'a':
            get_empty_seats(empty_seat_indices, &empty_seats_count);
            printf("number of empty seats is %d\n", empty_seats_count);
            break;
        case 'b':
            get_empty_seats(empty_seat_indices, &empty_seats_count);
            printf("List of empty seats:\n");
            for (int i = 0; i < empty_seats_count; i++) {
                printf("Seat %d is empty.\n", airline[empty_seat_indices[i]].seat_num);
            }
            break;
        case 'c':
            printf("dashed\n");
            break;
        case 'd':
            printf("dashed\n");
            break;
        case 'e':
            printf("dashed\n");
            break;
        default:
            break;
        }
    } while (cmd != 'f');

    fclose (pseats);
    getchar();
    return 0;
}

void get_empty_seats(int *empty_seats, int *result) {
    *result = 0;  // 初始化结果为0
    for (int i = 0; i < MAXSEAT; i++) {
        if (airline[i].is_booking == 'n') {
            empty_seats[(*result)++] = i;
        }
    }
}

char * s_gets (char * st, int n) {
    char * ret_val;
    char * find;
    ret_val = fgets(st, n, stdin);
    if (ret_val) {
        find = strchr(st, '\n');
        if (find)
            *find = '\0'; // 最难理解的，应该就是这个，当执行 *find='\0' 后，ret_val 的值也变成 '\0' 也就是空字符串，你先想想为什么会这样？
        else
            while (getchar() != '\n')
                continue;
    }
    return ret_val;
}