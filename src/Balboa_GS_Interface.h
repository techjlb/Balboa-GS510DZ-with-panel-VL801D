// 2025-06-28 Version 1.3

#ifndef Balboa_GS_Interface_h
#define Balboa_GS_Interface_h

#include <Arduino.h>

// Balboa 510DZ & VL801D
const byte displayDataBufferSize       		= 74;		// 74 Size of display data buffer|
const byte displayDataBits             		= 71;   	// 71 71 bits length of display data within a cycle
const byte buttonDataBits              		= 4;   		// 0-4  4  bits length of button data within a cycle
const byte totalDataBits               		= 75;  		// 56-75, 75 total number of pulses within a cycle 
const unsigned int durationNewCycle    		= 10000;	// How many microsecounds to detect new cycle if no interrupt occurs 
const unsigned long buttonPressTimerMillis  = 500; 		// Timer between update temperature button presses 0.5sec
const long WaterTempInterval 				= 10000;    // Timer for uppdating water tempreture every 10sec

// Pump icon classification / stabilization tuning
const unsigned long pumpVisualWindowMillis  = 1000;    // Sampling window used to classify off/flashing/solid
const byte          pumpFlashToggleThreshold = 2;      // Minimum raw bit toggles within a window to call it "flashing"
const unsigned long pumpModeStableMillis    = 1000;    // A newly detected mode must persist this long before it is published



class BalboaInterface {

  public:
	
	BalboaInterface(byte setClockPin, byte setReadPin, byte setWritePin);

	// Semantic pump modes, derived from the display icon behaviour (off / flashing / solid)
	enum Pump1Mode { PUMP1_MODE_OFF = 0, PUMP1_MODE_LOW = 1, PUMP1_MODE_HIGH = 2 };
	enum Pump2Mode { PUMP2_MODE_OFF = 0, PUMP2_MODE_ON  = 1 };

	String pump1ModeString();							// "off" | "low" | "high"
	String pump2ModeString();							// "off" | "on"
	
	// Interface control
	void begin();										// Initializes the stream output to Serial by default
    	bool loop();                                    // Returns true if valid data is available
    	void stop();                                    // Disables the clock hardware interrupt 
    	void resetStatus();                             // Resets the state of all status components as changed for sketches to get the current status	
	void updateTemperature(float Temperature);			// Function to set the water temperature 	
	void HVACupdateTemperature(float Temperature);		// Function to set the HVAC water temperature 
	
	bool isInitialized = false;							// Define a flag to track if the initialization has been done of setTempreture on start up
	bool ModeChange = false;	
	// Status tracking
	float waterTemperature;                				// Water temperature
	// float SetTemp;                						// Set water temp  
	float setTemperature;                				// The wanted set temperature
 			  
/*
Missing bits that I belive should exist out of fualty codes pdf.
https://www.allswimltd.com/pdf/balboa-fault-codes.pdf?srsltid=AfmBOorRqk3oc4-cBM6PYkwiPf8y5Q_3vPP1zIlX71N8ZhzvZEACxIgO&utm_source=chatgpt.com

Bit triggering the "Low Temp" under 4°C freeze protection mode
ICE, IC, or FREEZE COND Message
OHH, HH, or HTR TEMP LMT SERVICE REQD Message
Bit triggering Pr – Priming mode 
"CFE" or "CONFIG ERROR"
"CrC" Checksum Error
Standby Mode, "Drain" (drn) Mode
gFI or GFCI FAILURE Message
HFL, HL, or HTR FLOW LOW Message
LF or LOW FLOW Message
PHH or PH IS HIGH LOWER PH Message
PSt or PERSIST FAIL Message
rt9 or TEST GFCI Message
rCA or CHANGE MINERAL CARTRIDGE Message
rCO or CLEAN COVER Message
rdr or DRAIN WATER Message
*/

