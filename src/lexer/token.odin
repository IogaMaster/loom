#+feature dynamic-literals
package loom_lexer

token :: struct {
	lexeme:   string,
	kind:     token_kind,
	position: token_position,
}

token_position :: struct {
	line, col: int,
}

token_kind :: enum {
	eof,
	identifier,

	// Keywords
	fn,
	use,
	ret,
	match,
	if_kw, // Renamed to avoid Odin keyword conflict
	for_kw, // Same thing

	// Core Structural Operators
	colon, // :  (Starts mutable variable declaration or match arm block)
	pipe, // |  (Starts constant declaration or explicit type separator)
	eq, // =  (Assignment operator)

	// Brackets & Delimiters
	lparen, // (
	rparen, // )
	lcurly, // {
	rcurly, // }
	lbracket, // [
	rbracket, // ]
	comma, // ,
	semicolon, // ;
	dot, // .
	newline, // \n

	// Control Flow & Modifiers
	arrow, // -> (Function return type marker)
	double_dot, // .. (Inclusive pattern range)
	bang, // !  (Immediate invocation / execute shorthand)
	hash, // #  (Compile-time operator)
	tilde, // ~  (Async function modifier)

	// Math, Logic & Memory
	plus, // +
	minus, // -
	star, // *
	slash, // /
	percent, // %
	amp, // &  (Immutable Reference)
	caret, // ^  (Pointer Type / Dereference / Address-of)
	eq_eq, // == (Equality comparison)
	not_eq, // != (Inequality comparison)
	lt, // <
	gt, // >
	lt_eq, // <=
	gt_eq, // >=

	// Compound Assignments
	plus_eq, // += (Added: explicitly used in `positions += c`)
	minus_eq, // -=
	star_eq, // *=
	slash_eq, // /=

	// Literals
	number_literal,
	float_literal,
	string_literal,
	char_literal,
	boolean_literal,
	nil_literal,
}

keywords := map[string]token_kind {
	"fn"    = .fn,
	"use"   = .use,
	"ret"   = .ret,
	"match" = .match,
	"if"    = .if_kw,
	"for"   = .for_kw,
}

symbols := map[string]token_kind {
	// Assignment & Types
	":"  = .colon,
	"|"  = .pipe,
	"="  = .eq,

	// Brackets
	"("  = .lparen,
	")"  = .rparen,
	"{"  = .lcurly,
	"}"  = .rcurly,
	"["  = .lbracket,
	"]"  = .rbracket,
	","  = .comma,
	";"  = .semicolon,
	"."  = .dot,
	"\n" = .newline,

	// Loom Modifiers
	"->" = .arrow,
	".." = .double_dot,
	"!"  = .bang,
	"#"  = .hash,
	"~"  = .tilde,

	// Math & Memory
	"+"  = .plus,
	"-"  = .minus,
	"*"  = .star,
	"/"  = .slash,
	"%"  = .percent,
	"&"  = .amp,
	"^"  = .caret,

	// Comparisons
	"==" = .eq_eq,
	"!=" = .not_eq,
	"<"  = .lt,
	">"  = .gt,
	"<=" = .lt_eq,
	">=" = .gt_eq,

	// Compound Assignments
	"+=" = .plus_eq,
	"-=" = .minus_eq,
	"*=" = .star_eq,
	"/=" = .slash_eq,

	// Whitespace
	" "  = nil,
	"\t" = nil,
	"\r" = nil,
}
