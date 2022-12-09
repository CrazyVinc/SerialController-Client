// Include SimpleCLI Library
#include <SimpleCLI.h>

// Create CLI Object
SimpleCLI cli;

// Commands
Command RGB;

// Callback function for sum command
void RGBCallback(cmd* c) {
    Command cmd(c);               // Create wrapper object
    int argNum = cmd.countArgs(); // Get number of arguments

    // Go through all arguments and set the pin value's
    for (int i = 0; i<argNum; i++) {
        Argument arg      = cmd.getArg(i);
        String   argValue = arg.getValue();

        int split = argValue.indexOf(':');
        if(split == -1) continue;
        
        int pin = argValue.substring(0, split).toInt();
        int value = argValue.substring(split+1).toInt();
        analogWrite(pin, value);
    }
    Serial.println("OK");
}

// Callback in case of an error
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}

void setup() {
    // Startup stuff
    Serial.begin(9600);
    Serial.println("Ready!");
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 255);

    cli.setOnError(errorCallback); // Set error Callback

    RGB = cli.addBoundlessCommand("RGB", RGBCallback);
}






const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void loop() {
    recvWithEndMarker();
    showNewData();
    if (cli.errored()) {
        CommandError cmdError = cli.getError();

        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        cli.parse(receivedChars);
        newData = false;
    }
}
