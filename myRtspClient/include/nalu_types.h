//   Copyright 2015-2016 Ansersion
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

/*******************************/
/* More info refer to RFC 6184 */
/*******************************/

#ifndef NALU_TYPES_H
#define NALU_TYPES_H

#include <string>
#include <stdint.h>

#define NAL_UNIT_TYPE_NUM 		32
#define PACKETIZATION_MODE_NUM 	3

#define PACKET_MODE_SINGAL_NAL 			0
#define PACKET_MODE_NON_INTERLEAVED 	1
#define PACKET_MODE_INTERLEAVED 		2

#define IS_PACKET_MODE_VALID(P) 	\
	((P) >= PACKET_MODE_SINGAL_NAL && (P) <= PACKET_MODE_INTERLEAVED)

/* More info refer to H264 'nal_unit_type' */
#define IS_NALU_TYPE_VALID(N) 		\
	( \
      ((N) >= 1 && (N) <= 12) || \
      ((N) == STAP_A::STAP_A_ID) || \
      ((N) == STAP_B::STAP_B_ID) || \
      ((N) == MTAP_16::MTAP_16_ID) || \
      ((N) == MTAP_24::MTAP_24_ID) || \
      ((N) == FU_A::FU_A_ID) || \
      ((N) == FU_B::FU_B_ID) \
	)

class NALUTypeBase
{
	public:
		// NALU types map for h264 
		static NALUTypeBase * NalUnitType_H264[PACKETIZATION_MODE_NUM][NAL_UNIT_TYPE_NUM];
	public:
		virtual ~NALUTypeBase() {};
	public:
		virtual uint16_t ParseNALUHeader_F(const uint8_t * RTPPayload) = 0;
		virtual uint16_t ParseNALUHeader_Type(const uint8_t * RTPPayload) = 0;

		virtual uint16_t ParseNALUHeader_NRI(const uint8_t * RTPPayload) = 0; // only for h264
		virtual uint16_t ParseNALUHeader_Layer_ID(const uint8_t * RTPPayload) = 0; // only for h265
		virtual uint16_t ParseNALUHeader_Temp_ID_Plus_1(const uint8_t * RTPPayload) = 0; // only for h265
		
		virtual bool IsPacketStart(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketEnd(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketReserved(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketThisType(const uint8_t * rtp_payload) = 0;
		virtual size_t CopyData(uint8_t * buf, uint8_t * data, size_t size) = 0;
		virtual std::string GetName() const = 0;
		virtual bool GetEndFlag() = 0;
		virtual bool GetStartFlag() = 0;
	protected:
		std::string Name;
		bool EndFlag;
		bool StartFlag;
};

#endif
