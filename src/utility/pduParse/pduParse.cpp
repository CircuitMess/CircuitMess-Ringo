/* 
Copyright (C) 2010- Alexander Chudnovets <effractor@gmail.com>

Based on SMS Server Tools 3
Copyright (C) 2006- Keijo Kasvi
http://smstools3.kekekasvi.com/

This program is free software unless you got it under another license directly
from the author. You can redistribute it and/or modify it under the terms of
the GNU General Public License as published by the Free Software Foundation.
Either version 2 of the License, or (at your option) any later version.
*/
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <iconv.h>
#include "pduParse.h"
iconv_t iconv_open(const char*, const char*);
size_t iconv (iconv_t, char **, size_t *, char **, size_t *);
int iconv_close (iconv_t);
// Global constants. FIXME: set correct values
const int max_number = 64;
const int max_number_type = 1024;
const int max_smsc = 64;
const int max_date = 32;
const int max_time = 32;
const int max_udh_data = 512;
const int max_udh_type = 512;
const int max_err = 1024;
const int maxsms_binary = 140;
const int max_message = maxsms_binary * 4;
const int validity_period = 255;
const int max_pdu = 160;

// Utility functions
// Converts an octet to a 8-Bit value
int octet2bin(const char* octet) 
{
    int result = 0;
    if (octet[0] > 57)
        result += octet[0] - 55;
    else
        result += octet[0] - 48;
    result = result << 4;
    if (octet[1] > 57)
        result += octet[1] - 55;
    else
        result += octet[1] - 48;
    return result;
}

// Converts an octet to a 8bit value, returns < in case of error.
int octet2bin_check(const char *octet)
{
    if (octet[0] == 0)
        return -1;
    if (octet[1] == 0)
        return -2;
    if (!isxdigit(octet[0]))
        return -3;
    if (!isxdigit(octet[1]))
        return -4;
    return octet2bin(octet);
}

// Swap every second character
void swapchars(char* string) 
{
    char c;
    int length = strlen(string);
    for (int i = 0; i < length - 1; i += 2)
    {
        c = string[i];
        string[i] = string[i + 1];
        string[i + 1] = c;
    }
}

// Returns a length of udh (including UDHL), -1 if error.
// pdu is 0-terminated ascii(hex) pdu string with
// or without spaces.
int explain_udh(char *udh_type, const char *pdu)
{
    int udh_length;
    int idx;
    char *pdu_ptr;
    char *p;
    const char *p1 = NULL;
    int i;
    char tmp[512];
    char buffer[1024];

    *udh_type = 0;
    if (strlen(pdu) >= sizeof(buffer))
        return -1;
    strcpy(buffer, pdu);
    while ((p = strchr(buffer, ' ')))
    	memmove(p, p + 1, strlen(p + 1) + 1);
        //strcpyo(p, p +1);
    
    if ((udh_length = octet2bin_check(buffer)) < 0)
        return -1;
    udh_length++;
    if ((size_t)(udh_length *2) > strlen(buffer))
        return -1;
    sprintf(udh_type, "Length=%i", udh_length);
    idx = 1;
    while (idx < udh_length)
    {
        pdu_ptr = buffer + idx * 2;
        p = NULL;
        i = octet2bin_check(pdu_ptr);
        switch (i)
        {
        case -1: return -1;

        // 3GPP TS 23.040 version 6.8.1 Release 6 - ETSI TS 123 040 V6.8.1 (2006-10)
        case 0x00: p1 = "Concatenated short messages, 8-bit reference number"; break;
        case 0x01: p1 = "Special SMS Message Indication"; break;
        case 0x02: p1 = "Reserved"; break;
        //case 0x03: p = "Value not used to avoid misinterpretation as <LF> character"; break;
        case 0x04: p1 = "Application port addressing scheme, 8 bit address"; break;
        case 0x05: p1 = "Application port addressing scheme, 16 bit address"; break;
        case 0x06: p1 = "SMSC Control Parameters"; break;
        case 0x07: p1 = "UDH Source Indicator"; break;
        case 0x08: p1 = "Concatenated short message, 16-bit reference number"; break;
        case 0x09: p1 = "Wireless Control Message Protocol"; break;
        case 0x0A: p1 = "Text Formatting"; break;
        case 0x0B: p1 = "Predefined Sound"; break;
        case 0x0C: p1 = "User Defined Sound (iMelody max 128 bytes)"; break;
        case 0x0D: p1 = "Predefined Animation"; break;
        case 0x0E: p1 = "Large Animation (16*16 times 4 = 32*4 =128 bytes)"; break;
        case 0x0F: p1 = "Small Animation (8*8 times 4 = 8*4 =32 bytes)"; break;
        case 0x10: p1 = "Large Picture (32*32 = 128 bytes)"; break;
        case 0x11: p1 = "Small Picture (16*16 = 32 bytes)"; break;
        case 0x12: p1 = "Variable Picture"; break;
        case 0x13: p1 = "User prompt indicator"; break;
        case 0x14: p1 = "Extended Object"; break;
        case 0x15: p1 = "Reused Extended Object"; break;
        case 0x16: p1 = "Compression Control"; break;
        case 0x17: p1 = "Object Distribution Indicator"; break;
        case 0x18: p1 = "Standard WVG object"; break;
        case 0x19: p1 = "Character Size WVG object"; break;
        case 0x1A: p1 = "Extended Object Data Request Command"; break;
        case 0x20: p1 = "RFC 822 E-Mail Header"; break;
        case 0x21: p1 = "Hyperlink format element"; break;
        case 0x22: p1 = "Reply Address Element"; break;
        case 0x23: p1 = "Enhanced Voice Mail Information"; break;
        }

        if (!p1)
        {
            if (i >= 0x1B && i <= 0x1F)
                p1 = "Reserved for future EMS features";
            else if (i >= 0x24 && i <= 0x6F)
                p1 = "Reserved for future use";
            else if (i >= 0x70 && i <= 0x7F)
                p1 = "(U)SIM Toolkit Security Headers";
            else if (i >= 0x80 && i <= 0x9F)
                p1 = "SME to SME specific use";
            else if (i >= 0xA0 && i <= 0xBF)
                p1 = "Reserved for future use";
            else if (i >= 0xC0 && i <= 0xDF)
                p1 = "SC specific use";
            else if (i >= 0xE0 && i <= 0xFF)
                p1 = "Reserved for future use";
        }
    
        if (!p1)
            p1 = "unknown";
        sprintf(tmp, ", [%.2s]%s", pdu_ptr, p1);
        if (strlen(udh_type) + strlen(tmp) >= (size_t)max_udh_type)
            return -1;
        sprintf(strchr(udh_type, 0), "%s", tmp);

        // Next octet is length of data:
        if ((i = octet2bin_check(pdu_ptr + 2)) < 0)
            return -1;
        if ((size_t)(i *2) > strlen(pdu_ptr + 4))
            return -1;
        idx += i + 2;
        if (idx > udh_length)
            return -1; // Incorrect UDL or length of Information Element.
    }

    return udh_length;
}

