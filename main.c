//Global Initialization
#include <p30f4011.h> 

//Device Configuration
//_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF)
_FOSC(CSW_FSCM_OFF & FRC_PLL16);    /* Internal Osc PLLx16 = 118MIP */
_FWDT(WDT_OFF);                     /* Watchdog Off */
_FBORPOR(PBOR_OFF & MCLR_EN);       /* Brownout Disabled - MCLR Enabled */
_FGS(CODE_PROT_OFF);                /* Code Protect OFF */

// ***************************************** Each of the following arrays has six different values, one for each string ***************************************************

// the bias voltages, used to determine what V is wiggling around
// in otherwords, VBIAS is the zero-line of the sine wave going into the ADC
int const VBIAS[6] = {511, 511, 511, 511, 511, 511};

// the threshold voltages for zero detection, hysteresis (sp?)
// thresholds are used to reduce error by creating more regions in which the sine wave can be tracked
int const VTHRESH[6] = {50, 40, 40, 30, 40, 40};

// these two arrays are used to minimized the number of arithmatic instructions required in the finite state machine
// they define the state boundries
//int VBPLUSVT[6] = {VBIAS[0]+VTHRESH[0], VBIAS[1]+VTHRESH[1], VBIAS[2]+VTHRESH[2], VBIAS[3]+VTHRESH[3], VBIAS[4]+VTHRESH[4], VBIAS[5]+VTHRESH[5]}; 
//int VBMINUSVT[6] = {VBIAS[0]-VTHRESH[0], VBIAS[1]-VTHRESH[1], VBIAS[2]-VTHRESH[2], VBIAS[3]-VTHRESH[3], VBIAS[4]-VTHRESH[4], VBIAS[5]-VTHRESH[5]};
int const VBPLUSVT[6] = {571, 551, 551, 541, 551, 551};
int const VBMINUSVT[6] = {461, 471, 471, 481, 471, 471};

const char MSPLUCKDELAY[6] = {6, 5, 5, 5, 5, 5};
const char QUARTERSECTIMEOUTDELAY = 4;

// this massive look up table is indexed by the period in terms of TMR units and returns the MIDI value for the note.
const char LUT[2159] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
79,79,79,79,79,79,79,79,78,78,78,78,78,78,78,78,77,77,77,77,77,77,77,77,76,76,76,76,76,76,76,76,76,75,75,75,75,75,75,75,75,75,75,74,74,74,74,74,74,74,74,74,73,73,73,73,73,73,73,
73,73,73,73,72,72,72,72,72,72,72,72,72,72,72,72,71,71,71,71,71,71,71,71,71,71,71,71,70,70,70,70,70,70,70,70,70,70,70,70,69,69,69,69,69,69,69,69,69,69,69,69,69,69,68,68,68,68,68,
68,68,68,68,68,68,68,68,68,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,64,64,
64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,61,61,61,
61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,	 59,59,59,59,59,59,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,52,52,52,52,52,52, 52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52, 52,52,52,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51, 51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,50,50,50,50,50,50,50,50, 50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50, 50,50,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49, 49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,48,48,48,48,48,48,48,48,48,48, 48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48, 48,48,48,48,48,48,48,48,48,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47, 47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47, 47,47,47,47,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46, 46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,45, 45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45, 45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45, 44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44, 44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44, 44,44,44,44,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43, 
43,43,43,43,43,43,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42, 42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42, 42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,41,41,41,41,41,41,41,41,41,41,41, 41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41, 41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41, 41,41,41,41,41,41,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40, 40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40, 40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,39,39, 39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39, 39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39, 39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,38, 38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38, 38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38, 38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,37,37,37, 37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37, 37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37, 37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37, 37,37,37,37,37,37,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36,36,36,36,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35, 35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35, 35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35, 35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35, 34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34, 34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34, 34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34, 34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34, 34,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33, 33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33, 33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33, 33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,32,32,32,32, 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32, 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32, 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32, 32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32, 32,32,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31, 31,31,31,31,31,31,31,31,31,31};

/* changed keeps track of changes caused by inputs
	the interrupts request a change and the actual change command to MIDI is implemented in the main loop
	when changed = 0, no changes are needed for the current input
	when changed = 1, a change is needed
	the first six elements represent changes to the guitar strings
	the following four represent the mute, volume up, volume down, and sustain digital buttons
*/
volatile char changed[9] = {0,0,0,0,0,0,0,0,0};			

// stores the current TMR value for timing out
volatile unsigned int timeOut[6] = {0,0,0,0,0,0,};

// this is incremented every .25 seconds by the TMR3 interrupt, used for timeout
unsigned int timeOutTMRVal = 0;

// stores the current TMR value for the initial 5 ms delay after hitting a note
volatile unsigned int wait5ms[6] = {0,0,0,0,0,0,};

