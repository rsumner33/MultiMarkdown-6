/**

	MultiMarkdown 6 -- Lightweight markup processor to produce HTML, LaTeX, and more.

	@file lexer.re

	@brief Description of the regular expressions used to define tokens, 
	used by re2c to create a lexer/tokenizer.


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

#include "lexer.h"
#include "libMultiMarkdown.h"
#include "parser.h"


// Basic scanner struct

#define YYCTYPE		char
#define YYCURSOR	s->cur
#define YYMARKER	s->ptr
#define YYCTXMARKER	s->ctx


int scan(Scanner * s, const char * stop) {

	scan:

	if (s->cur >= stop) {
		return 0;
	}

	s->start = s->cur;

	/*!re2c
		re2c:yyfill:enable = 0;

		NL								= "\r\n" | '\n' | '\r';
		SP								= [ \t]+;

		SPNL							= [ \t]* NL;

		INDENT_TAB 						= '\t';
		INDENT_SPACE 					= ' '{4};
		NON_INDENT_SPACE				= ' '{2,3};

		TEXT_LINEBREAK					= "  " NL;

		// The order of these seems to matter

		"{++"							{ return CRITIC_ADD_OPEN; }
		"++}"							{ return CRITIC_ADD_CLOSE; }

		"{--"							{ return CRITIC_DEL_OPEN; }
		"--}"							{ return CRITIC_DEL_CLOSE; }

		"{>>"							{ return CRITIC_COM_OPEN; }
		"<<}"							{ return CRITIC_COM_CLOSE; }

		"{~~"							{ return CRITIC_SUB_OPEN; }
		"~>"							{ return CRITIC_SUB_DIV; }
		"~~}"							{ return CRITIC_SUB_CLOSE; }

		"{=="							{ return CRITIC_HI_OPEN; }
		"==}"							{ return CRITIC_HI_CLOSE; }

		"["								{ return BRACKET_LEFT; }
		"]"								{ return BRACKET_RIGHT; }

		"!["							{ return BRACKET_IMAGE_LEFT; }
		"[^"							{ return BRACKET_FOOTNOTE_LEFT; }
		"[#"							{ return BRACKET_CITATION_LEFT; }
		"[%"							{ return BRACKET_VARIABLE_LEFT; }

		"("								{ return PAREN_LEFT; }
		")"								{ return PAREN_RIGHT; }

		"<"								{ return ANGLE_LEFT; }
		">"								{ return ANGLE_RIGHT; }

		"{{"							{ return BRACE_DOUBLE_LEFT; }
		"}}"							{ return BRACE_DOUBLE_RIGHT; }

		"\""							{ return QUOTE_DOUBLE; }
		"'"								{ return QUOTE_SINGLE; }
		// "``"							{ return QUOTE_LEFT_ALT; }
		"''"							{ return QUOTE_RIGHT_ALT; }
		"-"								{ return DASH_N; }
		"--"							{ return DASH_N; }
		"---"							{ return DASH_M; }
		"..."							{ return ELLIPSIS; }
		". . ."							{ return ELLIPSIS; }
		":"								{ return COLON; }
		'&amp;'							{ return AMPERSAND_LONG; }
		"&"								{ return AMPERSAND; }

		"\\."							{ return ESCAPED_CHARACTER; }
		"\\!"							{ return ESCAPED_CHARACTER; }
		"\\?"							{ return ESCAPED_CHARACTER; }

		"\\,"							{ return ESCAPED_CHARACTER; }
		"\\;"							{ return ESCAPED_CHARACTER; }
		"\\:"							{ return ESCAPED_CHARACTER; }

		"\\\""							{ return ESCAPED_CHARACTER; }
		"\\'"							{ return ESCAPED_CHARACTER; }
		"\\`"							{ return ESCAPED_CHARACTER; }
		"\\~"							{ return ESCAPED_CHARACTER; }

		"\\("							{ return ESCAPED_CHARACTER; }
		"\\)"							{ return ESCAPED_CHARACTER; }
		"\\{"							{ return ESCAPED_CHARACTER; }
		"\\}"							{ return ESCAPED_CHARACTER; }
		"\\["							{ return ESCAPED_CHARACTER; }
		"\\]"							{ return ESCAPED_CHARACTER; }

		"\\#"							{ return ESCAPED_CHARACTER; }
		"\\$"							{ return ESCAPED_CHARACTER; }
		"\\%"							{ return ESCAPED_CHARACTER; }
		"\\+"							{ return ESCAPED_CHARACTER; }
		"\\-"							{ return ESCAPED_CHARACTER; }
		"\\="							{ return ESCAPED_CHARACTER; }
		"\\<"							{ return ESCAPED_CHARACTER; }
		"\\>"							{ return ESCAPED_CHARACTER; }

		"\\&"							{ return ESCAPED_CHARACTER; }
		"\\@"							{ return ESCAPED_CHARACTER; }
		"\\\\"							{ return ESCAPED_CHARACTER; }
		"\\/"							{ return ESCAPED_CHARACTER; }
		"\\^"							{ return ESCAPED_CHARACTER; }

		"\\*"							{ return ESCAPED_CHARACTER; }
		"\\_"							{ return ESCAPED_CHARACTER; }

		"\\|"							{ return ESCAPED_CHARACTER; }

		"\\\\("							{ return MATH_PAREN_OPEN; }
		"\\\\)"							{ return MATH_PAREN_CLOSE; }
		"\\\\["							{ return MATH_BRACKET_OPEN; }
		"\\\\]"							{ return MATH_BRACKET_CLOSE; }
		"$"								{ return MATH_DOLLAR_SINGLE; }
		"$$"							{ return MATH_DOLLAR_DOUBLE; }

		'^'								{ return SUPERSCRIPT; }
		"~"								{ return SUBSCRIPT; }

		INDENT_TAB						{ return INDENT_TAB; }
		INDENT_SPACE					{ return INDENT_SPACE; }

		'#'	SP							{ return HASH1; }
		'#'	/ SPNL						{ return HASH1; }
		'#'{2} SP						{ return HASH2; }
		'#'{2} / SPNL					{ return HASH2; }
		'#'{3} SP						{ return HASH3; }
		'#'{3} / SPNL					{ return HASH3; }
		'#'{4} SP						{ return HASH4; }
		'#'{4} / SPNL					{ return HASH4; }
		'#'{5} SP						{ return HASH5; }
		'#'{5} / SPNL					{ return HASH5; }
		'#'{6} SP						{ return HASH6; }
		'#'{6} / SPNL					{ return HASH6; }


		[0-9]+ / ('.' (SP|NL))			{ return TEXT_NUMBER_POSS_LIST; }
		'.' / (SP|NL)					{ return TEXT_PERIOD; }

		TEXT_LINEBREAK					{ return TEXT_LINEBREAK; }
		NL								{ return TEXT_NL; }

		NON_INDENT_SPACE				{ return NON_INDENT_SPACE; }

		"*"								{ return STAR; }
		"+"								{ return PLUS; }
		"_"								{ return UL; }

		'`'+							{ return BACKTICK; }

		'|'+							{ return PIPE; }
		
		// Skip over anything else - '.' does not include '\n'
		.								{ goto scan; }
	*/
}
