/**
 * @origin	CC Dharmani, Chennai (India) - www.dharmanitech.com
 * @website http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html 
 * @author  Lukas Herudek
 * @email   lukas.herudek@gmail.com
 * @version v1.0
 * @ide     Atmel Studio 6.2
 * @license GNU GPL v3
 * @brief   SD card library for AVR
 * @verbatim
   ----------------------------------------------------------------------
    Copyright (C) Lukas Herudek, 2018
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU General Public License for more details.
	
	<http://www.gnu.org/licenses/>
@endverbatim
 */
 
#ifndef _SDcard_H_
#define _SDcard_H_

//**************************************************************
// ****** FUNCTIONS FOR SD RAW DATA TRANSFER *******
//**************************************************************
//use following macros if PB2 pin is used for Chip Select of SD
#define SD_CS_ASSERT     (PORTB&=~(1<<PINB2))
#define SD_CS_DEASSERT   (PORTB|=(1<<PINB2))


//SD commands, many of these are not used here
#define GO_IDLE_STATE            0
#define SEND_OP_COND             1
#define SEND_IF_COND			 8
#define SEND_CSD                 9
#define STOP_TRANSMISSION        12
#define SEND_STATUS              13
#define SET_BLOCK_LEN            16
#define READ_SINGLE_BLOCK        17
#define READ_MULTIPLE_BLOCKS     18
#define WRITE_SINGLE_BLOCK       24
#define WRITE_MULTIPLE_BLOCKS    25
#define ERASE_BLOCK_START_ADDR   32
#define ERASE_BLOCK_END_ADDR     33
#define ERASE_SELECTED_BLOCKS    38
#define SD_SEND_OP_COND			 41   //ACMD
#define APP_CMD					 55
#define READ_OCR				 58
#define CRC_ON_OFF               59

#define ON     1
#define OFF    0

volatile unsigned long startBlock, totalBlocks;
volatile unsigned char SDHC_flag, cardType, buffer[512];

unsigned char SPI_transmit(unsigned char data);
unsigned char SPI_receive(void);
unsigned char SD_init(void);
unsigned char SD_sendCommand(unsigned char cmd, unsigned long arg);
unsigned char SD_readSingleBlock(unsigned long startBlock);
unsigned char SD_writeSingleBlock(unsigned long startBlock);
unsigned char SD_erase (unsigned long startBlock, unsigned long totalBlocks);



//********************************************************
// **** ROUTINES FOR FAT32 IMPLEMATATION OF SD CARD *****
//********************************************************

//Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure
{
	unsigned char	nothing[446];		//ignore, placed here to fill the gap in the structure
	unsigned char	partitionData[64];	//partition records (16x4)
	unsigned int	signature;		//0xaa55
};

//Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure
{ 				
	unsigned char	status;				//0x80 - active partition
	unsigned char 	headStart;			//starting head
	unsigned int	cylSectStart;		//starting cylinder and sector
	unsigned char	type;				//partition type 
	unsigned char	headEnd;			//ending head of the partition
	unsigned int	cylSectEnd;			//ending cylinder and sector
	unsigned long	firstSector;		//total sectors between MBR & the first sector of the partition
	unsigned long	sectorsTotal;		//size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure
{
	unsigned char jumpBoot[3]; //default: 0x009000EB
	unsigned char OEMName[8];
	unsigned int bytesPerSector; //deafault: 512
	unsigned char sectorPerCluster;
	unsigned int reservedSectorCount;
	unsigned char numberofFATs;
	unsigned int rootEntryCount;
	unsigned int totalSectors_F16; //must be 0 for FAT32
	unsigned char mediaType;
	unsigned int FATsize_F16; //must be 0 for FAT32
	unsigned int sectorsPerTrack;
	unsigned int numberofHeads;
	unsigned long hiddenSectors;
	unsigned long totalSectors_F32;
	unsigned long FATsize_F32; //count of sectors occupied by one FAT
	unsigned int extFlags;
	unsigned int FSversion; //0x0000 (defines version 0.0)
	unsigned long rootCluster; //first cluster of root directory (=2)
	unsigned int FSinfo; //sector number of FSinfo structure (=1)
	unsigned int BackupBootSector;
	unsigned char reserved[12];
	unsigned char driveNumber;
	unsigned char reserved1;
	unsigned char bootSignature;
	unsigned long volumeID;
	unsigned char volumeLabel[11]; //"NO NAME "
	unsigned char fileSystemType[8]; //"FAT32"
	unsigned char bootData[420];
	unsigned int bootEndSignature; //0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
	unsigned long leadSignature; //0x41615252
	unsigned char reserved1[480];
	unsigned long structureSignature; //0x61417272
	unsigned long freeClusterCount; //initial: 0xffffffff
	unsigned long nextFreeCluster; //initial: 0xffffffff
	unsigned char reserved2[12];
	unsigned long trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure
{
	unsigned char name[11];
	unsigned char attrib; //file attributes
	unsigned char NTreserved; //always 0
	unsigned char timeTenth; //tenths of seconds, set to 0 here
	unsigned int createTime; //time file was created
	unsigned int createDate; //date file was created
	unsigned int lastAccessDate;
	unsigned int firstClusterHI; //higher word of the first cluster number
	unsigned int writeTime; //time of last write
	unsigned int writeDate; //date of last write
	unsigned int firstClusterLO; //lower word of the first cluster number
	unsigned long fileSize; //size of file in bytes
};

//Attribute definitions for file/directory
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0f


#define DIR_ENTRY_SIZE     0x32
#define EMPTY              0x00
#define DELETED            0xe5
#define GET     0
#define SET     1
#define READ	0
#define VERIFY  1
#define ADD		0
#define REMOVE	1
#define LOW		0
#define HIGH	1	
#define TOTAL_FREE   1
#define NEXT_FREE    2
#define GET_LIST     0
#define GET_FILE     1
#define DELETE		 2
#define END_OF_FILE		0x0fffffff


//************* external variables *************
volatile unsigned long firstDataSector, rootCluster, totalClusters;
volatile unsigned int  bytesPerSector, sectorPerCluster, reservedSectorCount;
unsigned long unusedSectors, appendFileSector, appendFileLocation, fileSize, appendStartCluster;

//global flag to keep track of free cluster count updating in FSinfo sector
unsigned char freeClusterCountUpdated;



//************* functions *************
unsigned char getBootSectorData (void);
unsigned long getFirstSector(unsigned long clusterNumber);
unsigned long getSetFreeCluster(unsigned char totOrNext, unsigned char get_set, unsigned long FSEntry);
struct dir_Structure* findFiles (unsigned char flag, unsigned char *fileName);
unsigned long getSetNextCluster (unsigned long clusterNumber,unsigned char get_set,unsigned long clusterEntry);
unsigned char readFile (unsigned char flag, unsigned char *fileName);
unsigned char convertFileName (unsigned char *fileName);
void writeFile (unsigned char *fileName);
void appendFile (void);
unsigned long searchNextFreeCluster (unsigned long startCluster);
void memoryStatistics (void);
void displayMemory (unsigned char flag, unsigned long memory);
void deleteFile (unsigned char *fileName);
void freeMemoryUpdate (unsigned char flag, unsigned long size);

#endif
