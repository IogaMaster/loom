#+feature dynamic-literals
package tether_lexer

token :: struct {
	lexeme:   string,
	kind:     token_kind,
	position: token_position,
}

token_kind :: enum {
	eof,
	// kerwords
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
	newline,

	// literals
	string_literal,
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

	// whitespace
	"\n" = .newline,
	" "  = nil,
}