// this is incremented every ..001 seconds by the TMR1 interrupt, used for initial delay
unsigned int wait5msTMRVal = 0;

// tracks if the volume up button was pressed
char volumeUpHit = 0;

//  makes sure that a change will occur after getting the first frequency value after Sync state
volatile char firstAfterSync[6] = {1,1,1,1,1,1};
			
//******** These arrays store information on period/frequency detection***************************
// stores the TMR value of when the last zero occurred, used for frequency detection
volatile unsigned int lastZero[6] = {0,0,0,0,0,0,};

// zeroCount stores of the current amount of zero-crossings, its used for period detection
volatile char zeroCount[6] = {0,0,0,0,0,0};

// period stores the calculated period
// it is the correct MIDI pitch
volatile char period[6] = {0,0,0,0,0,0};

// lastPeriod stores the last calculated period
// it is the correct MIDI pitch
volatile char lastPeriod[6] = {0,0,0,0,0,0};


//******** These arrays store information on amplitude/velocity detection**************************
// ampltiude stores the previous two values of the highest detected peaks, it is used for amplitude averaging
volatile int amplitude[2][6] = {{511,511,511,511,511,511},{511,511,511,511,511,511}};

// velocity stores the average highest detected value for a three periods 
// it is used to output the volume, or velocity, on MIDI
volatile int velocity[6] = {511,511,511,511,511,511};

volatile int lastVelocity[6] = {511,511,511,511,511,511};

//peakValue stores the calculated maximum value
//it is used to find the maximum amplitude of the signal each period
volatile int peakValue[6] = {0,0,0,0,0,0,};


// the STATE enumerator keeps track of the current state
// See the finite state machine section to understand this part
enum STATE            /* Defines an enumeration type    */
{
sync = 0,		//0 The sync state, all the outputs are set to 0 when the sync state is entered
trough,			//1 The signal is below the threshold- voltage
rise1,			//2 The signal is below the bias voltage, but above the theshold- voltage, a 0 is detected when it transfer to rise2
rise2,			//3 The signal is above the bias voltage, but below the threshold+ voltage
peak,			//4 The signal is above the threshold+ voltage, peak detection happends here, the highest peak is sent out on transfer to fall1
fall1,			//5 The signal is below the threshold+ voltage, but above the bias voltage
fall2,			//6 The signal is below the bias voltage, but above the threshold- voltage
wait			//7 Occurs after a note is struck when in sync
};
enum STATE currentState[6] = {sync, sync, sync, sync, sync, sync};

// this function will send a MIDI data segment out UART
void midiOut(char code, char data1, char data2)
{
	// each of these busy loops are to ensure that the transmit buffer is empty before a new value is written
	// a MIDI data segment consists of three 8-bit numbers sent one after another
		while(U2STAbits.UTXBF);
		U2TXREG = code;
		while(U2STAbits.UTXBF);
		U2TXREG = data1;
		while(U2STAbits.UTXBF);
		U2TXREG = data2;
}

