#pragma once

#include <kh-core/types.h>
#include <kh-astgen/lexer.h>

/*
 *  Stringifies a `kh_lexer_token_entry` resulting to the string
 *  value/representation for the token type and the token value
 *  both out parameters are optional and can be passed with a null
 *  to only obtain the one required. Returns true upon success and false
 *  upon failure such as insufficient buffer.
 */
kh_bool kh_extra_stringify_token_entry(
  const kh_lexer_context * ctx,
  const kh_lexer_token_entry * token,
  kh_utf8 * out_token_type_str,   kh_sz out_token_type_str_sz,
  kh_utf8 * out_token_value_str,  kh_sz out_token_value_str_sz,
  kh_utf8 * out_token_line_str,   kh_sz out_token_line_str_sz,
  kh_utf8 * out_token_column_str, kh_sz out_token_column_str_sz
);
