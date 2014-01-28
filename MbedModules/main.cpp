/**********************************
 * Author: Clifton Thomas
 * Date: 3/28/13
 * Institution: Georgia Tech
 *
 * Title: MAIN
 * Class: ECE2035
 * Assignment: Project 2
 * Student: Yingyan Samantha Wang
 **********************************/

//includes
#include <string.h>
#include <cstdarg>
#include "mbed.h"
#include "rtos.h"
#include "TextLCD.h"
#include "SDFileSystem.h"
#include "CommModule.h"
#include "wave_player.h"
#include "FATFileSystem.h"


//defines
#define BAUDRATE            9600
#define BUFFSIZE            100
#define BOMB_WAVFILE        "/sd/wavfiles/bomb.wav"
#define ACK                 "status-ack"
#define STATUS_KEY          "status"
#define HIT_KEY             "hit"
#define MISS_KEY            "miss"
#define SANK_KEY            "sank"


//function prototypes
void lowerCase(char *src);
void startGame(void);
void waitForAck(void);
int bomb(int rowIndex, int colIndex);
int** init(int** mark);
int* find(int** mark);
int col_ship(int row, int col, int **mark);
int row_ship(int row, int col, int **mark);
void playBombSound(void);
void print(const char *format, ...);


//declare functions (assembly subroutines)
extern "C" void setup_sequence(void);
extern "C" void seg_driver_initialize(void);
extern "C" void seg_driver(int value);


//initialize hardware
SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs
TextLCD lcd(p26, p25, p24, p23, p22, p21); // rs, e, d4-d7
AnalogOut DACout(p18);
wave_player waver(&DACout);


//communication device
commSerial serDevice(USBTX, USBRX, BAUDRATE); //tx, rx

int times;

//main
int main() {
    //initialize and clear 7-Segment Display (assembly subroutine)
    setup_sequence();
    seg_driver_initialize();
    int seg_times;
    for (seg_times=0;seg_times<10;seg_times++){
        seg_driver(seg_times);
        lcd.cls();
        lcd.printf("%d",seg_times);
        wait(0.5);
    }    
    //check for wav file
    lcd.cls();
    lcd.printf("Locating WAV file...");
    FILE *test_file;
    while(1) {
        test_file=fopen(BOMB_WAVFILE,"r");
        if(test_file != NULL) {break;}
        wait(0.5);
    }
    fclose(test_file);

    //notification
    lcd.cls();
    lcd.printf("Battleship");
    wait(1);
    
    //loop
    while(1) {
        //synchronize front end display
        startGame();
        seg_driver(0);
        /****   BEGIN - your code goes here   ****/
        //temp variables
        int i,j,k,end_row,end_col,ret,tempret;    
        int **mark;
        int *ndx;
        //allocate a 10 by 10 array to keep track of tiles that are
        //either checked,unchecked or no need to check
        mark = (int **)malloc(10 * sizeof(int *));
        if(mark == NULL){
            fprintf(stderr, "out of memory\n");
            return;
        }
        for(i = 0; i < 10; i++){
            mark[i] = (int *)malloc(10 * sizeof(int));
            if(mark[i] == NULL){
                fprintf(stderr, "out of memory\n");
                return;
            }
        }
        mark=init(mark);  
        i=0;
        j=0;       
        k=0;
        times=0;
        //debug statements
        print("testing");
        print("string: %s\nstring2: %s", "hello", "world");
        print("int: %d", 8);
        
        while (k!=5){
            ret=bomb(i,j);
            //mark tiles already checked
            mark[i][j]=1;
            //tiles no need to check
            if (i+1<10){
                mark[i+1][j]=2;
                //mark[i+2][j]=2;
            }
            if (j+1<10){
                mark[i][j+1]=2;
                //mark[i][j+2]=2;
            }
            //when a boat is hit
            if (ret==1){
                //row ship
                mark[i][j+1]=1;
                tempret=bomb(i,j+1);
                if (tempret==1){
                    end_col=row_ship(i,j+1,mark);
                    if(i<9){
                        mark[i+1][j]=1;
                        mark[i+1][j+1]=1;
                    }
                    if (end_col+1<10){
                        mark[i][end_col+1]=1;
                    }                       
                }
                //vertical ship
                else if (tempret!=2){
                    if (bomb(i,j-1)==0||j==0){
                        end_row=col_ship(i,j,mark);
                        if(end_row+1<10){
                            mark[end_row+1][j]=1;
                        }
                    }
                }
                k++;
                seg_driver(k);               
                
            }         
            ndx=find(mark);
            i=ndx[0];
            j=ndx[1];      
        }
        
        free(mark);
        
        //have fun... 
        
        /****    END - your code stops here   ****/  
    }
}

//init mask;
int** init(int **mark){
    int i,j;
    for (i=0;i<10;i++){
        for (j=0;j<10;j++){
            mark[i][j]=0;
        }
    }
    return mark;
}

