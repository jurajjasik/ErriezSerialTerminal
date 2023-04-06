/*
 * MIT License
 *
 * Copyright (c) 2018-2021 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \brief Serial Terminal getting started example - With post command handlers
 * \details
 *      Source:         https://github.com/Erriez/ErriezSerialTerminal
 *      Documentation:  https://erriez.github.io/ErriezSerialTerminal
 *      
 *      Open the Serial Console in a terminal emulator like PuTTY to see typed charecters appear as sent.
 */

#include <ErriezSerialTerminal.h>

// LED pin
#define LED_PIN     LED_BUILTIN

// Newline character '\r' or '\n'
char newlineChar = '\r';
// Separator character between commands and arguments
char delimiterChar = ' ';

// Flag to print numbers in loop()
bool flagPrintNumbers = false;
// Counter incremented in loop() when flagPrintNumbers is set
int counter = 0;

// Create serial terminal object
SerialTerminal term(newlineChar, delimiterChar);

// Function prototypes
void unknownCommand(const char *command);
void cmdHelp();
void cmdLedOn();
void cmdLedOff();
void cmdPrintNumbers();
void cmdInterrupt();

void printConsoleChar();

// Print number and increase the counter
void refreshDisplay()
{
	if (flagPrintNumbers)
	{
		Serial.println(counter++);
	}
}

// Refresh display every second
void maintainDisplay()
{
	static const unsigned long REFRESH_INTERVAL = 1000; // ms
	static unsigned long lastRefreshTime = 0;
	
	if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
	{
		lastRefreshTime += REFRESH_INTERVAL;
		refreshDisplay();
	}
}

void setup()
{
    // Startup delay to initialize serial port
    delay(500);

    // Initialize serial port
    Serial.begin(115200);
    Serial.println(F("\nSerial terminal example."));
    Serial.println(F("Type 'help' to display usage."));
    printConsoleChar();

    // Initialize the built-in LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Set default handler for unknown commands
    term.setDefaultHandler(unknownCommand);

    // Add command callback handlers
    term.addCommand("?", cmdHelp);
    term.addCommand("help", cmdHelp);
    term.addCommand("on", cmdLedOn);
    term.addCommand("off", cmdLedOff);
    term.addCommand("n", cmdPrintNumbers);
	
	//Set interrupt (CTRL-C) command
	term.setInterruptCommand(cmdInterrupt);

    //Enable Char Echoing
    term.setSerialEcho(true);
    //Set Post Command Handler
    term.setPostCommandHandler(printConsoleChar);
}

void loop()
{
    // Read from serial port and handle command callbacks
    term.readSerial();
	// Refresh display every second
	maintainDisplay();
}

void printConsoleChar()
{
    Serial.print(F("> "));
}

void unknownCommand(const char *command)
{
    // Print unknown command
    Serial.print(F("Unknown command: "));
    Serial.println(command);
}

void cmdHelp()
{
    // Print usage
    Serial.println(F("Serial terminal usage:"));
    Serial.println(F("  help or ?          Print this usage"));
    Serial.println(F("  on                 Turn LED on"));
    Serial.println(F("  off                Turn LED off"));
    Serial.println(F("  n                  Start to print increasing numbers. Press CTRL-C to interrupt"));
}

void cmdLedOn()
{
    // Turn LED on
    Serial.println(F("LED on"));
    digitalWrite(LED_PIN, HIGH);
}

void cmdLedOff()
{
    // Turn LED off
    Serial.println(F("LED off"));
    digitalWrite(LED_PIN, LOW);
}

void cmdPrintNumbers()
{
	// Start to print incrementing numbers
	counter = 0;
	flagPrintNumbers = true;
}

void cmdInterrupt()
{
	// Interrupt
	flagPrintNumbers = false;
}