void explain_status(char *dest, size_t size_dest, int status)
{
	const char *p = "unknown";
	switch (status)
	{
		case 0: p = "Ok, short message received by the SME"; break;
		case 1: p = "Ok, short message forwarded by the SC to the SME but the SC is unable to confirm delivery"; break;
		case 2: p = "Ok, short message replaced by the SC"; break;

		// Temporary error, SC still trying to transfer SM
		case 32: p = "Still trying, congestion"; break;
		case 33: p = "Still trying, SME busy"; break;
		case 34: p = "Still trying, no response sendr SME"; break;
		case 35: p = "Still trying, service rejected"; break;
		case 36: p = "Still trying, quality of service not available"; break;
		case 37: p = "Still trying, error in SME"; break;
		// 38...47: Reserved
		// 48...63: Values specific to each SC

		// Permanent error, SC is not making any more transfer attempts
		case 64: p = "Error, remote procedure error"; break;
		case 65: p = "Error, incompatible destination"; break;
		case 66: p = "Error, connection rejected by SME"; break;
		case 67: p = "Error, not obtainable"; break;
		case 68: p = "Error, quality of service not available"; break;
		case 69: p = "Error, no interworking available"; break;
		case 70: p = "Error, SM validity period expired"; break;
		case 71: p = "Error, SM deleted by originating SME"; break;
		case 72: p = "Error, SM deleted by SC administration"; break;
		case 73: p = "Error, SM does not exist"; break;
		// 74...79: Reserved
		// 80...95: Values specific to each SC

		// Permanent error, SC is not making any more transfer attempts
		case 96: p = "Error, congestion"; break;
		case 97: p = "Error, SME busy"; break;
		case 98: p = "Error, no response sendr SME"; break;
		case 99: p = "Error, service rejected"; break;
		case 100: p = "Error, quality of service not available"; break;
		case 101: p = "Error, error in SME"; break;
		// 102...105: Reserved
		// 106...111: Reserved
		// 112...127: Values specific to each SC
		// 128...255: Reserved

		default:
			if (status >= 48 && status <= 63)
				p = "Temporary error, SC specific, unknown";
			else if ((status >= 80 && status <= 95) || (status >= 112 && status <= 127))
				p = "Permanent error, SC specific, unknown";
		}
		snprintf(dest, size_dest, "%s", p);
}

int pdu2text(const char *pdu, char *text, int *text_length, int *expected_length,
             int with_udh, char *udh, char *udh_type, int *errorpos) 
{
    int bitposition;
    int byteposition;
    int byteoffset;
    int charcounter;
    int bitcounter;
    int septets;
    int octets;
    int udhsize;
    int octetcounter;
    int skip_characters = 0;
    char c;
    char binary = 0;
    int i;
    int result;

    if (udh) 
        *udh = 0;
    if (udh_type)
        *udh_type = 0;

    if ((septets = octet2bin_check(pdu)) < 0)
    {
        if (errorpos)
            *errorpos = -1 * septets -3;
        return (septets >= -2)? -2: -1;
    }

    if (with_udh)
    {
        // copy the data header to udh and convert to hex dump
        // There was at least one octet and next will give an error if there is no more data:
        if ((udhsize = octet2bin_check(pdu +2)) < 0)
        {
            if (errorpos)
                *errorpos = -1 * udhsize -3 +2;
            return (udhsize >= -2)? -2: -1;
        }

        i = 0;
        result = -1;
        for (octetcounter = 0; octetcounter < udhsize + 1; octetcounter++)
        {
            if (octetcounter *3 +3 >= max_udh_data)
            {
                i = octetcounter *2 +2;
                result = -2;
                break;
            }
            udh[octetcounter*3]=pdu[(octetcounter<<1)+2];
            if (!isxdigit(udh[octetcounter *3]))
            {
                i = octetcounter *2 +2;
                if (!udh[octetcounter *3])
                    result = -2;
                break;
            }
            udh[octetcounter*3+1]=pdu[(octetcounter<<1)+3];
            if (!isxdigit(udh[octetcounter *3 +1]))
            {
                i = octetcounter *2 +3;
                if (!udh[octetcounter *3 +1])
                    result = -2;
                break;
            }
            udh[octetcounter *3 +2] = ' ';
            udh[octetcounter *3 +3] = 0;
        }

        if (i)
        {
            if (errorpos)
            *errorpos = i;
            return result;
        }

        if (udh_type)
            if (explain_udh(udh_type, pdu + 2) < 0)
                if (strlen(udh_type) + 7 < (size_t)max_udh_type)
                    sprintf(strchr(udh_type, 0), "%sERROR", (*udh_type)? ", " : "");

        // Calculate how many text charcters include the UDH.
        // After the UDH there may follow filling bits to reach a 7bit boundary.
        skip_characters=(((udhsize+1)*8)+6)/7;
    }

    if (expected_length)
        *expected_length = septets - skip_characters;

    // Convert from 8-Bit to 7-Bit encapsulated in 8 bit 
    // skipping storing of some characters used by UDH.
    // 3.1beta7: Simplified handling to allow partial decodings to be shown.
    octets = (septets *7 +7) /8;     
    bitposition = 0;
    octetcounter = 0;
    for (charcounter = 0; charcounter < septets; charcounter++)
    {
        c = 0;
        for (bitcounter = 0; bitcounter < 7; bitcounter++)
        {
            byteposition = bitposition /8;
            byteoffset = bitposition %8;
            while (byteposition >= octetcounter && octetcounter < octets)
            {
                if ((i = octet2bin_check(pdu +(octetcounter << 1) +2)) < 0)
                {
                    if (errorpos)
                    {
                        *errorpos = octetcounter *2 +2;
                        if (i == -2 || i == -4)
                            (*errorpos)++;
                    }
                    if (text_length)
                        *text_length = charcounter -skip_characters;
                    return (i >= -2)? -2: -1;
                }
                binary = i;
                octetcounter++;
            }
            if (binary & (1 << byteoffset))
                c = c | 128;
            bitposition++;
            c = (c >> 1) & 127; // The shift fills with 1, but 0 is wanted.
        }
        if (charcounter >= skip_characters)
            text[charcounter -skip_characters] = c; 
    }

    if (text_length)
        *text_length = charcounter -skip_characters;

    if (charcounter -skip_characters >= 0)
        text[charcounter -skip_characters] = 0;
    return charcounter -skip_characters;
}