//Main Initialization
int main(void)
{
	// Non-Global Variable Initialization **************************************************************
	
	// stores the output MIDI volume
	char volume = 63;
	//  boolean character to keep track if the system is muted
	char isMuted = 0;
	// boolean character  to keep track ofthe system is in sustain
	char isInSustain = 0;
	
	// ADC initialization *******************************************************************************
	
	/*	ADCON1: A/D Control Register 1	
		bit 15 ADON: A/D Operating Mode bit
			0 = A/D converter is off
		bit 13 ADSIDL: Stop in Idle Mode bit
			1 = Discontinue module operation when device enters Idle mode
		bit 9-8 FORM<1:0>: Data Output Format bits
			00 = Integer (DOUT = 0000 00dd dddd dddd)
		bit 7-5 SSRC<2:0>: Conversion Trigger Source Select bits
111 = Internal counter ends sampling and starts conversion (auto convert)
bit 3 SIMSAM: Simultaneous Sample Select bit (only applicable when CHPS = 01 or 1x)
			0 = Samples multiple channels individually in sequence
			1 = Samples CH0, CH1, CH2, CH3 simultaneously (when CHPS = 1x)
				or
				Samples CH0 and CH1 simultaneously (when CHPS = 01)
		bit 2 ASAM: A/D Sample Auto-Start bit
1 = Sampling begins immediately after last conversion completes. SAMP bit is auto set
		bit 1 SAMP: A/D Sample Enable bit
			1 = At least one A/D sample/hold amplifier is sampling
		bit 0 DONE: A/D Conversion Status bit
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		   FEDCBA9876543210
	ADCON1 = 0b0000000011101110;
	
	/*
	ADCON2: A/D Control Register 2
		bit 15-13 VCFG<2:0>: Voltage Reference Configuration bits
				A/D VREFH 			A/D VREFL
			000 AVDD 				AVSS
bit 10 CSCNA: Scan Input Selections for CH0+ S/H Input for MUX A Input Multiplexer Setting bit
			1 = Scan inputs
		bit 9-8 CHPS<1:0>: Selects Channels Utilized bits
			1x = Converts CH0, CH1, CH2 and CH3
			When SIMSAM bit (ADCON1<3>) = 0 multiple channels sampled sequentially
When SMSAM bit (ADCON1<3>) = 1 multiple channels sampled as in CHPS<1:0>
		bit 7 BUFS: Buffer Fill Status bit
		bit 5-2 SMPI<3:0>: Sample/Convert Sequences Per Interrupt Selection bits
0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence
0011 = Interrupts at the completion of conversion for each 4th sample/convert sequence
			0100 5
			0101 6
			0110 7
			0111 8
		bit 1 BUFM: Buffer Mode Select bit
			0 = Buffer configured as one 16-word buffer ADCBUF(15...0)
		bit 0 ALTS: Alternate Input Sample Mode Select bit
1 = Uses MUX A input multiplexer settings for first sample, then alternate between MUX B and MUX A input
			multiplexer settings for all subsequent samples
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		   FEDCBA9876543210
	ADCON2 = 0b0000001000011101;
	
	/*
	ADCON3: A/D Control Register 3 
		bit 12-8 SAMC<4:0>: Auto-Sample Time bits
		    11111 = 31 TAD
			00010 = 2 TAD
		bit 7 ADRC: A/D Conversion Clock Source bit
			1 = A/D internal RC clock
			0 = Clock derived from system clock
		bit 5-0 ADCS<5:0>: A/D Conversion Clock Select bits
			000100 = TCY/2 • (ADCS<5:0> + 1) = 5 • TCY/2
			001011 = TCY/2 • (ADCS<5:0> + 1) = 12 • TCY/2 
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		     FEDCBA9876543210
	//ADCON3 = 0b0111111000001011;
	ADCON3 =   0b0000111100000111;
	
	/*
	ADCHS: A/D Input Channel Select Register (Simultaneous)
bit 15-14 CH123NB<1:0>: Channel 1, 2, 3 Negative Input Select for MUX B Multiplexer Setting bits
			Same definition as bits 6-7 (Note)
bit 13 CH123SB: Channel 1, 2, 3 Positive Input Select for MUX B Multiplexer Setting bit
			Same definition as bit 5 (Note)
bit 12 CH0NB: Channel 0 Negative Input Select for MUX B Multiplexer Setting bit
			Same definition as bit 4 (Note)
bit 11-8 CH0SB<3:0>: Channel 0 Positive Input Select for MUX B Multiplexer Setting bits
			Same definition as bits 3-0 (Note)
bit 7-6 CH123NA<1:0>: Channel 1, 2, 3 Negative Input Select for MUX A Multiplexer Setting bits
			0x = CH1, CH2, CH3 negative input is VREF-
bit 5 CH123SA: Channel 1, 2, 3 Positive Input Select for MUX A Multiplexer Setting bit
0 = CH1 positive input is AN0, CH2 positive input is AN1, CH3 positive input is AN2
bit 4 CH0NA: Channel 0 Negative Input Select for MUX A Multiplexer Setting bit
			0 = Channel 0 negative input is VREF-
bit 3-0 CH0SA<3:0>: Channel 0 Positive Input Select for MUX A Multiplexer Setting bits
			0010 = Channel 0 positive input is AN2
			0000 = Channel 0 positive input is AN0
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	ADCHS = 0b0010000000000000; 	// Connect RB2/AN2 as CH0 input and also connect
	
	/*
	ADPCFG: A/D Port Configuration Register
		bit 15-0 PCFG<15:0>: Analog Input Pin Configuration Control bits
1 = Analog input pin in Digital mode, port read input enabled, A/D input multiplexer input connected to AVSS
0 = Analog input pin in Analog mode, port read input disabled, A/D samples pin voltage
	*/
	// Hex index below for easy bit-finding
	// 		   FEDCBA9876543210
	ADPCFG = 0b1111111111000000;	// set the first six input pins to Analog mode, one for each guitar string
	
	/*
	ADCSSL: A/D Input Scan Select Register (Sequential)
		bit 15-0 CSSL<15:0>: A/D Input Pin Scan Selection bits
			1 = Select ANx for input scan
			0 = Skip ANx for input scan
	*/
	// Hex index below for easy bit-finding
	// 		   FEDCBA9876543210
	ADCSSL = 0b0000000000000000;	// select AN0-5 for input scan, skip the others
	
	
	
	//	Timer Initializations**************************************************************************

	/* 
	Timer 1 is used for initial delay after a note is struck
	T2CON: Timer 2 Control Register
		bit 15 TON: Timer On bit
			1 = Starts 16-bit timer
		bit 13 TSIDL: Stop in Idle Mode bit
			0 = Continue timer operation in Idle mode
		bit 6 TGATE: Timer Gated Time Accumulation Enable bit
			1 = Timer gated time accumulation enabled
			0 = Timer gated time accumulation disabled
			(TCS must be set to logic ‘0’ when TGATE = 1)
		bit 5-4 TCKPS<1:0>: Timer Input Clock Prescale Select bits
			11 = 1:256 prescale value
			10 = 1:64 prescale value
			01 = 1:8 prescale value
			00 = 1:1 prescale value
		bit 3 T32: 32-bit Timer Mode Select bits
			0 = TMRx and TMRy form separate 16-bit timer
		bit 1 TCS: Timer Clock Source Select bit
			0 = Internal clock (FOSC/4)
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	T1CON = 0b1010000000110000;

	/*
	PR1 is the period register for TMR1
		When TMR1 counts to the PR1 value, the TMR1 interrupt is thrown
	*/
	// .001 seconds
	PR1 = 116;

	/* 
	Timer 2 is used for period calculation
	T2CON: Timer 2 Control Register
		bit 15 TON: Timer On bit
			1 = Starts 16-bit timer
		bit 13 TSIDL: Stop in Idle Mode bit
			0 = Continue timer operation in Idle mode
		bit 6 TGATE: Timer Gated Time Accumulation Enable bit
			1 = Timer gated time accumulation enabled
			0 = Timer gated time accumulation disabled
			(TCS must be set to logic ‘0’ when TGATE = 1)
		bit 5-4 TCKPS<1:0>: Timer Input Clock Prescale Select bits
			11 = 1:256 prescale value
			10 = 1:64 prescale value
			01 = 1:8 prescale value
			00 = 1:1 prescale value
		bit 3 T32: 32-bit Timer Mode Select bits
			0 = TMRx and TMRy form separate 16-bit timer
		bit 1 TCS: Timer Clock Source Select bit
			0 = Internal clock (FOSC/4)
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	T2CON = 0b1010000000110000;
	
	/* 
	Timer 3 is used for the timeout condition
	T3CON: Timer 3 Control Register
		bit 15 TON: Timer On bit
			1 = Starts 16-bit timer
		bit 13 TSIDL: Stop in Idle Mode bit
			0 = Continue timer operation in Idle mode
		bit 6 TGATE: Timer Gated Time Accumulation Enable bit
			1 = Timer gated time accumulation enabled
			0 = Timer gated time accumulation disabled
			(TCS must be set to logic ‘0’ when TGATE = 1)
		bit 5-4 TCKPS<1:0>: Timer Input Clock Prescale Select bits
			11 = 1:256 prescale value
			10 = 1:64 prescale value
			01 = 1:8 prescale value
			00 = 1:1 prescale value
		bit 3 T32: 32-bit Timer Mode Select bits
			0 = TMRx and TMRy form separate 16-bit timer
		bit 1 TCS: Timer Clock Source Select bit
			0 = Internal clock (FOSC/4)
		other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	T3CON = 0b1010000000110000;
	
	/*
	PR3 is the period register for TMR3
		When TMR3 counts to the PR3 value, the TMR3 interrupt is thrown
	*/
	// .25 seconds
	PR3 = 29034;

	// Interrupt Initializations ***************************************************************
	
	/* 
	Interrupt Priorities
	INT0IP<2:0>: External Interrupt 0 Priority bits
	INT11P<2:0>: External Interrupt 1 Priority bits
	INT2IP<2:0>: External Interrupt 2 Priority bits
	ADIP<2:0>: A/D Conversion Complete Interrupt Priority bits
		111 = Interrupt is priority 7 (highest priority interrupt)
		•
		•
		•
		001 = Interrupt is priority 1
		000 = Interrupt source is disabled
	*/

	IPC2bits.ADIP = 0b0000000000000110;	// ADC interrupt is very important, has high priority
	IPC0bits.INT0IP  = 0b0000000000000100;
	IPC4bits.INT1IP  = 0b0000000000000011;
	IPC5bits.INT2IP = 0b0000000000000011;
	IPC0bits.T1IP = 0b0000000000000101;
	IPC1bits.T3IP = 0b0000000000000101;
	
	/*
	Interrupt Flag Resets
		ADIF: ADC Interrupt Flag bit
		INT0IF: External Interrupt 0 Flag bit
		INT1IF: External Interrupt 0 Flag bit
		INT2IF: External Interrupt 0 Flag bit
	*/	
	IFS0bits.ADIF = 0;	
	IFS0bits.INT0IF = 0;
	IFS1bits.INT1IF = 0;
	IFS1bits.INT2IF = 0;
	IFS0bits.T1IF = 0;
	IFS0bits.T3IF = 0;
	
	/*
	Interrupt Enable Sets
		AD1E: ADC Interrupt Enable bit
		INT0IE: External Interrupt 0 Enable bit
		INT1IE: External Interrupt 1 Enable bit
		INT2IE: External Interrupt 2 Enable bit
	*/	
	IEC0bits.ADIE = 1;
	IEC0bits.INT0IE = 1;
	IEC1bits.INT1IE = 1;
	IEC1bits.INT2IE = 1;
	
	// IO Initializations ********************************************************************
	
	/*
	UART Mode Register
	bit 15 UARTEN: UART Enable bit
1 = UART is enabled. UART pins are controlled by UART as defined by UEN<1:0> and UTXEN control bits.
	bit 13 USIDL: Stop in Idle Mode bit
		1 = Discontinue operation when device enters Idle mode
	bit 10 ALTIO: UART Alternate I/O Selection bit
		0 = UART communicates using UxTX and UxRX I/O pins
	bit 7 WAKE: Enable Wake-up on Start bit Detect During Sleep Mode bit
		0 = Wake-up disabled
	bit 6 LPBACK: UART Loopback Mode Select bit
		1 = Enable Loopback mode
		0 = Loopback mode is disabled
	bit 5 ABAUD: Auto Baud Enable bit
		1 = Input to Capture module from UxRX pin
		0 = Input to Capture module from ICx pin
	bit 2-1 PDSEL<1:0>: Parity and Data Selection bitsy
		00 = 8-bit data, no parity
	bit 0 STSEL: Stop Selection bit
		0 = 1 Stop bit
	other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		   FEDCBA9876543210
	U2MODE = 0b1010000000100000;
	
	/*
	UART Status and Control Register
	bit 15 UTXISEL: Transmission Interrupt Mode Selection bit
1 =Interrupt when a character is transferred to the Transmit Shift register and as result, the transmit buffer becomes empty
	bit 11 UTXBRK: Transmit Break bit
		1 = UxTX pin is driven low, regardless of transmitter state
		0 = UxTX pin operates normally
	bit 10 UTXEN: Transmit Enable bit
		1 = UART transmitter enabled, UxTX pin controlled by UART (if UARTEN = 1)
	bit 9 UTXBF: Transmit Buffer Full Status bit (Read Only)
	bit 8 TRMT: Transmit Shift Register is Empty bit (Read Only)
	bit 7-6 URXISEL<1:0>: Receive Interrupt Mode Selection bit
	bit 5 ADDEN: Address Character Detect (bit 8 of received data = 1)
		0 = Address Detect mode disabled
	bit 4 RIDLE: Receiver Idle bit (Read Only)
	bit 3 PERR: Parity Error Status bit (Read Only)
	bit 2 FERR: Framing Error Status bit (Read Only)
	bit 1 OERR: Receive Buffer Overrun Error Status bit (Read/Clear Only)
	bit 0 URXDA: Receive Buffer Data Available bit (Read Only)
	other bits are unimplemented, 0 is used
	*/
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	U2STA = 0b0000010000000000;
	
	/*
	UART Baud Rate Generator Register
		bit 15-0 BRG<15:0>: Baud Rate Divisor bits
		BRG = Fosc/(16*MIDI_Baud_Rate)-1 = 3*10^7/(16*31250) = 59
		0% baud rate error
	*/
	U2BRG = 58;

	/*
		TRIS sets 1=input 0=output
		Port B is used for the 6 guitar strings; RB0, RB1, RB2, RB3, RB4, RB5
		Port D is used for the input for volume control and sustain; RD0, RD1, RD2
		Port E is used for the input for mute; RE5
		Port F (BIT 4 Note:incorrect documentation says BIT 3) Used for Midi Output; RF3
	*/
	PORTF = 0x0000;
	// Hex index below for easy bit-finding
	// 		  FEDCBA9876543210
	TRISF = 0b0000000000000000;


	char i, c, v;
	char inRange = 1;
	
	// turn on ADC
	ADCON1bits.ADON = 1;
	// turn on TMR3 interrupt
	IEC0bits.T3IE = 1;
	// turn on TMR1 interrupt
	IEC0bits.T1IE = 1;
 
Main Loop
while(1)
	{
		PORTFbits.RF4 = 1;
		// step through each element in the changed array, to check for any changes
		for(i = 0; i < 6; i ++)
		{
// if the 'i'th element of changed is 1, then a change has occured and needs to be set
			if(changed[i] == 1)
			{
				// if the 'i'th string has changed
				if(i < 6)
				{
					// note change for channel i
// send the seven most significant bits of the amplitude
// send the correct pitch using the pitch LUT with the period as its index
// the first message is a note off command, the second is a note on
					switch(i)
					{
						case(1):
							if(period[i] > 55)
								inRange = 0;
						break;
						case(2):
							if(period[i] > 60 || period < 36)
								inRange = 0;
						break;
						case(3):
							if(period[i] > 55 || period < 41)
								inRange = 0;
						break;
						case(4):
							if(period[i] > 65 || period < 46)
								inRange = 0;
						break;
						case(5):
							if(period[i] > 70 || period < 50)
								inRange = 0;
						break;
						case(6):
							if(period < 55)
								inRange = 0;
						break;	
					}
					if((inRange == 1 || period[i] == 0) && velocity[i] > lastVelocity[i] - 25)
					{
						midiOut(128+i, lastPeriod[i], 0);
						v = (char)((velocity[i]-VBIAS[i])>>2);
						if(v < 42)
							v = v*3;
						midiOut(144+i, period[i], v);
						lastPeriod[i] = period[i];
						lastVelocity[i] = velocity[i];
					}
					else
					{
						inRange = 1;
					}
				}
				
				// if the MUTE button has been hit
				else if(i == 6)
				{
					// if the volume is not muted, mute it
					if(isMuted == 0)
					{
						// control/mode change for all 6 channels
						// select master volume control
						// set master volume to 0
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 7, 0);
						}
						// set the muted variable to on
						isMuted = 1;
					}
					else
					{
						// control/mode change for all 6 channels
						// select master volume control
						// set master volume to value of volume
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 7, volume);
						}
						// set the muted variable to off
						isMuted = 0;
					}	
					// enable external interrupt 0
					// this code is placed here instead of in the interrupt in order to reduce bouncing
					IEC0bits.INT0IE = 1;
				}
				
				// if the VOLUME button has been hit
				else if(i == 7)
				{
					// if down up was hit
					if(volumeUpHit == 0)
					{
// ensure that volume will not over underflow its 7-bit limit
						if(volume < 5)
							volume = 0;
						else
							volume -= 5;
						// control/mode change for all 6 channels
						// select master volume control
						// set master volume to value of volume
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 7, volume);
						}	
					}
					// if volume down was hit
					else
					{
// ensure that volume will not over overflow its 7-bit limit
						if(volume > 122)
							volume = 127;
						else
							volume += 5;						
						// control/mode change for all 6 channels
						// select master volume control
						// set master volume to value of volume
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 7, volume);
						}
					}
					// enable external interrupt 1
					// this code is placed here instead of in the interrupt in order to reduce bouncing
					IEC1bits.INT1IE = 1;					
				}
				
				// if the SUSTAIN button has been hit
				else
				{
					if(isInSustain == 0)
					{
						// control/mode change for all channels
						// set the MIDI control to sustain
						// set the sustain on
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 64, 127);
						}
						// set the sustain variable to on
						isInSustain = 1;
					}
					else
					{
						// control/mode change for all channels
						// set the MIDI control to sustain
						// set the sustain off
						for(c=0; c<6; c++)
						{
							midiOut(176+c, 64, 0);
						}
						// set the sustain variable to off
						isInSustain = 0;
					}
					// enable external interrupt 2
					// this code is placed here instead of in the interrupt in order to reduce bouncing
					IEC1bits.INT2IE = 1;					
				}
			
				// reset changed after the change has been made
				changed[i] = 0;
			}
		}
	}
