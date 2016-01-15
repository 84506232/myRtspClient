#include "nalu_types.h"
#include <string.h>

STAP_A 	STAP_AObj;
STAP_B 	STAP_BObj;
MTAP_16 MTAP_16Obj;
MTAP_24 MTAP_24Obj;
FU_A 	FU_AObj;
FU_B 	FU_BObj;
NALUTypeBase NaluBaseTypeObj;

NALUTypeBase * NALUTypeBase::NalUnitType[PACKETIZATION_MODE_NUM][NAL_UNIT_TYPE_NUM] =
{
	/* Packetization Mode: Single NAL */ 
	{
		NULL,                 &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL
	},

	/* Packetization Mode: Non-interleaved */ 
	{
		NULL,                 &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     &NaluBaseTypeObj,            &NaluBaseTypeObj,            &NaluBaseTypeObj, 
		&NaluBaseTypeObj,     NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		&STAP_AObj,           NULL,                        NULL,                        NULL, 
		&FU_AObj,             NULL,                        NULL,                        NULL
	},

	/* Packetization Mode: Interleaved */ 
	{
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 NULL,                        NULL,                        NULL, 
		NULL,                 &STAP_BObj,                  &MTAP_16Obj,                 &MTAP_24Obj, 
		&FU_AObj,             &FU_BObj,                    NULL,                        NULL
	}
};

uint8_t NALUTypeBase::ParseNALUHeader_F(const uint8_t * rtp_payload) 
{
	if(!rtp_payload) return 0;
	uint8_t NALUHeader_F_Mask = 0x80; // binary: 1000_0000
	return (rtp_payload[0] & NALUHeader_F_Mask);
}

uint8_t NALUTypeBase::ParseNALUHeader_NRI(const uint8_t * rtp_payload) 
{
	if(!rtp_payload) return 0;
	uint8_t NALUHeader_NRI_Mask = 0x60; // binary: 0110_0000
	return (rtp_payload[0] & NALUHeader_NRI_Mask);
}

uint8_t NALUTypeBase::ParseNALUHeader_Type(const uint8_t * rtp_payload) 
{
	if(!rtp_payload) return 0;
	uint8_t NALUHeader_Type_Mask = 0x1F; // binary: 0001_1111
	return (rtp_payload[0] & NALUHeader_Type_Mask);
}

bool NALUTypeBase::IsPacketThisType(const uint8_t * rtp_payload) 
{
	// NAL type is valid in the range of [1,12]
	uint8_t NalType = ParseNALUHeader_Type(rtp_payload);
	return ((1 <= NalType) && (NalType <= 12));
}

size_t NALUTypeBase::CopyData(uint8_t * buf, uint8_t * data, size_t size) 
{
	size_t CopySize = 0;
	if(!buf || !data) return 0;

	EndFlag = IsPacketEnd(NULL);

	// NALU start code: 0x00000001 
	buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 1;
	CopySize += 4; 

	memcpy(buf + CopySize, data, size);
	CopySize += size;

	return CopySize;
}

const uint8_t STAP_A::STAP_A_ID = 0x18; // decimal: 24
const uint8_t STAP_B::STAP_B_ID = 0x19; // decimal: 25
const uint8_t MTAP_16::MTAP_16_ID = 0x1A; // decimal: 26
const uint8_t MTAP_24::MTAP_24_ID = 0x1B; // decimal: 27
const uint8_t FU_A::FU_A_ID = 0x1C; // decimal: 28
const uint8_t FU_B::FU_B_ID = 0x1D; // decimal: 29

bool STAP_A::IsPacketStart(const uint8_t * rtp_payload) 
{
	return true;
}

bool STAP_A::IsPacketEnd(const uint8_t * rtp_payload)
{
	return true;
}

bool STAP_A::IsPacketThisType(const uint8_t * rtp_payload)
{
	if(!rtp_payload) return false;
	return (STAP_A_ID == (rtp_payload[0] & STAP_A_ID));
}