int pdu2text0(char *pdu, char *text)
{
    return pdu2text(pdu, text, 0, 0, 0, 0, 0, 0);
}

// Converts a PDU string to binary. Return -1 if there is a PDU error, -2 if PDU is too short.
// Version > 3.0.9, > 3.1beta6 handles also udh.
int pdu2binary(const char* pdu, char* binary, int *data_length, 
               int *expected_length, int with_udh, char *udh, char *udh_type, 
               int *errorpos)
{
    int octets;
    int octetcounter;
    int i;
    int udhsize = 0;
    int skip_octets = 0;
    int result;

    *udh = 0;
    *udh_type = 0;

    if ((octets = octet2bin_check(pdu)) < 0)
    {
        *errorpos = -1 * octets -3;
        return (octets >= -2)? -2: -1;
    }

    if (with_udh)
    {
        // copy the data header to udh and convert to hex dump
        // There was at least one octet and next will give an error if there is no more data:
        if ((udhsize = octet2bin_check(pdu +2)) < 0)
        {
            *errorpos = -1 * udhsize -3 +2;
            return (udhsize >= -2)? -2: -1;
        }

        i = 0;
        result = -1;
        for (octetcounter = 0; octetcounter < udhsize +1; octetcounter++)
        {
            if (octetcounter *3 +3 >= max_udh_data)
            {
                i = octetcounter *2 +2;
                result = -2;
                break;
            }
            udh[octetcounter *3] = pdu[(octetcounter << 1) +2];
            if (!isxdigit(udh[octetcounter *3]))
            {
                i = octetcounter *2 +2;
                if (!udh[octetcounter *3])
                    result = -2;
                break;
            }
            udh[octetcounter *3 +1] = pdu[(octetcounter << 1) +3];
            if (!isxdigit(udh[octetcounter *3 +1]))
            {
                i = octetcounter *2 +3;
                if (!udh[octetcounter *3 +1])
                    result = -2;
                break;
            }
            udh[octetcounter *3 +2] = ' '; 
            udh[octetcounter *3 +3] = 0; 
        }

        if (i)
        {
            *errorpos = i;
            return result;
        }

        if (udh_type)
            if (explain_udh(udh_type, pdu + 2) < 0)
                if (strlen(udh_type) + 7 < (size_t)max_udh_type)
                    sprintf(strchr(udh_type, 0), "%sERROR", (*udh_type)? ", " : "");

        skip_octets = udhsize + 1;
    }

    *expected_length = octets -skip_octets;

    for (octetcounter = 0; octetcounter < octets - skip_octets; octetcounter++)
    {
        if ((i = octet2bin_check(pdu +(octetcounter << 1) +2 +(skip_octets *2))) < 0)
        {
            *errorpos = octetcounter *2 +2 +(skip_octets *2);
            if (i == -2 || i == -4)
                (*errorpos)++;
            *data_length = octetcounter;
            return (i >= -2)? -2: -1;
        }
        else
            binary[octetcounter] = i;
    }

    if (octets -skip_octets >= 0)
        binary[octets -skip_octets] = 0;
    *data_length = octets -skip_octets;
    return octets -skip_octets;
}

// Converts an ascii text to a pdu string 
// text might contain zero values because this is a valid character code in sms
// character set. Therefore we need the length parameter.
// If udh is not 0, then insert it to the beginning of the message.
// The string must be in hex-dump format: "05 00 03 AF 02 01". 
// The first byte is the size of the UDH.
int text2pdu(char* text, int length, char* pdu, char* udh)
{
    char tmp[500];
    char octett[10];
    int pdubitposition;
    int pdubyteposition = 0;
    int character;
    int bit;
    int pdubitnr;
    int counted_characters = 0;
    int udh_size_octets;   // size of the udh in octets, should equal to the first byte + 1
    int udh_size_septets;  // size of udh in septets (7bit text characters)
    int fillbits;          // number of filler bits between udh and ud.
    int counter;

    pdu[0]=0;
    // Check the udh
    if (udh)
    {
        udh_size_octets = (strlen(udh) + 2) / 3;
        udh_size_septets = ((udh_size_octets) * 8 + 6) / 7;
        fillbits = 7 - (udh_size_octets % 7);
        if (fillbits == 7)
            fillbits = 0;

        // copy udh to pdu, skipping the space characters
        for (counter = 0; counter < udh_size_octets; counter++)
        {
            pdu[counter * 2] = udh[counter * 3];
            pdu[counter * 2 + 1] = udh[counter * 3 + 1];
        }
        pdu[counter * 2] = 0;
    } 
    else
    {
        udh_size_octets = 0;
        udh_size_septets = 0; 
        fillbits = 0;
    }
    // limit size of the message to maximum allowed size
    if (length > max_pdu - udh_size_septets)
    length = max_pdu - udh_size_septets;
    //clear the tmp buffer
    for (character = 0; (size_t)character < sizeof(tmp); character++)
        tmp[character] = 0;
    // Convert 8bit text stream to 7bit stream
    for (character = 0; character < length; character++)
    {
        counted_characters++;
        for (bit = 0; bit < 7; bit++)
        {
            pdubitnr= 7 * character + bit + fillbits;
            pdubyteposition = pdubitnr / 8;
            pdubitposition = pdubitnr % 8;
            if (text[character] & (1 << bit))
                tmp[pdubyteposition] = tmp[pdubyteposition] | (1 << pdubitposition);
            else
                tmp[pdubyteposition] = tmp[pdubyteposition] & ~(1 << pdubitposition);
        }
    }
    tmp[pdubyteposition + 1] = 0;
    // convert 7bit stream to hex-dump
    for (character = 0; character <= pdubyteposition; character++)
    {
        sprintf(octett, "%02X", (unsigned char)tmp[character]);
        strcat(pdu, octett);
    }
    return counted_characters + udh_size_septets;
}

