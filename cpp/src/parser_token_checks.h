// The checks of the C++ lexer and recognizer on the records that
// tools/capture_parser_tokens.py writes: `parse_tokens`, the tokens
// parser.py's lexer gave Lark's parser for a source and how the parse
// ended, and `tokenize`, what lexer.py's `tokenize` gave. Each is a
// stage call with a signature for `check-captures`, appended to those of
// step 6 (passes/front_stage.h).
#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "passes/stage_registry.h"

namespace c6502::parser_token_checks {

// Returns the signatures of `parse_tokens` and `tokenize`.
std::vector<passes::StageSignature> signatures();

// Returns the result a `parse_tokens` record holds for `source`, as the
// JSON text `[tokens, outcome]` (tools/capture_parser_tokens.py), with
// the identifiers at the positions `lex_typedefs` taken as typedef
// names in scope when they were lexed, and the lookaheads at the
// positions `retype_typedefs` taken as typedef names in scope when a
// `for` statement ended. A record whose parse failed in an action of
// the parser has `fed`, the number of tokens shifted before it, which
// is what the C++ side runs to.
std::string parse_tokens(std::string_view source, const std::vector<int>& lex_typedefs,
                         const std::vector<int>& retype_typedefs, long long action_fed = -1);

// Returns the result of a `tokenize` record for `source`, the JSON text
// of the tokens, or throws its LexError.
std::string tokenize_json(std::string_view source);

// Returns `s` as Python's `json.dumps` writes a `str`.
std::string json_string(std::string_view s);

}  // namespace c6502::parser_token_checks