	String LCD_display;				// The text shown on display					| Bit 1-28
	bool displayButton;        		// Up/Down button pressed 						| Bit 29
	bool TimeMenu;        			// Time menu button								| Bit 30
	bool displayBit31;        		// Still unknown functionality, if at all used! | Bit 31 
	bool displayBit32;        		// Still unknown functionality, if at all used!	| Bit 32
	bool displayBit33;        		// ? ON ?										| Bit 33
	bool displayBit34;				// Still unknown functionality, if at all used! | Bit 34 
	bool displayBit35;				// Still unknown functionality, if at all used!	| Bit 35 
	bool Filter1;        			// Filter 1										| Bit 36
	bool Filter2;					// Filter 2 									| Bit 37
   	bool TempUp;					// Temp UP 										| Bit 38 
   	bool Heater;					// Heater running or not  						| Bit 39
   	bool TempMenu;					// Display Temp menu / Set						| Bit 40
   	bool displayBit41;				// Still unknown functionality, if at all used! | Bit 41
	bool Blower;                  	// Blower running or not 						| Bit 42
	bool Filtration;				// Show Filtrations								| Bit 43			*shows on in menu under filters and also when filter 1 & 2 are on.			
	bool displayBit44;				// Jumps on/off									| Bit 44			*Pulse on/off 1sec every 5-15min and sometimes every 30sec- 2min, Start the temp sensor ? 
	bool displayBit45;				// Jumps on/off									| Bit 45			*Pulse on/off 1 sec every 1-5min
	bool displayBit46;				// Still unknown functionality, if at all used! | Bit 46			* shows as 0 or off havent got it to change
	bool Lights;        			// SPA lights activated or not 					| Bit 47
	bool rawPump1;					// Raw Pump 1 display icon segment: on while solid, and toggling on/off while flashing | Bit 48
	bool rawPump2;					// Raw Pump 2 display icon segment: on while solid, and toggling on/off while flashing | Bit 49
	bool Pump1;        				// Compatibility bool, derived from pump1Mode (true if not off)
	bool Pump2;        				// Compatibility bool, derived from pump2Mode (true if not off)
	Pump1Mode pump1Mode = PUMP1_MODE_OFF;	// Semantic, stabilized Pump 1 mode: off/low/high
	Pump2Mode pump2Mode = PUMP2_MODE_OFF;	// Semantic, stabilized Pump 2 mode: off/on
	bool STOP;						// Fillter STOP time  							| Bit 50			
	bool displayBit51;				// Still unknown functionality, if at all used! | Bit 51
	bool displayBit52;				// Still unknown functionality, if at all used! | Bit 52
	bool displayBit53;				// Still unknown functionality, if at all used! | Bit 53
	bool displayBit54;				// Still unknown functionality, if at all used! | Bit 54	
	bool displayTIME;				// Display show TEXT "TIME" small I belive		| Bit 55 
	bool displaySET;				// Display show TEXT "SET" small I belive		| Bit 56
	bool displayBit57;				// Still unknown functionality, if at all used! | Bit 57
	bool START; 					// Fillter START time  							| Bit 58			
	bool StandardMode;            	// Standard mode activated or not 				| Bit 59
	bool EcoMode;                  	// Eco mode mode activated or not 				| Bit 60 
	bool displayPM;					// Display PM under TIME menu					| Bit 61
	bool displayBit62;				// Display SET under Prog/mode menu ?			| Bit 62
	bool displayAM;					// Display AM under TIME menu					| Bit 63
	bool TempDown;					// Temp DOWN 									| Bit 64
	bool ModeProg;					// Mode/Prog button (0 on | 1 off )				| Bit 65
	bool displayBit66;				// Still unknown functionality, if at all used! | Bit 66 			
	bool displayBit67;				// Still unknown functionality, if at all used! | Bit 67			 
	bool displayBit68;				// Still unknown functionality, if at all used! | Bit 68
	bool displayBit69;				// Still unknown functionality, if at all used! | Bit 69
	bool displayBit70;				// Still unknown functionality, if at all used! | Bit 70
	bool displayBit71;				// Still unknown functionality, if at all used! | Bit 71

	
	

	static bool displayDataBufferOverflow;		
	
	// Write button data to control unit  
	static bool writeDisplayData;            	// If something should be written to button data line  
	static bool writeButtonUp;
	static bool writeButtonDown;
	static bool writeTempUp;
	static bool writeTempDown;
	static bool writeLights;
	static bool writePump1;
	static bool writePump2;			
	static bool writeBlower;
	static bool writeSTDMode;			
	static bool writeEcoMode;
	static bool writeTimeMenu;
	static bool writeModeProg;

  private:
	
	static void clockPinInterrupt();
	void decodeDisplayData();
	String lockup_LCD_character(int LCD_character);
	int LCD_segment_1;
	int LCD_segment_2;
	int LCD_segment_3;
	int LCD_segment_4;
	String LCD_display_1;
	String LCD_display_2;
	String LCD_display_3;
	String LCD_display_4;  

	// Pump icon classification: distinguish "off" / "flashing" / "solid" visual states
	// from the raw display bit, since the panel blinks the icon to indicate low speed.
	enum PumpVisualState { PUMP_VISUAL_OFF, PUMP_VISUAL_FLASHING, PUMP_VISUAL_SOLID };
	// Shared window/toggle-detection helper used by both pumps. Returns true (and a fresh
	// visual classification via 'visualState') once a full sampling window has elapsed.
	// 'windowStarted' seeds windowStartMillis on the first call instead of relying on an
	// uninitialized 0 value (the unsigned millis() subtraction itself already tolerates rollover).
	bool updatePumpVisualState(bool rawState, bool &prevRaw, unsigned long &windowStartMillis, bool &windowStarted,
								byte &toggleCount, PumpVisualState &visualState);
	// Shared stability-timer helper: a newly detected 'target' mode must be seen consistently
	// for pumpModeStableMillis before it is reported as stable (returns true), so a single
	// transient/misclassified window can't immediately flip the published pump mode.
	bool applyStableMode(int target, int currentMode, int &pendingMode, unsigned long &pendingSinceMillis);
	void classifyPump1();
	void classifyPump2();

	bool pump1PrevRaw;
	unsigned long pump1WindowStartMillis;
	bool pump1WindowStarted;
	byte pump1ToggleCount;
	PumpVisualState pump1VisualState;
	int pump1PendingMode;
	unsigned long pump1PendingSinceMillis;

	bool pump2PrevRaw;
	unsigned long pump2WindowStartMillis;
	bool pump2WindowStarted;
	byte pump2ToggleCount;
	PumpVisualState pump2VisualState;
	int pump2PendingMode;
	unsigned long pump2PendingSinceMillis;
	static byte displayDataBuffer[displayDataBufferSize]; 	// Array of display data measurements 
	static unsigned long clockInterruptTime;
	static int clockBitCounter;               		 		// Counter of pulses within a cycle
	static byte dataIndex; 									
	static bool displayDataBufferReady;            			// Is buffer available to be decoded
	static byte clockPin;
    static byte displayPin;
    static byte buttonPin;

	int updateTempDirection;
	int updateTempButtonPresses;
	unsigned long buttonPressTimerPrevMillis; 
};

  
#endif  // Balboa_GS_Interface_h