/* Converts binary to PDU string, this is basically a hex dump. */
void binary2pdu(char* binary, int length, char* pdu)
{
    int character;
    char octett[10];

    if (length > maxsms_binary)
        length = maxsms_binary;
    pdu[0] = 0;
    for (character = 0; character < length; character++)
    {
        sprintf(octett, "%02X", (unsigned char)binary[character]);
        strcat(pdu, octett);
    }
}

// Constructors/destructor
PDU::PDU() :
    m_pdu(NULL), m_pdu_ptr(NULL), m_message(NULL), m_message_len(-1), 
    m_smsc(NULL), m_number(NULL), m_number_type(NULL), m_number_fmt(NF_UNKNOWN), 
    m_date(NULL), m_time(NULL), m_udh_type(NULL), m_udh_data(NULL), m_err(NULL),
    m_with_udh(false), m_report(false), m_is_statusreport(false), m_replace(0),
    m_alphabet(-1), m_flash(false), m_mode(NULL), m_validity(170), 
    m_system_msg(0), m_replace_msg(0)
{
    m_mode = "new";
    m_pdu = (char*)malloc(max_pdu);
}

PDU::PDU(const char *pdu) :
    m_pdu(NULL), m_pdu_ptr(NULL), m_message(NULL), m_message_len(-1), 
    m_smsc(NULL), m_number(NULL), m_number_type(NULL), m_number_fmt(NF_UNKNOWN), 
    m_date(NULL), m_time(NULL), m_udh_type(NULL), m_udh_data(NULL), m_err(NULL),
    m_with_udh(false), m_report(false), m_is_statusreport(false), m_replace(0),
    m_alphabet(-1), m_flash(false), m_mode(NULL), m_validity(170), 
    m_system_msg(0), m_replace_msg(0)
{
    if (pdu)
    {
        m_pdu = strdup(pdu);
        m_pdu_ptr = m_pdu;
        m_err = (char*)malloc(max_err);
        m_err[0] = '\0';
    }
}

PDU::~PDU()
{
    if (m_pdu)
        free(m_pdu);
    
    if (m_number)
        free(m_number);
    
    if (m_number_type)
        free(m_number_type);
    
    if (m_smsc)
        free(m_smsc);
    
    if (m_message)
        free(m_message);
    m_message = 0;
    
    if (m_date)
        free(m_date);
    
    if (m_time)
        free(m_time);

    if (m_udh_type)
        free(m_udh_type);
    
    if (m_udh_data)
        free(m_udh_data);
    
    if (m_err)
        free(m_err);
}

// Utility methods
void PDU::reset()
{
    if (m_number)
        free(m_number);
    m_number = (char*)malloc(max_number);
    m_number[0] = '\0';
    
    if (m_number_type)
        free(m_number_type);
    m_number_type = (char*)malloc(max_number_type);
    m_number_type[0] = '\0';
    
    if (m_smsc)
        free(m_smsc);
    m_smsc = (char*)malloc(max_smsc);
    m_smsc[0] = '\0';
    
    if (m_message)
        free(m_message);
    m_message = (char*)malloc(max_message);
    m_message[0] = '\0';
    m_message_len = 0;
    
    if (m_date)
        free(m_date);
    m_date = (char*)malloc(max_date);
    m_date[0] = '\0';
    
    if (m_time)
        free(m_time);
    m_time = (char*)malloc(max_time);
    m_time[0] = '\0';
    
    if (m_udh_type)
        free(m_udh_type);
    m_udh_type = (char*)malloc(max_udh_type);
    m_udh_type[0] = '\0';
    
    if (m_udh_data)
        free(m_udh_data);
    m_udh_data = (char*)malloc(max_udh_data);
    m_udh_data[0] = '\0';
    
    if (!m_err)
        m_err = (char*)malloc(max_err);
    m_err[0] = '\0';
    
    m_with_udh = false;
    m_report = false;
    m_is_statusreport = false;
}

// int PDU::convert(const char *tocode, const char *fromcode)
// {
//     iconv_t cd = iconv_open(tocode, fromcode);
//     if (cd == (iconv_t)(-1) || !m_message)
//         return -1;
        
//     size_t inbytesleft = m_message_len, outbytesleft = maxsms_binary * 2;
    
//     char *tmp = (char*)malloc(outbytesleft);
//     char *msg = m_message;
//     char *out = tmp;
//     memset(tmp, 0, outbytesleft);
    
//     iconv(cd, &msg, &inbytesleft, &out, &outbytesleft);
    
//     if (m_message)
//         free(m_message);
//     m_message = tmp;
    
//     iconv_close(cd);
    
//     m_message_len = maxsms_binary - outbytesleft;
    
//     return m_message_len;
// }

