#pragma once

/**
 * CSV file¿« encoding
 */
enum KEMCSVEncoding
{
	KEMCSVEncodingUnknown = -1,
	KEMCSVEncodingANSI = 0,
	KEMCSVEncodingUTF8 = 1,
	KEMCSVEncodingUTF16LE,
	KEMCSVEncodingUTF16BE
};