/*************************************************************
These four interrupts control the digital IO interface.  Mute, Volume Control, and Sustain are implemented
**************************************************************/

// Mute
// External Interrupt 0 - mapped to TRISD0 - PORTDbits.RE8
void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{
	// disable external interrupt 0
	IEC0bits.INT0IE = 0;
	// reset the external interrupt flag
	IFS0bits.INT0IF = 0;
	// indicate that a change has been made
	changed[6] = 1;
}

// Volume Check
// External Interrupt 1 - mapped to TRISD1 - PORTDbits.RD0
void __attribute__((interrupt,no_auto_psv)) _INT1Interrupt(void)
{
	// disable external interrupt 1
	IEC1bits.INT1IE = 0;
	// reset the external interrupt flag
	IFS1bits.INT1IF = 0;
	// if RD2 is high, or volume up was hit
	if(PORTDbits.RD2 == 0)
		volumeUpHit = 1;
	// indicate that a change has been made
	changed[7] = 1;
}

// Sustain
// External Interrupt 2 - mapped to TRISD2 - PORTDbits.RD1
void __attribute__((interrupt,no_auto_psv)) _INT2Interrupt(void)
{
	// disable external interrupt 2
	IEC1bits.INT2IE = 0;
	// reset the external interrupt flag
	IFS1bits.INT2IF = 0;
	// indicate that a change has been made
	changed[8] = 1;
}

