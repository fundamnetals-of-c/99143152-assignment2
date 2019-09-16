/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 2
 * Name: James Walsh
 * Student ID: 99143152
 * Date of submission: 5/9/2019
 * A brief statement on what you could achieve (less than 50 words):
 * test 1 to 10, the format of the flightcode input can validate wrong input of
 * chars as well as determine spaces before after and during the input
 * A brief statement on what you could NOT achieve (less than 50 words):
 * i have tested all i could for test 11 and cannot find where im going wrong
 * 
*******************************************************************************/

/*******************************************************************************
 * List header files - do NOT use any other header files. Note that stdlib.h is
 * included in case you want to use any of the functions in there. However the
 * task can be achieved with stdio.h and string.h only.
*******************************************************************************/
#include <stdio.h> /*scanf printf fscanf fprintf fflush fopen fclose*/
#include <string.h> /*strlen strcmp strcpy*/
#include <stdlib.h> 

/*******************************************************************************
 * List preprocessing directives - you may define your own.
*******************************************************************************/
#define MAX_FLIGHTCODE_LEN 6
#define MAX_CITYCODE_LEN 3
#define MAX_NUM_FLIGHTS 5
#define DB_NAME "database"

/*defines for testing valid date_time inputs*/
#define MONTHS_LOWER 1
#define MONTHS_UPPER 12
#define DAYS_LOWER 1
#define DAYS_UPPER 32
#define HOURS_LOWER 0
#define HOURS_UPPER 23
#define MINUTES_LOWER 0
#define MINUTES_UPPER 60

#define FLIGHTCODE_LETTER_LEN 2
/*******************************************************************************
 * List structs - you may define struct date_time and struct flight only. Each
 * struct definition should have only the fields mentioned in the assignment
 * description.
*******************************************************************************/
typedef struct date_time
{
    int month;  /*inclusive 1 to 12*/
    int day;    /*inclusive 1 to 31*/
    int hour;   /*inclusive 0 to 11*/	
    int minute; /*inclusive 0 to 59*/
} date_time_t;

typedef struct flight
{
    char flightcode[MAX_FLIGHTCODE_LEN + 1];
    date_time_t departure_dt;
    char arrival_city[MAX_CITYCODE_LEN + 1];
    date_time_t arrival_dt;
} flight_t;
/*******************************************************************************
 * Function prototypes - do NOT change the given prototypes. However you may
 * define your own functions if required.
*******************************************************************************/
void print_menu (void);
void add_flights (struct flight flights[], int flight_num);
void dis_all_flights(const struct flight flightList[], int flight_num);
void save_all_flights(const struct flight flights[], int flight_num);
int read_all_flights(struct flight flights[]);

int validate_date_time(const struct date_time time);
int validate_flightcode(const char flightcode[]);
void format_flightcode(char flightcode[]);
void format_arrivalcode(char arrivalcode[]);

void flightcode(char flightcode[]);

/*******************************************************************************
 * Main
*******************************************************************************/
int main(void)
{
    /*SETUP OF VARIABLES USED IN THE MAJORITY OF THE CODE*/
	
    /*this will store the menu option*/
    int choice; 
	 
    /*a variable which stores the total number of flights added to the array*/
    int flight_num = 0;
	 
    /*Struct which will store the flights input by the user*/
    flight_t flight[MAX_NUM_FLIGHTS];
	

    /*this while loops allows for multiple entries from the print menu and will 
    direct the user according. the defaults to an error message upon invalid 
    user entry and accepts intergers 1 - 5 */
    while(1)
    {
        print_menu();

        /*store the user input into choice*/
        scanf("%d", &choice);
        switch(choice)
        {
            case 1 :
                    if(flight_num < MAX_NUM_FLIGHTS)
                    {
                        add_flights(flight, flight_num);
                        flight_num++;
                    } 
                    else
                    {
                        printf("Cannot add more flights - memory full\n");
                    }
                    break;
            case 2 :
                    dis_all_flights(flight, flight_num);
                    break;
            case 3 :
                    save_all_flights(flight, flight_num);
                    break;
            case 4 :
                    flight_num = read_all_flights(flight);
                    break;
            case 5 :
                    return 0;
            default:
                    printf("Invalid choice\n");
                    break;
        }
    }
    return 0;
}

