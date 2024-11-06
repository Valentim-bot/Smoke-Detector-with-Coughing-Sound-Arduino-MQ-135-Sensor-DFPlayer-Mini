#include "SoftwareSerial.h"         // Include the SoftwareSerial library for serial communication
#include "DFRobotDFPlayerMini.h"    // Include the DFRobotDFPlayerMini library for the DFPlayer Mini module

SoftwareSerial mySoftwareSerial(3, 2); // Create a software serial connection on pins 3 (RX) and 2 (TX)
DFRobotDFPlayerMini myDFPlayer;          // Create a DFPlayerMini object

const int analogPin = A0;             // Define the pin number for the analogPin variable  
bool isPlaying = false;                      // Flag to indicate if the track is playing
unsigned long lastPlayTime = 0;              // Store the last time the track was played
const unsigned long playDuration = 3500;     // Set the duration for which the track is played

void setup() {
    mySoftwareSerial.begin(9600);            // Start software serial communication at 9600 baud rate
    Serial.begin(115200);                    // Start serial communication at 115200 baud rate
    pinMode(analogPin, INPUT);         // Set analog  pin  A0 as input

    if (!myDFPlayer.begin(mySoftwareSerial)) { // Initialize the DFPlayer Mini module
        Serial.println(F("Not initialized:"));
        Serial.println(F("1. Check the DFPlayer Mini connections"));
        Serial.println(F("2. Insert an SD card"));
        while (true);                        // If initialization fails, print error messages and halt the program
    }

    Serial.println();
    Serial.println(F("DFPlayer Mini module initialized!")); // Print initialization success message
    myDFPlayer.setTimeOut(500);              // Set the timeout value for serial communication
    myDFPlayer.volume(30);                   // Set the volume level (0 to 30)
    myDFPlayer.EQ(0);                        // Set the equalizer setting (0: Normal, 1: Pop, 2: Rock, etc.)
}

void loop() {
    int reading = analogRead(analogPin);  // Read the current analog value from the analog  pin
   Serial.println(reading);

    // Check if the reading variable's value is greater than 82
    if (reading > 82) {                       // If the analog value is greater than 82

        if (!isPlaying) {                      // If no track is currently playing

            playSong(1);            

        } else if (millis() - lastPlayTime >= playDuration) { // Check if the play duration has passed

            playSong(1);                      // Restart the song if the play duration has passed
        }
    } else if (isPlaying && (millis() - lastPlayTime >= playDuration)) {
        stopSong();                           // Stop the song after play duration
    }
}

// Function to play a song on DFPlayer
void playSong(uint8_t song) {

    myDFPlayer.play(song);                   // Play the specified audio file

    lastPlayTime = millis();                 // Record the time the track started playing

    isPlaying = true;                        // Set the flag to indicate the track is playing

    Serial.println("Playing song " + String(song)); // Log the song playing
}

// Function to stop the song
void stopSong() {

    myDFPlayer.stop();                        // Stop the song

    isPlaying = false;                        // Reset the flag to allow the next analog value >82  to play the track again
    
    Serial.println("Track finished playing"); // Log track finish
}
