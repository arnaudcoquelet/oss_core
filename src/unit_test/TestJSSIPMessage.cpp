


/*
Basic Assertions

These assertions do basic true/false condition testing.
Fatal assertion 	Nonfatal assertion 	Verifies
ASSERT_TRUE(condition); 	EXPECT_TRUE(condition); 	condition is true
ASSERT_FALSE(condition); 	EXPECT_FALSE(condition); 	condition is false

Remember, when they fail, ASSERT_* yields a fatal failure and returns from the current function, while EXPECT_* yields a nonfatal failure, allowing the function to continue running. In either case, an assertion failure means its containing test fails.

Availability: Linux, Windows, Mac.
Binary Comparison

This section describes assertions that compare two values.

Fatal assertion 	Nonfatal assertion 	Verifies
ASSERT_EQ(expected, actual);	EXPECT_EQ(expected, actual);	expected == actual
ASSERT_NE(val1, val2); 	EXPECT_NE(val1, val2); 	val1 != val2
ASSERT_LT(val1, val2); 	EXPECT_LT(val1, val2); 	val1 < val2
ASSERT_LE(val1, val2); 	EXPECT_LE(val1, val2); 	val1 <= val2
ASSERT_GT(val1, val2); 	EXPECT_GT(val1, val2); 	val1 > val2
ASSERT_GE(val1, val2); 	EXPECT_GE(val1, val2); 	val1 >= val2

In the event of a failure, Google Test prints both val1 and val2 . In ASSERT_EQ* and EXPECT_EQ* (and all other equality assertions we'll introduce later), you should put the expression you want to test in the position of actual, and put its expected value in expected, as Google Test's failure messages are optimized for this convention.

Value arguments must be comparable by the assertion's comparison operator or you'll get a compiler error. Values must also support the << operator for streaming to an ostream. All built-in types support this.

These assertions can work with a user-defined type, but only if you define the corresponding comparison operator (e.g. ==, <, etc). If the corresponding operator is defined, prefer using the ASSERT_*() macros because they will print out not only the result of the comparison, but the two operands as well.

Arguments are always evaluated exactly once. Therefore, it's OK for the arguments to have side effects. However, as with any ordinary C/C++ function, the arguments' evaluation order is undefined (i.e. the compiler is free to choose any order) and your code should not depend on any particular argument evaluation order.

ASSERT_EQ() does pointer equality on pointers. If used on two C strings, it tests if they are in the same memory location, not if they have the same value. Therefore, if you want to compare C strings (e.g. const char*) by value, use ASSERT_STREQ() , which will be described later on. In particular, to assert that a C string is NULL, use ASSERT_STREQ(NULL, c_string) . However, to compare two string objects, you should use ASSERT_EQ.

Macros in this section work with both narrow and wide string objects (string and wstring).

Availability: Linux, Windows, Mac.
String Comparison

The assertions in this group compare two C strings. If you want to compare two string objects, use EXPECT_EQ, EXPECT_NE, and etc instead.

Fatal assertion 	Nonfatal assertion 	Verifies
ASSERT_STREQ(expected_str, actual_str); 	EXPECT_STREQ(expected_str, actual_str); 	the two C strings have the same content
ASSERT_STRNE(str1, str2); 	EXPECT_STRNE(str1, str2); 	the two C strings have different content
ASSERT_STRCASEEQ(expected_str, actual_str);	EXPECT_STRCASEEQ(expected_str, actual_str); 	the two C strings have the same content, ignoring case
ASSERT_STRCASENE(str1, str2);	EXPECT_STRCASENE(str1, str2); 	the two C strings have different content, ignoring case

Note that "CASE" in an assertion name means that case is ignored.

*STREQ* and *STRNE* also accept wide C strings (wchar_t*). If a comparison of two wide strings fails, their values will be printed as UTF-8 narrow strings.

A NULL pointer and an empty string are considered different.

Availability: Linux, Windows, Mac.

See also: For more string comparison tricks (substring, prefix, suffix, and regular expression matching, for example), see the [AdvancedGuide Advanced Google Test Guide].
*/

#include "gtest/gtest.h"
#include "OSS/JS/JSSIPMessage.h"


