package tether

import "core:fmt"
import "core:os"

import "lexer"

main :: proc() {
	data, err := os.read_entire_file("tests/lex.loom", context.allocator)
	if err != nil {
		return
	}
	defer delete(data, context.allocator)

	lx := lexer.lexer {
		source_code     = string(data),
		cursor_position = {0, 0},
	}

	buf: [1]u8
	for true {
		tok := lexer.lexer_next_token(&lx)
		fmt.println(tok, "")
		if tok.kind == .eof {
			break
		}
	}
}