// Parsing
bool PDU::parse()
{   
    if (!m_pdu)
    {
        sprintf(m_err, "PDU is NULL");
        return false;
    }
    
    // Patch for Wavecom SR memory reading:
    if (strncmp(m_pdu, "000000FF00", 10) == 0)
    {
        memmove(m_pdu, m_pdu + 8, strlen(m_pdu + 8) + 1); //Thanks to Keijo Kasvi :)
        while (strlen(m_pdu) < 52)
            strcat(m_pdu, "00");
    }
    // ------------------------------------
    reset();
    
    if (strlen(m_pdu) < 2)
    {
        sprintf(m_err, "PDU is too short");
        return false;
    }
    
    if (!parseSMSC())
        return false;
    
    if (strlen(m_pdu_ptr) < 2)
    {
        sprintf(m_err, "Reading First octet of the SMS-DELIVER PDU: PDU is too short");
        return false;
    }
    
    int tmp;
    if ((tmp = octet2bin_check(m_pdu_ptr)) < 0)
    {
         sprintf(m_err, "Reading First octet of the SMS-DELIVER PDU: Invalid character(s)");
         return false;
    }
    
    // Unused bits 3 and 4 should be zero, failure with this produces a warning:
    if (tmp & 0x18)
        sprintf(m_err, "Warning: Unused bits 3 and 4 are used in the first octet of the SMS-DELIVER PDU.\n");
    
    if (tmp & 0x40) // Is UDH bit set?
        m_with_udh = true;
    
    if (tmp & 0x20) // Is status report going to be returned to the SME?
        m_report = true;
    
    int type = tmp & 3;
    if (type == 0) // SMS Deliver
    {
        m_pdu_ptr += 2;
        if (!parseDeliver())
            return false;
        // if (m_alphabet == 2)
        //     convert("UTF8", "UTF16BE");
    }
    else if (type == 2) // Status Report
    {
        m_pdu_ptr += 2;
        if(!parseStatusReport())
            return false;
        m_is_statusreport = true;
    }
    else if (type == 1) // Sent message
    {
        sprintf(m_err, "This is a sent message. Can only decode received messages.");
        return false;
    }
    else
    {
        sprintf(m_err, "Unsupported type");
        return false;
    }

    return true;
}

bool PDU::parseSMSC()
{
    int length;
    
    if ((length = octet2bin_check(m_pdu_ptr)) < 0)
    {
        sprintf(m_err, "Invalid character(s) in first octet");
        return false;
    }

    if (length == 0)
    {
        m_pdu_ptr += 2;
        return true;
    }

    if (length < 2 || length > max_smsc)
    {
        sprintf(m_err, "Invalid sender SMSC address length");
        return false;
    }
    
    length = length * 2 - 2;
    // No padding because the given value is number of octets.
    if (strlen(m_pdu) < (size_t)(length + 4))
    {
        sprintf(m_err, "Reading SMSC address: PDU is too short");
        return false;
    }
    
    m_pdu_ptr += 2;
    // Read SMSC address type
    int addr_type = octet2bin_check(m_pdu_ptr);
    if (addr_type < 0)
    {
        sprintf(m_err, "Invalid sender SMSC address type");
        return false;
    }
    if (addr_type < 0x80)
    {
        sprintf(m_err, "Missing bit 7 in sender SMSC address type");
        return false;
    }
    
    // Read SMSC address
    m_pdu_ptr += 2;
    strncpy(m_smsc, m_pdu_ptr, (size_t)length);
    m_smsc[length] = '\0';
    swapchars(m_smsc); 
    
    // Does any SMSC use alphanumeric number?
    if ((addr_type & 112) == 80)
    {
        // There can be only hex digits from the original PDU.
        // The number itself is wrong in this case.
        for (size_t i = 0; i < (size_t)length; i++)
        {
            if (!isxdigit(m_smsc[i]))
            {
                sprintf(m_err, "Invalid character(s) in alphanumeric SMSC address");
                return false;
            }
        }
    }
    else
    {
        // Last character is allowed as F (and dropped) but all other non-numeric will produce an error:
        if (m_smsc[length - 1] == 'F')
            m_smsc[length - 1] = '\0';
        for (size_t i = 0; i < strlen(m_smsc); i++)
        {
            if (!isdigit(m_smsc[i]))
            {
                sprintf(m_err, "Invalid character(s) in numeric SMSC address");
                return false;
            }
        }
    }
    
    m_pdu_ptr += length;
    return true;
}

int PDU::explainAddressType(const char *octet_char, int octet_int)
{
    int result;
    const char *p = "reserved";
    
    if (octet_char)
        result = octet2bin_check(octet_char);
    else
        result = octet_int;

    if (result != -1)
    {
        switch ((result & 0x70) >> 4)
        {
            case 0: p = "unknown"; break;
            case 1: p = "international"; break;
            case 2: p = "national"; break;
            case 3: p = "network specific"; break;
            case 4: p = "subsciber"; break;
            case 5: p = "alphanumeric"; break;
            case 6: p = "abbreviated"; break;
        }
        
        sprintf(m_number_type, p);
        
        switch (result & 0x0F)
        {
            case 0: p = "unknown"; break;
            case 1: p = "ISDN/telephone"; break;
            case 3: p = "data"; break;
            case 4: p = "telex"; break;
            case 8: p = "national"; break;
            case 9: p = "private"; break;
            case 10: p = "ERMES"; break;
        }
        strcat(m_number_type, ", ");
        strcat(m_number_type, p);
    }
    return result;
}

