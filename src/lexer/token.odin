#+feature dynamic-literals
package loom_lexer

token :: struct {
	lexeme:   string,
	kind:     token_kind,
	position: token_position,
}

token_kind :: enum {
	eof,
	// keywords
	fn,
	identifier, // this is a crazy thing

	// symbols
	lparen,
	rparen,
	lcurly,
	rcurly,
	comma,
	colon,
	semicolon,
	arrow,
	minus,
	plus,
	newline,

	// literals
	integer_literal,
	float_literal,
	string_literal,
	char_literal,
}

token_position :: struct {
	line, col: int,
}

keywords := map[string]token_kind {
	"fn" = .fn,
}

symbols := map[string]token_kind {
	"("  = .lparen,
	")"  = .rparen,
	"{"  = .lcurly,
	"}"  = .rcurly,
	","  = .comma,
	":"  = .colon,
	";"  = .semicolon,
	"-"  = .minus,
	"+"  = .plus,
	"->" = .arrow,

	// whitespace
	"\n" = .newline,
	" "  = nil,
}