/*******************************************************************************
 * This function prints the initial menu with all instructions on how to use
 * this program.
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/
void print_menu (void)
{
    printf("\n"
    "1. add a flight\n"
    "2. display all flights to a destination\n"
    "3. save the flights to the database file\n"
    "4. load the flights from the database file\n"
    "5. exit the program\n"
    "Enter choice (number between 1-5)>\n");
}

/*******************************************************************************
 * This function inputs and modifys a flight code to inncorperate spaces and
 * have a large range of test cases
 * inputs:
 * - flightcode
 * outputs:
 * - none
 * post:
 * will fill the flightcode array with a string of chars representing the flight
 * code
*******************************************************************************/
void flightcode(char flightcode[])
{
    /* setting up variables to store flight code information into first char
    and rest of the string */
    char check[1];
    char *ptr;
    /*ignore the first chars this eliminates the new line charater*/
    scanf("%*c");
    /*store the first char into check to test for space and accpet all other
    inputs until the new line is reached to accpet spaces during and end*/
    scanf("%c %m[^\n]s", &check[0], &ptr);
    fflush(stdin);
    
    /*combine the char check with the string by pacing it at the front*/

    flightcode[0] = check[0];
    int i = 0;
    if(strlen(ptr) > MAX_FLIGHTCODE_LEN - 1)
    {
        strcpy(flightcode, "000000");
        return;
    }
    for(i = 0; i < strlen(ptr) || i < MAX_FLIGHTCODE_LEN; i++)
    {
        flightcode[i + 1] = ptr[i];
    }
    /*uses to detect end of string*/
    flightcode[MAX_FLIGHTCODE_LEN] = '\0';
}

/*******************************************************************************
 * This function inputs and adds to the struct flight_t array adding new flight
 * information into the array
 * inputs:
 * - flight_t, flight number
 * outputs:
 * - none
 * post:
 * this will add a flight to the flights array
*******************************************************************************/
void add_flights (struct flight flights[], int flight_num)
{
    /*initlising the time date structures*/
    date_time_t date_time_d;
    date_time_t date_time_a;
    /*entering the departure flight information*/
    /*loop until a valid flightcode is entered*/
    while(validate_flightcode(flights[flight_num].flightcode) == -1)
    {
        printf("Enter flight code>\n");
        flightcode(flights[flight_num].flightcode);
        /*check is input is valid and if not print error message*/
        if(validate_flightcode(flights[flight_num].flightcode) == -1)
        {
            printf("Invalid input\n");
        }
    }
    format_flightcode(flights[flight_num].flightcode);
	
    printf("Enter departure info for the flight leaving SYD.\n");
    /*loop until a valid date time is entered*/
    while(validate_date_time(date_time_d) == -1)
    {
        printf("Enter month, date, hour and minute separated by spaces>\n");
        scanf("%d", &date_time_d.month);
        scanf("%d", &date_time_d.day);
        scanf("%d", &date_time_d.hour);
        scanf("%d", &date_time_d.minute);
        /*check is input is valid and if not print error message*/
        if(validate_date_time(date_time_d) ==  -1)
        {
            printf("Invalid input\n");
        }
    }
    /*store that departure date_time_t into the struct*/
    flights[flight_num].departure_dt = date_time_d;

    /*entering the destination arrival information*/
    printf("Enter arrival city code>\n");
    scanf("%s", flights[flight_num].arrival_city);
    format_arrivalcode(flights[flight_num].arrival_city);
    
    printf("Enter arrival info.\n");
    /*loop until a valid date time is entered*/
    while(validate_date_time(date_time_a) == -1)
    {
        printf("Enter month, date, hour and minute separated by spaces>\n");
        scanf("%d", &date_time_a.month);
        scanf("%d", &date_time_a.day);
        scanf("%d", &date_time_a.hour);
        scanf("%d", &date_time_a.minute);
        /*check is input is valid and if not print error message*/
        if(validate_date_time(date_time_a) ==  -1)
        {
            printf("Invalid input\n");
        }
    }
    /*store the arrival date_time_t into the struct*/
    flights[flight_num].arrival_dt = date_time_a;
}

