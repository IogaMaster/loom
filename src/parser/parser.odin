package loom_parser

import lx "../lexer"
import "core:fmt" // For tokens

Node :: struct {
	tok:   lx.token,
	left:  ^Node,
	right: ^Node,
}

parse_tokens :: proc(tokens: []lx.token) {
	for tok, i in tokens {
		next_tok: lx.token // Peek
		if len(tokens) != i + 1 {
			next_tok = tokens[i + 1]
		}

		#partial switch tok.kind {
		case .number_literal:
			if next_tok.kind == .plus {
				fmt.println(tok.lexeme, "+")
			}
		}
	}
}
