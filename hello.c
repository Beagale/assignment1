//Referenced code from Quick Start Guide section (5.2)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
 
#define trigger_file_name "/sys/class/leds/beaglebone:green:usr0/trigger"
#define trigger_file_name1 "/sys/class/leds/beaglebone:green:usr1/trigger"
#define trigger_file_name2 "/sys/class/leds/beaglebone:green:usr2/trigger"
#define trigger_file_name3 "/sys/class/leds/beaglebone:green:usr3/trigger"

#define brightness_file_name "/sys/class/leds/beaglebone:green:usr0/brightness" 
#define brightness_file_name1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#define brightness_file_name2 "/sys/class/leds/beaglebone:green:usr2/brightness"  
#define brightness_file_name3 "/sys/class/leds/beaglebone:green:usr3/brightness" 
//#define user_button 

static long long getTimeInMs(void);
static void sleepForMs(long long);
int readFromFileToScreen(char *); 
static void runCommand(char *);

//Code taken from Assignment 1 PDF (page 5 and 6)
//Retrieve the current time
static long long getTimeInMs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000
    + nanoSeconds / 1000000;
    return milliSeconds;
}
//Code to wait a number of milliseconds
static void sleepForMs(long long delayInMs)
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}
//Code to run a Linux command within C program
static void runCommand(char* command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
            // printf("--> %s", buffer); // Uncomment for debugging
        }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

//Code taken from GPIO Guide 
//Reads data from a file
int readFromFileToScreen(char *fileName)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
    printf("ERROR: Unable to open file (%s) for read\n", fileName);
    exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    
    //line below helps to check what value is read
    //printf("Read: '%s'\n", buff);

    int i = 0;
    sscanf(buff, "%i", &i); //convert buff into an int 
    if(i == 0)
    {
        return 0;
    }
    else{
        return 1;
    }
}


