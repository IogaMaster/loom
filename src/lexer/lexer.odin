package loom_lexer

import "core:fmt"
import "core:strings"

Lexer :: struct {
	current_byte: int, // position in file
	cursor:       struct {
		line, col: int, // human readable for tokens to store
	},
}

tokenize_file :: proc(lx: ^Lexer, source_code: string) -> []token {
	result := [dynamic]token{} // List of tokens in the source_code
	tok := "" // A buffer for building a token

	in_string_literal := false // used to skip chars for a literal
	in_number_literal := false
	do_escape := false

	// Lex characters in the file
	source_code := strings.split(source_code, "") // redefine as a split array
	for char, i in source_code {
		lx.current_byte += 1 // Move the counters forward
		if char == "\n" {
			lx.cursor.line += 1
			lx.cursor.col = 1
		} else {
			lx.cursor.col += 1
		}
		if char in symbols {
			if symbols[char] == nil && !in_string_literal {continue} 	// skip if char is not tokenizable, "UNLESS CAPTURING INTO A STRING LITERAL"
		}
		tok = strings.concatenate({tok, char}) // add the token to the builder

		next_char: string = ""
		next_index := i + 1
		if !(len(source_code) == next_index) {
			next_char = source_code[next_index]
		}

		/// =========
		/// STRING LITERAL
		/// =========
		if char == "\\" {
			do_escape = true
			continue
		}
		if char == "\"" && !do_escape {
			in_string_literal = !in_string_literal
		} else if char == "\"" {
			do_escape = false
			continue
		}
		if in_string_literal {continue}

		/// =========
		/// NUMBER LITERAL
		/// =========
		switch rune(char[0]) {
		case '0' ..= '9':
			fmt.println(tok)
			if len(tok) < 2 {in_number_literal = true}
			if in_number_literal {
				if next_char == "_" {continue} 	// underscores too
				if next_char == "." {continue} 	// dots can be inside a number literal
			}
		}
		if next_char != "" {
			switch rune(next_char[0]) { 	// stupid hack, but it works
			case '0' ..= '9':
				if char == "-" || char == "." {
					if len(tok) < 2 {
						in_number_literal = true
						continue
					}
				}
			}
		}

		candidate := strings.concatenate({tok, next_char})
		if candidate in symbols {continue} 	// maximal munch

		if tok in symbols {
			append(
				&result,
				token {
					lexeme = tok,
					kind = symbols[tok],
					position = token_position {
						line = lx.cursor.line,
						col = lx.cursor.col - len(tok),
					},
				},
			)
			tok = ""
			continue
		}

		// break on next char is symbol
		if next_char in symbols {
			// keywords
			if tok in keywords {
				append(
					&result,
					token {
						lexeme = tok,
						kind = keywords[tok],
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
				tok = ""
				continue
			}

			// Parse a string literal
			if !in_string_literal && char == "\"" {
				append(
					&result,
					token {
						lexeme = tok,
						kind = .string_literal,
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
				tok = ""
				continue
			}

			if in_number_literal {
				append(
					&result,
					token {
						lexeme = tok,
						kind = token_kind.number_literal,
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
				in_number_literal = false
				tok = ""
				continue
			}

			// Parse a number literal

			// if NOTHING else can be parsed, identifiers are made
			switch tok {
			case "true", "false":
				append(
					&result,
					token {
						lexeme = tok,
						kind = token_kind.boolean_literal,
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
			case "nil":
				append(
					&result,
					token {
						lexeme = tok,
						kind = token_kind.nil_literal,
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
			case:
				append(
					&result,
					token {
						lexeme = tok,
						kind = token_kind.identifier,
						position = token_position {
							line = lx.cursor.line,
							col = lx.cursor.col - len(tok),
						},
					},
				)
			}
			tok = ""
			continue
		}

	}
	return result[:]
}
