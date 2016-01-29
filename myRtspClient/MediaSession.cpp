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

#include "MediaSession.h"
#include "myRtpSession.h"
#include "nalu_types.h"

#define MEDIA_SESSION_OK 		1
#define MEDIA_SESSION_ERROR 	0

MediaSession::MediaSession():
	MediaType(""),
	Protocol(""),
	EncodeType(""),
	TimeRate(0),
	ControlURI(""),
	SessionID(""),
	// RTSPSockfd(-1),
	RTPPort(0),
	// RTPSockfd(-1),
	RTCPPort(0),
	// RTCPSockfd(-1),
	Packetization(PACKET_MODE_SINGAL_NAL),
	RTPInterface(NULL),
	NalUnitType(NULL)

{
}

MediaSession::~MediaSession()
{
	RTP_Teardown();

	if(RTPInterface) delete RTPInterface;
}

int MediaSession::RTP_SetUp()
{
	if(0 == TimeRate) return MEDIA_SESSION_ERROR;
	if(0 == RTPPort) return MEDIA_SESSION_ERROR;
	if(RTPInterface) {
		printf("RTP SetUp already\n");
		return MEDIA_SESSION_OK;
	}

	RTPInterface = new MyRTPSession;
	if(!RTPInterface->MyRTP_SetUp(this)) return MEDIA_SESSION_ERROR;

	return MEDIA_SESSION_OK;
}

int MediaSession::RTP_Teardown(struct timeval * tval)
{
	if(!RTPInterface) {
		return MEDIA_SESSION_OK;
	}
	if(0 == RTPPort) return MEDIA_SESSION_ERROR;

	RTPPort = 0;
	// RTPSockfd = -1;
	RTCPPort = 0;
	// RTCPSockfd = -1;

	RTPInterface->MyRTP_Teardown(this, tval);
	delete RTPInterface;
	RTPInterface = NULL;
	return MEDIA_SESSION_OK;
}

uint8_t * MediaSession::GetMediaData(uint8_t * buf, size_t * size, unsigned long timeout) {
	if(!RTPInterface) return NULL;
	return  RTPInterface->GetMyRTPData(buf, size, timeout);
}

uint8_t * MediaSession::GetMediaPacket(uint8_t * buf, size_t * size, unsigned long timeout) {
	if(!RTPInterface) return NULL;
	return  RTPInterface->GetMyRTPPacket(buf, size, timeout);
}
