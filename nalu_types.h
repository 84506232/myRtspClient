//   Copyright 2015 Ansersion
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

#ifndef NALU_TYPES_H
#define NALU_TYPES_H

#include <string>
#include <stdint.h>

class NALUTypeBase
{
	public:
		NALUTypeBase() {Name.assign("");};
		virtual ~NALUTypeBase() {};
	public:
		virtual uint8_t ParseNALUHeader_F(const uint8_t * RTPPayload) = 0;
		virtual uint8_t ParseNALUHeader_NRI(const uint8_t * RTPPayload) = 0;
		virtual uint8_t ParseNALUHeader_Type(const uint8_t * RTPPayload) = 0;
		virtual bool IsPacketStart(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketEnd(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketReserved(const uint8_t * rtp_payload) = 0;
		virtual bool IsPacketThisType(const uint8_t * rtp_payload) = 0;
	protected:
		std::string Name;
};

class STAP_A : public NALUTypeBase
{
	public:
		STAP_A() {};
		virtual ~STAP_A() {};
};

class STAP_B : public NALUTypeBase
{
	public:
		STAP_B() {};
		virtual ~STAP_B() {};
};

class MTAP_16 : public NALUTypeBase
{
	public:
		MTAP_16() {};
		virtual ~MTAP_16() {};
};

class MTAP_24 : public NALUTypeBase
{
	public:
		MTAP_24() {};
		virtual ~MTAP_24() {};
};

#define FU_A_ERR 	0xFF

class FU_A : public NALUTypeBase
{
	public:
		FU_A();
		virtual ~FU_A() {};

	public:
		/* Function: "ParseNALUHeader_*":
		 * 	Return 'FU_A_ERR'(0xFF) if error occurred */
		uint8_t ParseNALUHeader_F(const uint8_t * rtp_payload);
		uint8_t ParseNALUHeader_NRI(const uint8_t * rtp_payload);
		uint8_t ParseNALUHeader_Type(const uint8_t * rtp_payload);
	public:
		static const uint8_t FU_A_ID;

	public:
		/* if FU_A payload type */
		bool IsPacketThisType(const uint8_t * rtp_payload);

		/* Packet Start Flag */
		bool IsPacketStart(const uint8_t * rtp_payload);

		/* Packet End Flag */
		bool IsPacketEnd(const uint8_t * rtp_payload);

		/* Reserved */
		bool IsPacketReserved(const uint8_t * rtp_payload);

};

class FU_B : public NALUTypeBase
{
	public:
		FU_B();
		virtual ~FU_B() {};
};

#endif
