/**

	MultiMarkdown 6 -- Lightweight markup processor to produce HTML, LaTeX, and more.

	@file char_lookup.c

	@brief Create lookup table for char.c


	@author	Fletcher T. Penney
	@bug	

**/

/*

	Copyright © 2016 - 2017 Fletcher T. Penney.


	The `MultiMarkdown 6` project is released under the MIT License..
	
	GLibFacade.c and GLibFacade.h are from the MultiMarkdown v4 project:
	
		https://github.com/fletcher/MultiMarkdown-4/
	
	MMD 4 is released under both the MIT License and GPL.
	
	
	CuTest is released under the zlib/libpng license. See CuTest.c for the text
	of the license.
	
	
	## The MIT License ##
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

*/


#include <stdlib.h>
#include <stdio.h>

#include "char.h"

// Shortcuts to assign values to characters in lookup table
#define punctuation(x)	table[x] |= CHAR_PUNCTUATION
#define whitespace(x)	table[x] |= CHAR_WHITESPACE
#define alpha(x)		table[x] |= CHAR_ALPHA
#define digit(x)		table[x] |= CHAR_DIGIT
#define line_ending(x)	table[x] |= CHAR_LINE_ENDING


int main( int argc, char** argv ) {
	unsigned char table[256] = {0};

	// Define punctuation
	// TODO: Need to go through extended ASCII codes for 
	// additional whitespace characters
	punctuation('.');
	punctuation('!');
	punctuation('?');

	punctuation(',');
	punctuation(';');
	punctuation(':');

	punctuation('"');
	punctuation('\'');
	punctuation('`');
	punctuation('~');

	punctuation('(');
	punctuation(')');
	punctuation('{');
	punctuation('}');
	punctuation('[');
	punctuation(']');

	punctuation('#');
	punctuation('$');
	punctuation('%');
	punctuation('+');
	punctuation('-');
	punctuation('=');
	punctuation('<');
	punctuation('>');

	punctuation('&');
	punctuation('@');
	punctuation('\\');
	punctuation('/');
	punctuation('^');

	punctuation('*');
	punctuation('_');

	punctuation('|');


	// Define whitespace
	// TODO: Need to go through extended ASCII codes for 
	// additional whitespace characters
	whitespace(' ');
	whitespace('\t');


	// Define line endings
	line_ending('\n');
	line_ending('\r');
	line_ending('\0');		// Count EOF as line ending


	// Define digits
	for (char i = '0'; i <= '9'; ++i)
	{
		digit(i);
	}

	// Define alpha
	// TODO: Need to go through extended ASCII codes for 
	// additional alpha characters
	for (char i = 'a'; i <= 'z'; ++i)
	{
		alpha(i);
	}
	for (char i = 'A'; i <= 'Z'; ++i)
	{
		alpha(i);
	}

	// Print output as 16 x 16 table
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			fprintf(stdout, "%3d,", table[i * 16 + j]);
		}

		fprintf(stdout, "\n");
	}
}
