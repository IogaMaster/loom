package tether

import "core:fmt"
import "core:os"
import "core:strings"

import "lexer"

main :: proc() {
	data, err := os.read_entire_file("tests/lex.loom", context.allocator)
	if err != nil {
		return
	}
	defer delete(data, context.allocator)

	lx := lexer.Lexer {
		cursor = {line = 1, col = 1},
	}

	tokens := lexer.tokenize_file(&lx, string(data))

	for tok in tokens {
		fmt.println(tok)
	}
}