size_t STAP_A::CopyData(uint8_t * buf, uint8_t * data, size_t size)
{
	size_t CopySize = 0;
	uint16_t NALU_Size = 0;
	uint8_t * DataPointer = data;

	if(!buf || !data) return 0;

	StartFlag = IsPacketStart(data);
	EndFlag = IsPacketEnd(data);

	uint8_t STAP_A_Header = 0;
	const int STAP_A_HeaderSize = sizeof(STAP_A_Header);
	DataPointer += STAP_A_HeaderSize;

	do {
		// Not enough data left or only padding data left;
		if(DataPointer - data >= size - sizeof(NALU_Size)) break;

		// Transform network byte order to host order
		NALU_Size = (uint16_t)(*DataPointer); DataPointer++;
		NALU_Size = (NALU_Size << 8) + (uint16_t)(*DataPointer); DataPointer++;
		if(0 == NALU_Size) break;

		// NALU start code: 0x00000001 
		buf[CopySize + 0] = 0; buf[CopySize + 1] = 0; buf[CopySize + 2] = 0; buf[CopySize + 3] = 1;
		CopySize += 4; 
		memcpy(buf + CopySize, DataPointer, NALU_Size);
		CopySize += NALU_Size;
		DataPointer += NALU_Size;
	} while(0 != NALU_Size);

	return CopySize;
}

bool FU_A::IsPacketThisType(const uint8_t * rtp_payload)
{
	if(!rtp_payload) return false;
	return (FU_A_ID == (rtp_payload[0] & FU_A_ID));
}

uint8_t FU_A::ParseNALUHeader_F(const uint8_t * rtp_payload)
{
	if(!rtp_payload) return FU_A_ERR;
	if(FU_A_ID != (rtp_payload[0] & FU_A_ID)) return FU_A_ERR;

	uint8_t NALUHeader_F_Mask = 0x80; // binary: 1000_0000

	// "F" at the byte of rtp_payload[0]
	return (rtp_payload[0] & NALUHeader_F_Mask);
}

uint8_t FU_A::ParseNALUHeader_NRI(const uint8_t * rtp_payload)
{
	if(!rtp_payload) return FU_A_ERR;
	if(FU_A_ID != (rtp_payload[0] & FU_A_ID)) return FU_A_ERR;

	uint8_t NALUHeader_NRI_Mask = 0x60; // binary: 0110_0000

	// "NRI" at the byte of rtp_payload[0]
	return (rtp_payload[0] & NALUHeader_NRI_Mask);

}

uint8_t FU_A::ParseNALUHeader_Type(const uint8_t * rtp_payload)
{
	if(!rtp_payload) return FU_A_ERR;
	if(FU_A_ID != (rtp_payload[0] & FU_A_ID)) return FU_A_ERR;

	uint8_t NALUHeader_Type_Mask = 0x1F; // binary: 0001_1111

	// "Type" at the byte of rtp_payload[0]
	return (rtp_payload[1] & NALUHeader_Type_Mask);
}

bool FU_A::IsPacketStart(const uint8_t * rtp_payload)
{
	if(!IsPacketThisType(rtp_payload)) return false;

	uint8_t PacketS_Mask = 0x80; // binary:1000_0000

	return (rtp_payload[1] & PacketS_Mask);
}

bool FU_A::IsPacketEnd(const uint8_t * rtp_payload)
{
	if(!IsPacketThisType(rtp_payload)) return false;

	uint8_t PacketE_Mask = 0x40; // binary:0100_0000

	return (rtp_payload[1] & PacketE_Mask);
}

bool FU_A::IsPacketReserved(const uint8_t * rtp_payload)
{
	if(!IsPacketThisType(rtp_payload)) return false;

	uint8_t PacketR_Mask = 0x20; // binary:0010_0000

	return (rtp_payload[1] & PacketR_Mask);
}

size_t FU_A::CopyData(uint8_t * buf, uint8_t * data, size_t size) 
{
	size_t CopySize = 0;
	if(!buf || !data) return 0;

	StartFlag = IsPacketStart(data);
	EndFlag = IsPacketEnd(data);

	uint8_t NALUHeader = 0;
	NALUHeader = (  
			ParseNALUHeader_F(data)      | 
			ParseNALUHeader_NRI(data)    | 
			ParseNALUHeader_Type(data)
			);

	if(StartFlag) {

		// NALU start code size
		buf[0] = 0; buf[1] = 0; buf[2] = 0; buf[3] = 1;
		CopySize += 4; 
		memcpy(buf + CopySize, &NALUHeader, sizeof(NALUHeader));
		CopySize += sizeof(NALUHeader);
	}
	const int FU_A_HeaderSize = 2;
	memcpy(buf + CopySize, data + FU_A_HeaderSize, size - FU_A_HeaderSize);
	CopySize += size - FU_A_HeaderSize;

	return CopySize;
}