bool PDU::parseDeliver()
{
    if (strlen(m_pdu_ptr) < 4)
    {
        sprintf(m_err, "Reading address length and address type: PDU is too short");
        return false;
    }
    
    int padding = 0;
    int length = octet2bin_check(m_pdu_ptr);
    if (length < 0 || length > max_number)
    {
        sprintf(m_err, "Invalid sender address length");
        return false;
    }
    if (length == 0)
        m_pdu_ptr += 4;
    else
    {
        padding = length % 2;
        m_pdu_ptr += 2;
        int addr_type = explainAddressType(m_pdu_ptr, 0);
        if (addr_type < 0)
        {
            sprintf(m_err, "Invalid sender address type");
            return false;
        }
        if (addr_type < 0x80)
        {
            sprintf(m_err, "Missing bit 7 in sender address type");
            return false;
        }
        
        m_pdu_ptr += 2;
        if ((addr_type & 112) == 80) // Sender is alphanumeric
        {
            if (strlen(m_pdu_ptr) < (size_t)(length + padding))
            {
                sprintf(m_err, "Reading sender address (alphanumeric): PDU is too short");
                return false;
            }
            char tmpsender[100];
            char sender[100];
            snprintf(tmpsender, length + padding + 3, "%02X%s", length * 4 / 7, m_pdu_ptr);
            if (pdu2text0(tmpsender, sender) < 0)
            {
                sprintf(m_err, "Reading alphanumeric sender address: Invalid character(s)");
                return false;
            }
            strcpy(m_number, sender);
        }
        else // Sender is numeric
        {
            strncpy(m_number, m_pdu_ptr, length + padding);
            m_number[length + padding] = '\0';
            swapchars(m_number);
           
            int end = length + padding - 1;
            if (padding)
            {
                if (m_number[end] != 'F')
                    sprintf(m_err, "Length of numeric sender address is odd, but not terminated with 'F'.\n");
                else
                    m_number[end] = '\0';
            }
            else
            {
                if (m_number[end] == 'F')
                {
                    sprintf(m_err, "Length of numeric sender address is even, but still was terminated with 'F'.\n");
                    m_number[end] = '\0';
                }
            }
            
            for (size_t i = 0; i < strlen(m_number); i++)
            {
                if (!isdigit(m_number[i]))
                {
                    sprintf(m_err, "Invalid character(s) in sender address.\n");
                    break;
                }
            }
        }
    }
    m_pdu_ptr += length + padding;
    
    // Next there should be:
    // XX protocol identifier
    // XX data encoding scheme
    // XXXXXXXXXXXXXX time stamp, 7 octets
    // XX length of user data
    // ( XX... user data  )
    if (strlen(m_pdu_ptr) < 20)
    {
        sprintf(m_err, "Reading TP-PID, TP-DSC, TP-SCTS and TP-UDL: PDU is too short");
        return false;
    }
    // PID
    int byte_buf;
    if ((byte_buf = octet2bin_check(m_pdu_ptr)) < 0)
    {
        sprintf(m_err, "Invalid protocol identifier");
        return false;
    }
    if ((byte_buf & 0xF8) == 0x40)
        m_replace = (byte_buf & 0x07);
    
    // Alphabet
    m_pdu_ptr += 2;
    if ((byte_buf = octet2bin_check(m_pdu_ptr)) < 0)
    {
        sprintf(m_err, "Invalid data encoding scheme");
        return false;
    }
    m_alphabet = (byte_buf & 0x0C) >> 2;
    if (m_alphabet == 3)
    {
        // TODO: or should this be a warning? If so, GSM alphabet is then used as a default.
        sprintf(m_err, "Invalid alphabet in data encoding scheme: value 3 is not supported");
        return false;
    }
    if (m_alphabet == 0)
        m_alphabet = -1;
    
    // 3.1: Check if this message was a flash message:
    if (byte_buf & 0x10)
        if (!(byte_buf & 0x01))
            m_flash = true;
    
    // Date
    m_pdu_ptr += 2;
    char str_buf[100];
    sprintf(str_buf, "%c%c-%c%c-%c%c", m_pdu_ptr[1], m_pdu_ptr[0], m_pdu_ptr[3],
            m_pdu_ptr[2], m_pdu_ptr[5], m_pdu_ptr[4]);
    if (!isdigit(str_buf[0]) || !isdigit(str_buf[1]) || !isdigit(str_buf[3]) || 
    	!isdigit(str_buf[4]) || !isdigit(str_buf[6]) || !isdigit(str_buf[7]))
    {
        sprintf(m_err, "Invalid character(s) in date of Service Centre Time Stamp");
        return false;
    }
    else if (atoi(str_buf + 3) > 12 || atoi(str_buf + 6) > 31)
    {
        sprintf(m_err, "Invalid values(s) in date of Service Centre Time Stamp.\n");
    }
    sprintf(m_date, str_buf);
    
    // Time
    m_pdu_ptr += 6;
    sprintf(str_buf, "%c%c:%c%c:%c%c", m_pdu_ptr[1], m_pdu_ptr[0], m_pdu_ptr[3],
            m_pdu_ptr[2], m_pdu_ptr[5], m_pdu_ptr[4]);
    if (!isdigit(str_buf[0]) || !isdigit(str_buf[1]) || !isdigit(str_buf[3]) 
        || !isdigit(str_buf[4]) || !isdigit(str_buf[6]) || !isdigit(str_buf[7]))
    {
        sprintf(m_err, "Invalid character(s) in time of Service Centre Time Stamp");
        return false;
    }
    else if (atoi(str_buf) > 23 || atoi(str_buf + 3) > 59 || atoi(str_buf + 6) > 59)
    {
        sprintf(m_err, "Invalid values(s) in time of Service Centre Time Stamp.\n");
    }
    sprintf(m_time, str_buf);
    
    m_pdu_ptr += 6;
    // Time zone is not used but bytes are checked:
    if (octet2bin_check(m_pdu_ptr) < 0)
    {
        sprintf(m_err, "Invalid character(s) in Time Zone of Service Centre Time Stamp");
        return false;
    }
    
    m_pdu_ptr += 2;
    
    // TODO: clean it
    int result = 0;
    char message[max_message]; message[0] = 0;
    int message_length;
    int expected_length;
    char udh_data[max_udh_data]; udh_data[0] = 0;
    char udh_type[max_udh_type]; udh_type[0] = 0;
    int errorpos = 0;
    int bin_udh = 1;    // UDH binary format flag
    
    if (m_alphabet <= 0)
    {
        if ((result = pdu2text(m_pdu_ptr, message, &message_length, 
                               &expected_length, (int)m_with_udh, udh_data, 
                               udh_type, &errorpos)) < 0)
        {
            sprintf(m_err, "Error while reading TP-UD (GSM text)");
            return false;
        }
    }
    else
    {
        // With binary messages udh is NOT taken from the PDU.
        int i = (int)m_with_udh;
        // 3.1.5: it should work now:
        if (bin_udh == 0)
            if (m_alphabet == 1)
                i = 0;
        if ((result = pdu2binary(m_pdu_ptr, message, &message_length, 
                                 &expected_length, i, udh_data, udh_type, 
                                 &errorpos)) < 0)
        {
            sprintf(m_err, "Error while reading TP-UD: %s", (m_alphabet == 1) ? "binary" : "UCS2 text");
            return false;
        }
    }
    
    memcpy(m_message, message, result);
    m_message_len = result;
    m_message[m_message_len] = '\0';
    strcpy(m_udh_type, udh_type);
    strcpy(m_udh_data, udh_data);
    
    return true;
}