TEST(JSTest, test_sip_message)
{
  using OSS::SIP::CRLF;
  
  std::ostringstream msg;
  msg << "INVITE sip:9001@192.168.0.152 SIP/2.0" << CRLF;
  msg << "To: <sip:9001@192.168.0.152>" << CRLF;
  msg << "From: 9011<sip:9011@192.168.0.103>;tag=6657e067" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.1;branch=001;rport, SIP/2.0/UDP 192.168.0.2;branch=002, SIP/2.0/UDP 192.168.0.3;branch=003" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.4;branch=004" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.5;branch=005" << CRLF;
  msg << "Call-ID: 885e5e180c04c509" << CRLF;
  msg << "CSeq: 1 INVITE" << CRLF;
  msg << "Contact: <sip:9011@192.168.0.152:9644>" << CRLF;
  msg << "Max-Forwards: 70" << CRLF;
  msg << "Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO" << CRLF;
  msg << "Content-Type: application/sdp" << CRLF;
  msg << "Route: <sip:10.0.0.1;lr>" << CRLF;
  msg << "Route: <sip:10.0.0.2;lr>" << CRLF;
  msg << "Route: <sip:10.0.0.3;lr>" << CRLF;
  msg << "Content-Length: 237" << CRLF;
  msg << CRLF; /// End of headers
  msg << "v=0" << CRLF;
  msg << "o=- 10818229 10818359 IN IP4 192.168.0.152" << CRLF;
  msg << "s=-" << CRLF;
  msg << "c=IN IP4 192.168.0.152" << CRLF;
  msg << "t=0 0" << CRLF;
  msg << "m=audio 35000 RTP/AVP 0 8 101" << CRLF;
  msg << "a=fmtp:101 0-15" << CRLF;
  msg << "a=rtpmap:101 telephone-event/8000" << CRLF;
  msg << "a=sendrecv" << CRLF;
  
  OSS::SIP::SIPMessage::Ptr pMsg(new OSS::SIP::SIPMessage(msg.str()));
  OSS::JS::JSSIPMessage jsMessage("test context");
  ASSERT_TRUE(jsMessage.initialize("data/js_test/jssipmessage.js", "handle_request"));
  ASSERT_TRUE(jsMessage.processRequest(pMsg));
}

TEST(JSTest, test_sip_message_basic_auth)
{
  using OSS::SIP::CRLF;

  std::ostringstream msg;
  msg << "INVITE sip:9001@192.168.0.152 SIP/2.0" << CRLF;
  msg << "To: <sip:9001@192.168.0.152>" << CRLF;
  msg << "From: 9011<sip:9011@192.168.0.103>;tag=6657e067" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.1;branch=001;rport, SIP/2.0/UDP 192.168.0.2;branch=002, SIP/2.0/UDP 192.168.0.3;branch=003" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.4;branch=004" << CRLF;
  msg << "Via: SIP/2.0/UDP 192.168.0.5;branch=005" << CRLF;
  msg << "Call-ID: 885e5e180c04c509" << CRLF;
  msg << "CSeq: 1 INVITE" << CRLF;
  msg << "Contact: <sip:9011@192.168.0.152:9644>" << CRLF;
  msg << "Max-Forwards: 70" << CRLF;
  msg << "Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO" << CRLF;
  msg << "Content-Type: application/sdp" << CRLF;
  msg << "Route: <sip:10.0.0.1;lr>" << CRLF;
  msg << "Route: <sip:10.0.0.2;lr>" << CRLF;
  msg << "Route: <sip:10.0.0.3;lr>" << CRLF;
  msg << "Content-Length: 237" << CRLF;
  msg << CRLF; /// End of headers
  msg << "v=0" << CRLF;
  msg << "o=- 10818229 10818359 IN IP4 192.168.0.152" << CRLF;
  msg << "s=-" << CRLF;
  msg << "c=IN IP4 192.168.0.152" << CRLF;
  msg << "t=0 0" << CRLF;
  msg << "m=audio 35000 RTP/AVP 0 8 101" << CRLF;
  msg << "a=fmtp:101 0-15" << CRLF;
  msg << "a=rtpmap:101 telephone-event/8000" << CRLF;
  msg << "a=sendrecv" << CRLF;

  OSS::SIP::SIPMessage::Ptr pMsg(new OSS::SIP::SIPMessage(msg.str()));
  pMsg->setProperty("packet-source-address", "192.168.0.1");
  OSS::JS::JSSIPMessage jsMessage("test context");
  ASSERT_TRUE(jsMessage.initialize("data/js_test/jssipmessageauth.js", "handle_request"));

  for (int i = 0; i < 10; i++)
  {
    ASSERT_TRUE(jsMessage.processRequest(pMsg));
    std::string authAction;
    ASSERT_TRUE(pMsg->getProperty("auth-action", authAction));
    ASSERT_STREQ(authAction.c_str(), "accept");
  }
}