/*************************************************************
	Timer 1 Interrupt
		This interrupt is used for initial delay after a string is struck
		it will be called every .001 seconds
*************************************************************/
void __attribute__((interrupt,no_auto_pvs)) _T1Interrupt(void)
{	
	_T1IF = 0;				// clear interrupt
	wait5msTMRVal ++;
	if(wait5msTMRVal == 65500)
		wait5msTMRVal = 0;
}

/*************************************************************
	Timer 3 Interrupt
		This interrupt is used for timeout on strings
		it will be called every .25 seconds
*************************************************************/
void __attribute__((interrupt,no_auto_pvs)) _T3Interrupt(void)
{	
	_T3IF = 0;				// clear interrupt
	timeOutTMRVal ++;
	if(timeOutTMRVal == 65500)
		timeOutTMRVal = 0;
}
ADC Interrupt
/***********************ADC Interrupt Service Routine********************
   This runs through our Finite state machine which
   finds the zeroes/period and the peak values in the
   fallzero/risezero and the peak states respectively.
   We are not going through the states in the integer order
   or the order of the state diagram, this is to increase efficiency
   by reducing the number of if statements for the more demanding states
*************************************************************************/
void __attribute__((interrupt,no_auto_psv)) _ADCInterrupt(void)
{
	// debug code *******************!!!!!!!1
	//Test how long the ADC Interrupt is taking
	//PORTFbits.RF0 = 1;


	// i is our for-loop character, static for memory management purposes
	static char i;
	// currentTimerVal is the stored value of period detection, static for memory management purposes
	static unsigned int currentTimerVal;
	// Vs stores a sampled voltage
	static int Vs;
	// index stores the period found from TMR subtraction and is used to index the MIDI LUT
	static unsigned int index;
	// disable the ADC1 interrupt
	IEC0bits.ADIE = 0;
	// clear the ADC1 interrupt flag
	IFS0bits.ADIF = 0;	
	// for each guitar string, the 'i'th string, run a step in its finite state machine
	for(i = 0; i < 6; i++)
	{ 
		if(i<3)
			Vs = *(&ADCBUF0 + i + 1);	// the sampled value for the 'i'th string from the ADC buffer is assigned to Vs 
		else
			Vs = *(&ADCBUF0 + i + 2);	// the sampled value for the 'i'th string from the ADC buffer is assigned to Vs 
		// a switch statement dependent on the current state for the 'i'th string, this is best understood with the finite state machine documentation
		switch (currentState[i]) {
			case(rise1): 
				if(Vs > VBIAS[i])
				{
					// increment the number of zeroes detected
					zeroCount[i] ++;
					// if six or more zeroes have been detected
					if(zeroCount[i] > 5)
					{
						// save the value of TMR2 so all the comparisons are consistant, this code may be not needed
						currentTimerVal = TMR2;
						
						// ****** the period is defined as the number of zeroes counted divided by the time elapsed
						// if the timer has overflowed and reset to zero, then a special case is needed in which the max value for an int is added to the current timer value
						if(currentTimerVal < lastZero[i])
						{
							index = ((int)(((long)currentTimerVal + 65535) - lastZero[i]))>>1;
						}
						else
						{
							index = (currentTimerVal - lastZero[i])>>1;
						}
						// reset the zero count and indicate that a change is needed in the output
						zeroCount[i] = 0;
						if(index > 125 && index < 2160)
						{
							period[i] = LUT[index];
							if(firstAfterSync[i]==0 && period[i]!=lastPeriod[i])
							{
								changed[i] = 1;
							}
							else if(firstAfterSync[i] == 1)
							{
								changed[i] = 1;
								firstAfterSync[i] = 0;
							}
							else
							{
								period[i] = 0;
							}
						}
						else
						{
							period[i] = 0;
						}
						// save the currentTimerVal as lastZero for the next time a zero is detected
						lastZero[i] = TMR2;
					}
					// set the next state to rise2
					currentState[i] = rise2;
				}
			break;
			
			case(fall1):
				if(Vs<VBIAS[i])
				{
					// increment the number of zeroes detected
					zeroCount[i] ++;
					// if six or more zeroes have been detected
					if(zeroCount[i] > 5)
					{
						currentTimerVal = TMR2;
						
						// ****** the period is defined as the number of zeroes counted divided by the time elapsed
						// if the timer has overflowed and reset to zero, then a special case is needed in which the max value for an int is added to the current timer value
						if(currentTimerVal < lastZero[i])
						{
							index = ((int)(((long)currentTimerVal + 65535) - lastZero[i]))>>1;
						}
						else
						{
							index = (currentTimerVal - lastZero[i])>>1;
						}
						// reset the zero count and indicate that a change is needed in the output
						zeroCount[i] = 0;
						if(index > 125 && index < 2160)
						{
							period[i] = LUT[index];
							if(firstAfterSync[i]==0 && period[i]!=lastPeriod[i])
							{
								changed[i] = 1;
							}
							else if(firstAfterSync[i] == 1)
							{
								changed[i] = 1;
								firstAfterSync[i] = 0;
							}
							else
							{
								period[i] = 0;
							}
						}
						else
						{
							period[i] = 0;
						}
						// save the currentTimerVal as lastZero for the next time a zero is detected
						lastZero[i] = TMR2;
					}
					// set the next state to rise2
					currentState[i] = fall2;
				}
			break;
			
			case(peak): 
				if(Vs>VBPLUSVT[i])
				{
// if the sampled peak is greater than the highest stored peak, then replace the previous highest peak with the new one
// this is used to fine the maximum peak amplitude in a period
					if(Vs > peakValue[i])
						peakValue[i] = Vs;
				}
				else if(Vs<VBPLUSVT[i]) 
				{
					currentState[i] = fall1;	// next state is fall1
					// reset all the main variables and indicate a change has been made
velocity[i] = (peakValue[i] + amplitude[0][i] + amplitude[1][i])/3; // store the average highest peak value	
					amplitude[1][i] = amplitude[1][i];
					amplitude[0][i] = peakValue[i];
					peakValue[i] = 0;	// reset peak
				}	
			break;

			case(rise2): 
				if(Vs>VBPLUSVT[i])
					currentState[i] = peak;	// next state is peak
// a timeout condition occurs if the signal abruptly goes to zero or a note has became too quiet to detect
				else if(timeOutTMRVal >= timeOut[i])
				{
currentState[i] = sync;	// the next state is sync	
// reset all the main variables and indicate a change has been made
					period[i] = 0;
					amplitude[0][i] = VBIAS[i];
					amplitude[1][i] = VBIAS[i];
					velocity[i] = VBIAS[i];
					lastVelocity[i] = VBIAS[i];
					zeroCount[i] = 0;
					firstAfterSync[i] = 1;
					changed[i] = 1;
				}
			break;
			
			case(fall2):
				if(Vs<VBMINUSVT[i]) 
					currentState[i] = trough; // next state is trough
// a timeout condition occurs if the signal abruptly goes to zero or a note has became too quiet to detect
				else if(timeOutTMRVal >= timeOut[i])
				{
currentState[i] = sync;	// the next state is sync	
// reset all the main variables and indicate a change has been made
					period[i] = 0;
					amplitude[0][i] = VBIAS[i];
					amplitude[1][i] = VBIAS[i];
					velocity[i] = VBIAS[i];
					lastVelocity[i] = VBIAS[i];
					zeroCount[i] = 0;
					firstAfterSync[i] = 1;
					changed[i] = 1;
				}
			break;
			 
			case(trough):
				if(Vs>VBMINUSVT[i]) 
				{
currentState[i] = rise1;	// the next state is rise1
					if(timeOutTMRVal < 65496)
timeOut[i] = timeOutTMRVal + QUARTERSECTIMEOUTDELAY;	
					else
						timeOut[i] = 0;
				}
				else if(timeOutTMRVal >= timeOut[i])
				{
currentState[i] = sync;	// the next state is sync	
// reset all the main variables and indicate a change has been made
					period[i] = 0;
					amplitude[0][i] = VBIAS[i];
					amplitude[1][i] = VBIAS[i];
					velocity[i] = VBIAS[i];
					lastVelocity[i] = VBIAS[i];
					zeroCount[i] = 0;
					firstAfterSync[i] = 1;
					changed[i] = 1;
				}
			break;
			 
			case(sync):
// while in a sync, the outputs are zero and we wait for a trough condition to occur
				if(Vs<VBMINUSVT[i])
				{
currentState[i] = wait;	// the next state is trough
					if(timeOutTMRVal < 65496 - QUARTERSECTIMEOUTDELAY)
timeOut[i] = timeOutTMRVal + QUARTERSECTIMEOUTDELAY;	
					else
						timeOut[i] = 0;

					if(wait5msTMRVal < 65550 - MSPLUCKDELAY[i])
						wait5ms[i] = wait5msTMRVal + MSPLUCKDELAY[i];	
					else
						wait5ms[i] = 0;
				}
			break;

			case(wait):
				if(wait5msTMRVal >= wait5ms[i])
					currentState[i] = trough;
			break;
		}
	}
	// enable the ADC1 interrupt
	IEC0bits.ADIE = 1;
//	PORTFbits.RF0 = 0;
}