bool PDU::parseStatusReport()
{
	// There should be at least message-id, address-length and address-type:
	if (strlen(m_pdu_ptr) < 6)
	{
        sprintf(m_err, "Reading message id, address length and address type: PDU is too short");
        return false;
    }
    
    int messageid;
    if ((messageid = octet2bin_check(m_pdu_ptr)) < 0)
	{
		sprintf(m_err, "Invalid message id");
		return false;
	}

	// get recipient address
	m_pdu_ptr += 2;
	int length = octet2bin_check(m_pdu_ptr);
	if (length < 1 || length > max_number)
	{
		sprintf(m_err, "Invalid recipient address length");
		return false;
	}
	int padding = length % 2;
	m_pdu_ptr += 2;
	
	int addr_type = explainAddressType(m_pdu_ptr, 0);
	if (addr_type < 0)
	{
		sprintf(m_err, "Invalid recipient address type");
		return false;
	}
	if (addr_type < 0x80)
	{
		sprintf(m_err, "Missing bit 7 in recipient address type");
		return false;
	}
	
	m_pdu_ptr += 2;
	if ((addr_type & 112) == 80) // Sender is alphanumeric
	{
		if (strlen(m_pdu_ptr) < (size_t)(length + padding))
		{
			sprintf(m_err, "Reading sender address (alphanumeric): PDU is too short");
			return false;
		}
		char tmpsender[100];
		char sender[100];
		snprintf(tmpsender, length + padding + 3, "%02X%s", length * 4 / 7, m_pdu_ptr);
		if (pdu2text0(tmpsender, sender) < 0)
		{
			sprintf(m_err, "Reading alphanumeric sender address: Invalid character(s)");
			return false;
		}
		strcpy(m_number, sender);
	}
	else // Sender is numeric
	{
		strncpy(m_number, m_pdu_ptr, length + padding);
		m_number[length + padding] = '\0';
		swapchars(m_number);
           
		int end = length + padding - 1;
		if (padding)
		{
			if (m_number[end] != 'F')
				sprintf(m_err, "Length of numeric sender address is odd, but not terminated with 'F'.\n");
			else
				m_number[end] = '\0';
		}
		else
		{
			if (m_number[end] == 'F')
			{
				sprintf(m_err, "Length of numeric sender address is even, but still was terminated with 'F'.\n");
				m_number[end] = '\0';
			}
		}
            
		for (size_t i = 0; i < strlen(m_number); i++)
		{
			if (!isdigit(m_number[i]))
			{
				sprintf(m_err, "Invalid character(s) in sender address.\n");
				break;
			}
		}
	}

    m_pdu_ptr += length + padding;
	if (strlen(m_pdu_ptr) < 14)
	{
		sprintf(m_err, "While trying to read SMSC Timestamp: PDU is too short");
		return false;
	}
	// get SMSC timestamp
	char str_buf[100];
	sprintf(str_buf, "%c%c-%c%c-%c%c", m_pdu_ptr[1], m_pdu_ptr[0], m_pdu_ptr[3],
			m_pdu_ptr[2], m_pdu_ptr[5], m_pdu_ptr[4]);
	if (!isdigit(str_buf[0]) || !isdigit(str_buf[1]) || !isdigit(str_buf[3]) || 
		!isdigit(str_buf[4]) || !isdigit(str_buf[6]) || !isdigit(str_buf[7]))
	{
		sprintf(m_err, "Invalid character(s) in date of SMSC Timestamp");
		return false;
	}
	else if (atoi(str_buf + 3) > 12 || atoi(str_buf + 6) > 31)
	{
		sprintf(m_err, "Invalid value(s) in date of SMSC Timestamp.");
	}
	sprintf(m_date, str_buf);
	
	m_pdu_ptr += 6;
	sprintf(str_buf, "%c%c:%c%c:%c%c", m_pdu_ptr[1], m_pdu_ptr[0], m_pdu_ptr[3],
			m_pdu_ptr[2], m_pdu_ptr[5], m_pdu_ptr[4]);
	if (!isdigit(str_buf[0]) || !isdigit(str_buf[1]) || !isdigit(str_buf[3]) || 
		!isdigit(str_buf[4]) || !isdigit(str_buf[6]) || !isdigit(str_buf[7]))
	{
		sprintf(m_err, "Invalid character(s) in time of SMSC Timestamp");
		return false;
	}
	else if (atoi(str_buf) > 23 || atoi(str_buf + 3) > 59 || atoi(str_buf + 6) > 59)
	{
		sprintf(m_err, "Invalid value(s) in time of SMSC Timestamp.");
	}
	sprintf(m_time, str_buf);

	m_pdu_ptr += 6;
	// Time zone is not used but bytes are checked:
	if (octet2bin_check(m_pdu_ptr) < 0)
	{
		sprintf(m_err, "Invalid character(s) in Time Zone of SMSC Time Stamp");
		return false;
	}
	m_pdu_ptr += 2;


	if (strlen(m_pdu_ptr) < 14)
	{
		sprintf(m_err, "While trying to read Discharge Timestamp: PDU is too short");
		return false;
	}

	// get Discharge timestamp
	sprintf(str_buf, "%c%c-%c%c-%c%c %c%c:%c%c:%c%c", m_pdu_ptr[1], 
			m_pdu_ptr[0], m_pdu_ptr[3], m_pdu_ptr[2], m_pdu_ptr[5], 
			m_pdu_ptr[4], m_pdu_ptr[7], m_pdu_ptr[6], m_pdu_ptr[9], 
			m_pdu_ptr[8], m_pdu_ptr[11], m_pdu_ptr[10]);
	if (!isdigit(str_buf[0]) || !isdigit(str_buf[1]) || !isdigit(str_buf[3]) || 
		!isdigit(str_buf[4]) || !isdigit(str_buf[6]) || !isdigit(str_buf[7]) || 
		!isdigit(str_buf[9]) || !isdigit(str_buf[10]) || 
		!isdigit(str_buf[12]) || !isdigit(str_buf[13]) || 
		!isdigit(str_buf[15]) || !isdigit(str_buf[16]))
	{
		sprintf(m_err, "Invalid character(s) in Discharge Timestamp");
		return false;
	}
	else if (atoi(str_buf + 3) > 12 || atoi(str_buf + 6) > 31 || 
			 atoi(str_buf + 9) > 24 || atoi(str_buf + 12) > 59 || 
			 atoi(str_buf + 16) > 59)
	{
		sprintf(m_err, "Invalid values(s) in Discharge Timestamp.");
	}

	m_pdu_ptr += 12;
	// Time zone is not used but bytes are checked:
	if (octet2bin_check(m_pdu_ptr) < 0)
	{
		sprintf(m_err, "Invalid character(s) in Time Zone of Discharge Time Stamp");
		return false;
	}
	m_pdu_ptr += 2;
	
	char discharge_timestamp[128];
	sprintf(discharge_timestamp, "%s", str_buf);
	
	if (strlen(m_pdu_ptr) < 2)
	{
		sprintf(m_err, "While trying to read Status octet: PDU is too short");
		return false;
	}
	
	int status;
	if ((status = octet2bin_check(m_pdu_ptr)) < 0)
	{
		sprintf(m_err, "Invalid Status octet");
		return false;
	}

	explain_status(str_buf, sizeof(str_buf), status);
	
	sprintf(m_message, "Discharge timestamp: %s\nMessage ID: %d\nStatus: %s",
			discharge_timestamp, messageid, str_buf);
	
    return true;
}

