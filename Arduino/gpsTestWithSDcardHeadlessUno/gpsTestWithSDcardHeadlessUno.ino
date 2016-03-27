 /*
Based on
 Example code for connecting a Parallax GPS module to the Arduino

 Igor Gonzalez Martin. 05-04-2007
 igor.gonzalez.martin@gmail.com

 English translation by djmatic 19-05-2007

 Listen for the $GPRMC string and extract the GPS location data from this.
 Display the result in the Arduino's //Serial monitor.
and 
created  28 Mar 2011
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe

 The circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
    Pin 4 used here for consistency with other Arduino examples

    TX GPS to pin 9 which is RX of software serial

 */ 

 #include <string.h>
 #include <ctype.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>

File myFile;

SoftwareSerial mySerial(9, 5); // RX, TX
 char byteGPS=-1;
 char linea[300] = "";
 char comandoGPR[7] = "$GPRMC";
 int cont=0;
 int bien=0;
 int conta=0;
 int indices[13];

 void setup() {
   //Serial.begin(9600);
   mySerial.begin(9600);


   for (int i=0;i<300;i++){       // Initialize a buffer for received data
     linea[i]=' ';
   }   
    if (!SD.begin(4)) {
    //Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
 }

 void loop() {
 
   byteGPS=mySerial.read();         // Read a byte of the //Serial port
   if (byteGPS == -1) {           // See if the port is empty yet
     delay(100); 
   } else {
     // note: there is a potential buffer overflow here!
     linea[conta]=byteGPS;        // If there is //Serial port data, it is put in the buffer
     conta++;                      
    // //Serial.print(byteGPS); 
     if (byteGPS==13){            // If the received byte is = to 13, end of transmission
       // note: the actual end of transmission is <CR><LF> (i.e. 0x13 0x10)
       
       cont=0;
       bien=0;
       // The following for loop starts at 1, because this code is clowny and the first byte is the <LF> (0x10) from the previous transmission.
       for (int i=1;i<7;i++){     // Verifies if the received command starts with $GPR
         if (linea[i]==comandoGPR[i-1]){
           bien++;
         }
       }
       if(bien==6){               // If yes, continue and process the data
         for (int i=0;i<300;i++){
           if (linea[i]==','){    // check for the position of the  "," separator
             // note: again, there is a potential buffer overflow here!
             indices[cont]=i;
             cont++;
           }
           if (linea[i]=='*'){    // ... and the "*"
             indices[12]=i;
             cont++;
           }
         }
        myFile = SD.open("data.txt",FILE_WRITE);
         for (int i=0;i<12;i++){
           switch(i){
           //  case 0 ://Serial.print("Time in UTC (HhMmSs): ");break;
             //case 1 ://Serial.print("Status (A=OK,V=KO): ");break;
             case 2 :
              //Serial.print("Latitude: ");
             myFile.print("Latitude: ");
             break;
             //case 3 ://Serial.print("Direction (N/S): ");break;
             case 4 :
             //Serial.print("Longitude: ");
              myFile.print("Longitude: ");
              break;
             /*case 5 ://Serial.print("Direction (E/W): ");break;
             case 6 ://Serial.print("Velocity in knots: ");break;
             case 7 ://Serial.print("Heading in degrees: ");break;
             case 8 ://Serial.print("Date UTC (DdMmAa): ");break;
             case 9 ://Serial.print("Magnetic degrees: ");break;
             case 10 ://Serial.print("(E/W): ");break;
             case 11 ://Serial.print("Mode: ");break;
             case 12 ://Serial.print("Checksum: ");break;*/
           }
           if(i==2){
             for (int j=indices[i];j<(indices[i+1]-1);j++){
               //Serial.print(linea[j+1]); 
                myFile.print(linea[j+1]);
             }
             //Serial.print(" ");
             myFile.print(" ");
           }
           if(i==4){
            for (int j=indices[i];j<(indices[i+1]-1);j++){
               //Serial.print(linea[j+1]); 
               myFile.print(linea[j+1]);
             }
             //Serial.println("");
             myFile.println("");
           }
           
         }
          myFile.close();
         
       }
       conta=0;                    // Reset the buffer
       for (int i=0;i<300;i++){    //  
         linea[i]=' ';             
       }                 
     }
   }
 }

