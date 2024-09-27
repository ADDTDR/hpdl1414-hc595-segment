#include <bluefruit.h>

int latchPin = D9; // Latch pin of 74HC595 is connected to Digital pin 10
int clockPin = D10;  // Clock pin of 74HC595 is connected to Digital pin 9
int dataPin = D8;   // Data pin of 74HC595 is connected to Digital pin 8

void setup() {
    // Set all the pins of 74HC595 as OUTPUT
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void updateShiftRegister(uint8_t val, uint8_t pos) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, val);
    shiftOut(dataPin, clockPin, MSBFIRST, pos);
    digitalWrite(latchPin, HIGH);
}

void place_on_display(uint8_t val, uint8_t pos) {
    // On 2 modules, 8 places address 0 - 7 
    uint8_t base = 0b11000000;

    if (pos <= 3) {
        base = 0b11100000;
    }
    pos = pos << 2; 
    updateShiftRegister(val << 1, base | pos);
}

void print_on_display(const String &str) {
    for (int i = str.length() - 1; i >= 0; --i) {
        place_on_display(str[i], str.length() - 1 - i);
    }
}

// Function to rotate the string and display it
void rotate_string_and_display(const String &str, int delay_ms) {
    String rotated_str = str;
    int cy = 0;

    // Run for a specified number of cycles
    while (cy < 85) {
        for (size_t i = 0; i < rotated_str.length(); ++i) {
            // Create a circular substring by appending the first characters at the end
            String display_str = rotated_str.substring(i, i + 8);
            if (display_str.length() < 8) {
                display_str += rotated_str.substring(0, 8 - display_str.length());
            }

            // Print the display substring
            print_on_display(display_str);
            delay(delay_ms);
            cy++;
        }
    }
}

// Array of jokes in uppercase
String jokes[] = {
    "I TOLD MY WIFE SHE WAS DRAWING HER EYEBROWS TOO HIGH. SHE LOOKED SURPRISED.",
    "WHY DON’T SKELETONS FIGHT EACH OTHER? THEY DON’T HAVE THE GUTS.",
    "I WOULD AVOID THE SUSHI IF I WAS YOU. IT’S A LITTLE FISHY.",
    "WHAT’S ORANGE AND SOUNDS LIKE A PARROT? A CARROT.",
    "I ONLY KNOW 25 LETTERS OF THE ALPHABET. I DON’T KNOW Y.",
    "WHY DID THE SCARECROW WIN AN AWARD? BECAUSE HE WAS OUT-STANDING IN HIS FIELD.",
    "I USED TO BE IN SHAPE. THEN I CHOSE ROUND.",
    "WHY CAN'T YOU GIVE ELSA A BALLOON? BECAUSE SHE WILL LET IT GO.",
    "WHAT DO YOU CALL FAKE SPAGHETTI? AN IMPASTA.",
    "I'M READING A BOOK ON ANTI-GRAVITY. IT'S IMPOSSIBLE TO PUT DOWN!",
    "WHY DID THE GOLFER BRING TWO PAIRS OF PANTS? IN CASE HE GOT A HOLE IN ONE.",
    "WHAT DO YOU CALL A BEAR WITH NO TEETH? A GUMMY BEAR.",
    "I TOLD MY COMPUTER I NEEDED A BREAK, AND NOW IT WON'T STOP SENDING ME BEACH PICTURES.",
    "WHY DID THE CHEMISTRY TEACHER BREAK UP WITH THE BIOLOGY TEACHER? THERE WAS NO CHEMISTRY."
};

void loop() {
    // Iterate over each joke and rotate the string to display it
    for (const String& joke : jokes) {
        rotate_string_and_display(joke, 120);
    }
}
