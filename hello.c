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

static long long getTimeInMs(void);
static void sleepForMs(long long);
int readFromFileToScreen(char *); 
static void runCommand(char *);

//Functions shown below before main() are taken/referenced from Assignment 1 PDF (unless indicated)
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

    //For assignment1, I used readFromFileToScreen() to read gpio72 value file
    //Thus, I converted buff (which would = the number written in the value file) into an int and returned the value
    int i = atoi(buff);
    if(i == 0)
    {
        return 0; 
        
    }
    else {
        return 1;
    }

}


int main()
{

    printf("Hello embedded world, from Abigail!\nClick the USER button to start the game!\n");

    //Set all LEDs' triggers
    //Code to set triggers is taken from LED Guide
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
    //Code taken from GPIO guide
    runCommand("config-pin p8.43 gpio");
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

    int user_value_start = 1; //Set flag that states the current number in gpio72 value file
    bool user_pressed_start = false; //Set flag that states that user has not pressed USER button
    long long best_time = 0;

    //NOTE: BEGIN THE LOOP HERE
    int play_game = 1;
    while(play_game == 1)
    {

        while (user_pressed_start == false)
        {
        
            user_value_start = readFromFileToScreen("/sys/class/gpio/gpio72/value"); //Read gpio72 value file to see if user has pressed button
            if(user_value_start == 0){
            
                user_pressed_start = true; //Exit while loop once user has pressed USER button
                user_value_start = 1; //Reset flag
            }
            else{
                user_pressed_start = false;
            }
        }

        //Set the brightness of LED 0 to ON
        //Code to set brightness taken from LED Guide
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
        fclose(pLedBrightnessFile);

        //Declare the remaining pLedBrightnessFile# for each LED brightness file, and open them for write mode
        //Code taken from LED Guide
        FILE *pLedBrightnessFile1 = fopen(brightness_file_name1, "w");
        FILE *pLedBrightnessFile2 = fopen(brightness_file_name2, "w");
        FILE *pLedBrightnessFile3 = fopen(brightness_file_name3, "w");
        //Declare the remaining charWritten_bright# for each LED brightness file
        int charWritten_bright1 = 0; 
        int charWritten_bright2 = 0;
        int charWritten_bright3 = 0;

        printf("When LED3 lights up, press the USER button!\n");
        sleepForMs(2000); //wait for 2ms

        int response_time = 0;
        //Check if user is already pressing the USER button
        //Code taken from GPIO Guide
        int user_value_after_wait = readFromFileToScreen("/sys/class/gpio/gpio72/value");
        if (user_value_after_wait == 0){
            //If user has not pressed the USER button after 5 seconds, display response time and exit game
            response_time = 5000;
            printf("Your response time is: %i ms\n",response_time);
            play_game = 0;
        }
        else{
            //Light up LED3
            //Set the brightness of LED 3 to ON
            //Code to set brightness taken from LED Guide
            if (pLedBrightnessFile3 == NULL)
            {
                printf("ERROR OPENING %s.", brightness_file_name3);
                exit(1);
            }
            charWritten_bright3 = fprintf(pLedBrightnessFile3, "1");
            if (charWritten_bright3 <= 0){
                printf("ERROR WRITING DATA");
                exit(1);
            }
            fclose(pLedBrightnessFile3);
           
            //Start timer
            long long start_timer = getTimeInMs();
            long long current_time = 0;
            long long time_difference = 0;
            long long temp_best_time = 0;

            int user_value_after_led3 = 1;
            bool user_pressed_after_led3 = false;
            
            while (user_pressed_after_led3 == false)
            {
                current_time = getTimeInMs();
                time_difference = current_time - start_timer;
                     
                user_value_after_led3 = readFromFileToScreen("/sys/class/gpio/gpio72/value");
                if(user_value_after_led3 == 0 && time_difference < 5000){
                
                    //Light up all LEDS
                    //Code to set brightness taken from LED Guide
                    //Set the brightness of LED 1 to ON
                    if (pLedBrightnessFile1 == NULL)
                    {
                        printf("ERROR OPENING %s.", brightness_file_name1);
                        exit(1);
                    }
                    charWritten_bright1 = fprintf(pLedBrightnessFile1, "1");
                    if (charWritten_bright1 <= 0){
                        printf("ERROR WRITING DATA");
                        exit(1);
                    }
                    fclose(pLedBrightnessFile1);
                    //Set the brightness of LED 2 to ON
                    if (pLedBrightnessFile2 == NULL)
                    {
                        printf("ERROR OPENING %s.", brightness_file_name2);
                        exit(1);
                    }
                    charWritten_bright2 = fprintf(pLedBrightnessFile2, "1");
                    if (charWritten_bright2 <= 0){
                        printf("ERROR WRITING DATA");
                        exit(1);
                    }
                    fclose(pLedBrightnessFile2);
                    //Check if current response time is the best response time so far
                    temp_best_time = time_difference;
                    if (temp_best_time > best_time && best_time == 0)
                    {
                        best_time = temp_best_time;
                        printf("New best time!\n");
                    }
                    else if (temp_best_time < best_time){
                        best_time = temp_best_time;
                    }

                    //Print out reaction time and best time
                    printf("Your reaction time was: %lli ms\n",time_difference);
                    printf("Best time so far in game is: %lli ms\n",best_time);
                    printf("Press the USER button to play again!\n");

                    user_pressed_after_led3 = true; //exit while loop once user has pressed USER button
                    user_pressed_start = false;
                    sleepForMs(1000);
                    
                }
                else if (time_difference >= 5000){
                    printf("No input within 5000ms; quitting!\n");
                    user_pressed_after_led3 = true;
                    play_game = 0;

                }
            }


        }

        //Turn off all LEDS
        //Code to turn off LEDs taken from LED Guide
        //Turn off brightness for LED0
        pLedBrightnessFile = fopen(brightness_file_name, "w");
        if (pLedBrightnessFile == NULL)
        {
            printf("ERROR OPENING %s.", brightness_file_name);
            exit(1);
        }
        charWritten_bright = fprintf(pLedBrightnessFile, "0");
        if (charWritten_bright <= 0){
            printf("ERROR WRITING DATA");
            exit(1);
        }
        fclose(pLedBrightnessFile);
        //Turn off brightness for LED1
        pLedBrightnessFile1 = fopen(brightness_file_name1, "w");
        if (pLedBrightnessFile1 == NULL)
        {
            printf("ERROR OPENING %s.", brightness_file_name1);
            exit(1);
        }
        charWritten_bright1 = fprintf(pLedBrightnessFile1, "0");
        if (charWritten_bright1 <= 0){
            printf("ERROR WRITING DATA");
            exit(1);
        }
        fclose(pLedBrightnessFile1);
        //Turn off brightness for LED2
        pLedBrightnessFile2 = fopen(brightness_file_name2, "w");
        if (pLedBrightnessFile2 == NULL)
        {
            printf("ERROR OPENING %s.", brightness_file_name2);
            exit(1);
        }
        charWritten_bright2 = fprintf(pLedBrightnessFile2, "0");
        if (charWritten_bright2 <= 0){
            printf("ERROR WRITING DATA");
            exit(1);
        }
        fclose(pLedBrightnessFile2);
        //Turn off brightness for LED3
        pLedBrightnessFile3 = fopen(brightness_file_name3, "w");
        if (pLedBrightnessFile3 == NULL)
        {
            printf("ERROR OPENING %s.", brightness_file_name3);
            exit(1);
        }
        charWritten_bright3 = fprintf(pLedBrightnessFile3, "0");
        if (charWritten_bright3 <= 0){
            printf("ERROR WRITING DATA");
            exit(1);
        }
        fclose(pLedBrightnessFile3);
    }
    
    
    //Close trigger files
    //Code taken from LED Guide
    fclose(pLedTriggerFile);
    fclose(pLedTriggerFile1);
    fclose(pLedTriggerFile2);
    fclose(pLedTriggerFile3);
    
    
    
    return 0;
}