//find the first unchecked tile
int* find(int **mark){
     int i,j,a;
     int *ndx;
     ndx = (int *)malloc(2 * sizeof(int ));
     a=0;
     for (i=0;i<10;i++){
         for (j=0;j<10;j++){
             if (mark[i][j]==0){   
                 a=1;
                 break;
             }
         }
         if (a==1){
             break;
         }
     }
     ndx[0]=i;
     ndx[1]=j;
     return ndx;
}

//bomb the entire vertical ship
int col_ship(int row,int col, int **mark){
    int backtrack=0;
    int tempret=1;
    int temprow;
    temprow=row;
    while (tempret!=2){
        if(tempret==1 && row<9 && backtrack==0){
                row++;
        }
        else if ((tempret==0||row==9) && backtrack==0){
            row=temprow-1;
            backtrack=1;
        }
        else if (backtrack==1){
            row--;
        }
        tempret=bomb(row,col);
        mark[row][col]=1;
        if (col+1<10){
            mark[row][col+1]=1;
        }
        if (col-1>=0){
            mark[row][col-1]=1;
        }
        //lcd.cls();
        //lcd.printf("Ret: %d", tempret); 
    }
    return row;
}

//bomb the entire horizontal ship
int row_ship(int row,int col,int **mark){
    int backtrack=0;
    int tempret=1;
    int tempcol;
    tempcol=col;
    while (tempret!=2){ 
        //continue to bomb the horizontal ship
        if (tempret==1 && col<9 && backtrack==0){
            col++;
        }
        //bomb the first tile of the ship
        else if ((tempret==0||col==9) && backtrack==0){
            col=tempcol-2;
            backtrack=1;
        }
        else if (backtrack==1){
            col--;
        }
        tempret=bomb(row,col);
        mark[row][col]=1;
        //the tiles below the ship are no need to check
        if(row+1<10){
            mark[row+1][col]=2;
        }
        //lcd.cls();
        //lcd.printf("Ret: %d", tempret);         
    }
    return col;
}


//fcn to get acknowledgement from serial peripheral
void waitForAck(void) {
    //get acknowlegement
    char buffer[BUFFSIZE];
    while(1) {
        serDevice.receiveData(buffer);
        lowerCase(buffer);
        if(strcmp(ACK, buffer) == 0) {
            break;
        }
        memset(&buffer[0],0,strlen(buffer));     
    }
    return;
}

//fcn to initialize the frontend display
void startGame(void) {
    //temp variables
    char buffer[BUFFSIZE];
    //construct message
    sprintf(buffer, "start");
    
    //send message
    serDevice.sendData(buffer);
    
    //wait for acknowledgement
    waitForAck();
}

//fcn to bomb a given coordinate
int bomb(int rowIndex, int colIndex) {
    //temp variables 
    char buffer[BUFFSIZE];
    //check if coordinate is valid
    times++;
    lcd.cls();
    lcd.printf("Bomb Used: %d", times); 
    if((rowIndex >= 0) && (rowIndex <= 9) && (colIndex >= 0) && (colIndex <= 9)) {
        //construct message
        sprintf(buffer, "bomb-%d-%d", rowIndex, colIndex);
        //send message
        serDevice.sendData(buffer);
        
        //wait for status response
        while(1) {
            //temp variables
            memset(&buffer[0],0,strlen(buffer));
            char *ptr = NULL;
            
            //receive status response
            serDevice.receiveData(buffer);
            
            //parse string to extract status key
            ptr = strstr(buffer, STATUS_KEY);
            if(ptr == NULL) {continue;}
            
            //if status key found, parse string to extract status message
            ptr+=(strlen(STATUS_KEY)+1);
            if(strcmp(ptr, HIT_KEY) == 0) {
                playBombSound();
                return(1);
            }
            else if(strcmp(ptr, MISS_KEY) == 0) {
                return(0);
            }
            else if(strcmp(ptr, SANK_KEY) == 0) {
                playBombSound();
                return(2);             
            }
            else {
                return(-1);
            }
        }            
    }
    return(-1);  
}

//fcn to play bomb noise
void playBombSound(void) {
    //open wav file
    FILE *wave_file;
    wave_file=fopen(BOMB_WAVFILE,"r");
    
    //play wav file
    waver.play(wave_file);
    
    //close wav file
    fclose(wave_file);
}

//fcn to print to console
void print(const char *format, ...) {
    //temp variables
    char buffer[BUFFSIZE];
    char temp[BUFFSIZE-6];
    
    //construct message part 1
    sprintf(buffer, "print-");
    
    //construct message part 2
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(temp, BUFFSIZE-7, format, arguments);
    va_end(arguments);
    
    //concatenate parts
    strcat(buffer, temp);
    
    //send message
    serDevice.sendData(buffer);
    
    //wait for acknowledgement
    waitForAck();  
}

//fcn to convert string to lowercase
void lowerCase(char *src) {
    int i=0;;
    while(src[i] != '\0') {
        if((src[i] > 64) && (src[i] < 91)) {
            src[i]+=32;
        }
        i++;
    }
    return;
}