// PDU Generation
void PDU::generate()
{
    int coding;
    int flags;
    char tmp[max_message];
    char tmp2[500];
    int numberformat;
    int numberlength;
    char *p;
    int l;
    char tmp_smsc[max_smsc];
    
    // if (m_alphabet == 2)
    //     convert("UTF16BE", "UTF8");

    if (m_number[0] == 's')  // Is number starts with s, then send it without number format indicator
    {
        numberformat = NF_UNKNOWN;
        snprintf(tmp, sizeof(tmp), "%s", m_number +1);
    }
    else
    {
        numberformat = NF_INTERNATIONAL;
        snprintf(tmp, sizeof(tmp), "%s", m_number);
    }

    numberlength = strlen(tmp);
    // terminate the number with F if the length is odd
    if (numberlength % 2)
        strcat(tmp, "F");
    // Swap every second character
    swapchars(tmp);

    // 3.1.12:
    *tmp_smsc = 0;
    if (m_smsc && m_smsc[0])
    {
        p = m_smsc;
        while (*p == '+')
            p++;
        snprintf(tmp_smsc, sizeof(tmp_smsc), "%s", p);
        if (strlen(tmp_smsc) % 2 && strlen(tmp_smsc) < sizeof(tmp_smsc) - 1)
            strcat(tmp_smsc, "F");
        swapchars(tmp_smsc);
    }

    flags = 1; // SMS-Sumbit MS to SMSC
    if (m_with_udh)
        flags += 64; // User Data Header
    if (strcmp(m_mode, "old") != 0)
        flags += 16; // Validity field
    if (m_report)
        flags += 32; // Request Status Report

    if (m_alphabet == 1)
        coding = 4; // 8bit binary
    else if (m_alphabet == 2)
        coding = 8; // 16bit
    else
        coding = 0; // 7bit
    if (m_flash)
        coding += 0x10; // Bits 1 and 0 have a message class meaning (class 0, alert)

    /* Create the PDU string of the message */
    if (m_alphabet == 1 || m_alphabet == 2)
    {
        // Unicode message can be concatenated:
        //if (alphabet == 2 && with_udh)
        // Binary message can be concatenated too:
        if (m_with_udh)
        {
            strcpy(tmp2, m_udh_data);
            while ((p = strchr(tmp2, ' ')))
            	memmove(p, p + 1, strlen(p + 1) + 1);
                //strcpyo(p, p + 1);
            l = strlen(tmp2) / 2;
            binary2pdu(m_message, m_message_len, strchr(tmp2, 0));
            m_message_len += l;
        }
        else
            binary2pdu(m_message, m_message_len, tmp2);
    }
    else
        m_message_len = text2pdu(m_message, m_message_len, tmp2, m_udh_data);

    int smsc_len = 0;
    /* concatenate the first part of the PDU string */
    if (strcmp(m_mode, "old") == 0)
        sprintf(m_pdu, "%02X00%02X%02X%s00%02X%02X", flags, numberlength, 
                numberformat, tmp, coding, m_message_len);
    else
    {
        int proto = 0;

        if (m_validity < 0 || m_validity > 255)
            m_validity = validity_period;

        if (m_system_msg)
        {
            proto = 0x40;
            coding = 0xF4;  // binary

            // 3.1.7:
            if (m_system_msg == 2)
            {
                proto += (0x7F - 0x40); // SS (no show)
                coding += 2;    // store to sim
            }
        }
        else if (m_replace_msg >= 1 && m_replace_msg <= 7)
            proto = 0x40 + m_replace_msg;

        if (*tmp_smsc)
            sprintf(m_pdu, "%02X%s%s", (int)strlen(tmp_smsc) / 2 + 1, 
                    (tmp_smsc[1] == '0') ? "81": "91", tmp_smsc);
        else
            strcpy(m_pdu, "00");
        
        smsc_len = strlen(m_pdu);
        
        sprintf(strchr(m_pdu, 0), "%02X00%02X%02X%s%02X%02X%02X%02X", flags, 
                numberlength, numberformat, tmp, proto, coding, m_validity, 
                m_message_len);
        
    }
    /* concatenate the text to the PDU string */
    strcat(m_pdu,tmp2);
    m_message_len = (strlen(m_pdu) - smsc_len)/2;
}

// Setters
void PDU::setMessage(const char* message, const int message_len)
{
    if (message)
    {
        m_message_len = (message_len < 0) ? strlen(message) : message_len;
        if (m_message)
            free(m_message);
        m_message = (char*)malloc(m_message_len + 1);
        memcpy(m_message, message, m_message_len);
        m_message[m_message_len] = '\0';
    }
}

void PDU::setSMSC(const char* smsc)
{
    if (smsc)
    {
        if (m_smsc)
            free(m_smsc);
        m_smsc = strdup(smsc);
    }
}

void PDU::setNumber(const char* number)
{
    if (number)
    {
        if (m_number)
            free(m_number);
        m_number = strdup(number);
    }
}

void PDU::setAlphabet(const Alphabet alphabet)
{
    m_alphabet = (int)alphabet;
}