/*******************************************************************************
 * This function inputs all flights and saves the into the file defined as
 * DB_NAME, this function will not modify flights or flight number
 * inputs:
 * - const flight_t, flight number
 * outputs:
 * - none
*******************************************************************************/
void save_all_flights(const struct flight flights[], int flight_num)
{
    /*incrementing value*/
    int  i = 0;
    /*file pointer to direct to datebase and open the file for writing*/
    FILE* fp = NULL;
    fp = fopen(DB_NAME, "w");
    /*check if the file pointer found the file*/
    if(fp == NULL)
    {
        printf("Read error\n");
        return;
    }
    /*Print each value stored in flights, using the incrementer i to indicate
    the line to print. It prints to a text file, and each line represents a
    different flight*/
    for(i = 0; i < flight_num; i++)
    {
        fprintf(fp, "%s %d %d %d %d %s %d %d %d %d\n",
            flights[i].flightcode,
            flights[i].departure_dt.month,
            flights[i].departure_dt.day,
            flights[i].departure_dt.hour,
            flights[i].departure_dt.minute,
            flights[i].arrival_city,
            flights[i].arrival_dt.month,
            flights[i].arrival_dt.day,
            flights[i].arrival_dt.hour,
            flights[i].arrival_dt.minute);
    
    }
    /*close the file at the file pointer to avoid conflictions from any other
    function accessing it*/
    fclose(fp);
}

/*******************************************************************************
 * This function inputs all flights and saves the into the file defined as
 * DB_NAME
 * inputs:
 * - flight_t, &flight number
 * outputs:
 * - -1 if error, or the flight number determined by the ammount of flights in
 * the database
 * post:
 * this will fill all the flight data into the flights array from the database 
 * file
*******************************************************************************/
int read_all_flights(struct flight flights[])
{
    /*reset the flight_num and store all new flightcodes*/
    int flight_num = 0;
    /*incrementing variable*/
    int i = 0;
    /*file pointer to direct to datebase and open the file for writing*/
    FILE* fp = NULL;
    fp = fopen(DB_NAME, "r");
    /*check if the file pointer found the file*/
    if(fp == NULL)
    {
        printf("Read error\n");
        return -1;
    }
	
    for(i = 0; i < MAX_NUM_FLIGHTS; i++)
    {
        /*scan the first string of the file to check it is a filghtcode if there
        is no flight code validate_flightcode will return and close the file*/
        fscanf(fp, "%s", flights[i].flightcode);
        if(validate_flightcode(flights[i].flightcode) == -1)
        {
            fclose(fp);
            return flight_num;
        }
        /*scan each value stored in file, using the incrementer i to indicate
        the line to scan. It stores into flights, and as each line represents a
        different flight*/
        else
        {
            format_flightcode(flights[i].flightcode);
                fscanf(fp, " %d %d %d %d %s %d %d %d %d\n",
                &flights[i].departure_dt.month,
                &flights[i].departure_dt.day,
                &flights[i].departure_dt.hour,
                &flights[i].departure_dt.minute,
                flights[i].arrival_city,
                &flights[i].arrival_dt.month,
                &flights[i].arrival_dt.day,
                &flights[i].arrival_dt.hour,
                &flights[i].arrival_dt.minute);
            format_arrivalcode(flights[i].arrival_city);
            flight_num = flight_num + 1;
        }
    }
    /*close the file at the file pointer to avoid conflictions from any other
    function accessing it*/    
    fclose(fp);
    return flight_num;
}

/*******************************************************************************
 * This function shows all the current flights within the flight array either
 * to a specified destination or if input * all flights. this function will not
 * change the flight array
 * inputs:
 * - flight_t, flight number
 * outputs:
 * - none
*******************************************************************************/
void dis_all_flights(const struct flight flightList[], int flight_num)
{
    /*input the destination code to filter the display by*/
    char destinationcode[MAX_CITYCODE_LEN + 1];
    printf("Enter arrival city code or enter * to show all destinations>\n");
    scanf("%s", destinationcode);
    
	/*initlise incrementing variables*/
    int i, count = 0;
 
    for(i = 0; i < flight_num; i++)
    {
        /*check if the input is to print all or compare it with the target 
        destination code*/
        if(strcmp(flightList[i].arrival_city, destinationcode) == 0 || 
        destinationcode[0] == '*')
        {
            /*print the header if nothing has been printed yet*/
            if(count == 0)
            {
                printf("Flight Origin          Destination\n");
                printf("------ --------------- ---------------\n");
            }
            count++;
            printf("%s SYD %02d-%02d %02d:%02d %s %02d-%02d %02d:%02d\n", 
                flightList[i].flightcode,
                flightList[i].departure_dt.month,
                flightList[i].departure_dt.day,
                flightList[i].departure_dt.hour,
                flightList[i].departure_dt.minute,
                flightList[i].arrival_city,
                flightList[i].arrival_dt.month,
                flightList[i].arrival_dt.day,
                flightList[i].arrival_dt.hour,
                flightList[i].arrival_dt.minute);
        }
    }
    /*if count is zero nothing matched for printing and will print no flights*/
    if(count == 0)
    {
        printf("No flights\n");
    }
}

