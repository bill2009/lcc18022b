//from tinyFat for arduino adapted for lcc1802/olduino by Bill Rowe Nov 2013
#include "mmc.h"
#include "tinyfat.h"
#include <fastspi.h>
#include <olduino.h>
#include <nstdlib.h>
#define putc(x) out(7,x)
#define spiRec() xferspif(0xff)
#define spiSend(b) xferspif(b)
static void spiSendLong(uint32_t data)
{
	union
	{
		unsigned long l;
		unsigned char c[4];
	} long2char;

	long2char.l = data;

	spiSend(long2char.c[0]);  //i have reversed these as a test!
	spiSend(long2char.c[1]);
	spiSend(long2char.c[2]);
	spiSend(long2char.c[3]);
}

uint8_t waitNotBusy(uint16_t timeoutMillis)
{
	uint16_t t0 = 255; //fixed count of tries for timeout
	do
	{
		if (spiRec() == 0XFF)	return true;
	}
	while (t0--);
	return false;
}

uint8_t mmc_cardCommand(uint8_t cmd, uint32_t arg)
{
	uint8_t status_,i;
	uint8_t crc = 0xFF;

	digitalWrite(SD_SS,LOW);
	(void)waitNotBusy(300);
	spiSend(cmd | 0x40);
	spiSendLong(arg);

	if (cmd == GO_IDLE_STATE)	crc = 0x95;  // correct crc for CMD0 with arg 0
	if (cmd == SEND_IF_COND)	crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
	spiSend(crc);

	for (i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++);
	return status_;
}

uint8_t mmc_cardAcmd(uint8_t cmd, uint32_t arg)
{
	(void)mmc_cardCommand(APP_CMD, 0);
	return mmc_cardCommand(cmd, arg);
}

uint8_t mmc_initialize(uint8_t speed)
{
	uint16_t t0 =255; //explicit cycle count
	uint32_t arg;
	uint8_t status_,i;
	digitalWrite(SD_SS, HIGH); // set sd card's slave select high
	for (i = 0; i < 10; i++) spiSend(0XFF);	//send clocks to put it in spi mode

	digitalWrite(SD_SS, LOW); // set sd card's slave select low
	while ((status_ = mmc_cardCommand(GO_IDLE_STATE, 0)) != STATUS_IN_IDLE)
	{
		if (!(t0--))
		{
			mmc__errorCode = SD_CARD_ERROR_CMD0;
			goto fail;
		}
	}

	// check SD version
	if ((mmc_cardCommand(SEND_IF_COND, 0x1AA) & STATUS_ILLEGAL_COMMAND))
	{
		printf("just as you suspected!\n");
		mmc__card_type = SD_CARD_TYPE_SD1;
	}
	else
	{
		// only need last byte of r7 response
		for (i = 0; i < 4; i++)	status_ = spiRec();
		if (status_ != 0XAA)
		{
			mmc__errorCode = SD_CARD_ERROR_CMD8;
			goto fail;
		}
		mmc__card_type = SD_CARD_TYPE_SD2;
	}

	// initialize card and send host supports SDHC if SD2
//	arg = _card_type == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;
	arg=0;
	t0=255;	//restart timeout
	while ((status_ = mmc_cardAcmd(SD_SEND_OP_COND, arg)) != STATUS_READY)
	{
		// check for timeout
		if (!(t0--))
		{
			mmc__errorCode = SD_CARD_ERROR_ACMD41;
			goto fail;
		}
	}
	// if SD2 read OCR register to check for SDHC card
	if (mmc__card_type == SD_CARD_TYPE_SD2)
	{
		if (mmc_cardCommand(READ_OCR, 0))
		{
			mmc__errorCode = SD_CARD_ERROR_CMD58;
			goto fail;
		}
		if ((spiRec() & 0XC0) == 0XC0)	mmc__card_type = SD_CARD_TYPE_SDHC;
		// discard rest of ocr - contains allowed voltage range
		for (i = 0; i < 3; i++) (void)spiRec();
	}
	digitalWrite(SD_SS, HIGH); // set sd card's slave select high

	return 0;

fail:
	digitalWrite(SD_SS, HIGH); // set sd card's slave select high
	return mmc__errorCode;
}


void main(){
	uint8_t rc=0;
	printf("sd/mmc tester here\n");
	rc=mmc_initialize(0);
	printf("mmc_initialize sez %cx\n",rc);
}
#include <olduino.c>
#include <nstdlib.c>
#include "fastspi.c"
