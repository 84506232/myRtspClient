#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include <gtest/gtest.h>
#include <map>

#include "rtspClient.h"

using namespace std;

TEST(rtspClient, RtspClient_ParseSDP_InvalidInput)
{
    RtspClient Client;

	string Invalid_SDP1("");
	string Invalid_SDP2("halsdjg3woiu");
	multimap<string, string> sdp_info;

	Client.ParseSDP();
	sdp_info = Client.GetSDPInfo();
	EXPECT_EQ(sdp_info.empty(), true);

	Client.ParseSDP(Invalid_SDP1);
	sdp_info = Client.GetSDPInfo();
	EXPECT_EQ(sdp_info.empty(), true);

	Client.ParseSDP(Invalid_SDP2);
	sdp_info = Client.GetSDPInfo();
	EXPECT_EQ(sdp_info.empty(), true);
}

TEST(rtspClient, RtspClient_ParseSDP_RegularInput)
{
	string SDP("\
	RTSP/1.0 200 OK\r\n\
		Server: VLC/2.1.6\r\n\
		Date: Sun, 06 Dec 2015 11:51:38 GMT\r\n\
		Content-Type: application/sdp\r\n\
		Content-Base: rtsp://127.0.0.1:554/ansersion\r\n\
		Content-Length: 572\r\n\
		Cache-Control: no-cache\r\n\
		Cseq: 2\r\n\
\r\n\
		v=0\r\n\
		o=- 15712671843665285047 15712671843665285047 IN IP4 localhost.localdomain\r\n\
		s=Unnamed\r\n\
		i=N/A\r\n\
		c=IN IP4 0.0.0.0\r\n\
		t=0 0\r\n\
		a=tool:vlc 2.1.6\r\n\
		a=recvonly\r\n\
		a=type:broadcast\r\n\
		a=charset:UTF-8\r\n\
		a=control:rtsp://127.0.0.1:554/ansersion\r\n\
		m=audio 0 RTP/AVP 14\r\n\
		b=AS:128\r\n\
		b=RR:0\r\n\
		a=rtpmap:14 MPA/90000\r\n\
		a=control:rtsp://127.0.0.1:554/ansersion/trackID=0\r\n\
		m=video 0 RTP/AVP 96\r\n\
		b=RR:0\r\n\
		a=rtpmap:96 H264/90000\r\n\
		a=fmtp:96 packetization-mode=1;profile-level-id=64000c;sprop-parameter-sets=Z2QADKzZQUH7ARAAAGewAA/DqPFCmWA=,aOvjyyLA;\r\n\
		a=control:rtsp://127.0.0.1:554/ansersion/trackID=1\r\n\r\n");

	RtspClient Client;
	Client.ParseSDP(SDP);
	multimap<string, string> SDPInfo = Client.GetSDPInfo();
	typedef multimap<string, string>::iterator It_type;
	pair<It_type, It_type> p = SDPInfo.equal_range("s");
	It_type It = p.first;
	map<string, MediaSession> MediaSessionMap = Client.GetMediaSessions();

	EXPECT_EQ(true, It->second == "Unnamed");
	EXPECT_EQ(SDPInfo.count("a"), 10);
	EXPECT_EQ(SDPInfo.count("m"), 2);
	EXPECT_EQ(SDPInfo.count("v"), 1);
	EXPECT_EQ(SDPInfo.count("x"), 0);

	EXPECT_EQ(true, MediaSessionMap["audio"].MediaType == "audio");
	EXPECT_EQ(true, MediaSessionMap["audio"].EncodeType == "MPA");
	EXPECT_EQ(true, MediaSessionMap["audio"].Protocol == "RTP/AVP");
	EXPECT_EQ(true, MediaSessionMap["audio"].TimeRate == 90000);
	EXPECT_EQ(true, MediaSessionMap["audio"].ControlURI == "rtsp://127.0.0.1:554/ansersion/trackID=0");

	EXPECT_EQ(true, MediaSessionMap["video"].MediaType == "video");
	EXPECT_EQ(true, MediaSessionMap["video"].EncodeType == "H264");
	EXPECT_EQ(true, MediaSessionMap["video"].Protocol == "RTP/AVP");
	EXPECT_EQ(true, MediaSessionMap["video"].TimeRate == 90000);
	EXPECT_EQ(true, MediaSessionMap["video"].ControlURI == "rtsp://127.0.0.1:554/ansersion/trackID=1");
}