/*******************************************************************************
 * This function inputs a flight code string and will validate that it is 
 * valid as per the assignment outline such as first two chars being letters and
 * having to be capital letters, followed by 1 to 4 intergers. this function 
 * will not change the flight code string
 * inputs:
 * - flightcode
 * outputs:
 * - 1 is input is valid, -1 is the input isn't valid
*******************************************************************************/
int validate_flightcode(const char flightcode[])
{
    /*initlise incrementing variable*/
    int i;
    /*test if flightcode is longer then the maximum determined size if it is 
    return an error*/
    if((int)strlen(flightcode) > MAX_FLIGHTCODE_LEN)
    {
        return -1;
    }
    /*iterate through the letters of the flight code checking they are capiital
	letters*/
    for(i = 0; i < FLIGHTCODE_LETTER_LEN; i++)
    {
        /*test if the flightcode letters are between A - Z or 65 - 90*/
        if(flightcode[i] < 'A' || flightcode[i] > 'Z')
        {
            return -1;
        }
    }
    for(i = FLIGHTCODE_LETTER_LEN; i < MAX_FLIGHTCODE_LEN; i++)
    {
        /*test if the flightcode is at the end of the string as flightcode can 
        be less then 6 chars*/
        if(flightcode[i] == '\0' && i > FLIGHTCODE_LETTER_LEN)
        {
            return 1;
        }
        /*test if the flightcode numbers are between '0' - '9' or 48 - 57*/
        else if(flightcode[i] < '0' || flightcode[i] > '9')
        {
            return -1;
        }
    }
    return 1;
}

/*******************************************************************************
 * This function inputs a date_time_t and will validate that all the inputs are
 * valid, eg can't have more then 59 minutes shown. this function will not 
 * chnage the date_time structure
 * inputs:
 * - date_time_t
 * outputs:
 * - 1 is input is valid, -1 is the input isn't valid
*******************************************************************************/
int validate_date_time(const struct date_time time)
{
    /*test all the variable within time date to validate they are logical based
    on the real world reprsentation of time*/
    if(time.month < MONTHS_LOWER ||
    time.month > MONTHS_UPPER ||
    time.day < DAYS_LOWER ||
    time.day >= DAYS_UPPER ||
    time.hour < HOURS_LOWER ||
    time.hour > HOURS_UPPER ||
    time.minute < MINUTES_LOWER ||
    time.minute >= MINUTES_UPPER)
    {
        return -1;
    }
    return 1;
}

/*******************************************************************************
 * This function inputs a flightcode and adds spaces at the end to make sure it 
 * is ready to print in printf format  
 * inputs:
 * - flightcode
 * outputs:
 * none
 * post:
 * flightcode will be six chars longs with a null charater at the end
*******************************************************************************/
void format_flightcode(char flightcode[])
{
    /*initlise incrementing variables*/
    int i, j = 0;
    /*interate through the flightcode untiil the end of the string is found*/
    for(i = 0; i < MAX_FLIGHTCODE_LEN; i++)
    {
        if(flightcode[i] == '\0')
        {
            /*fill the rest of the chars within the string as spaces to format
            the flight code to be printed the same no matter the size*/
            for(j = i; j < MAX_FLIGHTCODE_LEN; j++)
            {
                flightcode[j] = ' ';
            }
            flightcode[MAX_FLIGHTCODE_LEN] = '\0';
            return;
        }
    }
    flightcode[MAX_FLIGHTCODE_LEN] = '\0';
    return;
}

/*******************************************************************************
 * This function inputs a arrival code and adds spaces at the end to make sure it 
 * is ready to print in printf format  
 * inputs:
 * - arrivalcode
 * outputs:
 * none
 * post:
 * arrival code will be three chars longs with a null charater at the end
*******************************************************************************/
void format_arrivalcode(char arrivalcode[])
{
    /*initlise incrementing variables*/
    int i, j = 0;
    /*interate through the flightcode untiil the end of the string is found*/
    for(i = 0; i < MAX_CITYCODE_LEN; i++)
    {
        if(arrivalcode[i] == '\0')
        {
            /*fill the rest of the chars within the string as spaces to format
            the flight code to be printed the same no matter the size*/
            for(j = i; j < MAX_CITYCODE_LEN; j++)
            {
                arrivalcode[j] = ' ';
            }
            arrivalcode[MAX_CITYCODE_LEN] = '\0';
            return;
        }
    }
    arrivalcode[MAX_CITYCODE_LEN] = '\0';
    return;
}