int main()
{

    printf("Hello embedded world, from Abigail!\n");

    //Set all LEDs' triggers
    //Code taken from LED Guide
    //Set trigger for LED0
    FILE *pLedTriggerFile = fopen(trigger_file_name,"w");
    if(pLedTriggerFile == NULL){
        printf("ERROR OPENING %s.",trigger_file_name);
        exit(1);
    }
    int charWritten = fprintf(pLedTriggerFile, "none");
    if(charWritten <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }
    //Set trigger for LED1
    FILE *pLedTriggerFile1 = fopen(trigger_file_name1,"w");
    if(pLedTriggerFile1 == NULL){
        printf("ERROR OPENING %s.",trigger_file_name1);
        exit(1);
    }
    int charWritten1 = fprintf(pLedTriggerFile1, "none");
    if(charWritten1 <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }
    //Set trigger for LED2
    FILE *pLedTriggerFile2 = fopen(trigger_file_name2,"w");
    if(pLedTriggerFile2 == NULL){
        printf("ERROR OPENING %s.",trigger_file_name2);
        exit(1);
    }
    int charWritten2 = fprintf(pLedTriggerFile2, "none");
    if(charWritten2 <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }
    //Set trigger for LED3
    FILE *pLedTriggerFile3 = fopen(trigger_file_name3,"w");
    if(pLedTriggerFile3 == NULL){
        printf("ERROR OPENING %s.",trigger_file_name3);
        exit(1);
    }
    int charWritten3 = fprintf(pLedTriggerFile3, "none");
    if(charWritten3 <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }

    //Config USER button for gpio
    runCommand("config-pin p8.43 gpio");

    //Code taken from GPIO Guide
    //Writes to the USER button and makes it an input
    FILE *pFile = fopen("/sys/class/gpio/gpio72/direction", "w'");
    if(pFile == NULL){
        printf("ERROR: Unable to open export file.\n");
        exit(1);
    }
    //Write to data to the file using fprintf();
    fprintf(pFile, "%s", "in");
    //CLose file using fclose();
    fclose(pFile); 

    //Reads the USER button and checks if user has pressed button or not
    int user_value_start = 1;
    bool user_pressed_start = false;
    while (user_pressed_start == false)
    {
       user_value_start = readFromFileToScreen("/sys/class/gpio/gpio72/value");
        if(user_value_start == 0){
            printf("By clicking the USER button, you have started the game!\nTurning on LED0...");
            user_pressed_start = true; //exit while loop once user has pressed USER button
        }
        else{
            user_pressed_start = false;
        }
    }
    
    
    //Set the brightness of LED 0 to ON
    FILE *pLedBrightnessFile = fopen(brightness_file_name, "w");
    if (pLedBrightnessFile == NULL)
    {
        printf("ERROR OPENING %s.", brightness_file_name);
        exit(1);
    }
    int charWritten_bright = fprintf(pLedBrightnessFile, "1");
    if (charWritten_bright <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }
    
    printf("LED0 on!\nWhen LED3 lights up, press the USER button!\n");

    sleepForMs(2000); //wait for 2ms

    //Check if user is already pressing the USER button
    int response_time = 0;
    int user_value_after_wait = readFromFileToScreen("/sys/class/gpio/gpio72/value");
    if (user_value_after_wait == 0){
        response_time = 5000;
        printf("Your response time is: %i",response_time);
    }
    else{
        //Light up LED3
        //Set the brightness of LED 3 to ON
        FILE *pLedBrightnessFile3 = fopen(brightness_file_name3, "w");
        if (pLedBrightnessFile3 == NULL)
        {
            printf("ERROR OPENING %s.", brightness_file_name3);
            exit(1);
        }
        int charWritten_bright3 = fprintf(pLedBrightnessFile3, "1");
        if (charWritten_bright3 <= 0){
            printf("ERROR WRITING DATA");
            exit(1);
        }
         //Start timer
        long long start_timer = getTimeInMs();
        long long current_time = 0;
        long long time_difference = 0;
    
        fclose(pLedBrightnessFile3);

        int user_value_after_led3 = 1;
        bool user_pressed_after_led3 = false;
        while (user_pressed_after_led3 == false)
        {
            current_time = getTimeInMs();
            time_difference = current_time - start_timer;
            user_value_after_led3 = readFromFileToScreen("/sys/class/gpio/gpio72/value");
            if(user_value_after_led3 == 0 && time_difference < 5000){
                printf("Nice! time_difference = %lli\n", time_difference);
                printf("user_value_after_led3 = %i", user_value_after_led3);

                //Light up all LEDS
                //Set the brightness of LED 1 to ON
                FILE *pLedBrightnessFile1 = fopen(brightness_file_name1, "w");
                if (pLedBrightnessFile1 == NULL)
                {
                    printf("ERROR OPENING %s.", brightness_file_name1);
                    exit(1);
                }
                int charWritten_bright1 = fprintf(pLedBrightnessFile1, "1");
                if (charWritten_bright1 <= 0){
                    printf("ERROR WRITING DATA");
                    exit(1);
                }
                //Set the brightness of LED 2 to ON
                FILE *pLedBrightnessFile2 = fopen(brightness_file_name2, "w");
                if (pLedBrightnessFile2 == NULL)
                {
                    printf("ERROR OPENING %s.", brightness_file_name2);
                    exit(1);
                }
                int charWritten_bright2 = fprintf(pLedBrightnessFile2, "1");
                if (charWritten_bright2 <= 0){
                    printf("ERROR WRITING DATA");
                    exit(1);
                }

                //TEST
                printf("turned on LED2\n");

                fclose(pLedBrightnessFile1);
                fclose(pLedBrightnessFile2);

                user_pressed_after_led3 = true; //exit while loop once user has pressed USER button
                
            }
            else if (time_difference >= 5000){
                printf("No input within 5000ms; quitting!\n");
                user_pressed_after_led3 = true;

            }
        }


    }
    
    
    //printf("Start time: %lld", start_time);

        
    //Turn off all LEDS
    //Turn off brightness for LED0
   //pLedBrightnessFile = fopen(brightness_file_name, "w");
   /* if (pLedBrightnessFile == NULL)
    {
        printf("ERROR OPENING %s.", brightness_file_name);
        exit(1);
    }
    charWritten_bright = fprintf(pLedBrightnessFile, "0");
    if (charWritten_bright <= 0){
        printf("ERROR WRITING DATA");
        exit(1);
    }
*/
    
    //CLose brightness and trigger files
    fclose(pLedBrightnessFile);
    
    fclose(pLedTriggerFile);
    fclose(pLedTriggerFile1);
    fclose(pLedTriggerFile2);
    fclose(pLedTriggerFile3);
    
    
    
    return 0;
}
