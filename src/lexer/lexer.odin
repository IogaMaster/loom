package tether_lexer

import "core:strings"

lexer :: struct {
	source_code:     string,
	curr_byte:       int,
	cursor_position: struct {
		line, col: int,
	},
}


lexer_next_token :: proc(lx: ^lexer, peek := false) -> token {
	result := token{}
	tok := "" // build a token one char at a time

	b := lx.curr_byte
	loop: for char, i in strings.split(lx.source_code, "")[b:] {
		i := b + i
		if !peek {lx.curr_byte = i} 	// eat char if not peeking

		// track line number and column for error in parsing
		if char in symbols {
			// create a keyword token
			if tok in keywords {
				result = token {
					lexeme = tok,
					kind = keywords[tok],
					position = token_position {
						line = lx.cursor_position.line,
						col = lx.cursor_position.col - len(tok),
					},
				}
				tok = ""
				break loop
			}

			// create an identifier token
			if len(tok) > 0 {
				result = token {
					lexeme = tok,
					kind = .identifier,
					position = token_position {
						line = lx.cursor_position.line,
						col = lx.cursor_position.col - len(tok),
					},
				}
				tok = ""
				break loop
			}

			// create a symbol token
			result = token {
				lexeme = char,
				kind = symbols[char],
				position = token_position {
					line = lx.cursor_position.line,
					col = lx.cursor_position.col,
				},
			}
			tok = ""
			if char == "\n" {
				lx.cursor_position.line += 1
				lx.cursor_position.col = 0
			} else {
				lx.cursor_position.col += 1
			}
			lx.curr_byte += 1 // force advance

			if result.kind == nil {
				continue // skip on non lexed symbols
			}
			break loop
		}

		lx.cursor_position.col += 1
		tok = strings.concatenate({tok, char})
	}
	return result
}
